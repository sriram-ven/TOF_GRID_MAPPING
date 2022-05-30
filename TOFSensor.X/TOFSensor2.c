#include "Uart2.h"
#include "BOARD.h"
#include "timers.h"
//#include "serial.h"
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "Uart1.h"

#define UART_1
//#define UART_2

#ifdef UART_1

int main(void) {
    BOARD_Init();
    Uart1Init(9600);
    Uart1ChangeBaudRate(259);
    TIMERS_Init();

    static uint32_t flag = 0;

    while (1) {
        if ((TIMERS_GetMilliSeconds() - flag) >= 2000) {
            Uart1WriteByte(0x41);
            flag = TIMERS_GetMilliSeconds();
            Uart1StartTransmission();

        }
    }
}
#endif


#ifdef UART_2

int main(void) {
    BOARD_Init();
    Uart2Init(115200);
    Uart2ChangeBaudRate(21);
    TIMERS_Init();

    static uint32_t flag = 0;
    while (1) {
        if ((TIMERS_GetMilliSeconds() - flag) >= 2000) {
            Uart2WriteByte(0x41);
            flag = TIMERS_GetMilliSeconds();
        }
    }
}
#endif
