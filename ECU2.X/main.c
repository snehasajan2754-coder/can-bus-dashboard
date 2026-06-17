/*
 * File:   main.c
 * Author: SNEHA SAJAN
 *
 * Created on 7 January, 2026, 10:29 AM
 */


#include <xc.h>
#include "adc.h"
#include "can.h"
#include "dkp.h"

unsigned short prev_adc=0;   //previous adc value
unsigned int flag=0;         //gear flag
unsigned char gear_index=0;  //current gear index
char gears[8][3]={"GN","G1","G2","G3","G4","G5","GR","-C"};   //gear names

uint8_t read_speed(void)   //function to read speed
{
    unsigned short adc_val=read_adc(CHANNEL4)/10.23;   //read adc channel4 and scale
    if(adc_val!=prev_adc)   //check if adc value changed
    {
        prev_adc=adc_val;   //update previous adc value
    }
    return (uint8_t)prev_adc;   //return speed as 1 byte
}

uint8_t read_gear(void)   //function to read gear
{
    unsigned char key=read_digital_keypad(STATE_CHANGE);   //read keypad state change

    if(key==SW1&&flag==0)   //increment gear
    {
        if(gear_index<6)
        {
            gear_index++;
        }
    }
    else if(key==SW2&&flag==0)   //decrement gear
    {
        if(gear_index>0)
        {
            gear_index--;
        }
    }
    else if(key==SW3)   //select -C
    {
        gear_index=7;
        flag=1;
    }
    if(flag==1&&(key==SW1||key==SW2))   //reset after -C
    {
        flag=0;
        gear_index=0;
    }

    return gear_index;   //return gear index
}

void main(void)   //main function
{
    init_adc();             //initialize adc
    init_digital_keypad();  //initialize keypad
    init_can();             //initialize can

    while(1)   //infinite loop
    {
        uint8_t speed_byte=read_speed();   //get speed value
        can_transmit(SPEED_MSG_ID,&speed_byte,1);   //send speed over can
        for(int i=0;i<100;i++);   //small delay

        uint8_t gear_byte=read_gear();   //get gear value
        can_transmit(GEAR_MSG_ID,&gear_byte,1);   //send gear over can
        for(int i=0;i<100;i++);   //small delay
    }
}
