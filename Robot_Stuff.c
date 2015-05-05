#include "p24FJ64GA002.h"
#include "timer.h"
#include "PWM.h"
#include "final.h"
#include <stdlib.h>
#include <libpic30.h>

volatile int IR_PREVIOUS = 0;

void _ISR _T1Interrupt(void) {

    IFS0bits.T1IF = 0; //dropping the timer interrupt flag
    T1_Count++;

}
void startTest(void) {
    setDutyCycle_OC1(6);
    setDutyCycle_OC2(6);
    __delay_ms(500);
    __delay_ms(500);
    setDutyCycle_OC1(7);
    setDutyCycle_OC2(7);
    __delay_ms(500);
    __delay_ms(500);
    setDutyCycle_OC1(8);
    setDutyCycle_OC2(8);
    __delay_ms(500);
    __delay_ms(500);
    setDutyCycle_OC1(10);
    setDutyCycle_OC2(10);
    __delay_ms(500);
    __delay_ms(500);
    setDutyCycle_OC1(0);
    setDutyCycle_OC2(0);
    __delay_ms(500);
    __delay_ms(500);
    setDutyCycle_OC1(-5);
    setDutyCycle_OC2(-5);
    __delay_ms(500);
    __delay_ms(500);
    setDutyCycle_OC1(-10);
    setDutyCycle_OC2(-10);
    __delay_ms(500);
    __delay_ms(500);
    setDutyCycle_OC1(0);
    setDutyCycle_OC2(0);
    __delay_ms(500);
    __delay_ms(500);
    return;
}

void init_IR() {

    //DEVBOARD LED CONFIGURATION BITS (OUTPUT/OFF)
    //THESE HELP TO KNOW IF EVERYTHING IS WORKING PROPERLY
    TRISBbits.TRISB15 = 0; //LED4
    TRISBbits.TRISB14 = 0; //LED5
    TRISBbits.TRISB13 = 0; //LED6
    TRISBbits.TRISB12 = 0; //LED7
    LATBbits.LATB15 = 1;
    LATBbits.LATB14 = 1;
    LATBbits.LATB13 = 1;
    LATBbits.LATB12 = 1;


    //IR SENSOR CONFIGURATION BITS
    TRISAbits.TRISA0 = 1; //pin2 yellow
    TRISAbits.TRISA1 = 1; //pin3 blue
    TRISBbits.TRISB5 = 1; //pin14 green
    TRISBbits.TRISB3 = 1; //pin7 white
    //specifically, digital inputs:
    AD1PCFGbits.PCFG0 = 1;
    AD1PCFGbits.PCFG1 = 1;
    AD1PCFGbits.PCFG5 = 1;
    //internal pullups enabled
    CNPU1bits.CN2PUE = 1;
    CNPU1bits.CN3PUE = 1;
    CNPU2bits.CN27PUE = 1;
    CNPU1bits.CN7PUE = 1;
    //Enable interrupts for IR sensors
    CNEN1bits.CN2IE = 1;
    CNEN1bits.CN3IE = 1;
    CNEN2bits.CN27IE = 1;
    CNEN1bits.CN7IE = 1;
    //enabled interrupts in general, and put down the flag.
    IEC1bits.CNIE = 1;
    IFS1bits.CNIF = 0;
    return;
}


int getIRError(void) {

    switch (IR_STATUS) {
        case 0b0000:
            return 99;
        case 0b1110:
            return -4;
        case 0b1000:
            return -3;
        case 0b1100:
            return -2;
        case 0b0100:
            return -1;
        case 0b0110:
            return 0;
        case 0b0010:
            return 1;
        case 0b0011:
            return 2;
        case 0b0001:
            return 3;
        case 0b0111:
            return 4;
        case 0b1111:
            return 777; ///USE FOR DETECTING THE END OF THE TRACK
        default:
            return 333; //SCAN AGAIN

    }

}

void manualControl(int IR_ERROR) {

    switch (IR_ERROR) {
        case -4:
            setDutyCycle_OC1(5);
            setDutyCycle_OC2(9);
            __delay_ms(TURN_DELAY);
            break;
        case -3:
            setDutyCycle_OC1(6);
            setDutyCycle_OC2(9);
            __delay_ms(TURN_DELAY);
            break;
        case -2:
            setDutyCycle_OC1(7);
            setDutyCycle_OC2(9);
            __delay_ms(TURN_DELAY);
            break;
        case -1:
            setDutyCycle_OC1(8);
            setDutyCycle_OC2(9);
            __delay_ms(TURN_DELAY);
            break;
        case 0:
            setDutyCycle_OC1(9);
            setDutyCycle_OC2(9);
            __delay_ms(TURN_DELAY);
            break;
        case 1:
            setDutyCycle_OC1(9);
            setDutyCycle_OC2(8);
            __delay_ms(TURN_DELAY);
            break;
        case 2:
            setDutyCycle_OC1(9);
            setDutyCycle_OC2(7);
            __delay_ms(TURN_DELAY);
            break;
        case 3:
            setDutyCycle_OC1(9);
            setDutyCycle_OC2(6);
            __delay_ms(TURN_DELAY);
            break;
        case 4:
            setDutyCycle_OC1(9);
            setDutyCycle_OC2(5);
            __delay_ms(TURN_DELAY);
            break;
        case 99:
            setDutyCycle_OC1(7);
            setDutyCycle_OC2(5);
            while(!IR_STATUS);
//            __delay_ms(TURN_DELAY);
            break;
        case 777://Bump
//            setDutyCycle_OC1(-5);
//            setDutyCycle_OC2(-5);
            manualControl(IR_PREVIOUS);
//            __delay_ms(TURN_DELAY);
            break;

        default:
            break;
    }
     IR_PREVIOUS = IR_ERROR;
}

void turn_byDegrees(int direction, int degrees) {

    int i = 0;

    setDutyCycle_OC1(direction * 9);
    setDutyCycle_OC2(-1*direction*9);

    for(i = 0; i < degrees; i++) __delay_ms(7.5);

    setDutyCycle_OC1(0);
    setDutyCycle_OC2(0);

}

void measureDistance(void) {

    TRIGGER = 0;
    __delay_us(2);
    TRIGGER = 1;
    __delay_us(10);
    TRIGGER = 0;

}


void init_rangeFinder(void) {

    IEC1bits.CNIE = 1; //enable interrupt on board
    IFS1bits.CNIF = 0; //lower interrupt flag
    CNEN2bits.CN21IE = 1;//Enable ECHO interrupt

    TRISBbits.TRISB2 = 0; //Trigger output GREEN
    TRISBbits.TRISB9 = 1;//echo input BROWN
    TRIGGER = LOW;

}

void flash(void){

        LATBbits.LATB15 = LEDON;
    __delay_ms(1000);
    LATBbits.LATB15 = LEDOFF;
    __delay_ms(1000);
    LATBbits.LATB15 = LEDON;
    __delay_ms(1000);
    LATBbits.LATB15 = LEDOFF;
    __delay_ms(1000);
    LATBbits.LATB15 = LEDON;
    __delay_ms(1000);
    LATBbits.LATB15 = LEDOFF;
    __delay_ms(1000);
    LATBbits.LATB15 = LEDON;
    __delay_ms(1000);
    LATBbits.LATB15 = LEDOFF;
    __delay_ms(1000);
}