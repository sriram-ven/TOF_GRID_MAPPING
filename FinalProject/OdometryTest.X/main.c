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

int PotToSpeed();
void Delay(int time);

int main(){
    BOARD_Init();
    AD_Init();
    SERIAL_Init();
    TIMERS_Init();
    MOTORS_Init();
    ODOMETRY_Init();
    
    AD_AddPins(AD_A0);
    
    int speed = 0;
    MOTORS_SetSpeed(LEFT_MOTOR, speed);
    Delay(1000);
    while(speed <= 1000){
        speed += 1;
        MOTORS_SetSpeed(LEFT_MOTOR, speed);
        for(int i = 0; i < 5; i++){
            printf("\r%d, %f\n", speed, ODOMETRY_GetLeftWheelSpeed());
            Delay(10);
        }
    }
    speed = 0;
    MOTORS_SetSpeed(LEFT_MOTOR, 0);
    Delay(1000);
    
    while(speed >= -1000){
        speed -= 1;
        MOTORS_SetSpeed(LEFT_MOTOR, speed);
        for(int i = 0; i < 5; i++){
            printf("\r%d, %f\n", speed, ODOMETRY_GetLeftWheelSpeed());
            Delay(10);
        }
    }
    
    MOTORS_SetSpeed(LEFT_MOTOR, 0);
    while(1);
    BOARD_End();
}

void Delay(int time){
    int doneTime = TIMERS_GetMilliSeconds() + time;
    while(TIMERS_GetMilliSeconds() < doneTime);
}

int PotToSpeed(){
    int potVal = AD_ReadADPin(AD_A0);
    int speed = (potVal - (POT_RANGE / 2)) * 2 * MAX_MOTOR_SPEED / POT_RANGE;
    return speed;
}