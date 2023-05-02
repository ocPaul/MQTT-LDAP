#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <iostream>
#include <cstring>
#include <PubSubClient.h>

// pin for analog read
const int voltInput = 34;

float voltage;
int analogValue;

// wlan config
const char* ssid = "THAMONKEE";
const char* password = "Monkeeee";

// ip address of the mqtt-broker
const char* mqtt_server = "172.31.200.150";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

String output;

void setup_wifi() {
  // connecting to the wifi
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

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
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
  // start serial monitor
  Serial.begin(115200);
  
  setup_wifi();

  // set values of mqtt Server
  client.setServer(mqtt_server, 1883);
  }

void loop() {
  // reconnect mqtt if disconnected
  if (!client.connected()) {
   reconnect();
  }

  // read voltage + map to true value
  analogValue = analogRead(voltInput);
  voltage = analogValue / 4095.0 * 9.25;

  // convert float type to String with one decimal point 
  output = String(voltage, 1);
  
  // publish value on voltage
  client.publish("voltage", output.c_str());

  delay(1000);
}