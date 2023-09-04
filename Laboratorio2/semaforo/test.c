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
    PORTB = 0b00101001;
    if (delay_counter >= 10 && button_triggered) {
        next_me = parpadeo_led_b1;
        delay_counter = 0;  // Resetear contador
        button_triggered = false;  // Resetear la señal del botón
    }
    break;

case parpadeo_led_b1:
    if (delay_counter % 500 == 0) {  // Esto hará que parpadee cada 500ms
        PORTB ^= (1 << PB1);  // Invertir estado del LED B1
        blink_counter++;
    }

    if (blink_counter >= 6) {  // Esto debería dar 3 segundos si tu delay_counter se incrementa cada 500ms
        PORTB &= ~(1 << PB1);  // Apagar LED B1
        next_me = encender_led_b2;
        delay_counter = 0;  // Resetear contador
        blink_counter = 0;  // Resetear contador de parpadeo
    }
    break;



    case encender_led_b2:
        PORTB |= (1 << PB2); // Encender LED B2
        next_me = encender_led_b6;
        delay_counter = 1; // 1 segundo
        break;

    case encender_led_b6:
        PORTB |= (1 << PB6); // Encender LED B6
        PORTB &= ~(1 << PB1); // Apagar LED B1
        next_me = parpadeo_led_b6;
        delay_counter = 3; // 3 segundos
        break;

    case parpadeo_led_b6:
        if (delay_counter > 0) {
            delay_counter--;
            PORTB ^= (1 << PB6); // Invertir estado del LED B6
        } else {
            PORTB &= ~(1 << PB6); // Apagar LED B6
            PORTB |= (1 << PB5); // Encender LED B5
            next_me = estado_inicio;
            delay_counter = 1; // 1 segundo
        }
        break;
    }
}

ISR(INT0_vect) {
    button_triggered = true;
    actual_me = next_me;
}

ISR(TIMER0_OVF_vect) {
    if (delay_counter < 3000) {
        delay_counter++;
    }
}
