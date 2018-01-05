#include <SoftwareSerial.h>

SoftwareSerial SerialBT(8, 9); // RX, TX
char ch = ' ';
boolean b_programacion = false;
boolean b_control = false;

const int S1 = 2;
const int S2 = 4;
const int S3 = 7;
const int S4 = 12;
const int Led_R = 6;
const int Led_G = 5;
const int Led_B = 3;

#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_G6  1568
#define NOTE_DS6 1245
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_E7  2637
#define NOTE_G7  3136
#define NOTE_D7  2349

#define Buzzer 11
int size = 0;
int song = 0;
String trama_array[13] = {"0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"};
int cont = 0;
String dato = "", dd = "";

int t_inicio[] = {
  NOTE_G6, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0,  0,

};
int tempo_inicio[] = {
  12, 12, 12, 60,
  12, 12, 12, 12,
};

int t_sacudida[] = {
  NOTE_C6, NOTE_CS6, NOTE_D6, NOTE_DS6,
  NOTE_D7, NOTE_B6, 0, 0
};

int tempo_sacudida[] = {
  60, 60, 60, 60,
  60, 60, 60, 60,
};
void setup() {
  Serial.begin(115200);
  SerialBT.begin(115200);
  delay(100);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(S4, OUTPUT);
  pinMode(Led_R, OUTPUT);
  pinMode(Led_G, OUTPUT);
  pinMode(Led_B, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  digitalWrite(S1, LOW);
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  digitalWrite(S4, LOW);
  digitalWrite(Led_R, LOW);
  digitalWrite(Led_G, LOW);
  digitalWrite(Led_B, LOW);
  digitalWrite(Buzzer, LOW);
  sing(0);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (SerialBT.available() > 0) {
    ch = SerialBT.read();
    Serial.println(ch);  // depuración.

    if (b_programacion) {
      programacion(ch);
    } else if (b_control) {
      control();
    }
    if (ch == '?') {
      delay(500);
      SerialBT.println("v1~");
      SerialBT.flush();
    } else if (ch == 'c') {
      SerialBT.flush();
      b_programacion = false;
      b_control = true;
    } else if (ch == 'p') {
      delay(500);
      SerialBT.flush();
      b_programacion = true;
      b_control = false;
    }
  }
}

void programacion(char o) {

  if (o == '~') {
    delay(500);

    for (int j = 0; j <= 12; j++) {
      actuar (trama_array[j]);
      /*
        Serial.print(j);
        Serial.print(" = ");
        Serial.println(trama_array[j]);*/
    }
    press_cero();
    for (int k = 0; k <= 13; k++) {
      trama_array[k] = "0";
    }
    dato = "";
    cont = 0;
  } else {
    delay(80);
    if (o == ',') {
      trama_array[cont] = dd;
      dd = "";
      cont++;
    } else {
      dd = dd + ch;
    }
  }
}

void control() {
  if (ch == '0') {
    press_cero();
  } else if (ch == '1') {
    press_1();
  } else if (ch == '2') {
    press_2();
  } else if (ch == '3') {
    press_3();
  } else if (ch == '4') {
    press_4();
  } else if (ch == '5') {
    press_5(0, 50, 50);
  } else if (ch == '6') {
    press_6();
  }
}

void actuar(String comando) {
  String adelante = "31", atras = "32", derecha = "33", izquierda = "34",g_de_90 = "35", g_iz_90 = "36",  g_180 = "37", g_360 = "38";
  if (adelante.equals(comando)) a_adelante();
  else if (atras.equals(comando)) a_atras();
  else if (derecha.equals(comando)) a_derecha();
  else if (izquierda.equals(comando)) a_izquierda();
  else if (g_iz_90.equals(comando)) a_g90_iz();
  else if (g_de_90.equals(comando)) a_g90_de();
  else if (g_180.equals(comando)) a_g180();
  else if (g_360.equals(comando)) a_g360();
}

void press_cero() {
  SerialBT.flush();
  digitalWrite(S1, LOW);
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  digitalWrite(S4, LOW);
  digitalWrite(Led_R, LOW);
  digitalWrite(Led_G, LOW);
  digitalWrite(Led_B, LOW);
  ch = ' ';
}
void press_1() {
  digitalWrite(S1, LOW);
  digitalWrite(S2, HIGH);
  digitalWrite(S3, LOW);
  digitalWrite(S4, HIGH);
  digitalWrite(Led_R, LOW);
  digitalWrite(Led_G, HIGH);
  //Serial.println(" DERECHA");
  ch = ' ';
}
void press_2() {
  digitalWrite(S1, HIGH);
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  digitalWrite(S4, LOW);
  digitalWrite(Led_R, HIGH);
  digitalWrite(Led_G, LOW);
  //Serial.println(" IZQUIERDA");
  ch = ' ';
}

void press_3() {

  digitalWrite(S1, HIGH);
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  digitalWrite(S4, HIGH);
  digitalWrite(Led_R, HIGH);
  digitalWrite(Led_G, LOW);
  //Serial.println(" ABAJO");
  ch = ' ';
}

void press_4() {

  digitalWrite(S1, LOW);
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  digitalWrite(S4, LOW);
  digitalWrite(Led_R, LOW);
  digitalWrite(Led_G, HIGH);
  //Serial.println(" ARRIBA");
  ch = ' ';
}

void press_5(int r, int g, int b) {
  digitalWrite(Led_R, r);
  digitalWrite(Led_G, g);
  digitalWrite(Led_B, b);
}
void press_6() {
  sing(1);
}

void sing(int s) {
  song = s;
  switch (song) {
    case 0:
      size = sizeof(t_inicio) / sizeof(int);
      for (int thisNote = 0; thisNote < size; thisNote++) {
        int noteDuration = 1000 / tempo_inicio[thisNote];
        buzz(Buzzer, t_inicio[thisNote], noteDuration);
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        buzz(Buzzer, 0, noteDuration);
      }
      break;
    case 1:
      size = sizeof(t_sacudida) / sizeof(int);
      for (int thisNote = 0; thisNote < size; thisNote++) {
        int noteDuration = 1000 / tempo_sacudida[thisNote];
        buzz(Buzzer, t_sacudida[thisNote], noteDuration);
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        buzz(Buzzer, 0, noteDuration);
      }
      break;
  }
}

void buzz(int targetPin, long frequency, long length) {
  digitalWrite(Led_G, HIGH);
  long delayValue = 1000000 / frequency / 2;
  long numCycles = frequency * length / 1000;
  for (long i = 0; i < numCycles; i++) { // for the calculated length of time...
    digitalWrite(targetPin, HIGH); // write the buzzer pin high to push out the diaphram
    delayMicroseconds(delayValue); // wait for the calculated delay value
    digitalWrite(targetPin, LOW); // write the buzzer pin low to pull back the diaphram
    delayMicroseconds(delayValue); // wait again or the calculated delay value
  }
  digitalWrite(Led_G, LOW);
}

void a_adelante() {
  digitalWrite(S1, LOW);
  digitalWrite(S2, HIGH);
  digitalWrite(S3, LOW);
  digitalWrite(S4, HIGH);
  digitalWrite(Led_R, LOW);
  digitalWrite(Led_G, HIGH);
  delay(1000);
  ch = ' ';
}

void a_atras() {
  digitalWrite(S1, HIGH);
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  digitalWrite(S4, LOW);
  digitalWrite(Led_R, HIGH);
  digitalWrite(Led_G, LOW);
  delay(1000);
  ch = ' ';
}

void a_derecha() {

  digitalWrite(S1, HIGH);
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  digitalWrite(S4, HIGH);
  digitalWrite(Led_R, HIGH);
  digitalWrite(Led_G, LOW);
  delay(1000);
  ch = ' ';
}

void a_izquierda() {
  digitalWrite(S1, LOW);
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  digitalWrite(S4, LOW);
  digitalWrite(Led_R, LOW);
  digitalWrite(Led_G, HIGH);
  delay(1000);
  ch = ' ';
}

void a_g90_de() {
  digitalWrite(S1, HIGH);
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  digitalWrite(S4, HIGH);
  digitalWrite(Led_R, HIGH);
  digitalWrite(Led_G, LOW);
  delay(500);
  ch = ' ';
}

void a_g90_iz() {
  digitalWrite(S1, LOW);
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  digitalWrite(S4, LOW);
  digitalWrite(Led_R, LOW);
  digitalWrite(Led_G, HIGH);
  delay(500);
  ch = ' ';

}

void a_g180() {
  digitalWrite(S1, HIGH);
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  digitalWrite(S4, HIGH);
  digitalWrite(Led_R, HIGH);
  digitalWrite(Led_G, LOW);
  delay(2000);
  ch = ' ';
}

void a_g360() {
  digitalWrite(S1, HIGH);
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  digitalWrite(S4, HIGH);
  digitalWrite(Led_R, HIGH);
  digitalWrite(Led_G, LOW);
  delay(4000);
  ch = ' ';
}


