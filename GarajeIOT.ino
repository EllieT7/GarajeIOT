// Importamos las librerías
#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include "SPIFFS.h"
#include "config.h"
#include "DHT.h"
 // Valores ajustados para el SG90
#define COUNT_LOW 1638
#define COUNT_HIGH 7000
#define TIMER_WIDTH 16
#define PWM1_Ch1    1

String pwmValue1;

// Contadores
int aux = 0; //Para saber si el modo automatico esta activo o no.
//Alarma fondo
const int Trigger = 15 ;   //Pin digital 2 para el Trigger del sensor15-17
const int Echo = 35;   //Pin digital 3 para el Echo del sensor35-19
int freq = 2000;
int channel = 0;
int resolution = 8;
//Luces exteriores
const int ledPIN = 2;
//Luces internas
#define LEDAzul_GPIO 32
#define LEDVerde_GPIO 33
#define LEDRojo_GPIO 25
#define PWM1_Ch    0
#define PWM1_Res   8
#define PWM1_Freq  1000
#define PWM2_Ch    1
#define PWM2_Res   8
#define PWM2_Freq  1000
#define PWM3_Ch    2
#define PWM3_Res   8
#define PWM3_Freq  1000
int pwm1,pwm2,pwm3;
//Puerta final
const int boton1 = 22;
const int boton2 = 23;
int ENA = 13;
int IN1 = 5;
int IN2 = 4;
// VARIABLES DE ESTADO DE BOTONES - puerta
int estado1;
int estado2;
int c=0;
int d=0;
const int frequency = 100;
const int pwm_channel = 0;
int resolution1 = 4;
//Temperatura
#define DHT_SENSOR_PIN  21 // ESP32 pin GIOP21 conectado al sensor DHT11
#define DHT_SENSOR_TYPE DHT11
float humi,tempC,tempF;
DHT dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);
//Ultrasonico
const int Trigger2 = 17 ;   //Pin digital 2 para el Trigger del sensor
const int Echo2 = 19;   //Pin digital 3 para el Echo del sensor


AsyncWebServer server(80);



void setup() {
  Serial.begin(115200);   
  //Inicializacion de pines
  ledcSetup(1, 50, TIMER_WIDTH); // canal 1, 50 Hz, 16-bit width
  ledcAttachPin(32, 1);   // GPIO 22 asignado al canal 1
  //Alarma fondo
  ledcSetup(channel, freq, resolution);
  ledcAttachPin(18, channel);
  pinMode(Trigger, OUTPUT); //pin como salida
  pinMode(Echo, INPUT);  //pin como entrada
  digitalWrite(Trigger, LOW);//Inicializamos el pin con 0
  //Luces exteriores
  pinMode(ledPIN, OUTPUT);
  //Luces internas
  Serial.println("Inicializando el Puerto Serial");
  pinMode(LEDAzul_GPIO, OUTPUT);
  pinMode(LEDVerde_GPIO, OUTPUT);
  pinMode(LEDRojo_GPIO, OUTPUT);

  ledcAttachPin(LEDAzul_GPIO, PWM1_Ch);
  ledcSetup(PWM1_Ch, PWM1_Freq, PWM1_Res);

  ledcAttachPin(LEDVerde_GPIO, PWM2_Ch);
  ledcSetup(PWM2_Ch, PWM2_Freq, PWM2_Res);

  ledcAttachPin(LEDRojo_GPIO, PWM3_Ch);
  ledcSetup(PWM3_Ch, PWM3_Freq, PWM3_Res);
  //Puerta final
  pinMode(boton1, INPUT);
  pinMode(boton1, INPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT); 
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  ledcSetup(pwm_channel, frequency, resolution1);
  ledcAttachPin(ENA, pwm_channel);

  //Temperatura
  dht_sensor.begin(); // inicializando el DHT sensor

  //Ultrasonico
  pinMode(Trigger2, OUTPUT); //pin como salida
  pinMode(Echo2, INPUT);  //pin como entrada
  digitalWrite(Trigger2, LOW);//Inicializamos el pin con 0

  // Iniciamos el servidor web
  initConfig();
  
  // Rutas
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html",String(), false);
  });
  server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest * request) {
    aux = 0;
    request->send(SPIFFS, "/index.html", String(), false);
  });
  server.on("/index.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.css", "text/css");
  });
  server.on("/dashboard.html", HTTP_GET, [](AsyncWebServerRequest * request) {
    aux = 0;
    request->send(SPIFFS, "/dashboard.html", String(), false);
  });
  server.on("/dashboard.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/dashboard.css", "text/css");
  });
  server.on("/reportes.html", HTTP_GET, [](AsyncWebServerRequest * request) {
    aux = 0;
    request->send(SPIFFS, "/reportes.html", String(), false);
  });
  server.on("/reportes.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/reportes.css", "text/css");
  });

  // server.on("/RSSI", HTTP_GET, [](AsyncWebServerRequest *request){
  //   request->send_P(200, "text/plain",getRSSI().c_str());
  // });



  //Funcionalidad
  server.on("/ABRIR_PUERTA", HTTP_POST, [](AsyncWebServerRequest *request){
    pwmValue1 = "0";
    ledcWrite(PWM1_Ch1,  pwmValue1.toInt()); 
    request->redirect("/dashboard.html");    
  });
  server.on("/CERRAR_PUERTA", HTTP_POST, [](AsyncWebServerRequest *request){
    pwmValue1 = "4000";
    ledcWrite(PWM1_Ch1,  pwmValue1.toInt()); 
    request->redirect("/dashboard.html");    
  });
  //luces exteriores
  server.on("/LE_ON", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPIN , HIGH);
    request->redirect("/dashboard.html");    
  });
  server.on("/LE_OFF", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPIN , LOW);
    request->redirect("/dashboard.html");    
  });
  //luces interiores
  server.on("/LI_ON", HTTP_GET, [](AsyncWebServerRequest *request){
    pwm1=random(0, 255);
    pwm2=random(0, 255);
    pwm3=random(0, 255);
    ledcWrite(PWM1_Ch, pwm1);
    ledcWrite(PWM2_Ch, pwm2);
    ledcWrite(PWM3_Ch, pwm3);
    request->redirect("/dashboard.html");    
  });
  server.on("/LI_OFF", HTTP_GET, [](AsyncWebServerRequest *request){
    pwm1=0;
    pwm2=0;
    pwm3=0;
    ledcWrite(PWM1_Ch, pwm1);
    ledcWrite(PWM2_Ch, pwm2);
    ledcWrite(PWM3_Ch, pwm3);
    request->redirect("/dashboard.html");    
  });
  
  server.begin();
}

