#include <stdlib.h>
#include <stdio.h>
#include <xc.h>
#include <sys/attribs.h>
#include <BOARD.h>
#include <math.h>
#include "Matrix.h"
#include "Motors.h"
#include "Odometry.h"
#include "timers.h"

#define F_PB (BOARD_GetPBClock())
#define TIMER_PERIOD 10  // milliseconds
#define ENCODER_TICKS_TO_RADIANS (2 * M_PI / ENCODER_TICKS_PER_REVOLUTION)

#define WHEEL_RADIUS 43.18  // radius of the wheels in mm
#define BASE_WIDTH 200.0  // distance between two wheels in mm

#define MAX_SPEED 13.0
#define VALID_DISTANCE_RANGE 20

static Matrix pose; // [x; y; theta]
static Matrix wheelSpeeds; // [right wheel; left wheel]
static Matrix diffKinMatrix; // jacobian matrix used to convert wheel speed to pose

char odoMode;

char ODOMETRY_Init(char mode) {
    odoMode = mode;
    // uses timer 4 to update pose
    T4CON = 0;
    T4CONbits.TCKPS = 0b111; // set to 1:256 pre-scale
    PR4 = (F_PB / 1000 * TIMER_PERIOD) >> 8;
    T4CONbits.ON = 1;
    IFS0bits.T4IF = 0;
    IPC4bits.T4IP = 3;
    IEC0bits.T4IE = 1;

    MOTORS_Init();

    // initialize matrices
    pose = MATRIX_Init(3, 1);
    wheelSpeeds = MATRIX_Init(2, 1);
    diffKinMatrix = MATRIX_Init(3, 2);

    // last row are constants that depend on dimensions of robot, so they can be set now
    MATRIX_SetValue(diffKinMatrix, 2, 0, WHEEL_RADIUS / BASE_WIDTH);
    MATRIX_SetValue(diffKinMatrix, 2, 1, -1 * WHEEL_RADIUS / BASE_WIDTH);

    return SUCCESS;
}

// timer ISR

void __ISR(_TIMER_4_VECTOR) Timer4IntHandler(void) {
    IFS0bits.T4IF = 0;

    // calculate current wheel speeds
    static int prevLEncoder = 0;
    static int prevREncoder = 0;
    int curLEncoder = MOTORS_GetEncoderCount(LEFT_MOTOR);
    int curREncoder = MOTORS_GetEncoderCount(RIGHT_MOTOR);

    // speed can be directly obtained from motor lib
    float RSpeed = MOTORS_GetMotorSpeed(RIGHT_MOTOR) * MOTORS_GetDirection(RIGHT_MOTOR);
    float LSpeed = MOTORS_GetMotorSpeed(LEFT_MOTOR) * MOTORS_GetDirection(LEFT_MOTOR);

    // at lower speeds use derivative of encoder code
    if ((MOTORS_GetDutyCycle(LEFT_MOTOR) < DC_SWITCH) || (abs(LSpeed) >= MAX_SPEED)) {
        LSpeed =  ENCODER_TICKS_TO_RADIANS * (curLEncoder - prevLEncoder) / (TIMER_PERIOD / 1000.0);
    }
    if ((MOTORS_GetDutyCycle(RIGHT_MOTOR) < DC_SWITCH)  || (abs(RSpeed) >= MAX_SPEED)) {
        RSpeed = ENCODER_TICKS_TO_RADIANS * (curREncoder - prevREncoder) / (TIMER_PERIOD / 1000.0);
    }

    // if there is no change in motor position, set speed to zero
    if (curLEncoder == prevLEncoder) { // if no change in encoder position
        LSpeed = 0;
    }
    if (curREncoder == prevREncoder) { // if no change in encoder position
        RSpeed = 0;
    }

    MATRIX_SetValue(wheelSpeeds, 0, 0, RSpeed);
    MATRIX_SetValue(wheelSpeeds, 1, 0, LSpeed);

    prevREncoder = curREncoder;
    prevLEncoder = curLEncoder;

    // create jacobian matrix
    float currentDirection = MATRIX_GetValue(pose, 2, 0);

    MATRIX_SetValue(diffKinMatrix, 0, 0, cosf(currentDirection) * WHEEL_RADIUS / 2);
    MATRIX_SetValue(diffKinMatrix, 0, 1, cosf(currentDirection) * WHEEL_RADIUS / 2);
    MATRIX_SetValue(diffKinMatrix, 1, 0, sinf(currentDirection) * WHEEL_RADIUS / 2);
    MATRIX_SetValue(diffKinMatrix, 1, 1, sinf(currentDirection) * WHEEL_RADIUS / 2);

    // calculate change in position vector
    Matrix positionDot = MATRIX_Multiply(diffKinMatrix, wheelSpeeds);
    MATRIX_MultiplyScalar(positionDot, TIMER_PERIOD / 1000.0);

    // update position
    Matrix newPos = MATRIX_Add(pose, positionDot);
    
    // make sure that the value makes sense
    float distanceTraveled = sqrt(pow((MATRIX_GetValue(newPos, 0, 0) - MATRIX_GetValue(pose, 0, 0)),2) + pow((MATRIX_GetValue(newPos, 1, 0) - MATRIX_GetValue(pose, 1, 0)),2));
//    printf("\r%f\n", distanceTraveled);
    if(distanceTraveled < VALID_DISTANCE_RANGE){
        MATRIX_Set(pose, newPos);
    }

    // free matrices used
    MATRIX_Free(positionDot);
    MATRIX_Free(newPos);
}

float ODOMETRY_GetPositionX() {
    return MATRIX_GetValue(pose, 0, 0);
}

float ODOMETRY_GetPositionY() {
    return MATRIX_GetValue(pose, 1, 0);
}

float ODOMETRY_GetDirection() {
    return MATRIX_GetValue(pose, 2, 0);
}

float ODOMETRY_GetRightWheelSpeed() {
    return MATRIX_GetValue(wheelSpeeds, 0, 0);
}

float ODOMETRY_GetLeftWheelSpeed() {
    return MATRIX_GetValue(wheelSpeeds, 1, 0);
}

void ODEMTRY_ResetPose() {
    MATRIX_SetValue(pose, 0, 0, 0.0);
    MATRIX_SetValue(pose, 1, 0, 0.0);
    MATRIX_SetValue(pose, 2, 0, 0.0);
}
