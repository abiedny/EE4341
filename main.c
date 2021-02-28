#include <xc.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/attribs.h>

#pragma config POSCMOD = HS, FNOSC = PRIPLL

volatile int counter = 0;

void __ISR(_TIMER_1_VECTOR,IPL4SOFT) Timer1ISR(void) {
    if (counter == 0) {
        //ISRdevice0();
    }
    else if (counter == 1) {
        //ISRdevice1();
    }
    else if (counter == 2) {
        //ISRdevice2();
    }
    counter = (++counter) % 3;
}

uint16_t Buffer[64];
uint16_t *Front = &Buffer[0];
uint16_t *Back = &Buffer[0];

uint16_t Empty = 64;
uint16_t Full = 0;

void put(uint16_t val) {
    *Front = val;
    Front = (Front + 1) % 64;
}

uint16_t get(void) {
    uint16_t ret = *Back;
    Back = (Back + 1) % 64;
    return ret;
}

int main(void)
{
    T1CON = 0x0;
    TMR1 = 0x0;
    PR1 = 0xFFFF;
    
    IPC1SET = 0x0001;
    
    T1CONbits.ON = 1;
    
    while(1)
    {
        //gcd(18, 12);
    }

    return 0;
}