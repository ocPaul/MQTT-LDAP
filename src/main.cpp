#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <iostream>
#include <cstring>
#include <PubSubClient.h>

const char* ssid = "THAMONKEE";
const char* password = "Monkeeee";

const char* mqtt_server = "172.31.200.149";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

const int ledPin1 = 12;
const int ledPin2 = 13;
float voltage = 0;

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
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  
  if (!client.connected()) {
   reconnect();
  }
  client.subscribe("voltage");
  client.loop();

  if (voltage > 3.0) {
    digitalWrite(ledPin1, HIGH);
    digitalWrite(ledPin2, LOW);
  } else {
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, HIGH);
  }
  delay(100);
}

