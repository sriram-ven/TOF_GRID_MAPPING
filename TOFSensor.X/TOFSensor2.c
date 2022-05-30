#include "Uart2.h"
#include "BOARD.h"
#include "timers.h"
#include "serial.h"

int main(void) {
    BOARD_Init();
    Uart2Init(115200);
    TIMERS_Init();

    static uint16_t flag = 0;

    while (1) {
        if ((TIMERS_GetMilliSeconds() - flag) >= 2000) {
            Uart2WriteData("penis", 5);
        }
    }
}

