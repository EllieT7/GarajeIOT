// Importamos las librerías
#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include "SPIFFS.h"
#include "config.h"
 // Valores ajustados para el SG90
#define COUNT_LOW 1638
#define COUNT_HIGH 7000
#define TIMER_WIDTH 16
#define PWM1_Ch1    1

String pwmValue1;

// Contadores
int aux = 0; //Para saber si el modo automatico esta activo o no.

AsyncWebServer server(80);



void setup() {
  Serial.begin(115200);   
  //Inicializacion de pines
  ledcSetup(1, 50, TIMER_WIDTH); // canal 1, 50 Hz, 16-bit width
  ledcAttachPin(32, 1);   // GPIO 22 asignado al canal 1

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
    request->redirect("/puerta.html");    
  });
  server.on("/CERRAR_PUERTA", HTTP_POST, [](AsyncWebServerRequest *request){
    pwmValue1 = "4000";
    ledcWrite(PWM1_Ch1,  pwmValue1.toInt()); 
    request->redirect("/puerta.html");    
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
      delay(1000);
      Serial.print("Servo: ");
      Serial.println(pwmValue1.toInt());
}
