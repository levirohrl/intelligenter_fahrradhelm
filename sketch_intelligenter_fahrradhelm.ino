const int audio1Pin = 0;  // Mikrofon1 an Pin A0
const int motor1Pin = 3;  // Vibrationsmotor1 an Pin3
const int audio2Pin = 1;  // Mikrofon2 an Pin A1
const int motor2Pin = 5;  // Vibrationsmotor2 an Pin5
const int audio3Pin = 2;  // Mikrofon3 an Pin A2
const int motor3Pin = 6;  // Vibrationsmotor3 an Pin6
const int audio4Pin = 3;  // Mikrofon4 an Pin A3
const int motor4Pin = 9;  // Vibrationsmotor4 an Pin9

const int MOTOR_INTENSITY = 1024;  // Variable für die maximale Motorintensität
const int THRESHOLD_VOLTAGE= 2.5;  // Variable für den Schwellenwert

const int sampleWindow = 100; // Abtastfenster in Millisekunden (ms)
unsigned int sample1; // Aktueller Messwerte der Frequenzwellen
unsigned int sample2;
unsigned int sample3;
unsigned int sample4;

void setup() {  
  Serial.begin(115200);  // Baudrate wird definiert
  // Ausgänge werden definiert
  pinMode(motor1Pin, OUTPUT);  
  pinMode(motor2Pin, OUTPUT);
  pinMode(motor3Pin, OUTPUT);
  pinMode(motor4Pin, OUTPUT);
}

void loop() {  // Folgende Variablen werden jedes Mal, wenn die loop()-Schleife von vorne beginnt, neu deklariert
  unsigned long start = millis();  // Start des Abtastfensters
  unsigned int peakToPeak1 = 0;     // Deklarieren der Peak-to-peak Level
  unsigned int peakToPeak2 = 0;
  unsigned int peakToPeak3 = 0;
  unsigned int peakToPeak4 = 0;

  unsigned int signalMax1 = 0;
  unsigned int signalMin1 = 1024;
  unsigned int signalMax2 = 0;
  unsigned int signalMin2 = 1024;
  unsigned int signalMax3 = 0;
  unsigned int signalMin3 = 1024;
  unsigned int signalMax4 = 0;
  unsigned int signalMin4 = 1024;

  // Sammeln von Messwerten für n Millisekunden
  while (millis() - start < sampleWindow) {
    sample1 = analogRead(audio1Pin);
    if (sample1 < 1024)  // Das ist Maximum des 10-bit ADC. Deshalb enthält der loop-Teil alle Sensorabfragen
    {
      if (sample1 > signalMax1) {
        signalMax1 = sample1;  // Speichern des Höchstwerts
      } else if (sample1 < signalMin1) {
        signalMin1 = sample1;  // Speichern des Mindestwerts
      }
    }

    sample2 = analogRead(audio2Pin);
    if (sample2 < 1024)  // Das ist Maximum des 10-bit ADC. Deshalb enthält der loop-Teil alle Sensorabfragen
    {
      if (sample2 > signalMax2) {
        signalMax2 = sample2;  // Speichern des Höchstwerts
      } else if (sample2 < signalMin2) {
        signalMin2 = sample2;  // Speichern des Mindestwerts
      }
    }

    sample3 = analogRead(audio3Pin);
    if (sample3 < 1024)  // Das ist Maximum des 10-bit ADC. Deshalb enthält der loop-Teil alle Sensorabfragen
    {
      if (sample3 > signalMax3) {
        signalMax3 = sample3;  // Speichern des Höchstwerts
      } else if (sample3 < signalMin3) {
        signalMin3 = sample3;  // Speichern des Mindestwerts
      }
    }

    sample4 = analogRead(audio4Pin);
    if (sample4 < 1024)  // Das ist Maximum des 10-bit ADC. Deshalb enthält der loop-Teil alle Sensorabfragen
    {
      if (sample4 > signalMax4) {
        signalMax4 = sample4;  // Speichern des Höchstwerts
      } else if (sample4 < signalMin4) {
        signalMin4 = sample4;  // Speichern des Mindestwerts
      }
    }

  }
  peakToPeak1 = signalMax1 - signalMin1;        // Höchstwert - Mindestwert = Peak-to-peak-Amplitude
  double volts1 = (peakToPeak1 * 3.3) / 1024;  // Änderung zu Spannungswert
  peakToPeak2 = signalMax2 - signalMin2;        // Höchstwert - Mindestwert = Peak-to-peak-Amplitude
  double volts2 = (peakToPeak2 * 3.3) / 1024;  // Änderung zu Spannungswert
  peakToPeak3 = signalMax3 - signalMin3;        // Höchstwert - Mindestwert = Peak-to-peak-Amplitude
  double volts3 = (peakToPeak3 * 3.3) / 1024;  // Änderung zu Spannungswert
  peakToPeak4 = signalMax4 - signalMin4;        // Höchstwert - Mindestwert = Peak-to-peak-Amplitude
  double volts4 = (peakToPeak4 * 3.3) / 1024;  // Änderung zu Spannungswert

  // Folgender Bereich soll das durchgehende Vibrieren der Vibrationsmotoren vermeiden
  int motorPin = motor1Pin;
  double maximum_volts = volts1;

  if(volts2 > maximum_volts)
  {
    motorPin = motor2Pin;
    maximum_volts = volts2;
  }

  if(volts3 > maximum_volts)
  {
    motorPin = motor3Pin;
    maximum_volts = volts3;
  }

  if(volts4 > maximum_volts)
  {
    motorPin = motor4Pin;
    maximum_volts = volts4;
  }


  if(maximum_volts > THRESHOLD_VOLTAGE)
  {
    analogWrite(motorPin, MOTOR_INTENSITY * (maximum_volts / 3.3));
  }
  else
  {
    analogWrite(motorPin, 0);
  }
  // Ausgabe der Mikrofonmesswerte am seriellen Monitor
  Serial.println(" ");
  Serial.print("Mikrofon1: ");
  Serial.print(volts1);
  Serial.print(", Mikrofon2: ");
  Serial.print(volts2);
  Serial.print(", Mikrofon3: ");
  Serial.print(volts3);
  Serial.print(", Mikrofon4: ");
  Serial.print(volts4);
}