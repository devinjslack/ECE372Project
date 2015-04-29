/*
 * File:   final.h
 * Author: Kevin
 *
 * Created on April 16, 2015, 7:54 PM
 */

#ifndef FINAL_H
#define	FINAL_H
#define FCY  14745600
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

#define LEFT -1
#define RIGHT 1
#define TURN_DELAY 100
#define IR1  0b1000
#define IR2  0b0100
#define IR3  0b0010
#define IR4  0b0001

void startTest(void);
void init_IR(void);
void raiseFlag(void);
int getIRError(int IR_STATUS);
void turn_byDegrees(int direction, int degrees);
void manualControl(int IR_ERROR);
#endif	/* FINAL_H */

