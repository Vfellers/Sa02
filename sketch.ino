
#include <WiFi.h>  //biblioteca
#include <HTTPClient.h>
#include "DHTesp.h"

#define sensorDHT 15
DHTesp sensor;

String apiKey = "7AMH6IYM7XVZ63WZ";     
const char *ssid =  "Wokwi-GUEST";    
const char *pass =  "";
const char* server = "api.thingspeak.com";
WiFiClient client;

void setup() {
   Serial.begin(115200);
    Serial.println("Olá");
    
    WiFi.begin(ssid, pass);                     // id e password, iniciar conexao

    while(WiFi.status() != WL_CONNECTED){       //enquanto wifi for diferente de conectado
      delay(500);
      Serial.println("Conectando ao Wifi...");
    } 
    Serial.println("Conectado\n");

    sensor.setup(sensorDHT, DHTesp::DHT22);
}

void loop() {
  delay(10000); 

  float temperatura = sensor.getTemperature();
  float umidade = sensor.getHumidity();

  if (client.connect(server,80))                                 //api.thingspeak.com
    {                    
    String postStr = apiKey;
    postStr +="&field1=";
    postStr += String(temperatura);
    postStr += "&field2=";
    postStr += String(umidade);
    postStr += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
  
  Serial.println("Temperatura: " + String(temperatura) + "C°");
  Serial.println("Umidade: " + String(umidade) + "%");
  Serial.println("Enviado ao Thingspeak... \n\n");
  }
}
