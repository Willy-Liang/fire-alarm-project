/*
 * File:   Buzzer.c
 * Author: willy
 *
 * Created on 2024?1?9?, ?? 10:22
 */

#include <xc.h>
#define _XTAL_FREQ 1000000

//void buzzer_ring();
//void buzzer_close();

void Buzzer_Initialize(void) {
    // PWM mode, P1A, P1C active-high; P1B, P1D active-high
    CCP1CONbits.CCP1M = 0b1100;
    // Timer2 -> On, prescaler -> 4
    T2CONbits.TMR2ON = 0b1;
    T2CONbits.T2CKPS = 0b00;
    
    // PWM period
    PR2 = 0x00;
    // Duty cycle
    CCPR1L = 0x00;
    CCP1CONbits.DC1B = 0b00;
    return;
}

void buzzer_ring(void) {
    PR2 = 0xC2;
    CCPR1L = 0x32;
    __delay_ms(1000);
    //
    PR2 = 0xff;
    CCPR1L = 0x32;
}

void buzzer_close(void) {
    PR2 = 0x00;
    CCPR1L = 0x00;
}