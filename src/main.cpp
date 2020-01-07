#include <mbed.h>
#include "output.h"

Serial serial1(USBTX, USBRX);
AnalogIn joystickY(A0);
AnalogIn joystickX(A1);
DigitalIn button(A2);
DigitalOut led_ratoliOJoystick(D5);
DigitalOut led_nord(D11);
DigitalOut led_sud(D10);
DigitalOut led_est(D9);
DigitalOut led_oest(D6);
int minX = 0;
int minY = 0;
int maxX = 0;
int maxY = 0;
double yCalib=0;
double xCalib=0;
char fourChars[4] = {'+','0','0','0'};


void setup() {
}

void setLEDValue(int nord, int sud, int est, int oest){
    led_nord.write(nord);
    led_sud.write(sud);
    led_est.write(est);
    led_oest.write(oest);
}

int estaAlCentre(double x, double y){
  if ((x>minX && x<maxX) && (y>minY && y<maxY)) {
    return 1;
  } else {
    return 0;
  }
}

int eixXDesplacat(double x) {
  if (x>minX && x<maxX) {
    return 0;
  } else {
    if (x>maxX) {
      return 1; //EST
    } else {
      return 2; //OEST
    }
  }
}

int eixYDesplacat(double y) {
  if (y>minY && y<maxY) {
    return 0;
  } else {
    if (y>maxY) {
      return 1; //NORD
    } else {
      return 2; //SUD
    }
  }
}

void calibracio() {
  int i = 0;
  wait(2);
  serial1.printf("S'esta calibrant el dispositiu, siusplau esperi... (%i/10)\n", i);
  while(i<10){
    yCalib += joystickY.read()*100;
    xCalib += joystickX.read()*100;
    i++;
    serial1.printf("S'esta calibrant el dispositiu, siusplau esperi... (%i/10)\n", i);
    wait_ms(200);
  }
  yCalib = yCalib/10;
  xCalib = xCalib/10;

  maxY = yCalib+3;
  minY = yCalib-3;
  maxX = xCalib+3;
  minX = xCalib-3;
}


int main() {
  // put your setup code here, to run once:

  calibracio();

  int xAnt = (int)xCalib;
  int yAnt = (int)yCalib;
  int valorConversioX = 0;
  int valorConversioY = 0;
  while(1) {

    if(button.read() == 0) {
      led_ratoliOJoystick.write(1);
      double y = joystickY.read()*100;
      yAnt = y;
      //serial1.printf("y: %lf", y);
      double x = joystickX.read()*100;
      xAnt = x;
      //serial1.printf("x: %lf", x);
      //serial1.printf("\n");
      //estem mirant els 4 punts cardinals sense borrar tots els leds, falta mirar 4 positions mixtes
      int valorX = eixXDesplacat(x);

      //Mirem si els dos eixos estan al centre.
      if(estaAlCentre(x,y)){
        setLEDValue(0,0,0,0);
        xAnt=(int)xCalib;
        yAnt=(int)yCalib;
      } else {
        //Mirem estat de l'eix x.
        if (valorX != 0) {
          if (valorX == 1) {
            setLEDValue(led_nord.read(),led_sud.read(),1,0);
          } else {
            setLEDValue(led_nord.read(),led_sud.read(),0,1);
          }
        } else {            
          setLEDValue(led_nord.read(),led_sud.read(),0,0);
        }

        int valorY = eixYDesplacat(y);
        //Mirem estat de l'eix y.
        if (valorY != 0) {
          if (valorY == 1) {
            setLEDValue(1,0,led_est.read(),led_oest.read());
          } else {
            setLEDValue(0,1,led_est.read(),led_oest.read());
          }
        } else {
          setLEDValue(0,0,led_est.read(),led_oest.read());
        }
      }
    } else {
      led_ratoliOJoystick.write(0);
      setLEDValue(0,0,0,0);
    }
    //Es capturarà la informació cada 0,2 segons
    wait_ms(200);
      valorConversioX = 0.0;
      valorConversioY = 0.0;

    if (xAnt > maxX){
      valorConversioX = (xAnt-(int)xCalib)*127/(100-(int)xCalib);
    } else if (xAnt < minX) {
      valorConversioX = xAnt*127/(int)xCalib;
      valorConversioX = -(128 - valorConversioX);
    } else {
      valorConversioX = xAnt*127/(int)xCalib;
      valorConversioX = -(127 - valorConversioX);
    }

    if (yAnt > maxY){
      valorConversioY = (yAnt-(int)yCalib)*127/(100-(int)yCalib);
    } else if (yAnt < minY) {
      valorConversioY = yAnt*127/(int)yCalib;
      valorConversioY = -(128 - valorConversioY);
    } else {
      valorConversioY = yAnt*127/(int)yCalib;
      valorConversioY = -(127 - valorConversioY);
    }

    Output stringGenerator((int)valorConversioX, (int)valorConversioY, (char)button);
    stringGenerator.printResult();
  }
}