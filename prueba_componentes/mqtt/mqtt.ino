/*void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
*/

#include <WiFi.h>
#include <PubSubClient.h>

// Configuración WiFi
const char* ssid = "tuSSID";
const char* password = "tuContraseña";

// Configuración del broker MQTT
const char* mqtt_server = "193.147.79.118";
const int mqtt_port = 21883;
const char* mqtt_topic = "/SETR/2024/ESP32/";

// Cliente WiFi y MQTT
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);

  // Conexión WiFi
  Serial.print("Conectando a WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado!");

  // Configuración del broker MQTT
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  // Conexión al broker MQTT
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Publicar un mensaje
  Serial.println("Publicando mensaje...");
  client.publish(mqtt_topic, "Hello from ESP32!");
  delay(5000);  // Publicar cada 5 segundos
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

