/*CODIGO PRINCIPAL DEL PROYECTO DE PERCEPCION */

#include <Servo.h>
#include <LEANTEC_ControlMotor.h>

double inputs[2];
int reglas=9;
double entradas = 2;//NTI=NTV-1;

int tabla_reglas[9][4]=    {{1,	1,	1,	1},
                            {1,	2,	2,	2},
                            {1,	3,	2,	3},
                            {2,	1,	1,	1},
                            {2,	2,	2,	2},
                            {2,	3,	3,	3},
                            {3,	1,	2,	1},
                            {3,	2,	3,	2},
                            {3,	3,	3,	3}};
int trigger=3;
int echo=2;
Servo servito; //declara servo
long distanciaOb;
int direccionOb;
long distanciaNew;
long avanceOb;
int giroOb;
double giroAuto,velAuto;


void setup(){
servito.attach(10); //definir puerto para servo
pinMode(trigger,OUTPUT);
pinMode(echo,INPUT);
}


void loop(){
 distanciaOb=medicionSensor(); //obtiene dato del sensor
 int theta=deteccion(distanciaOb); //obtiene posicion del servo si hay objeto a 5 cm de distancia
 if (theta > 90){direccionOb=servito.read()-90;} //ajusta con respecto al centro
 else if (theta <90){direccionOb=90-servito.read();}
 else if(theta ==90){direccionOb=theta;}
 /*se realiza nueva medicion para saber si el objeto cambió su posición*/
 distanciaNew=medicionSensor(); //nueva medicion
 dirNew=deteccion(distanciaNueva); //nueva deteccion
 /*NUEVAS VARIABLES*/
 giroOb=dirNew-direccionOb; //objeto se desplaza a un lado
 avanceOb=distanciaNew-distanciaOb; //objeto avanza
 /******************************/
   inputs[0]=double(giroOb); //CONVERSION y guardado en array PARA SISTEMA DIFUSO
   inputs[1]=double(avanceOb);
      
   giroAuto= (((FuzzySysY1(inputs,tabla_reglas))-(-100))*(1-(-1))/(100-(-100)))-1; //desnormalizando salidas
   velAuto = (((FuzzySySY2()-(-254))*(1-(-1))/(254-(-254)))-1;;
   
   control.Motor(velAuto,giroAuto); //SALIDA FINAL A LOS MOTORES
 
}
           

int deteccion(long x){
  int theta; //desplazamiento rotacional del servo
   for(int i=0;i<=180;i++){
    servito.write(i);
    if(x>=4 && x<=6){theta=servito.read();} //si encuentra algo entre 4 y 6, el objeto se toma como detectado
    return theta;
    }
    delay(100);
    for(int j=180;jj>=0;j--){
    servito.write(j);
    if(x>=4 && x<=6){theta=servito.read();} //si encuentra algo entre 4 y 6, el objeto se toma como detectado
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
  /*------------------*/
  t=pulseIn(echo,HIGH); //mide el tiempo que tarda en salir y regresar el pulso sonico
 //calculo de la distancia: velocidad= 2*distancia/tiempo ya que recorre la distancia que sale y en la que regresa
  d=t/59; (velocidad del sonido 340 m/s se convierte a cm/s)
  return d; //salida final del sensor
}
              
 double trapezoidmf(double x,double a,double b,double c,double d){ //FUNCION DE MEMBRESÍA TIPO TRAPEZOIDE
  double mf=max(min(min((x-a)/(b-a+0.000001),1),(d-x)/(d-c+0.0000001)),0.000000);
  return mf;
}
double trianglemf(double x,double a,double b,double c){ //FUNCION DE MEMBRESIA TIPO TRIANGULAR
  double mf=max(min((x-a)/(b-a+0.000001),(c-x)/(c-b+0.000001)),0.000000);
  return mf;
}
double Type1FS(double x,int n,double V[]){
  double a=V[0];
  double b=V[1];
  double c=V[2];
  double mf;
  if (n== 1){
      mf=trapezoidmf(x,a-1.0001,a,b,c);
    return mf;
    }
  if (n==2){
  mf=trianglemf(x,a,b,c);
  return mf;
   
    }
    if (n==3){
    mf=trapezoidmf(x,a,b,c,c+1);
    return mf;
    }
    if (n==0){
    mf=1;
    return mf;
    }
  }
  
double FuzzySysY1(double X[],int DB[][4])
{
    double PARAM[3][3]={{-1,-0.5,0}
                      ,{-0.5, 0, 0.5}
                      ,{0, 0.5, 1}};
                      
    double V[3];
    double AC[3]={0,0.5,1};
    double Fo[reglas];
    for(int r=0;r<=(reglas-1);r++){
      double sumin=1;
      int n;
      for(int i=0;i<=(entradas-1);i++){
            n=DB[r][i]-1;
            if(n>-1){
            V[0]=PARAM[n][0];
            V[1]=PARAM[n][1];
            V[2]=PARAM[n][2];
         }
          double mf=Type1FS(X[i],(n+1),V);
          sumin=min(sumin,mf);
       }
     Fo[r] = sumin;
   }
      double sum1=0;
      double sum2=0.00000001;
      double sum3=0;
      double sum4=0.00000001;
      
      for(int r=0;r<=(reglas);r++){
         sum1=(sum1+(Fo[r]*AC[DB[r][3]-1]));
         sum2=(sum2+Fo[r]);
         }
      double y1=sum1/sum2;
      for(int s=0;s<=(reglas);s++){
         sum3=(sum3+(Fo[s]*AC[DB[s][4]-1]));
         sum4=(sum4+Fo[s]);
         }
       double y2=sum3/sum4;
      FuzzySySY2(y2);
      return y1;
}
double FuzzySySY2(double y2){
 return y2;
}

 
              
