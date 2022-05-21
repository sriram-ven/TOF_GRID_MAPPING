/*
 * File:   part3.c
 * Author: rwaltman@ucsc.edu
 *
 * Created on May 8, 2022, 2:53 PM
 */
#include "xc.h"
#include "BOARD.h"
#include <stdio.h>
#include <stdlib.h>
#include "serial.h"
#include "timers.h"
#include "BNO055.h"
#include "Matrix.h"
#include "math.h"

#define SAMPLE_TIME 20 // time between samples: 50Hz -> 20ms
#define WAIT_TIME 2000 // wait time before bias calibration
#define CAL_TIME 2000 // 2 seconds of collecting samples for bias calibration

#define DEGREE_CONVERSION 131.068

#define ACCEL_UNIT_SCALING 1000.0

void Delay(int time); // delay function that utilizes timer library (blocking)

void InitGyroCalibration(); // initializes the readings and bias vectors
void CalculateGyroBiases(); // calculates bias and stores them in module level variables
void UpdateGyroReadings(); // initializes gyro matrices and calculates initial biases
void IntegrateClosedLoop(); // closed loop integration with accelerometer feedback

void InitAccelCalibration(); // initializes accelerometer matrices
void UpdateAccelReadings(); // calculates the new accelerometer readings with calibration

Matrix gyroBiases = NULL; // stores the bias of each gyro axis
Matrix gyroReadings = NULL; // stores the current readings of each gyro axis
Matrix gyroDCM = NULL; // accumulated rotation matrix

Matrix accelReadings = NULL; // stores the calibrated acceleration readings
Matrix accelAMatrix = NULL; // stores the calibration scale matrix
Matrix accelBMatrix = NULL; // stores the calibration bias vector
Matrix accelInertial = NULL; //inertial acceleration

int main(void) {
    BOARD_Init();
    SERIAL_Init();
    TIMERS_Init();
    BNO055_Init();

    printf("\rLab 4: Closed Loop Gyro Angle Integration\n");
    InitGyroCalibration();
    InitAccelCalibration();

    int prevTime = 0;
    while (1) {
        int curTime = TIMERS_GetMilliSeconds();
        if (abs(curTime - prevTime) > SAMPLE_TIME) {
            UpdateAccelReadings();
            UpdateGyroReadings();

            IntegrateClosedLoop();
            float* angles = MATRIX_GetEulerAngles(gyroDCM);
            printf("\rangles: - X: %f, Y: %f, Z: %f\n", angles[0], angles[1], angles[2]);
            free(angles);
            
            prevTime = curTime;
        }
    }
    BOARD_End();
}

void CalculateGyroBiases() {
    float gyroBias[3] = {0, 0, 0};
    Delay(WAIT_TIME);
    int doneTime = TIMERS_GetMilliSeconds() + CAL_TIME;
    int amountSamples = 0;
    while (TIMERS_GetMilliSeconds() < doneTime) {
        gyroBias[0] += BNO055_ReadGyroX();
        gyroBias[1] += BNO055_ReadGyroY();
        gyroBias[2] += BNO055_ReadGyroZ();
        amountSamples++;
        Delay(SAMPLE_TIME);
    }
    gyroBias[0] /= amountSamples;
    gyroBias[1] /= amountSamples;
    gyroBias[2] /= amountSamples;

    MATRIX_SetValue(gyroBiases, 0, 0, gyroBias[0]);
    MATRIX_SetValue(gyroBiases, 1, 0, gyroBias[1]);
    MATRIX_SetValue(gyroBiases, 2, 0, gyroBias[2]);
}

void InitGyroCalibration() {
    gyroReadings = MATRIX_Init(3, 1);
    gyroBiases = MATRIX_Init(3, 1);
    float intialAngles[3] = {0, 0, 0};
    gyroDCM = MATRIX_ConstructEulerMatrix(intialAngles);

    CalculateGyroBiases();
    MATRIX_Print(gyroBiases);
}

void InitAccelCalibration() {
    accelReadings = MATRIX_Init(3, 1);

    // A and B calibration matrices from lab3
    accelAMatrix = MATRIX_Init(3, 3);
    MATRIX_SetValue(accelAMatrix, 0, 0, 1.0066);
    MATRIX_SetValue(accelAMatrix, 0, 1, -0.0041);
    MATRIX_SetValue(accelAMatrix, 0, 2, 0.0086);
    MATRIX_SetValue(accelAMatrix, 1, 0, -0.00046);
    MATRIX_SetValue(accelAMatrix, 1, 1, 0.9947);
    MATRIX_SetValue(accelAMatrix, 1, 2, -0.000521);
    MATRIX_SetValue(accelAMatrix, 2, 0, 0.0081);
    MATRIX_SetValue(accelAMatrix, 2, 1, -0.0017);
    MATRIX_SetValue(accelAMatrix, 2, 2, 0.99);

    accelBMatrix = MATRIX_Init(3, 1);
    MATRIX_SetValue(accelBMatrix, 0, 0, 0.0002607);
    MATRIX_SetValue(accelBMatrix, 1, 0, 0.0264);
    MATRIX_SetValue(accelBMatrix, 2, 0, 0.0149);

    // inertial acceleration vector
    accelInertial = MATRIX_Init(3, 1);
    MATRIX_SetValue(accelInertial, 2, 0, -1.0);
}

