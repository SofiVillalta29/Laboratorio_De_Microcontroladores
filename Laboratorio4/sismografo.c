#include <stdint.h>
#include <stdio.h>
#include <string.h>

// Includes de lcd-serial
#include "lcd-spi.h"
#include "clock.h"
#include "console.h"
#include "sdram.h"
#include "gfx.h"
#include "usart.h"

#include "adc.h"

#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

int main(void)
{	
	console_setup(115200);

	clock_setup();
	sdram_init();
	lcd_spi_init();

	gfx_init(lcd_draw_pixel, 240, 320);
	
	while (1) {
	  	gfx_fillScreen(LCD_WHITE);
    	// Desplegar en pantalla
    	gfx_setTextColor(LCD_BLACK, LCD_BLACK);

    	gfx_setTextSize(2);
    	gfx_setCursor(15, 20);
    	gfx_puts("Sismografo");

    	gfx_setTextSize(1);
    	gfx_setCursor(15, 45);
    	gfx_puts("Sofia Villalta\n Elias Alvarado");

    	lcd_show_frame();
		
		int i;
		for (i = 0; i < 80000; i++)    /* Wait a bit. */
			__asm__("nop");
		
	}
	return 0;
}
