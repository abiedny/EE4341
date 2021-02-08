#include "accelerommeter_spi.h"

void spi2_setup(void)
{
    TRISGbits.TRISG6 = 0x0000;	    // set serial clock as an output
    TRISGbits.TRISG9 = 0x0000;	    // set Accelerometer Chip Select as an output
    
    // disable chip select "ACCEL_CS"
    ACCEL_CS = 0;
        
    // clear SPI2CON register
    // clear SPI2BUF register
    SPI2CON = 0x0000;
    SPI2BUF = 0x0000;
    
    // 39.06 kHz baud
    SPI2BRG =   511;   
               
    // clear overflow flag in SPI2STAT register
    SPI2STATbits.SPIROV = 0;
    
    // framed support off
    SPI2CONbits.FRMEN = 0;
    // 16 bit communication
    SPI2CONbits.MODE16 = 1;

    SPI2CONbits.CKE = 0; // data change on idle->active edge
    SPI2CONbits.CKP = 1; // clock idle high

    // master mode
    SPI2CONbits.MSTEN = 1;
    // sample data at the end of clock time
    SPI2CONbits.SMP = 1;
    
    // enable SPI2 (ON)
    SPI2CONbits.ON = 1;
}

void spi2_write_register(uint8_t address, uint8_t data)
{   
    uint16_t write_frame;
    uint16_t trash;
    
    write_frame = ((uint16_t)address << 8) | data;
            
    delay(1);

    // enable accelerometer chip select
    ACCEL_CS = 1;

    // send "write_frame" to the SPI2 buffer
    SPI2BUF = write_frame;
    // wait for the SPI2 buffer full bit    
    while(!SPI2STATbits.SPIRBF);
    // clear SPI2 buffer using the "trash" variable                                        
    trash = SPI2BUF;
    // disable accelerometer chip select
    ACCEL_CS = 0;
}

int16_t spi2_read_register(uint8_t address)
{
    uint16_t read_frame;
    uint16_t value;
    
    read_frame = (((uint16_t)address|0x80) << 8) | 0x00;

    delay(1);

    // enable accelerometer chip select
    ACCEL_CS = 1;

    // send "read_frame to SPI2 buffer          
    SPI2BUF = read_frame;
    // wait for the SPI2 buffer full bit    
     while(!SPI2STATbits.SPIRBF);
    // read the SPI2 buffer contents with the "value" variable                                      
     value = SPI2BUF;
    // disable accelerometer chip select
     ACCEL_CS = 0;
     
    return value;
}

void accel_setup(void)
{
    spi2_write_register(CTRL_REG1, 0x47);
    spi2_write_register(CTRL_REG2, 0x00);
    spi2_write_register(CTRL_REG3, 0x00);
    spi2_write_register(CTRL_REG4, 0x88);
    spi2_write_register(CTRL_REG5, 0x40);
    spi2_write_register(CTRL_REG6, 0x00);
    spi2_write_register(REFERENCE, 0x00);
    spi2_write_register(INT1_THS, 0x00);
    spi2_write_register(INT1_DUR, 0x00);
    spi2_write_register(INT1_CFG, 0x00);
    spi2_write_register(FIFO_CTRL, 0x80);
}

float accel_read_x(void)
{
   int16_t X_H; 
   int16_t X_L; 
   int16_t X;
   
   X_H = spi2_read_register(OUT_X_H);
   X_L = spi2_read_register(OUT_X_L);
   
   // Combine data from both registers
   // See Lab2 manual for instructions
   X = ( (X_H << 8) & 0xF0) | (X_L & 0x0F);

   float value = X * 0.000061;            // Convert to units of g
   return value;
}

float accel_read_y(void)
{
   int16_t Y_H; 
   int16_t Y_L; 
   int16_t Y;
   
   Y_H = spi2_read_register(OUT_Y_H);
   Y_L = spi2_read_register(OUT_Y_L);
   
   // Combine data from both registers
   // See Lab2 manual for instructions
   Y = ( (Y_H << 8) & 0xF0) | (Y_L & 0x0F);
   
   float value = Y * 0.000061;            // Convert to units of g
   return value;
}

float accel_read_z(void)
{
   int16_t Z_H; 
   int16_t Z_L; 
   int16_t Z;
   
   Z_H = spi2_read_register(OUT_Z_H);
   Z_L = spi2_read_register(OUT_Z_L);
   
   // Combine data from both registers
   // See Lab2 manual for instructions
   Z = ( (Z_H << 8) & 0xF0) | (Z_L & 0x0F);
   
   float value = Z * 0.000061;            // Convert to units of g
   return value;
}