/*|----------------------------------------------------------|*/
/*|Connection sketch to eduroam network (WPA/WPA2) Enteprise |*/
/*|Suitable for almost any ESP32 microcontroller with WiFi   |*/
/*|Raspberry or Arduino WiFi CAN'T USE THIS LIBRARY!!!       |*/
/*|Edited by: Martin Chlebovec (martinius96)                 |*/
/*|Compilation under 2.0.3 Arduino Core and higher worked    |*/
/*|Compilation can be done only using STABLE releases        |*/
/*|Dev releases WILL NOT WORK. (Check your Ard. Core .json)  |*/
/*|WiFi.begin() have more parameters for PEAP connection     |*/
/*|----------------------------------------------------------|*/


#include <WiFi.h> //Wifi library
#include "recetacoco.h"
#include <PubSubClient.h>

#define RXD2 33
#define TXD2 4
#define init "{init}"
#define ping "{ping}"
#define fin "{fin}"
#define line_lost "{line_lost}"

//SSID NAME
const char* ssid = "eduroam"; // eduroam SSID

// Configuración del broker MQTT
const char* mqtt_server = "193.147.79.118";
const int mqtt_port = 21883;
const char* mqtt_topic = "/SETR/2024/13/";
unsigned long time_start = millis();
unsigned long time_lap;
unsigned long time_ping;
bool end_flag = false;
int state = 0;
int group = 13;

WiFiClient espClient;
PubSubClient client(espClient);


void setup() {
  //Serial.begin(115200);
  Serial.begin(9600);
  delay(10);
  Serial.println(ssid);
  //WiFi.disconnect(true); 
  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, WPA2_AUTH_PEAP, EAP_IDENTITY, EAP_USERNAME, EAP_PASSWORD); 

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi is connected!");
  Serial.println("IP address set: ");
  Serial.println(WiFi.localIP()); //print LAN IP


  client.setServer(mqtt_server, mqtt_port);

  // Serial port to communicate with Arduino UNO
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  
  delay(5000);
  
  /*while(!Serial2.available()){
    Serial.print("espersndo serial");

  }*/
  Serial2.println("{ Connected to wifi }");
  Serial.print("Messase sent! to Arduino");
}

void reconnect() {
  // Reintentar conexión MQTT hasta que se logre
  while (!client.connected()) {
    Serial.print("Intentando conectar al broker MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println("Conectado!");
    } else {
      Serial.print("Fallo, rc=");
      Serial.print(client.state());
      Serial.println(" Intentando de nuevo en 5 segundos...");
      delay(5000);
    }
  }
}


String data_clean(String buff){
  buff.remove(0, 1); // Elimina el primer carácter '{'
  buff.remove(buff.length() - 1, 1);
  return buff;
}

String sendBuff;

void loop() {
  yield();

  if (Serial2.available()) {

    char c = Serial2.read();
    sendBuff += c;
    
    if (c == '}')  {            
      Serial.print("Received data in serial port from Arduino: ");
      Serial.println(sendBuff);

      if (!client.connected()) {
        reconnect();
      }
      client.loop();
      sendBuff.trim(); // Elimina espacios en blanco, saltos de línea y caracteres no imprimibles

      /*if (end_flag){

        String distance = data_clean(sendBuff);
        Serial.println("Publicando mensaje...");
        String payload2 = "{\"team_name\": \"13_pitufos\", \"id\": \"13\", \"action\": \"OBSTACLE_DETECTED\", \"distance\": " + distance + "}";
        client.publish(mqtt_topic, payload2.c_str());

        Serial.println("Publicando mensaje...");
        String payload = "{\"team_name\": \"13_pitufos\", \"id\": \"13\", \"action\": \"END_LAP\, \"time\": " + String(time_lap) + "}";
        client.publish(mqtt_topic, payload.c_str());
        end_flag = false;

      } else*/ 
      if (sendBuff == "{init}"){
        Serial.println("Publicando mensaje...");
        String payload = "{\"team_name\":\"13_pitufos\",\"id\":\"13\",\"action\":\"START_LAP\"}";
        client.publish(mqtt_topic, payload.c_str());
        time_start = millis();


      } else if (sendBuff.equals("{ping}")){
        // Publicar un mensaje
        time_ping = millis() - time_start;
        Serial.println("Publicando mensaje...");
        String payload = "{\"team_name\":\"13_pitufos\",\"id\":\"13\",\"action\":\"PING\",\"time\":" + String(time_ping) + "}";
        client.publish(mqtt_topic, payload.c_str());

      } else if (sendBuff == "{end}" || end_flag ) {
        time_lap = millis() - time_start;
        end_flag = true;
        
        if (state == 0) {
          state = 1;

        } else if (state == 1){

          String distance = data_clean(sendBuff);
          Serial.println("Publicando mensaje...");
          String payload = "{\"team_name\":\"13_pitufos\",\"id\":\"13\",\"action\":\"OBSTACLE_DETECTED\",\"distance\":" + distance + "}";
          client.publish(mqtt_topic, payload.c_str());

          Serial.println("Publicando mensaje...");
          String payload2 = "{\"team_name\":\"13_pitufos\",\"id\":\"13\",\"action\":\"END_LAP\",\"time\":" + String(time_lap) + "}";
          client.publish(mqtt_topic, payload2.c_str());
          state = 2;

        } else if (state == 2) {
          Serial.println("Publicando mensaje...");
          String number = data_clean(sendBuff);
          String payload = "{\"team_name\":\"13_pitufos\",\"id\":\"13\",\"action\":\"VISIBLE_LINE\",\"value\":" +  number +"}";
          client.publish(mqtt_topic, payload.c_str());

        }

      } else if (sendBuff == "{line_lost}"){

        Serial.println("Publicando mensaje...");
        String payload = "{\"team_name\":\"13_pitufos\",\"id\":\"13\",\"action\":\"LINE_LOST\"}";
        client.publish(mqtt_topic, payload.c_str());

      } else if (sendBuff == "{line_found}"){
        
        Serial.println("Publicando mensaje...");
        String payload = "{\"team_name\":\"13_pitufos\",\"id\":\"13\",\"action\":\"LINE_FOUND\"}";
        client.publish(mqtt_topic, payload.c_str());

      
      } /*else {
        Serial.println("Publicando mensaje...");
        String number = data_clean(sendBuff);
        String payload = "{\"team_name\": \"13_pitufos\", \"id\": \"13\", \"action\": \"VISIBLE_LINE\, \"value\": " +  number + "%}";
        client.publish(mqtt_topic, payload.c_str());", \"id\": \"13\",

      }*/

      sendBuff = "";

    }


  }

}