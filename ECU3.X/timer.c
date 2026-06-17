/*
 * File:   timer.c
 * Author: SNEHA SAJAN
 *
 * Created on 8 January, 2026, 10:28 AM
 */


#include <xc.h>

extern unsigned int blink_flag;   // declared elsewhere

void init_timer2(void)
{
    TMR2=0;
    PR2=249;     //1ms interrupt (20MHz clock)

    PIR1bits.TMR2IF=0;   //clear flag
    PIE1bits.TMR2IE=1;   //enable timer2 interrupt

    PEIE=1;     //peripheral interrupt enable
    GIE=1;      //global interrupt enable

    T2CONbits.TMR2ON=1;  //start timer2
}

void __interrupt() isr(void)
{
    static unsigned int count = 0;

    if(PIR1bits.TMR2IF)   //check timer2 interrupt flag
    {
        PIR1bits.TMR2IF=0;   //clear flag
        count++;

        if(count>=10000)     //toggle every 10 seconds
        {
            blink_flag=!blink_flag;   //toggle blink flag
            count=0;
        }
    }
}
