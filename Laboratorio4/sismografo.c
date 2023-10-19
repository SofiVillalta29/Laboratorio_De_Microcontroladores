#include <stdint.h>
#include <stdio.h>
#include <string.h>

// Includes de lcd-serial
#include "lcd-spi.h"
#include "clock.h"
#include "console.h"
#include "sdram.h"
#include "gfx.h"


#include "grp.h"
#include "gyr_spi.c"
#include "gyr_spi.h"




#include "usart.h"

#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/adc.h>


int main(void){
	char lcd_out[100];
    char int_to_str[20];
    int16_t x, y, z;
    uint16_t voltage;
    float battery;
    int usart_switch = 0;

    clock_setup();
    console_setup(115200);
    sdram_init();
    gyr_setup();
    //adc_setup();
    lcd_spi_init();
    //led_usart_setup();
    gfx_init(lcd_draw_pixel, 240, 320);

    while (1) {
        gfx_fillScreen(LCD_WHITE);
        gfx_setTextSize(2);
        gfx_setCursor(25,15 );
        gfx_puts("Sismografo");

        x = gyr_readX();
        y = gyr_readY();
        z = gyr_readZ();

        // Mostrar valor de X en la pantalla como decimal
        sprintf(lcd_out, "X: %.2f", (float)x/100); // Asumiendo que la escala es 100
        gfx_setCursor(15, 36);
        gfx_puts(lcd_out);

        // Mostrar valor de Y en la pantalla como decimal
        sprintf(lcd_out, "Y: %.2f", (float)y/100); // Asumiendo que la escala es 100
        gfx_setCursor(15, 90);
        gfx_puts(lcd_out);

        // Mostrar valor de Z en la pantalla como decimal
        sprintf(lcd_out, "Z: %.2f", (float)z/100); // Asumiendo que la escala es 100
        gfx_setCursor(15, 144);
        gfx_puts(lcd_out);
    	
		lcd_show_frame();

		int i;
		for (i = 0; i < 8000; i++)    /* Wait a bit. */
			__asm__("nop");
		
	}
	return 0;
}
