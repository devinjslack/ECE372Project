#include "p24FJ64GA002.h"
//#include "ADC.h"
#include "timer.h"
#include <stdio.h>
#include "PWM.h"
#include "final.h"
#include "pid.h"
#include <stdlib.h>
#include <libpic30.h>
volatile int T2_Count = 0;
volatile int bumpCounter = 0;
volatile int PREV_ERROR = 1111111;
volatile int checkeredFlag = 0;
volatile  int turnEnable = 1;
volatile int turnRadius = 180;
volatile int turnDirection = 0;
volatile  int turnID = 1;
void _ISR _T2Interrupt() {
    TIMER1FLAG = LOW; //Lower the flag
    T2_Count++;
   // if (T2_Count == 20) turn_byDegrees(LEFT,90);
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
    //  AD1PCFG = 0xFFFF;

    TRISBbits.TRISB15 = 0; //LED4
    TRISBbits.TRISB14 = 0; //LED5
    TRISBbits.TRISB13 = 0; //LED6
    TRISBbits.TRISB12 = 0; //LED7
    LATBbits.LATB15 = 1;
    LATBbits.LATB14 = 1;
    LATBbits.LATB13 = 1;
    LATBbits.LATB12 = 1;

    TRISAbits.TRISA0 = 1; //pin2purple
    TRISAbits.TRISA1 = 1; //pin3blue
    //  TRISBbits.TRISB2 = 1; //pin6green
    TRISBbits.TRISB3 = 1; //pin7yellow
    AD1PCFGbits.PCFG0 = 1;
    AD1PCFGbits.PCFG1 = 1;
    //AD1PCFGbits.PCFG4 = 1;
    AD1PCFGbits.PCFG5 = 1;
    CNPU1bits.CN2PUE = 1;
    CNPU1bits.CN3PUE = 1;
    //CNPU1bits.CN6PUE = 1;
    CNPU1bits.CN7PUE = 1;

    TRISBbits.TRISB5 = 1;
    CNPU2bits.CN27PUE = 1;
    CNEN2bits.CN27IE = 1;

    CNEN1bits.CN2IE = 1;
    CNEN1bits.CN3IE = 1;
    // CNEN1bits.CN6IE = 1;
    CNEN1bits.CN7IE = 1;

    IEC1bits.CNIE = 1;
    IFS1bits.CNIF = 0;
    return;
}

void raiseFlag(void) {

    //    __delay_us(1000);__delay_us(1000);__delay_us(1000);__delay_us(1000);

}

int getIRError(int IR_STATUS) {
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
    int i = 1;



    //finish line  ID = 1, 180 L/R
    //loop enter ID = 2. 90 L
    //loop ID 3, 90 R  ID 4 = 90 R
    //loop exit ID 5 90 Left
    switch(turnID){
        case 1:
        turnRadius = 180;
        turnDirection = LEFT;
        break;
        case 2:
            turnRadius = 90;
        turnDirection = LEFT;
        break;
        case 3:
        turnRadius = 90;
        turnDirection = RIGHT;
        break;
        case 4:
            turnRadius = 90;
        turnDirection = RIGHT;
        break;
        case 5:
            turnRadius = 90;
        turnDirection = LEFT;
//        turnEnable = 0;
        break;
        default:
            turnRadius = 33;
        turnDirection = turnDirection*-1;
        break;
    }

    switch (IR_ERROR) {
        case -4:
            setDutyCycle_OC1(0);
            setDutyCycle_OC2(9);
            __delay_ms(TURN_DELAY);
            break;
        case -3:
            setDutyCycle_OC1(5);
            setDutyCycle_OC2(9);
            __delay_ms(TURN_DELAY);
            break;
        case -2:
            setDutyCycle_OC1(6);
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
            //            Forward_Check = 1;
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
            setDutyCycle_OC1(6);
            setDutyCycle_OC2(9);
            __delay_ms(TURN_DELAY);
            break;
        case 4:
            setDutyCycle_OC1(9);
            setDutyCycle_OC2(0);
            __delay_ms(TURN_DELAY);
            break;
        case 99://BUMP
            
                if(turnEnable == 1){
                     turn_byDegrees(turnDirection, turnRadius);
                     turnID++;
                     
                    __delay_ms(TURN_DELAY);
                    setDutyCycle_OC1(7);
                    setDutyCycle_OC2(7);
                     __delay_ms(400);


//                    TMR2 = 0;
//                    T2_Count = 0;
                 }

            
            //            }
            break;
        case 777:
            setDutyCycle_OC1(-5);
            setDutyCycle_OC2(-5);
            __delay_ms(TURN_DELAY);
            break;

            //        case 777:
            //
            //            checkeredFlag = HIGH;
            //            setDutyCycle_OC1(7);
            //            setDutyCycle_OC2(7);
            //            __delay_ms(TURN_DELAY);
            //            break;

        default:
            break;
    }
}

void turn_byDegrees(int direction, int degrees) {

    int i = 0;
    IEC1bits.CNIE = 0;
    setDutyCycle_OC1(direction * 9);
    setDutyCycle_OC2(-1 * direction * 9);

    for (i = 0; i < degrees; i++) {
        __delay_ms(4);
    }

    setDutyCycle_OC1(0);
    setDutyCycle_OC2(0);

    for (i = 0; i < 2; i++) {
        LATBbits.LATB15 = 0;
        LATBbits.LATB14 = 0;
        LATBbits.LATB13 = 1;
        LATBbits.LATB12 = 1;
        __delay_ms(200);
        LATBbits.LATB15 = 1;
        LATBbits.LATB14 = 1;
        LATBbits.LATB13 = 0;
        LATBbits.LATB12 = 0;
        __delay_ms(200);
    }

    IEC1bits.CNIE = 1;
}