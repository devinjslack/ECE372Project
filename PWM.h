/* 
 * File:   PWM.h
 * Author: Kevin
 *
 * Created on March 19, 2015, 11:45 AM
 */

#ifndef PWM_H
#define	PWM_H

#define PRVAL 587//587 eq. 16-5 in FRM, prescalar = 1, 25 kHZ pwm freq, which gives 9bit resolution

#define VERT_OFFSET 0.2
#define HORIZ_OFFSET 0.2
#define VERT_MULTIPLIER 3.2
#define HORIZ_MULTIPLIER 3.2
#define VERT_CENTER 0.5
#define HORIZ_CENTER 0.5

void init_Timer3_PWM();
void init_Output_Compare_1();
void setDutyCycle_OC1(double);
void setDutyCycle_OC2(double);//toil and trouble
void init_Output_Compare_2();

void continueForward();
void softLeft();
void softRight();
void hardLeft();
void hardRight();
void turn180();
void doABarrelRoll();

#endif	/* PWM_H */

