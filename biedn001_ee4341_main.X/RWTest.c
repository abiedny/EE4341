/*
** RWTest.c
**
*/
// configuration bit settings, Fcy=72 MHz, Fpb=36 MHz

#pragma config POSCMOD=XT, FNOSC=PRIPLL
#pragma config FPLLIDIV=DIV_2, FPLLMUL=MUL_18, FPLLODIV=DIV_1
#pragma config FPBDIV=DIV_2, FWDTEN=OFF, CP=OFF, BWP=OFF

#include "SDMMC.h"
#include <stdio.h>
#include <string.h>
#include <xc.h>

#define START_ADDRESS 0 // start block address
#define N_BLOCKS 1 // number of blocks 

#define LED3 _RD2  // visual feedback about SD usage status - fail
#define LED2 _RD1  // visual feedback about SD usage status - pass

void delay(int ms){
    int j;
    for (j = 0; j < ms; j++) {
        // 1 ms delay
        int i;
        for (i = 0; i < 9996; i++) {
            asm("NOP");
        }
    }
}
 
main(void){
    
    // set RD1 and RD2 as outputs:
    TRISD = 0x0003;
    TRISB = 0x0000;
    
    LED3 = 0;
    LED2 = 0;
    
    LBA addr;
    int i, j, r;
    
    // 1. initialize data
    initData();
    // 2. initialize SD/MMC module
    initSD();
    // 3. fill the buffer with pattern
    
    // 4. wait for the card to be inserted
    while(!getCD());
    delay(100);
    if (initMedia()) {
        goto End;
    }
    // 5. fill 16 groups of N_BLOCK sectors with data
    LED2 = 1;
    addr = START_ADDRESS;
    for (j = 0; j < 16; j++) {
        for (i = 0; i < N_BLOCKS; i++) {
            if (!writeSECTOR(addr+i*j, data)) {
                goto End;
            }
        }
    }
    // 6. verify the contents of each sector written
    addr = START_ADDRESS;
    for (j = 0; j < 16; j++) {
        for (i = 0; i < N_BLOCKS; i++) {
            if (!readSECTOR(addr+i*j, buffer)) {
                goto End;
            }
            if (memcmp(data, buffer, B_SIZE)) {
                goto End;
            }
        }
    }
    
    // 7. indicate successful execution
    LED3 = 1;
    
    // main loop
    while(1);

    // If this is reached --> failure   
    End:
    
    LED2 = 1;
    
    // main loop
    while(1);
}