void Delay(int time) {
    int doneTime = TIMERS_GetMilliSeconds() + time;
    while (TIMERS_GetMilliSeconds() < doneTime);
}

void UpdateGyroReadings() {
    // remove raw biases from sensor readings
    float gyroX = BNO055_ReadGyroX() - MATRIX_GetValue(gyroBiases, 0, 0);
    float gyroY = BNO055_ReadGyroY() - MATRIX_GetValue(gyroBiases, 1, 0);
    float gyroZ = BNO055_ReadGyroZ() - MATRIX_GetValue(gyroBiases, 2, 0);

    // convert to degrees per second and then total degree change during sample
    MATRIX_SetValue(gyroReadings, 0, 0, (gyroX / DEGREE_CONVERSION) * M_PI / 180.0);
    MATRIX_SetValue(gyroReadings, 1, 0, (gyroY / DEGREE_CONVERSION) * M_PI / 180.0);
    MATRIX_SetValue(gyroReadings, 2, 0, (gyroZ / DEGREE_CONVERSION) * M_PI / 180.0);
}

void UpdateAccelReadings() {
    // read raw values from BNO055 and scale the units to g's
    float rawX = (BNO055_ReadAccelX() / ACCEL_UNIT_SCALING);
    float rawY = (BNO055_ReadAccelY() / ACCEL_UNIT_SCALING);
    float rawZ = (BNO055_ReadAccelZ() / ACCEL_UNIT_SCALING);

    // store the readings in the accelReadings vector
    MATRIX_SetValue(accelReadings, 0, 0, rawX);
    MATRIX_SetValue(accelReadings, 1, 0, rawY);
    MATRIX_SetValue(accelReadings, 2, 0, rawZ);

    // calibrate the raw data
    Matrix v1 = MATRIX_Multiply(accelAMatrix, accelReadings);
    Matrix v2 = MATRIX_Add(v1, accelBMatrix);
    
    // normalize the readings
    float rX = MATRIX_GetValue(v2, 0, 0);
    float rY = MATRIX_GetValue(v2, 1, 0);
    float rZ = MATRIX_GetValue(v2, 2, 0);
    float norm = 1 / sqrt(pow(rX, 2) + pow(rY, 2) + pow(rZ, 2));
    
    MATRIX_MultiplyScalar(v2, 1 / norm);
    MATRIX_Set(accelReadings, v2);

    MATRIX_Free(v1);
    MATRIX_Free(v2);
    return;
}

void IntegrateClosedLoop() {
    float kp_a = 5;
    float ki_a = kp_a / 10;

    float accels[3];
    accels[0] = MATRIX_GetValue(accelReadings, 0, 0);
    accels[1] = MATRIX_GetValue(accelReadings, 1, 0);
    accels[2] = MATRIX_GetValue(accelReadings, 2, 0);

    // calculate wMeas
    Matrix abCross = MATRIX_ConstructCPMatrix(accels);
    Matrix aiR = MATRIX_Multiply(gyroDCM, accelInertial);
    Matrix wMeas = MATRIX_Multiply(abCross, aiR);

    // calculate gyroInputWithFeedback
    MATRIX_MultiplyScalar(wMeas, kp_a); // scale wMeas by kp
    Matrix gyroInputWithFeedback = MATRIX_Add(gyroReadings, wMeas);

    //calculate new bias Vector
    float deltaT = -1 * SAMPLE_TIME / 1000.0;
    MATRIX_MultiplyScalar(wMeas, (1 / kp_a)); // undo scaling on wMeas
    MATRIX_MultiplyScalar(wMeas, -1 * ki_a * deltaT); //bDot vec
    Matrix newBias = MATRIX_Add(gyroBiases, wMeas);

    //calculate new gyroDCM
    float pqrVec[3];
    pqrVec[0] = MATRIX_GetValue(gyroInputWithFeedback, 0, 0);
    pqrVec[1] = MATRIX_GetValue(gyroInputWithFeedback, 1, 0);
    pqrVec[2] = MATRIX_GetValue(gyroInputWithFeedback, 2, 0);
    Matrix pqrMatrix = MATRIX_ConstructCPMatrix(pqrVec);
    Matrix exp = MATRIX_Exponential(pqrMatrix, pqrVec[0], pqrVec[1], pqrVec[2], deltaT);
    Matrix newDCM = MATRIX_Multiply(exp, gyroDCM);

    MATRIX_Free(gyroBiases);
    MATRIX_Free(gyroDCM);

    gyroBiases = newBias;
    gyroDCM = newDCM;

    MATRIX_Free(abCross);
    MATRIX_Free(aiR);
    MATRIX_Free(wMeas);
    MATRIX_Free(gyroInputWithFeedback);
    MATRIX_Free(pqrMatrix);
    MATRIX_Free(exp);
}