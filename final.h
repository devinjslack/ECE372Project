/* 
 * File:   final.h
 * Author: Kevin
 *
 * Created on April 16, 2015, 7:54 PM
 */

#ifndef FINAL_H
#define	FINAL_H

#define GO_BABY_GO 1
//#define FCY  14745600
#define LOW 0
#define HIGH 1

#define ON 1
#define OFF 0
#define OUTPUT 0
#define INPUT 1
#define ENABLE 1
#define DISABLE 0
#define TRUE 1
#define FALSE 0

#define PWM_VERTICALMULTIPLIER 0.07
#define PWM_MINIMUM_DUTY_CYCLE 4
#define PWM_MAX_DUTY_CYCLE 10//motors will have a 6V power supply, so we need to tone it down a bit.

#define TRIGGER LATBbits.LATB2 //Trigger pin 6
#define ECHO PORTBbits.RB9 //ECHO PIN 18

#define SPEEDOFSOUND .034
#define SOS_CM_PER_uS 29.1

void measureDistance (void);
void init_rangeFinder(void);

#define LEFT -1
#define RIGHT 1
#define TURN_DELAY 100
#define IR1  0b1000
#define IR2  0b0100
#define IR3  0b0010
#define IR4  0b0001

#define LEDOFF 1
#define LEDON 0

#define RIGHT_WHEEL PORTAbits.RA4
#define LEFT_WHEEL PORTBbits.RB4

extern volatile int T1_Count;
extern volatile int sonicPulseFlag;
extern volatile float distance;
extern volatile int pulseDuration;
extern volatile int encoderFlag;
extern volatile int leftCount;
extern volatile int rightCount;
extern volatile int IR_STATUS;

void startTest(void);
void init_IR(void);
int getIRError(void);
void turn_byDegrees(int direction, int degrees);
void manualControl(int IR_ERROR);
void flash(void);

#endif	/* FINAL_H */

