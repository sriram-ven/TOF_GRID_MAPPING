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
#define TIMER_PERIOD 20  // milliseconds
#define ENCODER_TICKS_TO_RADIANS (2 * M_PI / ENCODER_TICKS_PER_REVOLUTION)

#define WHEEL_RADIUS 38.0  // radius of the wheels in mm
#define BASE_WIDTH 200.0  // distance between two wheels in mm

static Matrix pose; // [x; y; theta]
static Matrix wheelSpeeds; // [right wheel; left wheel]
static Matrix diffKinMatrix; // jacobian matrix used to convert wheel speed to pose

char ODOMETRY_Init(void)
{
    // uses timer 3 to update pose
    T3CON = 0;
    T3CONbits.TCKPS = 0b111; // set to 1:256 pre-scale
    PR3 = (F_PB / 1000 * TIMER_PERIOD) >> 8;
    T3CONbits.ON = 1;
    IFS0bits.T3IF = 0;
    IPC3bits.T3IP = 3;
    IEC0bits.T3IE = 1;

    // initialize matrices
    pose = MATRIX_Init(3,1);
    wheelSpeeds = MATRIX_Init(2,1);
    diffKinMatrix = MATRIX_Init(3,2);
    
    // last row are constants that depend on dimensions of robot, so they can be set now
    MATRIX_SetValue(diffKinMatrix, 2, 0, WHEEL_RADIUS / BASE_WIDTH);
    MATRIX_SetValue(diffKinMatrix, 2, 1, -1 * WHEEL_RADIUS / BASE_WIDTH);
    
    return SUCCESS;
}

// timer ISR
void __ISR(_TIMER_3_VECTOR) Timer3IntHandler(void)
{
    IFS0bits.T3IF = 0;

    // calculate current wheel speeds
    static int prevLEncoder = 0;
    static int prevREncoder = 0;

    int curLEncoder = MOTORS_GetEncoderCount(LEFT_MOTOR);
    int curREncoder = MOTORS_GetEncoderCount(RIGHT_MOTOR);

    MATRIX_SetValue(wheelSpeeds, 0, 0, ENCODER_TICKS_TO_RADIANS * (curREncoder - prevREncoder) / (TIMER_PERIOD / 1000.0));
    MATRIX_SetValue(wheelSpeeds, 1, 0, ENCODER_TICKS_TO_RADIANS * (curLEncoder - prevLEncoder) / (TIMER_PERIOD / 1000.0));
    
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
    
    MATRIX_Set(pose, newPos);
    // free matrices used
    MATRIX_Free(positionDot);
    MATRIX_Free(newPos);
}

float ODOMETRY_GetPositionX(){
    return MATRIX_GetValue(pose, 0, 0);
}

float ODOMETRY_GetPositionY(){
    return MATRIX_GetValue(pose, 1, 0);
}

float ODOMETRY_GetDirection(){
    return MATRIX_GetValue(pose, 2, 0);
}

float ODOMETRY_GetRightWheelSpeed(){
    return MATRIX_GetValue(wheelSpeeds, 0, 0);
}

float ODOMETRY_GetLeftWheelSpeed(){
    return MATRIX_GetValue(wheelSpeeds, 1, 0);
}