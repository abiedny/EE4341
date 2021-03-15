/* ************************************************************************** */
/** x500_main.c

  @Description
    Main file.
 
 */
/* ************************************************************************** */

#include "io_setup.h"
#include "biedn001_accelerometer_spi.h"

void test_accel(void)
{   
    putchar(' ');
    
    int16_t test = spi2_read_register(0x0F);  // read WHO_AM_I register
    
    if (test == 0x0033)
    {
        printf("\n\r\n\r\n\rPass\n\r");
    }
    else
    {
        printf("Fail\n\r");
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
        data = accel_read_z();
    }
    
    char buffer[50]; 
    sprintf(buffer, "%f", data); 
    printf("%s", buffer);
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

void setup(void)
{
    io_setup();
    // include any other setup functions here
}

/*int main(void)
{
    setup();
    delayms(100);                  // 100 ms delay as precaution
    test_accel();
    
    while(1)
    { 
        delayms(1);
        printf("X: ");
        accel_print_data('x');
        printf("    Y: ");
        accel_print_data('y');
        printf("    Z: ");
        accel_print_data('z');
        printf("\n\r");
        buttons();
    }

    return 0;
}*/

/* *****************************************************************************
 End of File
 */

