#include "p24fj64ga002.h"
#include "PWM.h"
#include <stdio.h>
#include "timer.h"
#define KP 1200
#define KI 0
#define KD 0
//#define TARGET 0
#define BASE_SPEED 8//This thing was going crazy, so I tried to see if changing this would slow it down... it didn't

//volatile variable declaration
volatile double LAST_ERROR = 0;
volatile double Integral = 0;
volatile double Derivative = 0;
volatile double Proportional = 0;
//volatile float difference = 0;
volatile double DELTA = 0;
volatile float dt = 0;

volatile unsigned int T1_Count = 0;

void _ISR _T1Interrupt() {
    TIMER1FLAG = LOW; //Lower the flag
    T1_Count++;
}

void PID(int ERROR) {
    //variable initialization
    //    float measuredposition = (float)ERROR;
    double newSpeed = 0;
    double leftMotion = 0;
    double rightMotion = 0;
    double ROC = 0;
//    float tempderivative = 0;

    //    float prevLeft = 0;
    //    float prevRight = 0;

    TIMER1ONOFF = ON;

    //    DELTA = ERROR - LAST_ERROR;
    //    Proportional = ERROR*DELTA

    //    integral += ERROR*dt; //difference *dt ????********YEah I think so
    //    tempintegral = integral *KI;
    //
    //    //D
    //
    ////Using initTime1 and initTime2 was confusing for me, so I thought just using dt and TMR1 was a bit easier
    dt = (float) (TMR1 + T1_Count * PR1);

    ROC = (DELTA / dt);

    Derivative = KD * ROC;
    Integral += ERROR * dt;
    Integral *= KI;

    newSpeed = Proportional + Derivative + Integral;
    //    tempderivative = KD* derivative;
    //
    //
    //    newSpeed = proportion + tempintegral + tempderivative;
    //
    //


    //    newSpeed = KP*ERROR + KD*(ERROR - LAST_ERROR);

    LAST_ERROR = ERROR;

    leftMotion = BASE_SPEED + newSpeed;
    rightMotion = BASE_SPEED + newSpeed;

    //    if(prevLeft > leftMotion) prevLeft = leftMotion;
    //    if(prevRight > rightMotion) prevRight = leftMotion;


    // initTime1 = TMR1;

    //Actual units of time shouldn't matter, we will adjust for all that with the constants
    dt = 0;
    T1_Count = 0;
    TMR1 = 0;
    //***********reset the clock to zero and start timing again for the next dt
    setDutyCycle_OC1(leftMotion); //0.9
    setDutyCycle_OC2(rightMotion);



    return;

}

void resetPID(void) {
    //If we stop the PID to do a particular thing, like turn or dodge, we will need to reset all these.
    LAST_ERROR = 0;
    Integral = 0;
    Derivative = 0;
    Proportional = 0;

    DELTA = 0;

    dt = 0;
    return;
}

/*There are some basic guidelines that will help reduce the tuning effort.


Start with Kp, Ki and Kd equalling 0 and work with Kp first. Try setting Kp to a value of 1 and observe the robot. The goal is to get the robot to follow the line even if it is very wobbly. If the robot overshoots and loses the line, reduce the Kp value. If the robot cannot navigate a turn or seems sluggish, increase the Kp value.
Once the robot is able to somewhat follow the line, assign a value of 1 to Kd (skip Ki for the moment). Try increasing this value until you see lesser amount of wobbling.
Once the robot is fairly stable at following the line, assign a value of 0.5 to 1.0 to Ki. If the Ki value is too high, the robot will jerk left and right quickly. If it is too low, you won't see any perceivable difference.  Since Integral is cumulative, the Ki value has a significant impact. You may end up adjusting it by .01 increments.
Once the robot is following the line with good accuracy, you can increase the speed and see if it still is able to follow the line. Speed affects the PID controller and will require retuning as the speed changes.

Pseudo Code:Here is a simple loop that implements the PID control:

start:
error = (target_position) - (theoretical_position)
integral = integral + (error*dt)
derivative = ((error) - (previous_error))/dt
output = (Kp*error) + (Ki*integral) + (Kd*derivative)
previous_error = error
wait (dt)
goto start*/