// String datos(const String& var) {
//   if (var == "IP") {
//     return WiFi.localIP().toString();
//   }else if (var == "HOSTNAME") {
//     return String(WiFi.SSID());
//   }else if (var == "STATUS") {
//     return String(WiFi.status());
//   }else if (var == "PSK") {
//     return String(WiFi.psk());
//   }else if (var == "RSSI") {
//     return String(WiFi.RSSI());
//   }
//   return var;
// }

void initConfig(){
  // Iniciamos  SPIFFS
  if (!SPIFFS.begin()){ 
    Serial.println("ha ocurrido un error al montar SPIFFS");
    return;
  }
  
  // Conectamos al Wi-Fi
  WiFi.begin(ssid, password);
  // Mientras no se conecte, mantenemos un bucle con reintentos sucesivos
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    // Esperamos un segundo
    Serial.println("Conectando a la red WiFi..");
  }
  Serial.println();
  Serial.println(WiFi.SSID());
  Serial.print("Direccion IP:\t");
  // Imprimimos la ip que le ha dado nuestro router
  Serial.println(WiFi.localIP());
}

void loop() {
    ledcWrite(PWM1_Ch1,  pwmValue1.toInt());
    // delay(1000);
    Serial.print("Servo: ");
    Serial.println(pwmValue1.toInt());

    //Alarma fondo
    long t; //timepo que demora en llegar el eco
    long d; //distancia en centimetros
    digitalWrite(Trigger, HIGH);
    delayMicroseconds(10);          //Enviamos un pulso de 10us
    digitalWrite(Trigger, LOW);
    
    t = pulseIn(Echo, HIGH); //obtenemos el ancho del pulso
    d = t/59;             //escalamos el tiempo a una distancia en cm
    
    Serial.print("Distancia: ");
    Serial.print(d);      //Enviamos serialmente el valor de la distancia
    Serial.print("cm");
    Serial.println();
    delay(100);          //Hacemos una pausa de 100ms

    if (d < 15){
      ledcWriteTone(channel, 2000);
      ledcWrite(channel, 255);
      delay(500);
      ledcWrite(channel, 0);
      delay(500);
    }
    if (d <= 15){
      ledcWriteTone(channel, 2000);
      ledcWrite(channel, 0);
    }
    //Luces exteriores

    //Luces interiores
    ledcWrite(PWM1_Ch, pwm1);
    ledcWrite(PWM2_Ch, pwm2);
    ledcWrite(PWM3_Ch, pwm3);

    //Puerta
    ledcWrite(pwm_channel, 100);

    // LEER EL ESTADO DE PINES DE BOTON A VARIABLES
    estado1 = digitalRead(boton1);
    estado2 = digitalRead(boton2);
    // SE OPRIMIO EL BOTON DE ENCENDIDO?
    if (estado1 == HIGH) {
      c=1;
    }
    while (c == 1) {
      estado1 = digitalRead(boton1);
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      delay(5);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      delay(100);
      if (estado1 == LOW){
        c = 0;
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
      }
    }
    

      if (estado2 == HIGH) {
      d=1;
    }
    while (d == 1) {
      estado2 = digitalRead(boton2);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      delay(10);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      delay(100);
      if (estado2 == LOW){
        d = 0;
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
      }
    }

    //Temperatura
    // lectura de Humedad
    humi  = dht_sensor.readHumidity();
    // Lectura de temperatura en Celsius
    tempC = dht_sensor.readTemperature();
    // Lectura de temperatura en Fahrenheit
    tempF = dht_sensor.readTemperature(true);
    // Verificacion de la lectura del sensor
    if ( isnan(tempC) || isnan(tempF) || isnan(humi)) {
      Serial.println("Error de lectura del sensor DHT!");
    } else {
      Serial.print("Humedad: ");
      Serial.print(humi);
      Serial.print("%");
      Serial.print("  |  ");
      Serial.print("Temperatura: ");
      Serial.print(tempC);
      Serial.print("°C  ~  ");
      Serial.print(tempF);
      Serial.println("°F");
    }
    //Ultrasonico
    long t2; //timepo que demora en llegar el eco
    long d2; //distancia en centimetros

    digitalWrite(Trigger2, HIGH);
    delayMicroseconds(10);          //Enviamos un pulso de 10us
    digitalWrite(Trigger2, LOW);
    
    t2 = pulseIn(Echo2, HIGH); //obtenemos el ancho del pulso
    d2 = t2/59;             //escalamos el tiempo a una distancia en cm
    
    Serial.print("Distancia: ");
    Serial.print(d2);      //Enviamos serialmente el valor de la distancia
    Serial.print("cm");
    Serial.println();
    delay(100);   

    //  Esperando 2 segundos entre lecturas
    delay(2000);
}
