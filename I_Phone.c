/*******************************************************
This program was created by the
CodeWizardAVR V3.12 Advanced
Automatic Program Generator
© Copyright 1998-2014 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : I_Phone
Version : 1
Date    : 23/10/2019
Author  : Lamyaa Zayed
Company : Just_Dream
Comments: 


Chip type               : ATmega328P
Program type            : Application
AVR Core Clock frequency: 8.000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 512
*******************************************************/

#include "I_Phone.h"

// Declare your global variables here
static unsigned char Speaker;             //PORTB

typedef enum {OFF=0,ON} SWITCH;
SWITCH Screen_Flag,Ringer_Flag,Speaker_Flag,CALL_COME_FLAG;

unsigned char LS_Status=0;         //ADC0
unsigned char VB_Status=0;         //ADC1
#define low (LS_Status<128u)
#define med (LS_Status==128u)
#define high (LS_Status>128u)


// Pin change 16-23 interrupt service routine
interrupt [PC_INT2] void pin_change_isr2(void)
{
// Place your code here
   static char Port_Value;
   Port_Value^=PIND;
   
   //VB_Come
   if(Port_Value & (1<<VB))
   {
     if((!(PIND&(1<<VB)))&& (Speaker_Flag==ON))
    /*{*/
     if(VB_Status<128)
     {   
        if( Speaker<100)
         Speaker--;
     } 
     else if(VB_Status>128)
     {
        if( Speaker<100)
         Speaker++;
     }     
    /*}*/
   }  
   
   //END_CALL
   else if (Port_Value & (1<<END_CALL))
   {
    if ((CALL_COME_FLAG==ON) && (!(PIND&(1<<END_CALL))) )
    {
    PORTC&=~(1<<Screen);;
    Screen_Flag=OFF;
    PORTD&=~(1<<Ringer);;
    Ringer_Flag=OFF;
    Speaker=00;
    Speaker_Flag=OFF; 
    }
   } 
    
   //Accept_CALL
    else if (Port_Value & (1<<ACCEPT_CALL))
   {
    if ((CALL_COME_FLAG==ON) && (!(PIND&(1<<ACCEPT_CALL))) )
    {
     PORTD&=~(1<<Ringer);;
     Ringer_Flag=OFF; 
     Speaker=50;
     Speaker_Flag=ON; 
     }
   } 
   
   Port_Value=PIND;
}

// External Interrupt 0 service routine
interrupt [EXT_INT0] void ext_int0_isr(void)
{
// Place your code here
//CALL_COME
    CALL_COME_FLAG=ON;
    PORTC|=(1<<Screen);;
    Screen_Flag=ON;
    PORTD|=(1<<Ringer);;
    Ringer_Flag=ON;
   
    //CALL_COME_FLAG=OFF
}

// External Interrupt 1 service routine
interrupt [EXT_INT1] void ext_int1_isr(void)
{
// Place your code here
//HB_OR_SWB
   if(Screen_Flag==OFF) 
    {
        PORTC|=(1<<Screen);;
        Screen_Flag=ON;  
    } 
    else
    {
        PORTC&=~(1<<Screen);;
        Screen_Flag=OFF;   
    } 

}

// ADC interrupt service routine
interrupt [ADC_INT] void adc_isr(void)
{
unsigned char adc_data;   // Read the 8 most significant bits
adc_data=ADCH;            // of the AD conversion result
// Place your code here
    
if( (ADMUX &(1<<MUX0)) != 0x01)//this is channel 0
{           
    LS_Status=adc_data ;                                                 
    ADMUX |= (1<<MUX0);//select channel 1
}
else  
{
    VB_Status=adc_data ; 
    ADMUX &=~ (1<<MUX0);//select channel 0           
}   
ADCSRA |= (1<<ADSC);

}

void I_Phone_init()
{
    PORTC&=~(1<<Screen);;
    Screen_Flag=OFF;
    PORTD&=~(1<<Ringer);;
    Ringer_Flag=OFF;
    Speaker=0;
    Speaker_Flag=OFF;     
}

void Speaker_Display()
{
   unsigned char Unit_Digit,Tenth_Digit; 
   if( Speaker<100)
	{
    Unit_Digit=Speaker%10u;
	Tenth_Digit=Speaker/10u;
    PORTB=((Tenth_Digit<<4)|Unit_Digit);
    }
}

void main(void)
{
// Declare your local variables here

I_Phone_Conf();

I_Phone_init();
// Global enable interrupts
#asm("sei")

while (1)
      {
      // Place your code here    
        //Speaker=VB_Status%100; //for test
        Speaker_Display();
        
       if(Screen_Flag==ON)
         {
          
            if (low) 
            {
                PORTD&=~(1<< Luminous_high);
                PORTD|=(1<< Luminous_low);
            }
            else if  (med)
            {
               PORTD&=~(1<< Luminous_low);
               PORTD&=~(1<< Luminous_high);
            }
            else if  (high)
            {  
                PORTD&=~(1<< Luminous_low);
                PORTD|=(1<< Luminous_high);
            }
          }
       if((PINC&(1<<RSS))) 
        {
            PORTD&=~(1<<Ringer);;
            Ringer_Flag=OFF;   
        } 
       /* else
        {
            if (CALL_COME_FLAG==ON)
            {
            PORTD|=(1<<Ringer);;
            Ringer_Flag=ON;
            }
            else
            {
            PORTD&=~(1<<Ringer);;
            Ringer_Flag=OFF;
            }   
        }*/     
         }
      }

