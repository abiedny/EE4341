
#ifndef _ACCEL_DEFINE_H    /* Guard against multiple inclusion */
#define _ACCEL_DEFINE_H

// this file is only necessary when creating your library
// however, before the library, you will need to copy/paste the define statements... 
// ...in your main file for the other functions to work

#include <xc.h>
#include <stdint.h>
#include <stdlib.h>

// Registers and their respective addresses are defined below

#define CTRL_REG1       0x20
#define CTRL_REG2       ?address?
#define CTRL_REG3       ?address?
#define CTRL_REG4       ?address?
#define CTRL_REG5       ?address?
#define CTRL_REG6       ?address?
#define REFERENCE       0x26
#define INT1_THS        ?address?
#define INT1_DUR        0x33
#define INT1_CFG        ?address?
#define FIFO_CTRL       0x2E

#define OUT_X_L         0x28
#define OUT_X_H         ?address?
#define OUT_Y_L         ?address?
#define OUT_Y_H         ?address?
#define OUT_Z_L         ?address?
#define OUT_Z_H         0x2D

// The chip select pin being used for the accelerometer

#define ACCEL_CS              LATGbits.LATG9


void delay(int ms);



#endif /* _ACCEL_DEFINE_H */

/* *****************************************************************************
 End of File
 */
