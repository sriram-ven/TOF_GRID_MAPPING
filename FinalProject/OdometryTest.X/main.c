#include <stdlib.h>
#include <stdio.h>
#include "xc.h"
#include "BOARD.h"
#include "AD.h"
#include "serial.h"
#include "timers.h"
#include "Matrix.h"
#include "Motors.h"
#include "Odometry.h"

#define POT_RANGE 1023
#define ENCODER_CLOSE_ENOUGH 100

typedef enum {
    INIT,
    TURN, 
    MOVE_FORWARD, 
} robotState;

int PotToSpeed();
void MotorSpeedTest();
void OdometryTest();
void Delay(int time);

int main() {
    BOARD_Init();
    AD_Init();
    SERIAL_Init();
    TIMERS_Init();
    MOTORS_Init();
    ODOMETRY_Init();

    AD_AddPins(AD_A0);

    MOTORS_SetSpeed(LEFT_MOTOR, 800);
    while (1){
        printf("\rR: %f, L: %f\n", ODOMETRY_GetRightWheelSpeed(), ODOMETRY_GetLeftWheelSpeed());
        Delay(21);
    }
    BOARD_End();
}

void Delay(int time) {
    int doneTime = TIMERS_GetMilliSeconds() + time;
    while (TIMERS_GetMilliSeconds() < doneTime);
}

int PotToSpeed() {
    int potVal = AD_ReadADPin(AD_A0);
    int speed = (potVal - (POT_RANGE / 2)) * 2 * MAX_MOTOR_SPEED / POT_RANGE;
    return speed;
}

void MotorSpeedTest() {
    int speed = 0;
    MOTORS_SetSpeed(LEFT_MOTOR, speed);
    Delay(1000);
    while (speed <= 1000) {
        speed += 1;
        MOTORS_SetSpeed(LEFT_MOTOR, speed);
        for (int i = 0; i < 5; i++) {
            printf("\r%d, %f\n", speed, ODOMETRY_GetLeftWheelSpeed());
            Delay(10);
        }
    }
    speed = 0;
    MOTORS_SetSpeed(LEFT_MOTOR, 0);
    Delay(1000);

    while (speed >= -1000) {
        speed -= 1;
        MOTORS_SetSpeed(LEFT_MOTOR, speed);
        for (int i = 0; i < 5; i++) {
            printf("\r%d, %f\n", speed, ODOMETRY_GetLeftWheelSpeed());
            Delay(10);
        }
    }

    MOTORS_SetSpeed(LEFT_MOTOR, 0);
}

void OdometryTest() {
    MOTORS_SetSpeed(LEFT_MOTOR, 800);
    for (int i = 0; i < 100; i++) {
        printf("\r%f, %f, %f\n", ODOMETRY_GetPositionX(), ODOMETRY_GetPositionY(), ODOMETRY_GetDirection());
        Delay(21);
    }
    MOTORS_SetSpeed(LEFT_MOTOR, 0);
    Delay(1000);
    MOTORS_SetSpeed(LEFT_MOTOR, -800);
    for (int i = 0; i < 100; i++) {
        printf("\r%f, %f, %f\n", ODOMETRY_GetPositionX(), ODOMETRY_GetPositionY(), ODOMETRY_GetDirection());
        Delay(21);
    }
    MOTORS_SetSpeed(LEFT_MOTOR, 0);
}

void RunSimpleRouteSM(){
    static robotState currentState = INIT;
    static int REncoderTarget = 0;
    
    int REncoder = MOTORS_GetEncoderCount(RIGHT_MOTOR);
    switch(currentState){
        case INIT:
            // set up turn amount
            REncoderTarget = REncoder + (int)(1.25 * ENCODER_TICKS_PER_REVOLUTION);
            MOTORS_SetSpeed(RIGHT_MOTOR, 900);
            MOTORS_SetSpeed(LEFT_MOTOR, -900);
            currentState = TURN;
            break;
        case TURN:
            if(abs(REncoder - REncoderTarget) < ENCODER_CLOSE_ENOUGH){
                // begin moving forward
                MOTORS_SetSpeed(RIGHT_MOTOR, 900);
                MOTORS_SetSpeed(LEFT_MOTOR, 900);
                // target distance is 10 wheel revolutions
                REncoderTarget = REncoder + (int)(10 * ENCODER_TICKS_PER_REVOLUTION);
                currentState = MOVE_FORWARD;
            }
            break;
        case MOVE_FORWARD:
            if(abs(REncoder - REncoderTarget) < ENCODER_CLOSE_ENOUGH){
                // begin turning
                REncoderTarget = REncoder + (int)(1.25 * ENCODER_TICKS_PER_REVOLUTION);
                MOTORS_SetSpeed(RIGHT_MOTOR, 900);
                MOTORS_SetSpeed(LEFT_MOTOR, -900);
                currentState = TURN;
            }
            break;
    }
}