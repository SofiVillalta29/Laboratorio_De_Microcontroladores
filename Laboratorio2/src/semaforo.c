#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#define LDPV    PINB0
#define LDVD    PINB1
#define LDPP    PINB2
#define LDPD    PINB3
#define B1      PIND3
#define B2      PIND2

// Definiciones de estados y tiempos
#define T_Vehiculos 5
#define T_Peatones 6


void setup() {
    TCCR0A = 0x00; //Modo normal
    TCCR0B = 0x00; 
    TCCR0B |= (1 << CS00) | (1 << CS02); //Prescaling a 1024 
    sei();// Interrupción global
    TCNT0 = 0;
    TIMSK |= (1 << TOIE0); //Habilitando la interrupción del timer0 
}
void interruptor_externo()  {
    // Configuración de interrupciones externas para botones.
    DDRB |= (1 << LDPV) | (1 << LDVD) | (1 << LDPP) | (1 << LDPD);
    GIMSK |= (1 << INT0); // Habilitar interrupción externa para B1.
    MCUCR |= (1 << ISC01)|(1 << ISC11); // Configurar interrupción en flanco de bajada.
  

}

//Esta funcion se encarga de llamar a las funciones que permiten configurar las interrupciones usadas en el laboratorio
void Setup_INTERRUPCIONES(){
    interruptor_externo();
    setup();
}

typedef enum {
    LDPV_STATE,
    LDVD_STATE,
    LDPP_STATE,
    LDPD_STATE
} estados_t;

estados_t estado_actual = LDPV_STATE;
int contador = 0;
bool boton_presionado = false;

void cambiarEstado(estados_t nuevo_estado) {
    estado_actual = nuevo_estado;
    contador = 0;
    boton_presionado = false;
}

ISR(TIMER0_OVF_vect) {
    contador++;
    switch (estado_actual) {
        case LDPV_STATE:
            if (contador >= 10 || boton_presionado) {
                cambiarEstado(LDVD_STATE);
            }
            break;
        case LDVD_STATE:
            if (contador >= 1) {
                cambiarEstado(LDPP_STATE);
            }
            break;
        case LDPP_STATE:
            if (contador >= 1) {
                cambiarEstado(LDPD_STATE);
            }
            break;
        case LDPD_STATE:
            if (contador >= 10) {
                cambiarEstado(LDPV_STATE);
            }
            break;
    }
}

ISR(INT0_vect) {
    boton_presionado = true;
}

ISR(INT1_vect) {
    boton_presionado = true;
}

void Semaforos_FMS() {
    switch (estado_actual) {
        case LDPV_STATE:
            PORTB = (0 << PB3) | (1 << PB2) | (1 << PB1) | (0 << PB0);
            break;
        case LDVD_STATE:
            PORTB = (1 << PB1) | (1 << PB3);
            break;
        case LDPP_STATE:
            if (estado_actual == T_Vehiculos) {
                if (contador == 30 || contador == 90 || contador == 150)
                    PORTB = (0 << PB3) | (0 << PB2) | (1 << PB1) | (0 << PB0);
                else if (contador == 60 || contador == 120)
                    PORTB = (0 << PB3) | (1 << PB2) | (1 << PB1) | (0 << PB0);
            } else if (estado_actual == T_Peatones) {
                if (contador == 30 || contador == 90 || contador == 150)
                    PORTB = (1 << PB3) | (0 << PB2) | (0 << PB1) | (0 << PB0);
                else if (contador == 60 || contador == 120)
                    PORTB = (1 << PB3) | (0 << PB2) | (0 << PB1) | (1 << PB0);
            }
            break;
        case LDPD_STATE:
            PORTB = (1 << PB1) | (1 << PB3);
            break;
    }
}
int main(void) {
    PORTB &= (0<<PB0) | (0<<PB1)| (0<<PB2)|(0<<PB3); // Se entablecen los pines B0,B1,B2 y B3 como salidas
    boton_presionado = 0; //Se inicializa el valor del pulso de los botones en 0
  Setup_INTERRUPCIONES();
  while (1) {
    Semaforos_FMS(); //Maquina de estados de los semaforos empieza a funcionar
  }
  return 0;
}
