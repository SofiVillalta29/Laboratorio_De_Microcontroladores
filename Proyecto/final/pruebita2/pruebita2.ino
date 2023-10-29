#include <PDM.h>
#include <arduinoFFT.h>

#define SAMPLES 256
#define SAMPLING_FREQUENCY 16000

short sampleBuffer[SAMPLES];
volatile int samplesRead;
arduinoFFT FFT = arduinoFFT();

double vReal[SAMPLES];
double vImag[SAMPLES];

void setup() {
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);

  Serial.begin(9600);
  while (!Serial);

  PDM.onReceive(onPDMdata);
  if (!PDM.begin(1, SAMPLING_FREQUENCY)) {
    Serial.println("Failed to start PDM!");
    while (1);
  }
}

void checkFrequencyAndLightLED(double frequency) {
  double correctFrequency = 0.0;

  if (frequency > 70 && frequency < 90) {
    correctFrequency = 82.4;
  } else if (frequency > 100 && frequency < 120) {
    correctFrequency = 110;
  } else if (frequency > 135 && frequency < 155) {
    correctFrequency = 146.8;
  } else if (frequency > 186 && frequency < 205) {
    correctFrequency = 196;
  } else if (frequency > 235 && frequency < 255) {
    correctFrequency = 246.9;
  } else if (frequency > 320 && frequency < 340) {
    correctFrequency = 329.6;
  }

  // Imprime la frecuencia actual y la frecuencia corregida en el Serial Plotter
  Serial.print("Frecuencia Actual: ");
  Serial.print(frequency);
  Serial.print(" Hz, Frecuencia Corregida: ");
  Serial.print(correctFrequency);
  Serial.println(" Hz");

  // Imprime la frecuencia en el Serial Plotter
  Serial.println(frequency);


  if (correctFrequency == 0.0) {
    digitalWrite(LEDR, LOW);
    digitalWrite(LEDG, LOW);
    digitalWrite(LEDB, LOW);
  } else if (abs(frequency - correctFrequency) < 2) {  // Cambiado de 2 a 3
    // Nota afinada
    digitalWrite(LEDG, HIGH);
    digitalWrite(LEDR, LOW);
    digitalWrite(LEDB, LOW);
  } else {
    // Nota desafinada
    digitalWrite(LEDR, HIGH);
    digitalWrite(LEDG, LOW);
    digitalWrite(LEDB, LOW);
  }
}

void loop() {
  if (samplesRead) {
    for (int i = 0; i < samplesRead; i++) {
      vReal[i] = (double)sampleBuffer[i];
      vImag[i] = 0.0;
    }

    FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);

    double peakFrequency = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);

    checkFrequencyAndLightLED(peakFrequency);

    samplesRead = 0;
  }
  delay(1000);
}

void onPDMdata() {
  int bytesAvailable = PDM.available();
  PDM.read(sampleBuffer, bytesAvailable);
  samplesRead = bytesAvailable / 2;
}
