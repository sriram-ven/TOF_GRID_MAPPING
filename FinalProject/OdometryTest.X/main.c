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

#define BASE_MOTOR_SPEED 250
#define TURN_SPEED 800
#define TURN_TIME 5000 // turns for 2 seconds
#define FORWARD_TIME 3000 // moves forward for 3 seconds

typedef enum {
    INIT,
    TURN,
    MOVE_FORWARD,
} robotState;

int PotToSpeed();
void MotorSpeedTest();
void OdometryTest();
void Delay(int time);
void RunSimpleRouteSM();

int main() {
    BOARD_Init();
    SERIAL_Init();
    TIMERS_Init();
    ODOMETRY_Init(ODO_MODE2);

    MOTORS_SetSpeed(LEFT_MOTOR, 800);
    MOTORS_SetSpeed(RIGHT_MOTOR, 500);
    Delay(100);
    ODEMTRY_ResetPose();
    int times = 0;
    while (1) {
        //        RunSimpleRouteSM();
        printf("\r%f, %f, %f, %f, %f\n", ODOMETRY_GetPositionX(), ODOMETRY_GetPositionY(), ODOMETRY_GetDirection(), ODOMETRY_GetRightWheelSpeed(), ODOMETRY_GetLeftWheelSpeed());
        times++;
        if (times == 500) {
            MOTORS_SetSpeed(LEFT_MOTOR, 0);
            MOTORS_SetSpeed(RIGHT_MOTOR, 0);
            while(1);
        }
        Delay(20);
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

void RunSimpleRouteSM() {
    static robotState currentState = INIT;
    static int doneTime = 0;

    int curTime = TIMERS_GetMilliSeconds();
    switch (currentState) {
        case INIT:
            Delay(10000);
            MOTORS_SetSpeed(RIGHT_MOTOR, BASE_MOTOR_SPEED);
            MOTORS_SetSpeed(LEFT_MOTOR, BASE_MOTOR_SPEED);

            currentState = MOVE_FORWARD;
            doneTime = curTime + FORWARD_TIME;
            break;
        case TURN:
            if (doneTime - curTime < 0) {
                MOTORS_SetSpeed(RIGHT_MOTOR, BASE_MOTOR_SPEED);
                MOTORS_SetSpeed(LEFT_MOTOR, BASE_MOTOR_SPEED);

                doneTime = curTime + FORWARD_TIME;
                currentState = MOVE_FORWARD;
            }
            break;
        case MOVE_FORWARD:
            if (doneTime - curTime < 0) {
                MOTORS_SetSpeed(RIGHT_MOTOR, TURN_SPEED);
                MOTORS_SetSpeed(LEFT_MOTOR, -TURN_SPEED);

                doneTime = curTime + TURN_TIME;
                currentState = TURN;
            }
            break;
    }
}
