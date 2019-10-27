#ifndef __I_PHONE_H__
#define __I_PHONE_H__

#include <mega328p.h>
#include <delay.h>

extern void I_Phone_Conf();
#define VB  4
#define END_CALL 1
#define ACCEPT_CALL 0
#define CALL_COME 2
#define HB_OR_SWB 3
#define Luminous_low 5
#define Luminous_high 6
#define Ringer 7
#define Screen 5
#define RSS 2

// Voltage Reference: AREF pin
#define ADC_VREF_TYPE ((0<<REFS1) | (0<<REFS0) | (1<<ADLAR))

#endif 