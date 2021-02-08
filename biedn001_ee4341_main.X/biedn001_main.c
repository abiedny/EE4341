/* ************************************************************************** */
/** x500_main.c

  @Description
    Main file.
 
 */
/* ************************************************************************** */

#include "io_setup.h"


void setup(void)
{
    io_setup();
    // include any other setup functions here
}

int main(void)
{
    setup();
    delay(100);                  // 100 ms delay as precaution

    while(1)
    { 
        buttons();
        test_accel();
    }

    return 0;
}

void test_accel(void)
{   
    putchar(' ');
    
    int16_t test = spi2_read_register(0x0F);  // read WHO_AM_I register
    
    if (test == 0x0033)
    {
        printf("Pass");
    }
    else
    {
        printf("Fail");
    }
}

void accel_print_data(char axis)
{
    float data;
    
    if (axis == 'x')
    {
        data = accel_read_x();
    }
    else if (axis == 'y')
    {
        data = accel_read_y();
    }
    else if (axis == 'z')
    {
        data = accel_read_x();
    }
    
    char buffer[50]; 
    sprintf(buffer, "%f", data); 
    printf("%s \n", buffer);
}

void accel_move_cursor(void)
{
    if (accel_read_x() > 0.4)
    {
        putchar(' ');
    }
    else if (accel_read_x() < -0.4)
    {
        putchar(0x8);
    }
    
}


/* *****************************************************************************
 End of File
 */
