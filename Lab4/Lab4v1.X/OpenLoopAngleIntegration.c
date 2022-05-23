/*
 * File:   part3.c
 * Author: rwaltman@ucsc.edu
 *
 * Created on May 8, 2022, 2:53 PM
 */
#include "BOARD.h"
#include <stdio.h>
#include <stdlib.h>
#include "xc.h"
#include "serial.h"
#include "timers.h"
#include "BNO055.h"
#include "Matrix.h"
#include "math.h"

#define SAMPLE_TIME 20 // time between samples: 50Hz -> 20ms
#define WAIT_TIME 2000 // wait time before bias calibration
#define CAL_TIME 2000 // 10 seconds of collecting samples for bias calibration

#define DEGREE_CONVERSION 131.068

void CalculateBiases(); //calculates bias and stores them in module level variables
void Delay(int time); //delay function that utilizes timer library (blocking)
void UpdateGyroReadings(); //integrates current sample and adds it to total angle
void UpdateDCM_forwardIntegration();
void UpdateDCM_MatrixExponential();

int gyroBiases[3];
float scaleFactors[3] = {1.067, 1.036, 1.036}; //(x,y,z))
float GyroReadings[3] = {0, 0, 0};

Matrix gyroDCM;

int main(void) {
    BOARD_Init();
    SERIAL_Init();
    TIMERS_Init();
    BNO055_Init();
    
    gyroDCM = MATRIX_ConstructEulerMatrix(GyroReadings);
    
    printf("\rLab 4: Gyro Angle Integration\n");
    
    CalculateBiases();
    printf("\rBiases - X: %d, Y: %d, Z: %d\n", gyroBiases[0], gyroBiases[1], gyroBiases[2]);
    
    int prevTime = 0;
    while(1){
        int curTime = TIMERS_GetMilliSeconds();
        if(abs(curTime - prevTime) > SAMPLE_TIME){
            UpdateGyroReadings();
//            UpdateDCM_forwardIntegration();
            UpdateDCM_MatrixExponential();
            float* angles = MATRIX_GetEulerAngles(gyroDCM);
            printf("%f, %f, %f, ", GyroReadings[0], GyroReadings[1], GyroReadings[2]);
            printf("%f, %f, %f\n", angles[0], angles[1], angles[2]);
            free(angles);
            
            prevTime = curTime;
        }
    }
    BOARD_End();
}

void CalculateBiases(){
    gyroBiases[0] = 0;
    gyroBiases[1] = 0;
    gyroBiases[2] = 0;
    Delay(WAIT_TIME);
    int doneTime = TIMERS_GetMilliSeconds() + CAL_TIME;
    int amountSamples = 0;
    while(TIMERS_GetMilliSeconds() < doneTime){
        gyroBiases[0] += BNO055_ReadGyroX();
        gyroBiases[1] += BNO055_ReadGyroY();
        gyroBiases[2] += BNO055_ReadGyroZ();
        amountSamples++;
        Delay(SAMPLE_TIME);
    }
    gyroBiases[0] /= amountSamples;
    gyroBiases[1] /= amountSamples;
    gyroBiases[2] /= amountSamples;
}

void Delay(int time){
    int doneTime = TIMERS_GetMilliSeconds() + time;
    while(TIMERS_GetMilliSeconds() < doneTime);
}

void UpdateGyroReadings(){
    // remove raw biases from sensor readings
    int gyroX = BNO055_ReadGyroX() - gyroBiases[0];
    int gyroY = BNO055_ReadGyroY() - gyroBiases[1];
    int gyroZ = BNO055_ReadGyroZ() - gyroBiases[2];
    
    // convert to degrees per second and then total degree change during sample
    GyroReadings[0] = (gyroX / DEGREE_CONVERSION) * M_PI / 180.0;
    GyroReadings[1] = (gyroY / DEGREE_CONVERSION) * M_PI / 180.0;
    GyroReadings[2] = (gyroZ / DEGREE_CONVERSION) * M_PI / 180.0;
}

void UpdateDCM_forwardIntegration(){
    Matrix pqrMatrix = MATRIX_ConstructCPMatrix(GyroReadings);
    Matrix m1 = MATRIX_Copy(gyroDCM);
    MATRIX_MultiplyScalar(m1,(SAMPLE_TIME/1000.0));
    Matrix m2 = MATRIX_Multiply(pqrMatrix, m1);
    Matrix newDCM = MATRIX_Subtract(gyroDCM, m2);
    
    MATRIX_Free(gyroDCM);
    MATRIX_Free(m1);
    MATRIX_Free(m2);
    MATRIX_Free(pqrMatrix);
    gyroDCM = newDCM;
}

void UpdateDCM_MatrixExponential(){
    Matrix pqrMatrix = MATRIX_ConstructCPMatrix(GyroReadings);
    
    float deltaT = -1 * SAMPLE_TIME / 1000.0;
    float p = GyroReadings[0];
    float q = GyroReadings[1];
    float r = GyroReadings[2];
    
    Matrix exp = MATRIX_Exponential(pqrMatrix, p, q, r, deltaT);
    Matrix newDCM = MATRIX_Multiply(exp, gyroDCM);
    
    MATRIX_Free(pqrMatrix);
    MATRIX_Free(exp);
    MATRIX_Free(gyroDCM);
    gyroDCM = newDCM;
}