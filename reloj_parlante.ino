//librerias importadas
#include <SPI.h>
#include <Wire.h>
#include "RTClib.h"                     //libreria para el reloj
#include "Talkie.h"                    //libreria para la voz
#include "Vocab_US_Large.h"           //libreria de parabras
#include "Vocab_US_TI99.h"             //libreria de palabras
#include "BluetoothSerial.h"            //libreria de bluetooth
#include "DHT.h"                        //libreria del sensor de temperatura

//pin de temperatura

#define DHTPIN 27
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

//definicion de la variable del reloj

RTC_DS1307 rtc;

//variable de la libreria de voz
Talkie voice;

//Pines sensor ultrasonico
int TRIG = 13;      // trigger en pin 13
int ECO = 12;      // echo en pin 12
int Vibracion = 14;      // Vibracion en pin 14
int DURACION;
int DISTANCIA;

//definicion de variables de temperatura y humedad
int new_t;
float t;
float h;

//definicion de conexion bluetooth
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabVibracion! Please run `make menuconfig` to and enable it
#endif

//definicion de serial bluetooth emisor
BluetoothSerial SerialBT;

void setup() {

  pinMode(TRIG, OUTPUT);   // trigger como salida
  pinMode(ECO, INPUT);    // echo como entrada
  pinMode(Vibracion, OUTPUT);   // Vibracion como salida

  Serial.begin(9600);
  SerialBT.begin("ESP32test"); //nombre de bluetooth 
  dht.begin();

  //SONIDO DE INICIO DE ESP
  voice.say(sp4_POWER);
  voice.say(sp4_WINDOWS);
 //PRINT RELOJ
 if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  //rtc.adjust(DateTime(2022,8,11,11,05,11)); //Configuración de hora, solo se configura una vez y se comenta 

}

