/*
** SDMMC.c SD card interface
*/
#include <xc.h>
#include "SDMMC.h"

// I/O definitions
// NOTE: Do not use the pins given
// by the MassStorage.pdf file. To find the 
// correct pinout, use the SDCardBoard and
// IO Expansion Board Information Sheet PDF's
// to map the pins to the top-right slot on the
// expansion board.
#define SDWP _RF1 // Write Protect input
#define SDCD _RF0 // Card Detect input
#define SDCS _RB1 // Card Select output


// Macros from reference file go here
#define readSPI()  writeSPI(0xFF)
#define clockSPI() writeSPI(0xFF)
#define disableSD() SDCS = 1; clockSPI()
#define enableSD() SDCS = 0

// SD card commands from reference file go here
#define RESET           0    // a.k.a. GO_IDLE (CMD0)
#define INIT            1    // a.k.a. SEND_OP_COND (CMD1)
#define READ_SINGLE     17    
#define WRITE_SINGLE    24
#define DATA_START      0xFE
#define DATA_ACCEPT     0x05

#define I_TIMEOUT 10000
#define R_TIMEOUT 25000
#define W_TIMEOUT 250000

void initSD(void){

    // disable SD Card for initialization
    SDCS = 1;

    // Set SPI baud rate to 250 kHz
    SPI1CON = 0x8120; // ON, CKE=1; CKP=0, sample middle 
    SPI1BRG = 71; // 250 kHz clock

} // initSD

unsigned char writeSPI(unsigned char byte){
    SPI1BUF = byte;
    while (!SPI1STATbits.SPIRBF);
    return SPI1BUF;
}// write a byte using SPI

int sendSDCmd(unsigned char c, unsigned a, unsigned crc) {

    // enable SD card
    enableSD();

    // send a 6-byte command packet over SPI
    writeSPI(c | 0x40);
    writeSPI(a >> 24);
    writeSPI(a >> 16);
    writeSPI(a >> 8);
    writeSPI(a);
    writeSPI(crc);

    // read 8 bytes over SPI
    // if any byte is not 0xFF, return that byte
    int r, i;
    for (i = 0; i < 8; i++) {
        r = readSPI();
        if (r != 0xFF) break;
    }
    return r;
} // sendSDCmd

int initMedia(void){
    // Note: initMedia will return different values corresponding to error conditions
    // returns 0: if successful
    // return E_COMMAND_ACK: if failed to acknowledge reset command
    // return E_INIT_TIMEOUT: if failed to initialize

    int i, r;

    // 1. with the card NOT selected
    disableSD();
    
    // 2. send 80 clock cycles start up
    for (i = 0; i++; i < 10) clockSPI();
    
    // 3. now select the card
    enableSD();
    
    // 4. send a single RESET command, then disable SD, and
    // be sure to check if the command acknowledge failed
    r = sendSDCmd(RESET, 0, 0x95);
    disableSD();
    if (r != 1) return E_COMMAND_ACK;
    
    // 5. send repeatedly INIT until Idle terminates, and
    // be sure to check if the initialization failed due to timeout
    for (i = 0; i < I_TIMEOUT; i++) {
        r = sendSDCmd(INIT, 0, 0x95);
        disableSD();
        if (!r) break;
    }
    if (i == R_TIMEOUT) {
        return E_INIT_TIMEOUT;
    }
    
    // 6. increase speed: disable SPI first, change settings and re-enable
    SPI2CON = 0;
    SPI2BRG = 1;
    SPI2CON = 0x8120;
    
    return 0;
} // init media

int readSECTOR( LBA a, char *p){

    // a LBA of sector requested
    // p pointer to sector buffer
    // returns TRUE if successful

    int r, i;

    // 1. send READ command, and check if command was accepted
    r = sendSDCmd(READ_SINGLE, (a << 9), 0x95);
    if (r == 0) {
    // 2. wait for a DATA_START response for some time (use R_TIMEOUT)
        for (i = 0; i < R_TIMEOUT; i++) {
            r = readSPI();
            if (r == DATA_START)
                break;
        }

        // 3. if it did not timeout, read 512 byte of data
        if (i != R_TIMEOUT) {
            i = 512;
            do {
                *p++ = readSPI();
            } while (--i > 0);
            
            // 4. ignore CRC (read 2 bytes but do nothing with them)
            readSPI();
            readSPI();
        }
    }
    // 5. remember to disable the card, return TRUE if DATA_START was previously received, or
    // false otherwise.
    disableSD();
    
    return (r == DATA_START);
} // readSECTOR

int writeSECTOR(LBA a, char *p){

    // a LBA of sector requested
    // p pointer to sector buffer
    // returns TRUE if successful, FAIL if not

    unsigned r, i;
    int FAIL = 5; // arbitrary number
    
    // 0. check Write Protect
    if (getWP()) return FAIL;
    
    // 1. send WRITE command
    r = sendSDCmd(WRITE_SINGLE, (a << 9), 0x95);
    if (r == 0) {
        // 2. send data
        writeSPI(DATA_START);
        
        for (i = 0; i < 512; i++) {
            writeSPI(*p++);
        }
        
        // 3. send dummy CRC
        clockSPI();
        clockSPI();
        
        // 4. check if data accepted
        r = readSPI();
        if ( (r & 0xF) == DATA_ACCEPT) {
            // 5. wait for write completion
            for (i = 0; i < W_TIMEOUT; i++) {
                r = readSPI();
                if (r != 0) break;
            }
        }
        else {
            r = FAIL;
        }
    }
    // 6. remember to disable the card and return TRUE/FALSE
    disableSD();

    return r;
} // writeSECTOR

// SD card connector presence detection switch (1 line of code)
int getCD(void){
    return !SDCD;
}

// card Write Protect tab detection switch (1 line of code)
int getWP(void){
    return SDWP; 
}

// The function below is used in the other .c file
void initData(void){
    int idx, b = 0;
    for (idx = 0; idx < B_SIZE; idx++) {
        data[idx] = b;
        b ^= 1;
    }
}
