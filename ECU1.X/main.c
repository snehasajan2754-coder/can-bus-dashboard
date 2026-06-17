/*
 * File:   main.c
 * Author: SNEHA SAJAN
 *
 * Created on 7 January, 2026, 10:24 AM
 */


#include <xc.h>
#include "adc.h"
#include "can.h"
#include "dkp.h"

unsigned short prev_adc=0;   //previous adc value
unsigned int flag=0;         //indicator flag

uint8_t read_rpm(void)       //function to read rpm
{
    unsigned short adc_val=read_adc(CHANNEL4)/10.23;   //read adc channel4 and scale
    if(adc_val!=prev_adc)                             //check if adc value changed
    {
        prev_adc=adc_val;                             //update previous adc value
    }
    return (uint8_t)prev_adc;                         //return rpm as 1 byte
}

uint8_t read_indicator(void)  //function to read indicator
{
    unsigned char key=read_digital_keypad(STATE_CHANGE);   //read keypad state change

    switch(key)   //check which key pressed
    {
        case SW1:  
            flag=1;   //set flag for sw1
            break;
        case SW2:  
            flag=2;   //set flag for sw2
            break;
        case SW3:  
            flag=3;   //set flag for sw3
            break;
        case SW4:  
            flag=4;   //set flag for sw4
            break;
        default:   
            break;    //do nothing for other keys
    }
    return (uint8_t)flag;   //return indicator flag
}

void main(void)   //main function
{
    init_adc();             //initialize adc
    init_digital_keypad();  //initialize keypad
    init_can();             //initialize can

    while(1)   //infinite loop
    {
        uint8_t rpm_byte=read_rpm();                       //get rpm value
        can_transmit(RPM_MSG_ID,&rpm_byte,1);              //transmit rpm over can
        for(int i=0;i<100;i++);                            //small delay

        uint8_t indicator_byte=read_indicator();           //get indicator value
        can_transmit(INDICATOR_MSG_ID,&indicator_byte,1);  //transmit indicator over can
        for(int i=0;i<100;i++);                            //small delay
    }
}