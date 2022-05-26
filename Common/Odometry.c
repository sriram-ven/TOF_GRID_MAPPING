#include <xc.h>
#include <BOARD.h>
#include <math.h>
#include "Matrix.h"
#include "Motors.h"
#include "Odometry.h"

#define F_PB (BOARD_GetPBClock())
#define TIMER_FREQUENCY 20  // 50 Hz
#define TIMER_PERIOD = TIMER_FREQUENCY / 1000.0

#define WHEEL_RADIUS 10  // radius of the wheels in mm
#define BASE_WIDTH 20  // distance between two wheels in mm

static Matrix position; // [x; y; theta]
static Matrix wheelSpeeds; // [right wheel; left wheel]
static Matrix diffKinMatrix;

void ODOMETRY_Init(void)
{
    // uses timer 3 to update pos
    T3CON = 0;
    T3CONbits.TCKPS = 0b01;
    PR3 = (F_PB / TIMER_FREQUENCY) >> 1;
    T3CONbits.ON = 1;
    IFS0bits.T3IF = 0;
    IPC5bits.T3IP = 3;
    IEC0bits.T3IE = 1;

    // initialize matrices
    position = MATRIX_INit(3,1);
    wheelSpeeds = MATRIX_Init(3,1);
    diffKinMatrix = MATRIX_Init(3,2);
}

// timer ISR
void __ISR(_TIMER_3_VECTOR) Timer5IntHandler(void)
{
    IFS0bits.T3IF = 0;

    // calculate current wheel speeds
    static int prevLEncoder = 0;
    static int prevREncoder = 0;

    int curLEncoder = MOTORS_GetEncoderCount(LEFT_MOTOR);
    int curREncoder = MOTORS_GetEncoderCount(RIGHT_MOTOR);

    MATRIX_SetValue(wheelSpeeds, 0, 0, (curREncoder - prevREncdoer) * TIMER_PERIOD);
    MATRIX_SetValue(wheelSpeeds, 0, 1, (curLEncoder - prevLEncdoer) * TIMER_PERIOD);

    // create jacobian matrix
    float currentDirection = MATRIX_GetValue(position, 2, 0);

    MATRIX_SetValue(diffKinMatrix, 0, 0, cosf(currentDirection) * WHEEL_RADIUS / 2);
    MATRIX_SetValue(diffKinMatrix, 0, 1, cosf(currentDirection) * WHEEL_RADIUS / 2);
    MATRIX_SetValue(diffKinMatrix, 1, 0, sinf(currentDirection) * WHEEL_RADIUS / 2);
    MATRIX_SetValue(diffKinMatrix, 1, 1, sinf(currentDirection) * WHEEL_RADIUS / 2);
    MATRIX_SetValue(diffKinMatrix, 2, 0, WHEEL_RADIUS / BASE_WIDTH);
    MATRIX_SetValue(diffKinMatrix, 2, 1, -1 * WHEEL_RADIUS / BASE_WIDTH);

    // calculate change in position vector
    Matrix positionDot = MATRIX_Multiply(diffKinMatrix, wheelSpeeds);
    MATRIX_MultiplyScalar(positionDot, TIMER_PERIOD);

    // update position
    MATRIX_Add(position, positionDot);

    // free matrices used
    MATRIX_Free(positionDot);
}

float ODOMETRY_GetPositionX(){
    return MATRIX_GetValue(position, 0, 0);
}

float ODOMETRY_GetPositionY(){
    return MATRIX_GetValue(position, 1, 0);
}

float ODOMETRY_GetDirection(){
    return MATRIX_GetValue(position, 2, 0);
}

float ODOMETRY_GetRightWheelSpeed(){
    return MATRIX_GetValue(wheelSpeeds, 0, 0);
}

float ODOMETRY_GetLeftWheelSpeed(){
    return MATRIX_GetValue(wheelSpeeds, 1, 0);
}
