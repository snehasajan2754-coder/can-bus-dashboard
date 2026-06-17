/*
 * File:   main.c
 * Author: SNEHA SAJAN
 *
 * Created on 7 January, 2026, 10:35 AM
 */


#include <xc.h>
#include "can.h"
#include "clcd.h"
#include "message_handler.h"

void main(void)
{
    init_clcd();    //setup lcd
    init_can();     //setup can bus
    init_timer2();  //setup timer2 for blinking

    TRISB = TRISB & 0x3C;     //configure portb pins

    while(1)   //infinite loop
    {
        process_canbus_data();   //receive and handle can messages
    }
}
