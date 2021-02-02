/*  
Programm zur Kontrolle der Kühlung eines K40 CO²-Lasers. 
  Die Kühlung besteht aus zwei jeweils geschlossenen Wasserkreisläufen.
  Der erste Kreislauf beinhaltet in Flussrichtung: 
    - Wasserpumpe mit Ausgleichsbehälter
    - Alu-Wärmetauscher
    - CO²-Laserröhre
    - Durchflussanzeiger mit 10k-NTC-Thermometer
  Der zweite Kreislauf beinhaltet:
    - Wasserpumpe mit Ausgleichsbehälter
    - Alu-Wärmetauscher
    - Durchflussanzeiger mit 10k-NTC-Thermometer
    - Luft-Wasser-Wärmetauscher einer PC-Wasser-Kühlung mit 2 12V-Lüftern.
  Zwischen den beiden Alu-Wärmetauschern befinden sich 3 Stück TEC12706 Peltierelemente.
  Die TEC werden über Mosfet IRFZ44N angesteuert.
  PWM des Arduino muss für die TEC angepasst werden, da TEC langsames PWM nicht mögen. -> https://www.frickelpiet.de/arduino/flaschenkuehler
    Siehe hierzu auch: https://arduino-projekte.webnode.at/registerprogrammierung/fast-pwm/
  Die TEC und die Lüfter werden über PID geregelt.

Hardware:
  T_LW 10K NTC
    1 ->  GND
    2 ----->  A0
        '-->  R 10k ->  5V

  T_CW 10K NTC
    1 ->  GND
    2 ----->  A0
        '-->  R 10k ->  5V

  T_ENV 10K NTC
    1 ->  GND
    2 ----->  A0
        '-->  R 10k ->  5V

  Display 1602 mit I2C-Adapter
    Anschlüsse:
      GND ->  GND
      VCC ->  5V
      SDA ->  20
      SCL ->  21
      LED ->  D2
      POWER -> NC (NotConnected / NichtVerbunden / offen)

  Lüfter
    rot --------  12V
          |
        1N4148
          |
    schw--------- Source  |
        GND ----- Drain   |
               |          |
             R 15k        |IRFZ44N
               |          |
        D6-R 1k----- Gate |

  Peltier 1 bis 3
    rot --------  12V
          |
        FR107
          |
    schw--------- Source  |
        GND ----- Drain   |
               |          |
             R 15k        |IRFZ44N
               |          |
        Dx-R 1k----- Gate |   x = 4, 5, 6




*/
#include <Arduino.h>
#include <PWM.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define Pin_Peltier1 4     //PWM-Pin für Peltier1
#define Pin_Peltier2 5     //PWM-Pin für Peltier2
#define Pin_Peltier3 6     //PWM-Pin für Peltier3
#define Pin_Backlight 2    //PWM-Pin für Hintergrundbeleuchtung Display
#define Pin_Fan 3          //PWM-Pin für Lüfter 
#define Pin_T_LW A0        //Temperatur LaserWasser
#define Pin_T_CW A1        //Temperatur Kühlwasser
#define Pin_T_ENV A2       //Temperatur Umgebung

float Set_LW = 15.0;
float Set_CW = 20.0;
float Offset_CW = 0.5;      //offset zum Herunterkühlen des Wassers(T_ENV nicht erreichbar)
float Kp_LW, Ki_LW, Kd_LW;  //PID-Werte LaserWasser
float KP_CW, Ki_CW, Kd_CW;  //PID-Werte Kühlwasser
unsigned int Messintervall = 2;      //Intervall, in dem neue Messwerte erfasst werden in Sekunden
unsigned long Messtimer = 0;
float T_LW = 0;
float T_CW = 0;
float T_ENV = 0;

LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3,POSITIVE);   

float Temp(int Bitwert)     //Berechnen der Temperatur aus den Analogwerten der NTCs
{
  float R_NTC, T;
  float KelvinGrad = 273.15;  // °C in Kelvin
  float Tn = 25;              // Nenntemperatur in °C
  long Widerstand = 10000;    // Widerstand Spannungsteiler
  int BWert = 3950;           //B-Wert NTC
  R_NTC = Widerstand * (((double) Bitwert / 1024 ) / ( 1 - ((double) Bitwert / 1024 )));
  T = (1 / (( 1 / ( KelvinGrad + Tn ))+((double) 1 / BWert ) * log( (double) R_NTC / Widerstand )))-KelvinGrad;
  return T;
}

void setup() {
  Serial.begin(250000);
  pinMode(SS, OUTPUT);
  pinMode(Pin_Peltier1, OUTPUT);
  pinMode(Pin_Peltier2, OUTPUT);
  pinMode(Pin_Peltier3, OUTPUT);
  pinMode(Pin_Backlight, OUTPUT);
  pinMode(Pin_Fan, OUTPUT);
  pinMode(Pin_T_LW, INPUT);
  pinMode(Pin_T_CW, INPUT);
  pinMode(Pin_T_ENV, INPUT);
  lcd.begin(/*Spalten*/16,/*Zeilen*/2);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Initialisierung");
}

void loop() {
  if(millis() - Messtimer >= Messintervall * 1000)
  {
     T_LW = Temp(analogRead(Pin_T_LW));
     T_CW = Temp(analogRead(Pin_T_CW));
     T_ENV = Temp(analogRead(Pin_T_ENV));
  }
  //PID_AUTOTUNE
  //T_LW und T_CW messen im sekunentakt
  //PID_LW anwenden auf Peltier Soll = 15°C
  //PID_CW anwenden auf lüfter  Soll = T_ENV + Offset_CW
  //Anzeige T_LW, T_CW, P
}