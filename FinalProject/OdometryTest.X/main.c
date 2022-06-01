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

#define BASE_MOTOR_SPEED 900
#define TURN_SPEED 800
#define TURN_TIME 3000 // turns for 2 seconds
#define STOP_TIME 1000
#define FORWARD_TIME 3000 // moves forward for 3 seconds

typedef enum {
    INIT,
    TURN,
    MOVE_FORWARD,
    STOP,
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
    
    int speed = 0;
    int endTime = TIMERS_GetMilliSeconds() + 30000; //30 seconds
    Delay(5000);
    ODEMTRY_ResetPose();
    while (1) {
        if(speed > 1000){
            while(1);
        }
        MOTORS_SetSpeed(LEFT_MOTOR, speed);
        for(int i = 0; i < 10; i++){
            printf("\r%d, %f\n", speed, ODOMETRY_GetLeftWheelSpeed());
            Delay(10);
        }

        speed += 2;
        
//        if (TIMERS_GetMilliSeconds() > endTime) {
//            MOTORS_SetSpeed(LEFT_MOTOR, 0);
//            MOTORS_SetSpeed(RIGHT_MOTOR, 0);
//            while (1);
//        }
//        RunSimpleRouteSM();
        
//        printf("\r%f, %f, %f, %f, %f\n", ODOMETRY_GetPositionX(), ODOMETRY_GetPositionY(), ODOMETRY_GetDirection(), ODOMETRY_GetRightWheelSpeed(), ODOMETRY_GetLeftWheelSpeed());
        //        printf("\r%f, %f, %d, %d\n", ODOMETRY_GetLeftWheelSpeed(), ODOMETRY_GetRightWheelSpeed(), MOTORS_GetEncoderCount(LEFT_MOTOR), MOTORS_GetEncoderCount(RIGHT_MOTOR));
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
            MOTORS_SetSpeed(RIGHT_MOTOR, BASE_MOTOR_SPEED);
            MOTORS_SetSpeed(LEFT_MOTOR, BASE_MOTOR_SPEED);

            currentState = MOVE_FORWARD;
            doneTime = curTime + FORWARD_TIME;
            break;
            
        case MOVE_FORWARD:
            if (doneTime - curTime < 0) {
                MOTORS_SetSpeed(RIGHT_MOTOR, 0);
                MOTORS_SetSpeed(LEFT_MOTOR, 0);

                doneTime = curTime + STOP;
                currentState = STOP;
            }
            break;
            
        case STOP:
            if (doneTime - curTime < 0) {
                MOTORS_SetSpeed(RIGHT_MOTOR, BASE_MOTOR_SPEED);
                MOTORS_SetSpeed(LEFT_MOTOR, -BASE_MOTOR_SPEED);

                doneTime = curTime + TURN_TIME;
                currentState = TURN;
            }
            break;
            
        case TURN:
            if (doneTime - curTime < 0) {
                MOTORS_SetSpeed(RIGHT_MOTOR, BASE_MOTOR_SPEED);
                MOTORS_SetSpeed(LEFT_MOTOR, BASE_MOTOR_SPEED);

                doneTime = curTime + FORWARD_TIME;
                currentState = MOVE_FORWARD;
            }
            break;

    }
}
