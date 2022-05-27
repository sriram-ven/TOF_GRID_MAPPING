#include <stdlib.h>
#include <stdio.h>
#include "xc.h"
#include "BOARD.h"
#include "serial.h"
#include "timers.h"
#include "Matrix.h"
#include "Motors.h"
#include "Odometry.h"

int main(){
    BOARD_Init();
    SERIAL_Init();
    TIMERS_Init();
    MOTORS_Init();
    ODOMETRY_Init();
    
    MOTORS_SetSpeed(LEFT_MOTOR, -600);
    
    int prevTime = 0;
    while(1){
        int curTime = TIMERS_GetMilliSeconds();
        if (abs(curTime - prevTime) > 40) {
            printf("\r%f, %f, %f\n", ODOMETRY_GetPositionX(), ODOMETRY_GetPositionY(), ODOMETRY_GetDirection());
            prevTime = curTime;
        }
    }
    BOARD_End();
}