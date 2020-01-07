#include <string.h>
#include "output.h"
#include <mbed.h>

Serial serial(USBTX, USBRX);
Output::Output(int _valX, int _valY, char _boto){
  char x[10];
  char y[10];
  char xReal[10];
  char yReal[10];
  
  strcpy(resultat, "DATA,");
  int i=0;
  for(i=0; i<10;i++){
      x[i]='b';
      y[i]='b';
  }

  //int _valX = 5;
  //int _valY = -3;
  if(_valX > 0){
      sprintf(x, "%d", _valX);
      xReal[0]='+';
  }else{
      sprintf(x, "%d",_valX);
      xReal[0]='-';
  }

  if(_valY>0){
      sprintf(y, "%d", _valY);
      yReal[0]='+';
  }else{
      sprintf(y,"%d", _valY);
      yReal[0]='-';
  }

  i=0;
  int numReal=0;
  bool teSigneNegatiu = false;
  for(i=0;i<10;i++){

      if(x[i]!='b' && x[i]!='\0')
          numReal++;
      if(x[i] =='-')
          teSigneNegatiu=true;
  }

  if(numReal == 1){
      xReal[1]='0';
      xReal[2]='0';
      xReal[3]=x[0];
  }else if(numReal == 2 && teSigneNegatiu){
      xReal[1]='0';
      xReal[2]='0';
      xReal[3]=x[1];
  }else if (numReal == 2 && !teSigneNegatiu){
      xReal[1] ='0';
      xReal[2]= x[0];
      xReal[3]=x[1];
  }else if (numReal == 3 && !teSigneNegatiu){
      xReal[1]=x[0];
      xReal[2]=x[1];
      xReal[3]=x[2];
  }else if(numReal == 3 && teSigneNegatiu){
      xReal[1] ='0';
      xReal[2]= x[1];
      xReal[3]=x[2];
  }else if(numReal == 4){
      xReal[1] =x[1];
      xReal[2]= x[2];
      xReal[3]=x[3];
  }
  xReal[4]=',';
  xReal[5]='\0';


  i=0;
  numReal=0;
  teSigneNegatiu = false;
  for(i=0;i<10;i++){
      if(y[i]!='b' && y[i]!='\0')
          numReal++;
      if(y[i] =='-')
          teSigneNegatiu=true;
  }

  if(numReal == 1){
      yReal[1]='0';
      yReal[2]='0';
      yReal[3]=y[0];
  }else if(numReal == 2 && teSigneNegatiu){
      yReal[1]='0';
      yReal[2]='0';
      yReal[3]=y[1];
  }else if (numReal == 2 && !teSigneNegatiu){
      yReal[1] ='0';
      yReal[2]= y[0];
      yReal[3]=y[1];
  }else if (numReal == 3 && !teSigneNegatiu){
      yReal[1]=y[0];
      yReal[2]=y[1];
      yReal[3]=y[2];
  }else if(numReal == 3 && teSigneNegatiu){
      yReal[1] ='0';
      yReal[2]= y[1];
      yReal[3]=y[2];
  }else if(numReal == 4){
      yReal[1] =y[1];
      yReal[2]= y[2];
      yReal[3]=y[3];
  }
  yReal[4]=',';
  yReal[5]='\0';


  strcat(resultat,xReal);
  strcat(resultat,yReal);

  if(boto == 0){
      strcat(resultat,"0");
  }else if(boto==1){
      strcat(resultat, "1");
  }

  strcat(resultat, "X");
}

void Output::printResult(){
    char *resFinal = resultat;
    char value = *resFinal;
    while (value != 88){
        serial.printf("%c", value);
        resFinal++;
        value = *resFinal;
    }
    serial.printf("\n\n");
}