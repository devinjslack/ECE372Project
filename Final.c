/*
 * File:   Final_Part_One.c
 * Author: Kevin
 *
 * Created on April 9, 2015, 4:40 PM
 */

#include "p24FJ64GA002.h"
#include "timer.h"
#include "PWM.h"
#include "final.h"
#include <libpic30.h>


_CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & BKBUG_ON & COE_OFF & ICS_PGx1 &
        FWDTEN_OFF & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS32768)

_CONFIG2(IESO_OFF & SOSCSEL_SOSC & WUTSEL_LEG & FNOSC_PRIPLL & FCKSM_CSDCMD & OSCIOFNC_OFF &
        IOL1WAY_OFF & I2C1SEL_PRI & POSCMOD_XT)


typedef enum {
    Start, Follow_Line, Dodge, rangeFinder
} state;

volatile int T1_Count = 0;
volatile int sonicPulseFlag = 0;
volatile float distance = 0;
volatile int pulseDuration = 0;

volatile int IR_STATUS = 0b0000;
volatile state currState = Start;
volatile state nextState = Start;

volatile int ECHO_PREVIOUS = LOW;


volatile int leftCount = 0;
volatile int rightCount = 0;
volatile int RIGHT_PREVIOUS = 0;
volatile int LEFT_PREVIOUS = 0;

void _ISR _CNInterrupt(void) {

    IFS1bits.CNIF = 0;

    //Rangefinder Interrupt Logic:
    if (ECHO == LOW && ECHO_PREVIOUS == HIGH) {
        pulseDuration = T1_Count;
        ECHO_PREVIOUS = LOW;
    } else if (ECHO == HIGH && ECHO_PREVIOUS == LOW) {
        T1_Count = 0;
        TMR1 = 0;
        pulseDuration = 0;
        ECHO_PREVIOUS = HIGH;
    }


    //Infrared Sensor Interrupt Logic:
    if (PORTAbits.RA0 == 1) {
        LATBbits.LATB15 = 1; //YELLOW IR1
        IR_STATUS &= ~IR1;
    } else {
        LATBbits.LATB15 = 0;
        IR_STATUS |= IR1;
    }
    if (PORTAbits.RA1 == 1) {
        LATBbits.LATB14 = 1; //BLUE IR2
        IR_STATUS &= ~IR2;
    } else {
        LATBbits.LATB14 = 0;
        IR_STATUS |= IR2;
    }

    if (PORTBbits.RB5 == 1) {
        LATBbits.LATB13 = 1; //GREEN IR3
        IR_STATUS &= ~IR3;
    } else {
        LATBbits.LATB13 = 0;
        IR_STATUS |= IR3;
    }

    if (PORTBbits.RB3 == 1) {
        LATBbits.LATB12 = 1; //WHITE IR4
        IR_STATUS &= ~IR4;
    } else {
        LATBbits.LATB12 = 0;
        IR_STATUS |= IR4;
    }
    /*The hardware comparator provides an inverted output, which is just a little
     bit more difficult to understand while coding, so it is un-inverted here:*/
    IR_STATUS = ~(0xFFF0 | IR_STATUS);

    return;
}


int main(void) {

    int baseSpeed = 0;
    int IR_ERROR = 0;

    init_IR();
    init_Output_Compare_1();
    init_Output_Compare_2();
    init_Timer1();
    init_Timer3_PWM();
    init_rangeFinder();
    while(0){
        measureDistance();
    }
    while (GO_BABY_GO) {

        switch (currState) {
            case Start:
                nextState = Follow_Line;
                setDutyCycle_OC1(9);
                setDutyCycle_OC2(9);
                __delay_ms(200);

                break;
            case Follow_Line:

                IR_ERROR = getIRError();
                manualControl(IR_ERROR);
                nextState = rangeFinder;
                break;

            case rangeFinder:
                measureDistance();
                if (pulseDuration != 0) {
                    distance = pulseDuration * SPEEDOFSOUND / 2.0;
                    if (distance < 25) nextState = Dodge;
                }
                nextState = Follow_Line;

                break;

            case Dodge:
                /*basically shoot off the track to the right at a 45 degree
                 angle, then arc back until you find the track again*/
                baseSpeed = 7;

                turn_byDegrees(RIGHT, 35);
                setDutyCycle_OC1(9);
                setDutyCycle_OC2(9);
                __delay_ms(1100);
                setDutyCycle_OC1(baseSpeed);
                setDutyCycle_OC2(1.3 * baseSpeed);
                while (IR_STATUS != 0);
                while (!IR_STATUS);
                nextState = Follow_Line;

                break;

//            case Stop:
//                setDutyCycle_OC1(0);
//                setDutyCycle_OC2(0);
//                break;
            default:
                nextState = Follow_Line;
        }
        currState = nextState;
    }

    return (0);
}
