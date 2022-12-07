// Importamos las librerías
#include <WebServer.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <stdio.h>
#include <SPI.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "SPIFFS.h"

#include "config.h"
 // Valores ajustados para el SG90
#define COUNT_LOW 1638
#define COUNT_HIGH 7000
#define TIMER_WIDTH 16
#define PWM1_Ch1    1
#define PWM1_Ch2    2
#define PWM1_Ch3    3
#define PWM1_Ch4    4
String pwmValue1, pwmValue2, pwmValue3, pwmValue4;
int x1, x2, y, y2;
// int aSpeed,bSpeed,cSpeed;
int joyX1, joyY1, joyX2, joyY2;
// Contadores
int aux = 0; //Para saber si el modo automatico esta activo o no.
int contador = 0; //Vector secuencia
int c1 = 0;
int c2 = 0;
int c3 = 0;
int c4 = 0;
// Vectores

AsyncWebServer server(80);
//UDP un WiFi Serveru bibloteku instance
WiFiUDP udp;
WebServer server2(80);


String getRSSI(){
  return String(WiFi.RSSI());
}
void setup() {
  
  Serial.begin(115200);   
  
  ledcSetup(1, 50, TIMER_WIDTH); // canal 1, 50 Hz, 16-bit width
  ledcAttachPin(32, 1);   // GPIO 22 asignado al canal 1
  ledcSetup(2, 50, TIMER_WIDTH); // canal 2, 50 Hz, 16-bit width
  ledcAttachPin(33, 2);   // GPIO 19 asignado al canal 2
  ledcSetup(3, 50, TIMER_WIDTH); // canal 1, 50 Hz, 16-bit width
  ledcAttachPin(26, 3);   // GPIO 22 asignado al canal 3
  ledcSetup(4, 50, TIMER_WIDTH); // canal 2, 50 Hz, 16-bit width
  ledcAttachPin(25, 4);   // GPIO 19 asignado al canal 4

  // Iniciamos  SPIFFS
  if (!SPIFFS.begin()){ 
    Serial.println("ha ocurrido un error al montar SPIFFS");
    return;
  }
  
  // conectamos al Wi-Fi
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

  // Rutas
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html",String(), false, datos);
  });
  server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest * request) {
    aux = 0;
    request->send(SPIFFS, "/index.html", String(), false, datos);
  });
  server.on("/index.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.css", "text/css");
  });
  server.on("/RSSI", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain",getRSSI().c_str());
  });



  //Primera pestaña
  server.on("/ABRIR_PUERTA", HTTP_POST, [](AsyncWebServerRequest *request){
    pwmValue1 = "0";
    ledcWrite(PWM1_Ch1,  pwmValue1.toInt()); 
    request->redirect("/puerta.html");    
  });
  server.on("/CERRAR_PUERTA", HTTP_POST, [](AsyncWebServerRequest *request){
    pwmValue1 = "4000";
    ledcWrite(PWM1_Ch1,  pwmValue1.toInt()); 
    request->redirect("/puerta.html");    
  });
  server.begin();
}

String datos(const String& var) {
  if (var == "IP") {
    return WiFi.localIP().toString();
  }else if (var == "HOSTNAME") {
    return String(WiFi.SSID());
  }else if (var == "STATUS") {
    return String(WiFi.status());
  }else if (var == "PSK") {
    return String(WiFi.psk());
  }else if (var == "RSSI") {
    return String(WiFi.RSSI());
  }
  return var;
}

void loop() {
      ledcWrite(PWM1_Ch1,  pwmValue1.toInt());
      delay(1000);
      Serial.print("Servo: ");
      Serial.println(pwmValue1.toInt());
}
