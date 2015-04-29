#include "p24FJ64GA002.h"

#include "timer.h"




void init_Timer1(){

    T1CONbits.TCS = 0;//internal clock in use
    TIMER1FLAG = LOW;
    IEC0bits.T1IE = ENABLE;
    T1CONbits.TCKPS = 0b11;
    PR1 = (int) FCY/64;
    TIMER1ONOFF = OFF;
    TMR1 = 0;

}

void init_Timer2(){

    T2CONbits.TCS = 0;//internal clock in use
    TIMER2FLAG = LOW;
    IEC0bits.T2IE = ENABLE;
    T2CONbits.TCKPS = 0b11;
    PR2 = (int) FCY/64;
    TIMER2ONOFF = ON;
    TMR2 = 0;

}

//void delay_mS(unsigned int delay) {
//
//    unsigned long prVal;
//   // unsigned int elapsed;
//    T1_Count = 0;
//    T1CONbits.TCKPS = 0b10;//prescaler 1:64
//    prVal = FCY*ONE_MILISECOND/64;
//    PR1 = (int)prVal;
//    TIMER1ONOFF = ON;
//    TMR1 = 0;
//
//
//    while(T1_Count <= delay);
//
//    TIMER1ONOFF = OFF;
//
////}
//void delay_uS(unsigned int delay){
//
//    unsigned long prVal;
//
//    T1_Count = 0;
//    T1CONbits.TCKPS = 0b00;//prescaler 1:1
//    prVal = FCY*ONE_MICROSECOND;
//
//    PR1 = (int)prVal;
//    TIMER1ONOFF = ON;
//    TMR1 = 0;
//
//
//    while(T1_Count <= delay);
//
//    TIMER1ONOFF = OFF;
//
//}