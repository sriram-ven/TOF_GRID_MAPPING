/* 
 * File:   newmain.c
 * Author: srira
 *
 * Created on May 27, 2022, 12:13 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "xc.h"
#include "BOARD.h"


void DelayMicros(uint32_t microsec) {
    uint32_t tWait, tStart, tCurrent;

    // Calculate the amount of wait time in terms of core processor frequency.
    tWait = (80000000L / 2000000) * microsec;
    asm volatile("mfc0   %0, $9" : "=r"(tStart));
    tCurrent = tStart;
    while ((tCurrent - tStart) < tWait) {
        asm volatile("mfc0   %0, $9" : "=r"(tCurrent));
    }// wait for the time to pass
}

/*
 * 
 */
int main(int argc, char** argv) {
    BOARD_Init();
    
    float x = 0, y = 0, theta = 0, ping = 1;
//    char txBuf[20];
    
    while(1) {
        DelayMicros(100000);
        printf("%f,%f,%f,%f\n", x, y, theta, ping);
        theta = theta + 2.5;
    }
    return (EXIT_SUCCESS);
}