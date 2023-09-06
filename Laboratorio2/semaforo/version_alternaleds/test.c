#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

// Definicion de estados por utilizar
#define estado_inicio 1 // Estado inicial de la máquina de estados
#define vehiculos     2 // Estado para el paso de vehículos
#define peatones      3 // Estado donde tiene el paso de peatones
#define transicion    4 // Estado de transición

// Se declaran las variables
int actual_me;          // Estado actual de la máquina de estados
int next_me;            // Próximo estado de la máquina de estados
int out;                // Variable para controlar la salida
volatile bool change_state = false; // Variable que indica si se debe cambiar el estado

// Declaración de funciones por utilizar
void me();              // Función donde se establece la máquina de estados por manejar
void start_peatones_timer();
void init_timer0();     // Función para inicializar el temporizador TIMER0

// Contador para llevar el tiempo de peatones en segundos
volatile uint16_t peatones_timer = 0;

// Variable para controlar el parpadeo de PB0
bool pb0_blink = false;

// Función principal
int main(void) {
    // Inicialización de estados
    next_me = estado_inicio; // Establece el estado de inicio
    actual_me = next_me;

    // Configuración de los registros del microcontrolador
    PORTB = 0x00;
    PIND = 0X00;
    PORTD = 0x00;
    PORTB = 0b00000000;
    PIND = 0b00000000;
    PORTD = 0b00000000;

    // Configuración del puerto B como salidas
    DDRB = 0b01111111;
    // Configuración de los pines de GPIOS dos y tres como entradas
    DDRD = 0b11110011;

    // Configuración de las interrupciones INT0 y INT1
    GIMSK |= (1 << INT0);
    GIMSK |= (1 << INT1);
    // Configuración de los flancos de disparo para las interrupciones
    MCUCR |= (1 << ISC00) | (1 << ISC01);

    // Configuración del temporizador TIMER1 para generar una interrupción cada 1 segundo
    TCCR1B |= (1 << WGM12); // Modo de comparación CTC
    OCR1A = 7812; // Valor para contar 3 segundos (ajustar según la frecuencia de tu microcontrolador)
    TIMSK |= (1 << OCIE1A); // Habilitar interrupción de comparación
    TCCR1B |= (1 << CS12) | (1 << CS10); // Preescalador de 1024

    // Habilitar interrupciones globales
    sei();

    // Llamar a la función que maneja la máquina de estados
    while (1) {
        me();
    }

    return 0;
}

void me() {
    switch (actual_me) {
    case estado_inicio:
        out = 0b00101001;
        PORTB = out;
        next_me = vehiculos;
        break;

    case vehiculos:
        if (change_state) {
            out = 0b00101001;
            PORTB = out;
            next_me = peatones;
            start_peatones_timer();
        } else {
            out = 0b00101001;
            PORTB = out;
            change_state = true;
            next_me = peatones;
            start_peatones_timer();
        }
        break;

    case peatones:
        out = 0b01010100;
        PORTB = out;
        break;
    }
}

// Rutina de interrupción para el temporizador TIMER1
ISR(TIMER1_COMPA_vect) {
    // Incrementar el contador de tiempo de peatones en segundos
    peatones_timer++;

    // Comprobar si ha transcurrido 
    if (peatones_timer >= 7) {
        // Reiniciar el contador de tiempo de peatones
        peatones_timer = 0;
        // Cambiar al estado de vehículos
        actual_me = vehiculos;
    }
}

// Rutinas de interrupción para las entradas (INT0 y INT1)
ISR(INT0_vect) {
    actual_me = next_me;
}

ISR(INT1_vect) {
    actual_me = next_me;
}

void start_peatones_timer() {
    // Reiniciar el contador de tiempo de peatones
    peatones_timer = 0;
}
