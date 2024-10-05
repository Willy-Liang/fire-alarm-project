#include <xc.h>
#include <pic18f4520.h>
#define _XTAL_FREQ 1000000

void BlueTooth_Init(void) {
    
    // set input Rx, output Tx
    TRISCbits.TRISC6 = 1;            
    TRISCbits.TRISC7 = 1;
    
    //  Setting UART baud rate
    TXSTAbits.SYNC = 0;      
    TXSTAbits.BRGH = 1;
    BAUDCONbits.BRG16 = 0;     
    SPBRG = 25;      
    
    //   Serial enable
    RCSTAbits.SPEN = 1;             // enable asynchonous serial port
    
    // transmission
    TXSTAbits.TXEN = 1;             // enable transmission
    PIE1bits.TXIE = 0;              // transmit interrupt
    IPR1bits.TXIP = 0;
    PIR1bits.TXIF = 0;              // set when TXREG is empty(can input)
    // reception
    RCSTAbits.CREN = 1;             // enable reception
    PIE1bits.RCIE = 1;              // receive interrupt 
    IPR1bits.RCIP = 1;  
    PIR1bits.RCIF = 0;              // set when reception is complete
    
    return;
}