void loop() {
  
  h = dht.readHumidity();//humedad
  t = dht.readTemperature();//temperatura
  if (isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");//si falla el sensor DHT
    return;
  }
  new_t=(int)t;//temperatura en entero
  // se manda por bluetooth la informacion de humedad y temperatura
  SerialBT.print("Temperatura: ");
  SerialBT.print(new_t);
  SerialBT.print(", Humedad: ");
  SerialBT.print(h);
    
  //DISTANCIA
  digitalWrite(TRIG, HIGH);     // generacion del pulso a enviar
  delay(1);       // al pin conectado al trigger
  digitalWrite(TRIG, LOW);    // del sensor
  
  DURACION = pulseIn(ECO, HIGH);  // con funcion pulseIn se espera un pulso
            // alto en Echo
  DISTANCIA = DURACION / 58.2;    // distancia medida en centimetros
  Serial.println(DISTANCIA);    // envio de valor de distancia por monitor serial
  Serial.println("cm");
  SerialBT.print(",Distancia:"); 
  SerialBT.print(DISTANCIA); 
  SerialBT.println("cm");
  delay(200);       // demora entre datos
  
//la app bluetooth retorna una letra por reconocimiento de voz
// si se dice Hola, manda una B y la esp dice Hola
    char X=SerialBT.read();
    if(X=='B'){
        voice.say(spt_HELLO);
        X='Z';
    }
//A partir de menos 50 cm de distancia comienza a bibrar de diferente forma
  if (DISTANCIA <= 50 && DISTANCIA>=31){ 
    digitalWrite(Vibracion, HIGH);      
    delay(DISTANCIA * 5);   
    digitalWrite(Vibracion, LOW);    
    }
   if (DISTANCIA <= 30 && DISTANCIA >= 16){ 
    digitalWrite(Vibracion, HIGH);    
    delay(DISTANCIA * 20);     
    digitalWrite(Vibracion, LOW);     
    }
    if (DISTANCIA <= 15 && DISTANCIA >= 5){ 
    digitalWrite(Vibracion, HIGH);      
    delay(DISTANCIA * 500);     
    digitalWrite(Vibracion, LOW);     
    }

  
 //la app bluetooth retorna una letra por reconocimiento de voz
// si se dice Dime la Hora, manda una A y la esp dice LA HORA
  
  if (DISTANCIA < 5||X=='A'){ 
        DateTime now = rtc.now();
        int v=0;
        int h=now.hour();
        int m=now.minute();
        int s=now.second();
        
        if (h>11){
          v=1;
        }
       if (h>12) {
        h=h-12;
       }
         
         Serial.print(h);
       
      
         Serial.print(":");
         Serial.print(m);
         Serial.print(":");
         Serial.print(s);
         Serial.println();
        
         
          voice.say(sp3_THE);
          voice.say(sp3_TIME);
          voice.say(sp3_IS);
      
         if (h==1){
          voice.say(sp3_ONE);
         }
         if (h==2){
          voice.say(sp3_TWO);
         }
         if (h==3){
          voice.say(sp3_THREE);
         }
         if (h==4){
          voice.say(sp3_FOUR);
         }
         if (h==5){
          voice.say(sp3_FIVE);
         }
         if (h==6){
          voice.say(sp3_SIX);
         }
         if (h==7){
          voice.say(sp3_SEVEN);
         }
         if (h==8){
          voice.say(sp3_EIGHT);
         }
         if (h==9){
          voice.say(sp3_NINE);
         }
         if (h==10){
          voice.say(sp3_TEN);
         }
         if (h==11){
          voice.say(sp3_ELEVEN);
         }
         if (h==12){
          voice.say(sp3_TWELVE);
         }
        if (m==0){
        voice.say (sp3_OCLOCK);
      }
      if (m==1){
       voice.say (sp3_ZERO);
       voice.say (sp3_ONE);
      }
      if (m==2){
        voice.say (sp3_ZERO);
        voice.say (sp3_TWO);
      }
      if (m==3){
        voice.say (sp3_ZERO);
        voice.say (sp3_THREE);
      }
      if (m==4){
        voice.say (sp3_ZERO);
        voice.say (sp3_FOUR);
      }
      if (m==5){
        voice.say (sp3_ZERO);
        voice.say (sp3_FIVE);
      }
      if (m==6){
        voice.say (sp3_ZERO);
        voice.say (sp3_SIX);
      }
      if (m==7){
        voice.say (sp3_ZERO);
        voice.say (sp3_SEVEN);
      }
      if (m==8){
        voice.say (sp3_ZERO);
        voice.say (sp3_EIGHT);
      }
      if (m==9){
        voice.say (sp3_ZERO);
        voice.say (sp3_NINE);
      }
      if (m==10){
       
        voice.say (sp3_TEN);
      }
      if (m==11){
       
        voice.say (sp3_ELEVEN);
      }
      if (m==12){
       
        voice.say (sp3_TWELVE);
      }
      if (m==13){
       
        voice.say (sp3_THIRTEEN);
      }
      if (m==14){
       
        voice.say (sp3_FOURTEEN);
      }
      if (m==15){
       
        voice.say (sp3_FIFTEEN);
      }
      if (m==16){
       
        voice.say (sp3_SIXTEEN);
      }
      if (m==17){
       
        voice.say (sp3_SEVENTEEN);
      }
      if (m==18){
       
        voice.say (sp3_EIGHTEEN);
      }
      if (m==19){
       
        voice.say (sp3_NINETEEN);
      }
      if (m==20){
       
        voice.say (sp3_TWENTY);
      }
      
      if (m==21){
         voice.say (sp3_TWENTY);
         voice.say (sp3_ONE);
      }
      if (m==22){
         voice.say (sp3_TWENTY);
         voice.say (sp3_TWO);
      }
      if (m==23){
         voice.say (sp3_TWENTY);
         voice.say (sp3_THREE);
      }
      if (m==24){
         voice.say (sp3_TWENTY);
         voice.say (sp3_FOUR);
      }
      if (m==25){
         voice.say (sp3_TWENTY);
         voice.say (sp3_FIVE);
      }
      if (m==26){
         voice.say (sp3_TWENTY);
         voice.say (sp3_SIX);
      }
      if (m==27){
         voice.say (sp3_TWENTY);
         voice.say (sp3_SEVEN);
      }
      if (m==28){
         voice.say (sp3_TWENTY);
         voice.say (sp3_EIGHT);
      }
      if (m==29){
         voice.say (sp3_TWENTY);
         voice.say (sp3_NINE);
      }
      if (m==30){
       
        voice.say (sp3_THIRTY);
      }
      
      if (m==31){
         voice.say (sp3_THIRTY);
         voice.say (sp3_ONE);
      }
      if (m==32){
         voice.say (sp3_THIRTY);
         voice.say (sp3_TWO);
      }
      if (m==33){
         voice.say (sp3_THIRTY);
         voice.say (sp3_THREE);
      }
      if (m==34){
         voice.say (sp3_THIRTY);
         voice.say (sp3_FOUR);
      }
      if (m==35){
         voice.say (sp3_THIRTY);
         voice.say (sp3_FIVE);
      }
      if (m==36){
         voice.say (sp3_THIRTY);
         voice.say (sp3_SIX);
      }
      if (m==37){
         voice.say (sp3_THIRTY);
         voice.say (sp3_SEVEN);
      }
      if (m==38){
         voice.say (sp3_THIRTY);
         voice.say (sp3_EIGHT);
      }
      if (m==39){
         voice.say (sp3_THIRTY);
         voice.say (sp3_NINE);
      }
      if (m==40){
       
        voice.say (sp3_FOURTY);
      }
      
      if (m==41){
         voice.say (sp3_FOURTY);
         voice.say (sp3_ONE);
      }
      if (m==42){
         voice.say (sp3_FOURTY);
         voice.say (sp3_TWO);
      }
      if (m==43){
         voice.say (sp3_FOURTY);
         voice.say (sp3_THREE);
      }
      if (m==44){
         voice.say (sp3_FOURTY);
         voice.say (sp3_FOUR);
      }
      if (m==45){
         voice.say (sp3_FOURTY);
         voice.say (sp3_FIVE);
      }
      if (m==46){
         voice.say (sp3_FOURTY);
         voice.say (sp3_SIX);
      }
      if (m==47){
         voice.say (sp3_FOURTY);
         voice.say (sp3_SEVEN);
      }
      if (m==48){
         voice.say (sp3_FOURTY);
         voice.say (sp3_EIGHT);
      }
      if (m==49){
       voice.say (sp3_FOURTY);
         voice.say (sp3_NINE);
      }
      if (m==50){
       
        voice.say (sp3_FIFTY);
      }
      
      if (m==51){
         voice.say (sp3_FIFTY);
         voice.say (sp3_ONE);
      }
      if (m==52){
         voice.say (sp3_FIFTY);
         voice.say (sp3_TWO);
      }
      if (m==53){
         voice.say (sp3_FIFTY);
         voice.say (sp3_THREE);
      }
      if (m==54){
         voice.say (sp3_FIFTY);
         voice.say (sp3_FOUR);
      }
      if (m==55){
         voice.say (sp3_FIFTY);
         voice.say (sp3_FIVE);
      }
      if (m==56){
         voice.say (sp3_FIFTY);
         voice.say (sp3_SIX);
      }
      if (m==57){
         voice.say (sp3_FIFTY);
         voice.say (sp3_SEVEN);
      }
      if (m==58){
         voice.say (sp3_FIFTY);
         voice.say (sp3_EIGHT);
      }
      if (m==59){
         voice.say (sp3_FIFTY);
         voice.say (sp3_NINE);
      }
      if (v==0){ 
        voice.say (sp3_A_M);  
      }
      if (v==1){
        voice.say(sp3_P_M);   
      }
      X='Z';
      delay (2000);
  }
  
  //la app bluetooth retorna una letra por reconocimiento de voz
// si se dice Como esta la temperatura, manda una C y la esp dice la temperatura
  
  if(X=='C'){
     voice.say(sp3_THE);
     voice.say(sp3_TEMPERATURE);
     voice.say(sp3_IS);
     
      if (new_t==1){
       voice.say (sp3_ZERO);
       voice.say (sp3_ONE);
      }
      if (new_t==2){
        voice.say (sp3_ZERO);
        voice.say (sp3_TWO);
      }
      if (new_t==3){
        voice.say (sp3_ZERO);
        voice.say (sp3_THREE);
      }
      if (new_t==4){
        voice.say (sp3_ZERO);
        voice.say (sp3_FOUR);
      }
      if (new_t==5){
        voice.say (sp3_ZERO);
        voice.say (sp3_FIVE);
      }
      if (new_t==6){
        voice.say (sp3_ZERO);
        voice.say (sp3_SIX);
      }
      if (new_t==7){
        voice.say (sp3_ZERO);
        voice.say (sp3_SEVEN);
      }
      if (new_t==8){
        voice.say (sp3_ZERO);
        voice.say (sp3_EIGHT);
      }
      if (new_t==9){
        voice.say (sp3_ZERO);
        voice.say (sp3_NINE);
      }
      if (new_t==10){
       
        voice.say (sp3_TEN);
      }
      if (new_t==11){
       
        voice.say (sp3_ELEVEN);
      }
      if (new_t==12){
       
        voice.say (sp3_TWELVE);
      }
      if (new_t==13){
       
        voice.say (sp3_THIRTEEN);
      }
      if (new_t==14){
       
        voice.say (sp3_FOURTEEN);
      }
      if (new_t==15){
       
        voice.say (sp3_FIFTEEN);
      }
      if (new_t==16){
       
        voice.say (sp3_SIXTEEN);
      }
      if (new_t==17){
       
        voice.say (sp3_SEVENTEEN);
      }
      if (new_t==18){
       
        voice.say (sp3_EIGHTEEN);
      }
      if (new_t==19){
       
        voice.say (sp3_NINETEEN);
      }
      if (new_t==20){
       
        voice.say (sp3_TWENTY);
      }
      
      if (new_t==21){
         voice.say (sp3_TWENTY);
         voice.say (sp3_ONE);
      }
      if (new_t==22){
         voice.say (sp3_TWENTY);
         voice.say (sp3_TWO);
      }
      if (new_t==23){
         voice.say (sp3_TWENTY);
         voice.say (sp3_THREE);
      }
      if (new_t==24){
         voice.say (sp3_TWENTY);
         voice.say (sp3_FOUR);
      }
      if (new_t==25){
         voice.say (sp3_TWENTY);
         voice.say (sp3_FIVE);
      }
      if (new_t==26){
         voice.say (sp3_TWENTY);
         voice.say (sp3_SIX);
      }
      if (new_t==27){
         voice.say (sp3_TWENTY);
         voice.say (sp3_SEVEN);
      }
      if (new_t==28){
         voice.say (sp3_TWENTY);
         voice.say (sp3_EIGHT);
      }
      if (new_t==29){
         voice.say (sp3_TWENTY);
         voice.say (sp3_NINE);
      }
      if (new_t==30){
       
        voice.say (sp3_THIRTY);
      }
      
      if (new_t==31){
         voice.say (sp3_THIRTY);
         voice.say (sp3_ONE);
      }
      if (new_t==32){
         voice.say (sp3_THIRTY);
         voice.say (sp3_TWO);
      }
      if (new_t==33){
         voice.say (sp3_THIRTY);
         voice.say (sp3_THREE);
      }
      if (new_t==34){
         voice.say (sp3_THIRTY);
         voice.say (sp3_FOUR);
      }
      if (new_t==35){
         voice.say (sp3_THIRTY);
         voice.say (sp3_FIVE);
      }
      if (new_t==36){
         voice.say (sp3_THIRTY);
         voice.say (sp3_SIX);
      }
      if (new_t==37){
         voice.say (sp3_THIRTY);
         voice.say (sp3_SEVEN);
      }
      if (new_t==38){
         voice.say (sp3_THIRTY);
         voice.say (sp3_EIGHT);
      }
      if (new_t==39){
         voice.say (sp3_THIRTY);
         voice.say (sp3_NINE);
      }
      if (new_t==40){
       
        voice.say (sp3_FOURTY);
      }
      
      if (new_t==41){
         voice.say (sp3_FOURTY);
         voice.say (sp3_ONE);
      }
      if (new_t==42){
         voice.say (sp3_FOURTY);
         voice.say (sp3_TWO);
      }
      if (new_t==43){
         voice.say (sp3_FOURTY);
         voice.say (sp3_THREE);
      }
      if (new_t==44){
         voice.say (sp3_FOURTY);
         voice.say (sp3_FOUR);
      }
      if (new_t==45){
         voice.say (sp3_FOURTY);
         voice.say (sp3_FIVE);
      }
      if (new_t==46){
         voice.say (sp3_FOURTY);
         voice.say (sp3_SIX);
      }
      if (new_t==47){
         voice.say (sp3_FOURTY);
         voice.say (sp3_SEVEN);
      }
      if (new_t==48){
         voice.say (sp3_FOURTY);
         voice.say (sp3_EIGHT);
      }
      if (new_t==49){
       voice.say (sp3_FOURTY);
         voice.say (sp3_NINE);
      }
      if (new_t==50){
       
        voice.say (sp3_FIFTY);
      }
      voice.say(sp3_DEGREES);
      voice.say(sp3_CELCIUS);
       X='Z';
    }
  delay (1000);
} 
