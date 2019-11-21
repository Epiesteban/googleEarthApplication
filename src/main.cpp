#include <mbed.h>

Serial serial1(USBTX, USBRX);
AnalogIn joystickY(A0);
AnalogIn joystickX(A1);
AnalogIn button(A2);
DigitalOut led_nord(D11);
DigitalOut led_sud(D10);
DigitalOut led_est(D9);
DigitalOut led_oest(D6);
DigitalOut led_ratoliOJoystick(D5);
int minX = 72;
int minY = 73;
int maxX = 85;
int maxY = 82;

void setup() {
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

int main() {
  // put your setup code here, to run once:

  while(1) {
    double y = joystickY.read()*100;
    serial1.printf("y: %lf", y);
    double x = joystickX.read()*100;
    serial1.printf("x: %lf", x);
    serial1.printf("\n");
    wait_ms(200);
    //estem mirant els 4 punts cardinals sense borrar tots els leds, falta mirar 4 positions mixtes
    int valorX = eixXDesplacat(x);

    //Mirem si els dos eixos estan al centre.
    if(estaAlCentre(x,y)){
      led_nord.write(0);
      led_sud.write(0);
      led_est.write(0);
      led_oest.write(0);
    } else {
      //Mirem estat de l'eix x.
      if (valorX != 0) {
        if (valorX == 1) {
          led_est.write(1);
          led_oest.write(0);
        } else {
          led_est.write(0);
          led_oest.write(1);
        }
      } else {
        led_est.write(0);
        led_oest.write(0);
      }

      int valorY = eixYDesplacat(y);
      //Mirem estat de l'eix y.
      if (valorY != 0) {
        if (valorY == 1) {
          led_nord.write(1);
          led_sud.write(0);
        } else {
          led_nord.write(0);
          led_sud.write(1);
        }
      } else {
        led_nord.write(0);
        led_sud.write(0);
      }
    }
  }
}