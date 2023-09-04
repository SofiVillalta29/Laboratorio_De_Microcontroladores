#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#define estado_inicio 1
#define parpadeo_led_b1 2
#define encender_led_b2 3
#define encender_led_b6 4
#define parpadeo_led_b6 5

int actual_me;
int next_me;
bool button_pressed = false;
volatile int contador = 0;
volatile int delay_counter = 0;
volatile int button_counter = 0;
volatile bool button_triggered = false;
volatile int blink_counter = 0;


void me();

int main(void) {
    next_me = estado_inicio;
    actual_me = next_me;

    PORTB = 0b00000000;
    PIND = 0b00000000;
    PORTD = 0b00000000;

    DDRB = 0b01111111;
    DDRD = 0b11110011;

    GIMSK |= (1 << INT0);
    MCUCR |= (1 << ISC00) | (1 << ISC01);

    // Configurar Timer0 para interrupción cada 1 ms
    TCCR0A = 0;
    TCCR0B = (1 << CS02) | (1 << CS00); // Preescalador de 1024
    TIMSK |= (1 << TOIE0); // Habilitar interrupción de desbordamiento

    sei();

    while (1) {
        me();
    }

    return 0;
}



void me() {
    switch (actual_me) {

    case estado_inicio:
        PORTB = 0b00101001;  // Asumiendo que este es el estado inicial deseado.
        if (delay_counter >= 1000 && button_triggered) {
            next_me = parpadeo_led_b1;
            delay_counter = 0;
            button_triggered = false;
        }
        break;

    case parpadeo_led_b1:
        if (delay_counter < 300) {
            // Parpadea cada segundo para que sea visible
            if (blink_counter % 2 == 0) {
                PORTB |= (1 << PB1);  // Encender LED B1
            } else {
                PORTB &= ~(1 << PB1);  // Apagar LED B1
            }

            // Incrementar el contador de parpadeo cada segundo
            if (delay_counter != blink_counter) {
                blink_counter = delay_counter;
            }
        } else {
            PORTB &= ~(1 << PB1);  // Apagar LED B1 después de 3 segundos
            next_me = encender_led_b2;
            delay_counter = 0;  // Resetear el contador
        }
        break;

    case encender_led_b2:
        PORTB &= ~(1 << PB1);  // Asegurarse que B1 esté apagado
        PORTB |= (1 << PB2);  // Encender LED B2
        if (delay_counter >= 1) {  // Pasar al siguiente estado después de 1 segundo
            next_me = encender_led_b6;
            delay_counter = 0;  // Resetear el contador
        }
        break;

    case encender_led_b6:
        PORTB &= ~(1 << PB2);  // Asegurarse que B2 esté apagado
        PORTB |= (1 << PB6);  // Encender LED B6
        // Nota: No hay un contador de tiempo específico para este estado,
        // así que B6 permanecerá encendido indefinidamente hasta que algo 
        // más cambie el estado o altere el LED.
        break;
    }
    actual_me = next_me;
}


ISR(INT0_vect) {
    button_triggered = true;
}

ISR(TIMER0_OVF_vect) {
    delay_counter++;
}

