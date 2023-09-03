#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

// Definicion de estados por utilizar

#define estado_inicio 1 // Estado inicial de la maquina de estados
#define transicion    2 // Estado de transicion
#define peatones      3 // Estado donde tiene el paso de peatones
#define vehiculos     4 // Estado para el paso de vehiculos

// Se declaran las variables
int actual_me;          // Estado actual de la me
int next_me;            // Proximo estado de la me
int out;                // Variable para controlar la salida
bool change_state= false; // Variable que indica si se debe de cambiar el estado

// Declaración de funciones por utilizar
void me();              // Funcion donde se establece la maquina de estados por manejar
void delay();           // Funcion que introduce retardos

// Funcion main
int main(void){
    // Se realiza una inicializacion de estados
    next_me   = estado_inicio; //Establece el estado de inicio
    actual_me = next_me;
    
    //Se configuran los registros del microcontrolador
    // Se inicializan en cero los siguientes:
    PORTB = 0b00000000; 
    PIND  = 0b00000000;
    PORTD = 0b00000000;
    
    // Se presenta la configuración del puerto B como salidas
    DDRB = 0b01111111;

    // Se configuran los pines de GPIOS dos y tres como entradas
    DDRD = 0b11110011;  
 
    // Configuración de las interrupciones:
    // Se habilitan las interrupciones INT0 y INT1
    GIMSK |= (1<<INT0);  
    GIMSK |= (1<<INT1);
    // Configuración de los flancos de disparo para las interrupciones
    MCUCR |= (1<<ISC00)|(1<<ISC01);     

    // Habilitar interrupciones globales
    sei();
    // Llamar a la función que maneja la máquina de estados
    while(1){
        me();
    }

    return 0;
}
 
void me(){
    // En esta seccion se usa un switch para considerar cada caso de la me
   switch(actual_me){ 

    case estado_inicio:
        out = 0b00000000;    // Configuración de la salida
        PORTB = out;        // Se establece la salida en el puerto B
        next_me = vehiculos;// Cambio al estado de permitir vehículos

        break;

    case transicion:
        out = 0b00101001;   // Configuración de la salida
        PORTB = out;       // Configuración de la salida
        break;  

    case peatones:
        out = 0b00101001;   // Configuración de la salida
        PORTB = out;        // Se establece la salida en el puerto B 
        break;

    case vehiculos: 
        if (change_state) // Si se debe cambiar de estado
        {
            next_me = transicion; // Cambio al estado de transición
            out = 0b00101001;     // Configuración de la salida
            PORTB = out;          // Se establece la salida en el puerto B
            break;
        }
        else{
            next_me = transicion;   // Cambio al estado de transición
            out = 0b00101001;       // Configuración de la salida
            PORTB = out;
      break; 
      }
  }

}
// Las rutinas de interrupcion:

// Actualización del estado actual con el próximo estado
ISR (INT0_vect){actual_me = next_me;}
// Actualización del estado actual con el próximo estado
ISR (INT1_vect){actual_me = next_me;}
 