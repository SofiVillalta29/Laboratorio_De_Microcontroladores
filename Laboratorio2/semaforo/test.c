#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

// Definicion de estados por utilizar
#define estado_inicio 0
#define transicion    1
#define peatones      2
#define vehiculos     3

// Se declaran las variables
int actual_me;
int next_me;


// Declaración de funciones por utilizar
void me();
void delay();

// Funcion main
int main(void){

}

// En la siguiente funcion se busca implementar la máquina de estados
void me(){

}

// Funcion por implementar delay
void delay(){

}