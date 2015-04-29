/* 
 * File:   timer.h
 * Author: Kevin
 *
 * Created on March 16, 2015, 12:49 PM
 */

#ifndef TIMER_H
#define	TIMER_H

#define FCY  14745600

#define LOW 0
#define HIGH 1

#define ON 1
#define OFF 0

#define TIMER1FLAG IFS0bits.T1IF
#define TIMER1ONOFF T1CONbits.TON

#define TIMER2FLAG IFS0bits.T2IF
#define TIMER2ONOFF T2CONbits.TON

#define TIMER3FLAG IFS0bits.T3IF
#define TIMER3ONOFF T3CONbits.TON

#define ONE_MILISECOND .001
#define ONE_MICROSECOND .000001
#define ENABLE 1
#define DISABLE 0

void init_Timer1();
void init_Timer2();
//void delay_mS(unsigned int delay);
//void delay_uS(unsigned int delay);

#endif	/* TIMER_H */

