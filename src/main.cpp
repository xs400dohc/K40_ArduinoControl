/*  Programm zur Kontrolle der Kühlung eines K40 CO²-Lasers. 
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
*/
#include <Arduino.h>
#define PeltierPWM1 9
#define PeltierPWM2 10
#define PeltierPWM3 11
#define BacklightPWM 5
#define FanPWM 6

void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
}