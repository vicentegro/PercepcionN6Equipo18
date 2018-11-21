/*CODIGO PRINCIPAL DEL PROYECTO DE PERCEPCION */

#include <Servo.h>

int trigger=3;
int echo=2;
Servo servito; //declara servo
long distanciaOb;
int direccionOb;
long distanciaNueva;
long avanceOb;
int giroOb;
int reglas={1, }


void setup(){
servito.attach(10); //definir puerto para servo
pinMode(trigger,OUTPUT);
pinMode(echo,INPUT);
}


void loop(){
 distanciaOb=medicionSensor();
 direccionOb=deteccion(distanciaOb);
 if (direccionOb > 90){direccionOb=servito.read()-90;}
 else if (direccionOb <90){direccionOb=90-servito.read();}
 distanciaNew=medicionSensor(); //nueva medicion
 dirNew=deteccion(distanciaNueva); //nueva deteccion
 giroOb=dirNew-direccionOb; //objeto se desplaza a un lado
 avanceOb=distanciaNew-distanciaOb; //objeto avanza
 
 
}



int deteccion(long x){
  int theta;
   for(int i=0;i<=180;i++){
    servito.write(i);
    if(x>=4 && x<=6){theta=servito.read();}
    return theta;
    }
    delay(100);
    for(int j=180;jj>=0;j--){
    servito.write(j);
    if(x>=4 && x<=6){theta=servito.read();}
    return theta;
    }
    delay(100);
  }

long medicionSensor(){
  long t; //timepo que demora en llegar el eco
  long d;  //distancia a la que está el objeto
//envio de pulso de 10us
  digitalWrite(trigger,HIGH);
  delayMicroseconds(10); 
  digitalWrite(trigger,LOW);
  
  t=pulseIn(echo,HIGH); //mide el tiempo que tarda en salir y regresar el pulso sonico
 
 //calculo de la distancia: velocidad= 2*distancia/tiempo ya que recorre la distancia que sale y en la que regresa
  d=t/59; (velocidad del sonido 340 m/s se convierte a cm/s)
  
  return d;
}
