#include <Servo.h>

// dichiarazione servo
Servo servoBase;
Servo servoGomito;
Servo servoPinza;

// dichiarazione pin joystick
const int pinX = A0;
const int pinY = A1;
const int pinSW = 7;   // pulsante joystick

// pin servo
const int pinServoBase = 9;
const int pinServoGomito = 10;
const int pinServoPinza = 11;

// angoli iniziali
int angoloBase = 90;
int angoloGomito = 180;
int angoloPinza = 90;

// led
const int ledVerde = 2;
const int ledRosso = 3;

// valvola (selettore modalità)
const int valvola = 6;

// stato pinza
bool pinzaAperta = true;
bool statoPrecedente = HIGH;   // con INPUT_PULLUP: pulsante rilasciato = HIGH

void setup() {
  Serial.begin(9600);

  // attacchi servo
  servoBase.attach(pinServoBase);
  servoGomito.attach(pinServoGomito);
  servoPinza.attach(pinServoPinza);

  // pulsante chiusura pinza
  pinMode(pinSW, INPUT_PULLUP);

  // led
  pinMode(ledVerde, OUTPUT);
  pinMode(ledRosso, OUTPUT);

  // valvola
  pinMode(valvola, INPUT_PULLUP);

  // posizione iniziale
  servoBase.write(angoloBase);
  servoGomito.write(angoloGomito);
  servoPinza.write(angoloPinza);

  delay(1000);
  Serial.println("Pronto!");
}

void loop() {
  // valori joystick
  int valX = analogRead(pinX);
  int valY = analogRead(pinY);

  // stato valvola (LOW = manuale, HIGH = web)
  int statoValvola = digitalRead(valvola);

  // zona morta: +/- 50 attorno al centro (512)
  int deadZone = 50;

  if (statoValvola == LOW) {
    // modalità manuale
    //Serial.println("Sono nel loop manuale, puoi comandarmi!");
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledRosso, HIGH);

    // base
    if (abs(valX - 512) > deadZone) {
      angoloBase += map(valX, 0, 1023, -2, 2);
    }

    // gomito
    if (abs(valY - 512) > deadZone) {
      angoloGomito += map(valY, 0, 1023, -2, 2);
    }

    // limiti angoli
    angoloBase = constrain(angoloBase, 0, 180);
    angoloGomito = constrain(angoloGomito, 0, 180);

    // movimento servo
    servoBase.write(angoloBase);
    servoGomito.write(angoloGomito);

    // gestione pinza con pulsante
    bool statoAttuale = digitalRead(pinSW);

    // rileva transizione HIGH -> LOW (pulsante premuto)
    if (statoPrecedente == HIGH && statoAttuale == LOW) {
      pinzaAperta = !pinzaAperta;
      angoloPinza = pinzaAperta ? 90 : 30; // 90 = aperto, 30 = chiuso
      servoPinza.write(angoloPinza);
      delay(200); // debounce
    }

    // aggiorna stato
    statoPrecedente = statoAttuale;

  } else {
    // modalità pagina web
    //Serial.println("Sono nel loop pagina web!");
    digitalWrite(ledVerde, HIGH);
    digitalWrite(ledRosso, LOW);

    if (Serial.available()) {
      String data = Serial.readStringUntil('\n');
      Serial.println(data);

      int sep = data.indexOf(',');
      if (sep > 0) {
        int angolo1 = data.substring(0, sep).toInt();
        int angolo2 = data.substring(sep + 1).toInt();

        servoBase.write(angolo1);
        servoGomito.write(angolo2);

        Serial.print("Base: ");
        Serial.print(angolo1);
        Serial.print(" Gomito: ");
        Serial.println(angolo2);
      }
    }
  }

  delay(20);
}
