#include <avr/io.h>
#include <avr/interrupt.h>

#define LDPV             PINB0
#define LDVD             PINB1
#define LDPP             PINB2
#define LDPD             PINB3
#define B1               PIND3
#define B2               PIND2

void setup() {
    DDRB |= (1 << LDPV) | (1 << LDVD  ) | (1 << LDPP) | (1 << LDPD);  // Configura los pines de LEDs como salida.
    PORTB |= (1 << B1) | (1 << B2);  // Activar resistencias pull-up para botones.
    
    // Configuración del Timer.
    // Supongamos que estamos usando un prescaler de 1024 y el timer lo configuramos para interrumpir cada segundo.
    TCCR1B |= (1 << CS12) | (1 << CS10);  // Prescaler 1024.
    TCNT1 = 0;
    OCR1A = F_CPU/1024 - 1;  // Interrupción cada segundo.
    TCCR1B |= (1 << WGM12);  // Modo CTC.
    TIMSK1 |= (1 << OCIE1A);  // Habilitar interrupción del Timer.

    // Configuración de interrupciones externas para botones.
    GIMSK |= (1 << INT0);  // Habilitar interrupción externa para B1.
    MCUCR |= (1 << ISC01);  // Configurar interrupción en flanco de bajada.
    
    sei();  // Habilitar interrupciones globales.
}
typedef enum {
    VEHICULO_PASO,
    VEHICULO_DETENIDO,
    PEATON_PASO,
    PEATON_DETENIDO
} estados_t;

estados_t estado_actual = VEHICULO_PASO;
int contador = 0;
bool boton_presionado = false;

ISR(TIMER1_COMPA_vect) {
    switch(estado_actual) {
        case VEHICULO_PASO:
            contador++;
            if(contador >= 10 || boton_presionado) {
                contador = 0;
                boton_presionado = false;
                estado_actual = VEHICULO_DETENIDO;
            }
            break;
        case VEHICULO_DETENIDO:
            contador++;
            if(contador >= 3) {
                contador = 0;
                estado_actual = PEATON_PASO;
            }
            break;
        case PEATON_PASO:
            contador++;
            if(contador >= 1) {
                contador = 0;
                estado_actual = PEATON_DETENIDO;
            }
            break;
        case PEATON_DETENIDO:
            contador++;
            if(contador >= 10) {
                contador = 0;
                estado_actual = VEHICULO_PASO;
            }
            break;
    }
}

ISR(INT0_vect) {
    boton_presionado = true;
}

void loop() {
    switch(estado_actual) {
        case VEHICULO_PASO:
            PORTB |= (1 << LDPV);
            PORTB &= ~(1 << LDVD  );
            PORTB &= ~(1 << LDPP);
            PORTB |= (1 << LDPD);
            break;
        case VEHICULO_DETENIDO:
            PORTB &= ~(1 << LDPV);
            PORTB |= (1 << LDV);
            PORTB &= ~(1 << LDPP);
            PORTB |= (1 << LDPD);
            break;
        case PEATON_PASO:
            PORTB &= ~(1 << LDPV);
            PORTB |= (1 << LDV);
            PORTB |= (1 << LDPP);
            PORTB &= ~(1 << LDPD);
            break;
        case PEATON_DETENIDO:
            PORTB &= ~(1 << LDPV);
            PORTB |= (1 << LDV);
            PORTB &= ~(1 << LDPP);
            PORTB |= (1 << LDPD);
            break;
    }
}
