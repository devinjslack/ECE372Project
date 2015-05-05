#include "p24FJ64GA002.h"
#include "timer.h"

void init_Timer1(){

    T1CONbits.TCS = 0;//internal clock in use
    TIMER1FLAG = LOW;
    IEC0bits.T1IE = ENABLE;
    T1CONbits.TCKPS = 0b00;
    PR1 = 14;//ten microseconds
    TIMER1ONOFF = ON;
    TMR1 = 0;

}
