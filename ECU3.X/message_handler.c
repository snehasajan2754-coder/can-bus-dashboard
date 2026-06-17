/*
 * File:   message_handler.c
 * Author: SNEHA SAJAN
 *
 * Created on 7 January, 2026, 10:35 AM
 */

#include <xc.h>
#include "can.h"
#include "clcd.h"
#include "message_handler.h"
#include "msg_id.h"

unsigned int blink_flag=LED_OFF;   //indicator blink flag

void process_canbus_data(void)   //dispatcher for can messages
{
    uint16_t msg_id;
    uint8_t data;
    uint8_t len;

    can_receive(&msg_id,&data,&len);   //receive can message

    switch(msg_id)   //route message to handler
    {
        case SPEED_MSG_ID:
            handle_speed_data(&data,len);   //handle speed
            break;

        case GEAR_MSG_ID:
            handle_gear_data(&data,len);    //handle gear
            break;

        case RPM_MSG_ID:
            handle_rpm_data(&data,len);     //handle rpm
            break;

        case INDICATOR_MSG_ID:
            handle_indicator_data(&data,len);   //handle indicator
            break;

        default:
            break;   //ignore unknown ids
    }
}

void handle_speed_data(uint8_t *data,uint8_t len)   //speed handler
{
    uint16_t speed=*data;   //extract speed
    clcd_print("SPEED:    ",LINE1(0));   //print label
    clcd_putch((speed/100)+'0',LINE1(8));
    clcd_putch((speed/10)%10+'0',LINE1(9));
    clcd_putch(speed%10+'0',LINE1(10));
}

void handle_gear_data(uint8_t *data,uint8_t len)   //gear handler
{
    static char gears[8][3]={"GN","G1","G2","G3","G4","G5","GR","-C"};
    clcd_print("GEAR:   ",LINE2(0));   //print label
    clcd_print(gears[*data],LINE2(8)); //print gear string
}

void handle_rpm_data(uint8_t *data,uint8_t len)   //rpm handler
{
    uint16_t rpm=*data;   //extract rpm byte
    rpm=rpm*60;           //scale rpm

    clcd_print("RPM:     ",LINE1(0));   //print label
    clcd_putch((rpm/1000)+'0',LINE1(4));
    clcd_putch(((rpm/100)%10)+'0',LINE1(5));
    clcd_putch(((rpm/10)%10)+'0',LINE1(6));
    clcd_putch((rpm%10)+'0',LINE1(7));
}

void handle_indicator_data(uint8_t *data,uint8_t len)   //indicator handler
{
    clcd_print("ind:",LINE2(0));   //print label

    switch(*data)   //check indicator type
    {
        case 1:   //left indicator
            if(blink_flag)
            {
                LEFT_IND_ON();
                clcd_print("<-",LINE2(5));
            }
            else
            {
                LEFT_IND_OFF();
                clcd_print("    ",LINE2(5));
            }
            break;

        case 2:   //hazard (both indicators)
            if(blink_flag)
            {
                RIGHT_IND_ON();
                LEFT_IND_ON();
                clcd_print("<-->",LINE2(5));
            }
            else
            {
                LEFT_IND_OFF();
                RIGHT_IND_OFF();
                clcd_print("    ",LINE2(5));
            }
            break;

        case 3:   //right indicator
            if(blink_flag)
            {
                RIGHT_IND_ON();
                clcd_print("->",LINE2(5));
            }
            else
            {
                RIGHT_IND_OFF();
                clcd_print("    ",LINE2(5));
            }
            break;

        case 4:   //off
        default:
            LEFT_IND_OFF();
            RIGHT_IND_OFF();
            clcd_print("    ",LINE2(5));
            break;
    }
}