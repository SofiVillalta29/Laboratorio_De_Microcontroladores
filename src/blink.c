#include <pic14/pic12f675.h>

void delay(unsigned int tiempo);
unsigned char generarAleatorio();

void main(void) {
    TRISIO = 0b00100000; // Poner GP0 como entrada (para el botón) y el resto como salidas
    GPIO = 0x00; // Poner todos los pines en bajo

    unsigned char valor;

    // Loop forever
    while (1) {
        if (GP5 == 0) { // Si el botón es presionado
            valor = generarAleatorio();

            // Limpiar pines
            GPIO = 0x00;

            switch (valor) {
                case 1:
                    GP1 = 1;
                    break;

                case 2:
                    GP2 = 1;
                    break;

                case 3:
                    GP1 = 1;
                    GP2 = 1;
                    break;

                case 4:
                    GP1 = 1;
                    GP4 = 1;
                    break;

                case 5:
                    GP2 = 1;
                    GP4 = 1;
                    break;

                case 6:
                    GP1 = 1;
                    GP2 = 1;
                    GP4 = 1;
                    break;
            }

            delay(500); // Mostrar el valor por medio segundo
            GPIO = 0x00; // Apagar todos los LEDs
        }
    }
}

void delay(unsigned int tiempo) {
    unsigned int i;
    unsigned int j;

    for (i = 0; i < tiempo; i++)
        for (j = 0; j < 1275; j++);
}

unsigned char generarAleatorio() {
    static unsigned char lfsr = 0x07; // LFSR de 3 bits, valor inicial no puede ser 0
    unsigned char bit;  // Bit de salida

    do {
        bit = ((lfsr >> 0) ^ (lfsr >> 1)) & 1; 
        lfsr = (lfsr >> 1) | (bit << 2);
    } while (lfsr > 6);  // Nos aseguramos de solo obtener valores entre 1 y 6

    return lfsr;  
}