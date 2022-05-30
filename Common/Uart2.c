#include "CircularBuffer.h"
#include "Uart1.h"

//CSE13E Support Library
#include "BOARD.h"

#include <xc.h>
#include <sys/attribs.h>

static CircularBuffer uart2RxBuffer;
static uint8_t u2RxBuf[1024];
static CircularBuffer uart2TxBuffer;
static uint8_t u2TxBuf[1024];

/*
 * Private functions.
 */
void Uart2StartTransmission(void);

/**
 * Initialization function for the UART_USED peripheral.
 * Should be called in initialization code for the
 * model. This function configures the UART
 * for whatever baud rate is specified. It also configures two circular buffers
 * for transmission and reception.
 */
void Uart2Init(uint32_t baudRate)
{
    // First initialize the necessary circular buffers.
    CB_Init(&uart2RxBuffer, u2RxBuf, sizeof (u2RxBuf));
    CB_Init(&uart2TxBuffer, u2TxBuf, sizeof (u2TxBuf));

#ifdef PIC32MX
    //the next few lines below are redundant with actions performed in BOARD_Init():
    U2MODEbits.ON = 1; //turn on UART
    U2STAbits.UTXEN = 1; //enable TX pin
    U2STAbits.URXEN = 1; //enable RX oun

    // The FIFO mode here for transmission is not set to `*_TX_BUFFER_EMPTY` as that seems to fail
    // with some characters dropped. This method, waiting until transmission is finished, is
    // technically slower, but works quite nicely.
    
    U2STAbits.UTXISEL = 0b01; //interrupt when transmission is complete
    U2STAbits.URXISEL = 0b00; //interrupt when RX is not empty (has at least 1 character)


    // Configure UART interrupt for both RX and TX
    IEC1bits.U2RXIE = 1; //enable RX interrupt
    IEC1bits.U2TXIE = 1; //enable TX interrupt
    IPC8bits.U2IP = 6; //set UART interrupt priority to 6
    IPC8bits.U2IS = 0; //set interrupt subpriority to 0
#endif
}

void Uart2ChangeBaudRate(uint16_t brgRegister)
{
    uint8_t utxen = U2STAbits.UTXEN;

    // Disable the port;
    U2MODEbits.UARTEN = 0;

    // Change the BRG register to set the new baud rate
    U2BRG = brgRegister;

    // Enable the port restoring the previous transmission settings
    U2MODEbits.UARTEN = 1;
    U2STAbits.UTXEN = utxen;
}

uint8_t Uart2HasData(void)
{
    return (uart2RxBuffer.dataSize > 0);
}

/**
 * This function actually initiates transmission. It
 * attempts to start transmission with the first element
 * in the queue if transmission isn't already proceeding.
 * Once transmission starts the interrupt handler will
 * keep things moving from there. The buffer is checked
 * for new data and the transmission buffer is checked that
 * it has room for new data before attempting to transmit.
 */
void Uart2StartTransmission(void)
{
    while (uart2TxBuffer.dataSize > 0 && !U2STAbits.UTXBF) {
        // A temporary variable is used here because writing directly into U1TXREG causes some weird issues.
        uint8_t c;
        CB_ReadByte(&uart2TxBuffer, &c);
        U2TXREG = c;
    }
}

int Uart2ReadByte(uint8_t *datum)
{
    return CB_ReadByte(&uart2RxBuffer, datum);
}

/**
 * This function supplements the uart1EnqueueData() function by also
 * providing an interface that only enqueues a single byte.
 */
void Uart2WriteByte(uint8_t datum)
{
    CB_WriteByte(&uart2TxBuffer, datum);
    Uart2StartTransmission();
}

/**
 * This function enqueues all bytes in the passed data character array according to the passed
 * length.
 */
int Uart2WriteData(const void *data, size_t length)
{
    int success = CB_WriteMany(&uart2TxBuffer, data, length, FALSE);

    Uart2StartTransmission();

    return success;
}
#ifdef PIC32MX

void __ISR(_UART_2_VECTOR, ipl6auto) Uart2Interrupt(void)
{
    // if receive flag is set, handle received character input
    if (IFS1bits.U2RXIF) {
        // Keep receiving new bytes while the buffer has data.
        while (U2STAbits.URXDA == 1) {
            CB_WriteByte(&uart2RxBuffer, (uint8_t) U2RXREG);
        }

        // Clear buffer overflow bit if triggered
        if (U2STAbits.OERR == 1) {
            U2STAbits.OERR = 0;
        }

        // Clear the interrupt flag
        IFS1bits.U2RXIF = 0;
    }

    // Handle transmission interrupt
    if (IFS1bits.U2TXIF) {
        Uart2StartTransmission();

        // Clear the interrupt flag
        IFS1bits.U2TXIF = 0;
    }
}
#endif
