#include "Uart1.h"
//#include "Uart2.h"
#include "BOARD.h"
#include "timers.h"
//#include "serial.h"
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>

#define UART_1
//#define UART_2


#ifdef UART_1

int main(void) {
    BOARD_Init();
    Uart1Init(9600);

    TIMERS_Init();
    TRISE = 0x00;
    LATE = 0x00;

    static uint8_t msg;
    static uint8_t i = 0, j = 0;
    static uint8_t ps[20][5];

    Uart1WriteData("LET THE TEST BEGIN\n", 19);
    while (!U1STAbits.TRMT);

    /* this was me testing how many samples we can get in one second,
     i arbitrarily chose 20, found out we can only reach 10 samples
     in a second using this method*/
    while (TIMERS_GetMilliSeconds() < 1000) {
        if (Uart1HasData()) {
            Uart1ReadByte(&ps[j][i]);
            i++;
            if (i == 5) {
                i = 0;
                j++;
            }
        }
    }
    for (j = 0; j < 20; j++) {
        Uart1WriteData(&ps[j], 5);
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
