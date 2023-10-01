#include <Adafruit_PCD8544.h>

// Pines PCD8544
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

Adafruit_PCD8544 display = Adafruit_PCD8544(SCLK, DIN, DC, CS, RST);

void setup() {
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

  // Convertir a valor original
  if (voltage <= 0.21) voltage = 1;
  else if (voltage <= 0.42) voltage = 2;
  else if (voltage <= 0.62) voltage = 3;
  else if (voltage <= 0.83) voltage = 4;
  else if (voltage <= 1.04) voltage = 5;
  else if (voltage <= 1.25) voltage = 6;
  else if (voltage <= 1.46) voltage = 7;
  else if (voltage <= 1.67) voltage = 8;
  else if (voltage <= 1.87) voltage = 9;
  else if (voltage <= 2.08) voltage = 10;
  else if (voltage <= 2.29) voltage = 11;
  else if (voltage <= 2.5) voltage = 12;
  else if (voltage <= 2.71) voltage = 13;
  else if (voltage <= 2.99) voltage = 14;
  else if (voltage <= 3.12) voltage = 15;
  else if (voltage <= 3.33) voltage = 16;
  else if (voltage <= 3.54) voltage = 17;
  else if (voltage <= 3.75) voltage = 18;
  else if (voltage <= 3.96) voltage = 19;
  else if (voltage <= 4.17) voltage = 20;
  else if (voltage <= 4.37) voltage = 21;
  else if (voltage <= 4.58) voltage = 22;
  else if (voltage <= 4.79) voltage = 23;
  else if (voltage <= 5) voltage = 24;

    
  if (digitalRead(signPin) == LOW) {  // Si es negativo
    voltage *= -1;
  }

  return voltage;
}

void loop() {
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
  }
  
  displayVoltages(voltages);

  delay(1000); // Actualiza cada segundo
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
}
