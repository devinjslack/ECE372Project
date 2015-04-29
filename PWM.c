#include <p24FJ64GA002.h>
#include "PWM.h"
#include "timer.h"
#include <libpic30.h>

void _ISR _T3Interrupt() {
    // IFS0bits.OC1IF = LOW;
    TIMER3FLAG = LOW; //Lower the flag

}

void init_Timer3_PWM() {

    T3CONbits.TCS = 0; //internal clock in use
    T3CONbits.TCKPS = 0b00; //prescalar to 1:1
    TIMER3FLAG = LOW;
    TMR3 = 0;
    PR3 = PRVAL;
    IEC0bits.T3IE = ENABLE;
    TIMER3ONOFF = ON;
}

void init_Output_Compare_1() {

    // IEC0bits.OC1IE = ENABLE;
    OC1CONbits.OCM = 0x0000; // Turn off Output Compare 1 Module
    //    The following steps should be taken when configuring the output compare module for PWM
    //operation:
    //1. Set the PWM period by writing to the selected Timer Period register (PRy).
    init_Timer3_PWM();

    //2. Set the PWM duty cycle by writing to the OCxRS register.
    OC1RS = 0x00; //Writing to this register will change the duty cycle
    //3. Write the OCxR register with the initial duty cycle.
    OC1R = 0x00; //Initially everything will be at min value.
    //4. Enable interrupts, if required, for the timer and output compare modules. The output
    //compare interrupt is required for PWM Fault pin utilization.
    //DONE IN initTimer3();
    //5. Configure the output compare module for one of two PWM Operation modes by writing to
    //the Output Compare Mode bits, OCM<2:0> (OCxCON<2:0>).
    OC1CONbits.OCM = 0b110; // PWM mode on OC1; Fault pin, OCF1, is disabled
    OC1CONbits.OCTSEL = 0b01; //Timer3 is the clock source for Output Compare 1
    //6. Set the TMRy prescale value and enable the time base by setting TON (TxCON<15>) = 1.
    //DONE IN initTimer3();

    // Note: The OCxR register should be initialized before the output compare module is first
    //enabled. The OCxR register becomes a read-only duty cycle register when the
    //module is operated in the PWM modes. The value held in OCxR will become the
    //PWM duty cycle for the first PWM period. The contents of the duty cycle buffer
    //register,OCxRS, will not be transferred into OCxR until a time base period match
    //occurs.


    CMCONbits.C1EN = ENABLE; //enable comparator 1

    return;
}

void init_Output_Compare_2() {
    OC2CONbits.OCM = 0x0000; // Turn off Output Compare 2 Module
    //    The following steps should be taken when configuring the output compare module for PWM
    //operation:
    //1. Set the PWM period by writing to the selected Timer Period register (PRy).


    //2. Set the PWM duty cycle by writing to the OCxRS register.
    OC2RS = 0x00; //Writing to this register will change the duty cycle
    //3. Write the OCxR register with the initial duty cycle.
    OC2R = 0x00; //Initially everything will be at min value.
    //4. Enable interrupts, if required, for the timer and output compare modules. The output
    //compare interrupt is required for PWM Fault pin utilization.
    //DONE IN initTimer3();
    //5. Configure the output compare module for one of two PWM Operation modes by writing to
    //the Output Compare Mode bits, OCM<2:0> (OCxCON<2:0>).
    OC2CONbits.OCM = 0b110; // PWM mode on OC1; Fault pin, OCF1, is disabled
    OC2CONbits.OCTSEL = 0b01;
    //OC2CONbits.OCTSEL = 1; //Timer3 is the clock source for Output Compare 1 and 2
    //6. Set the TMRy prescale value and enable the time base by setting TON (TxCON<15>) = 1.
    //DONE IN initTimer3();

    // Note: The OCxR register should be initialized before the output compare module is first
    //enabled. The OCxR register becomes a read-only duty cycle register when the
    //module is operated in the PWM modes. The value held in OCxR will become the
    //PWM duty cycle for the first PWM period. The contents of the duty cycle buffer
    //register,OCxRS, will not be transferred into OCxR until a time base period match
    //occurs.


    CMCONbits.C2EN = ENABLE; //enable comparator 1

    return;
}

void setDutyCycle_OC1(double speed) {

    //10 is full speed forward, -10 is full speed reverse
    double velocity = 0;
    velocity = 0.06 * (speed * speed) + 4;


    if (velocity > 10) velocity = 10;
//    if (velocity < -10) velocity = -10;

    if (speed < 0) {
        //velocity *= -1;

        __builtin_write_OSCCONL(OSCCON & 0xBF);
        // Assign output compare 1 to pin 4
//        RPOR0bits.RP0R = 0;
//        RPOR0bits.RP1R = 18;
        RPOR0bits.RP0R = 18;
        RPOR0bits.RP1R = 0;
        __delay_ms(1);
        //Lock registers
        __builtin_write_OSCCONL(OSCCON | 0x40);
    }
    else if(speed > 0){

        __builtin_write_OSCCONL(OSCCON & 0xBF);
        // Assign output compare 1 to pin 5
//        RPOR0bits.RP0R = 18;
//        RPOR0bits.RP1R = 0;
        RPOR0bits.RP0R = 0;
        RPOR0bits.RP1R = 18;
        __delay_ms(1);
        //Lock registers
        __builtin_write_OSCCONL(OSCCON | 0x40);

    }else{
        OC1RS = 0;
        return;
    }

    __delay_ms(1);

    OC1RS = (int) ((velocity / 10.0) * PRVAL);
    return;
}

void setDutyCycle_OC2(double speed) {


    double velocity = 0;
    velocity = 0.06 * (speed * speed) + 4;

    if (velocity > 10) velocity = 10;
//    if (velocity < -10) velocity = -10;

    if (speed < 0) {
       // velocity *= -1;

        __builtin_write_OSCCONL(OSCCON & 0xBF);
        // Assign output compare 1 to pin 18
        RPOR3bits.RP6R = 0;
        RPOR3bits.RP7R = 19;
        __delay_ms(1);
        //Lock registers
        __builtin_write_OSCCONL(OSCCON | 0x40);
    }
    else  if(speed > 0){

        __builtin_write_OSCCONL(OSCCON & 0xBF);
        // Assign output compare 1 to pin 17
        RPOR3bits.RP6R = 19;
        RPOR3bits.RP7R = 0;
        
        __delay_ms(1);
        //Lock registers
        __builtin_write_OSCCONL(OSCCON | 0x40);

    }
    else{
        OC2RS = 0;
        return;
    }


    __delay_ms(1);

    OC2RS = (int) ((velocity / 10.0) * PRVAL);
    return;
}




