#include "Setting.h"
#include "Buzzer.h"
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#define _XTAL_FREQ 1000000

bool tempHigh(int);
bool smokeDetected(int);
void Write();
void Write_char(char);
void Write_string(char*);
int State = 0;
bool SmokeFlag = false;
bool TempFlag = false;
char a[] = "alarm_";

void __interrupt(high_priority)H_ISR(){
    if (INTCONbits.INT0IF == 1){
        State = (State + 1) % 4;
        switch(State){
            case 1:
                LATC = 1;
                Mode_1_Setting();
                Write();
                break;
            case 2:
                LATC = 2;
                Mode_2_Setting();
                Write();
                break;
            case 3:
                LATC = 3;
                Mode_3_Setting();
                Write();
                break;
            default:
                LATC = 0;
                System_Init();
                Write();
        }        
//        __delay_ms(100);
        INTCONbits.INT0IF = 0;
    }else if (INTCONbits.TMR0IF == 1){
        INTCONbits.TMR0IF = 0;
    }else if (PIR1bits.ADIF == 1){
        int Hvalue = ADRESH;
        int Lvalue = ADRESL;
        Hvalue = Hvalue & 0b11;
        int Value = Hvalue*256 + Lvalue;
        if (State == 1){
            if (smokeDetected(Value)){
                buzzer_ring();
                Write_string(a);
                Write();
            }else{
                buzzer_close();
            }
        }else if (State == 2){
            if (tempHigh(Value)){
                buzzer_ring();
                Write_string(a);
                Write();
            }else{
                buzzer_close();
            }
        }else{                          // state 3
            if (ADCON0bits.CHS == 0){
                SmokeFlag = smokeDetected(Value);
            }else{
                TempFlag = tempHigh(Value);
            }
            if (SmokeFlag == true && TempFlag == true){
                Write_string(a);
                Write();
            }else{
                buzzer_close();
            }
        }
        PIR1bits.ADIF = 0;
        __delay_ms(5);
        if (State == 3){
            if (ADCON0bits.CHS == 0){
                ADCON0bits.CHS = 1;
            }else{
                ADCON0bits.CHS = 0;
            }
        }
        ADCON0bits.GO = 1;
    }
    if(RCIF) {
        char data = RCREG;
        if (data == '1'){
            LATC = 1;
            State = 1;
            Mode_1_Setting();
            Write();
        } else if (data == '2'){
            LATC = 2;
            State = 2;
            Mode_2_Setting();
            Write();
        } else if (data == '3'){
            LATC = 3;
            State = 3;
            Mode_3_Setting();
            Write();
        } else if (data == '0'){
            LATC = 0;
            State = 0;
            System_Init();
            Write();
        }
//         } else {
//            LATC = 0;
//            State = 0;
//            System_Init();
//            Write();
//        }
        __delay_ms(300);
        PIR1bits.RCIF = 0;
    }
    return;
}

void main(void){
    System_Init();
    State = 0;
        while(1);
    return;
}

bool tempHigh(int Value){
    if (Value >= 63)
        return true;
    return false;
}

bool smokeDetected(int Value){
    if (Value >= 64)
        return true;
    return false;
}

void Write(){
    char data =(char) State + '0';
    TXREG = data;
    while(!TXIF);  
    while(!TRMT);
    TXREG = '\n';                   // don't know
    __delay_ms(1000);
}

void Write_char(char data){
    TXREG = data;
    while(!TXIF);  
    while(!TRMT);
    __delay_ms(10);
}
void Write_string(char* text){
    for(int i=0;text[i]!='\0';i++)
        Write_char(text[i]);
//    Write_char('\n');
}