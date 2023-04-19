#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <iostream>
#include <cstring>
#include <PubSubClient.h>

const int voltInput = 34;
float voltage;
int analogValue = 0;

const char* ssid = "THAMONKEE";
const char* password = "Monkeeee";

const char* mqtt_server = "172.31.200.150";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

std::string message ="ö";
char* arry;

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Value arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Value: ");
  String valueTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    valueTemp += (char)message[i];
  }
  // Serial.print(messageTemp);
  Serial.println();
  voltage = valueTemp.toFloat();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/output");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  }

void loop() {
  if (!client.connected()) {
   reconnect();
  }
  message = String(voltage, 4);
  client.publish("topic/float_value", message.c_str());

  analogValue = analogRead(voltInput);
  voltage = analogValue / 4095.0 * 9.25;
  Serial.println(voltage);
  delay(1000);
}