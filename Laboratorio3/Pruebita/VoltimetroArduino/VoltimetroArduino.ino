#include <Adafruit_PCD8544.h>

#include <SPI.h>

// Conexiones del PCD8544
#define SCLK 13
#define DIN 11
#define DC 10
#define CS 9
#define RST 8

// Pines de verificación de signo
#define SIGN_CHANNEL1 7
#define SIGN_CHANNEL2 2
#define SIGN_CHANNEL3 A5
#define SIGN_CHANNEL4 A4

// Pines para LEDs de alarma y entradas analógicas
#define LED1 3
#define LED2 4
#define LED3 5
#define LED4 6
#define CHANNEL1 A3
#define CHANNEL2 A2
#define CHANNEL3 A1
#define CHANNEL4 A0

#define TRIGGER_PIN 12 // Pin de entrada a verificar (en este caso, pin 12)

Adafruit_PCD8544 display = Adafruit_PCD8544(SCLK, DIN, DC, CS, RST);

void setup() {

  Serial.begin(9600);
  pinMode(RST, OUTPUT);
  pinMode(CS, OUTPUT);
  pinMode(DC, OUTPUT);
  pinMode(DIN, OUTPUT);
  pinMode(SCLK, OUTPUT);


  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);

  pinMode(SIGN_CHANNEL1, INPUT);
  pinMode(SIGN_CHANNEL2, INPUT);
  pinMode(SIGN_CHANNEL3, INPUT);
  pinMode(SIGN_CHANNEL4, INPUT);

  display.begin();
  display.setContrast(50); 
  display.clearDisplay();


}

float readVoltage(int pin, int signPin) {
  float voltage = analogRead(pin) * (5.0 / 1023.0);

  // Aplicar la regla de tres
  float originalValue = (voltage * 24)/5;

  if (digitalRead(signPin) == LOW) {  // Si es negativo
    originalValue *= -1;
  }

  return originalValue;
}



void loop() {
  if (digitalRead(TRIGGER_PIN) == HIGH) {
    float voltages[4];
    voltages[0] = readVoltage(CHANNEL1, SIGN_CHANNEL1);
    voltages[1] = readVoltage(CHANNEL2, SIGN_CHANNEL2);
    voltages[2] = readVoltage(CHANNEL3, SIGN_CHANNEL3);
    voltages[3] = readVoltage(CHANNEL4, SIGN_CHANNEL4);

    for (int i = 0; i < 4; i++) {
      if (voltages[i] < -20 || voltages[i] > 20) {
        digitalWrite(LED1 + i, HIGH); // Enciende el LED de alarma
      } else {
        digitalWrite(LED1 + i, LOW); // Apaga el LED de alarma
      }
      displayVoltages(voltages);
    }

    // Impresión de los valores en el Monitor Serie solo cuando el pin 12 está en alto
    Serial.println("Interruptor cerraddo para transmision de datos");
    for (int i = 0; i < 4; i++) {
      Serial.print("Ch");
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(voltages[i]);
      Serial.println("V");
    }
  } else {
    // Si el pin 12 está en bajo, puedes hacer otras acciones o simplemente esperar
    // Borra la pantalla si se requiere
    display.clearDisplay();
    display.display();
    
    // Imprime un mensaje en el Monitor Serie indicando que el interruptor se ha abierto
    Serial.println("El interruptor se ha abierto.");
  }

  delay(1); // Espera un segundo antes de volver a verificar
}

void displayVoltages(float voltages[4]) {
  display.clearDisplay();
  for (int i = 0; i < 4; i++) {
    display.setCursor(0, i * 8);
    display.print("Ch");
    display.print(i + 1);
    display.print(": ");
    display.print(voltages[i]);
    display.print("V");
 
  }
  display.display(); 
  delay(1);
  display.clearDisplay();

}
