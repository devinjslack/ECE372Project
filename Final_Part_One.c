/* 
 * File:   Final_Part_One.c
 * Author: Kevin
 *
 * Created on April 9, 2015, 4:40 PM
 */

#include "p24FJ64GA002.h"
//#include "ADC.h"
#include "timer.h"
#include <stdio.h>
#include "PWM.h"
#include "final.h"
#include "pid.h"
#include <stdlib.h>
#include <libpic30.h>

#define GO_BABY_GO 1

_CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & BKBUG_ON & COE_OFF & ICS_PGx1 &
        FWDTEN_OFF & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS32768)

_CONFIG2(IESO_OFF & SOSCSEL_SOSC & WUTSEL_LEG & FNOSC_PRIPLL & FCKSM_CSDCMD & OSCIOFNC_OFF &
        IOL1WAY_OFF & I2C1SEL_PRI & POSCMOD_XT)
/*
 * 
 */


typedef enum {
    Start, Follow_Line, backUp, Stop
} state;

volatile int IR_STATUS = 0b0000;

volatile state currState = Start;
volatile state nextState = Start;
//TRISBbits.TRISB15 = 0;//LED4
//    TRISBbits.TRISB14 = 0;//LED5
//    TRISBbits.TRISB13 = 0;//LED6
//    TRISBbits.TRISB12 = 0;//LED7
//
//    TRISAbits.TRISA0 = 1;//pin2
//    TRISAbits.TRISA1 = 1;//pin3
//    TRISBbits.TRISB2 = 1;//pin6
//    TRISBbits.TRISB3 = 1;//pin7

void _ISR _CNInterrupt(void) {

    IFS1bits.CNIF = 0;

    if (PORTAbits.RA0 == 1) {
        LATBbits.LATB15 = 1; //PURPLE IR1
        IR_STATUS &= ~IR1;
    } else {
        LATBbits.LATB15 = 0;
        IR_STATUS |= IR1;
    }
    if (PORTAbits.RA1 == 1) {
        LATBbits.LATB14 = 1; //PURPLE IR1
        IR_STATUS &= ~IR2;
    } else {
        LATBbits.LATB14 = 0;
        IR_STATUS |= IR2;
    }

    if (PORTBbits.RB5 == 1) {
        LATBbits.LATB13 = 1; //PURPLE IR3
        IR_STATUS &= ~IR3;
    } else {
        LATBbits.LATB13 = 0;
        IR_STATUS |= IR3;
    }

    if (PORTBbits.RB3 == 1) {
        LATBbits.LATB12 = 1; //PURPLE IR1
        IR_STATUS &= ~IR4;
    } else {
        LATBbits.LATB12 = 0;
        IR_STATUS |= IR4;
    }
        IR_STATUS = ~(0xFFF0|IR_STATUS);

    return;
}

int main(void) {

    __delay_ms(1);
    int IR_ERROR = 0;
//    int LAST_ERROR = 0;
    init_IR();
    init_Output_Compare_1();
    init_Output_Compare_2();
    init_Timer1();
    init_Timer2();
    init_Timer3_PWM();
    __delay_ms(2000);
//    int tempTime = 0;
    ////********************************FIXME******************
    //LEFT WHEEL WON'T SPIN BACKWARDS IF DEVBOARD IS NOT IN DEBUG MODE.  NEED TO MOVE OC1 TO PINS 23 AND 24
    while (GO_BABY_GO) {
        //raiseFlag();

        switch (currState) {
            case Start:
                nextState = Follow_Line;
                __delay_ms(1000);
//                                     startTest();
                //                    raiseFlag();
                break;
            case Follow_Line:

                IR_ERROR = getIRError(IR_STATUS);
                manualControl(IR_ERROR);
                break;
                 //
//                if (IR_ERROR == 333) PID(LAST_ERROR);
//                else if (IR_ERROR == 777 || IR_ERROR == 99) {
//                    resetPID();
//                    setDutyCycle_OC1(-4); //0.9
//                    __delay_ms(125);
//                    setDutyCycle_OC2(-4);
//                    while (IR_ERROR == 777 || IR_ERROR == 99) {
//                        IR_ERROR = getIRError(IR_STATUS);
//                    }
//                    //                    __delay_ms(200);
//                    setDutyCycle_OC1(0);
//                    setDutyCycle_OC2(0);
//                } else {
//                    LAST_ERROR = IR_ERROR;
//                    PID(IR_ERROR);
//                    break;
//                }
//                //            case backUp:
//                //
//                //                while(IR_ERROR == 777){
//                //                setDutyCycle_OC1(-6); //0.9
//                //                setDutyCycle_OC2(-6);
//                //                __delay_ms(200);
//                //
//                //                }
//                //                resetPID();
//                //                nextState = Follow_Line;
//                //                break;
            case Stop:
                setDutyCycle_OC1(0); //0.9
                setDutyCycle_OC2(0);
                break;
            default:
                nextState = Follow_Line;
        }
        currState = nextState;
    }

    return (0);
}

