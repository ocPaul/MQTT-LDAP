#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <iostream>
#include <cstring>
#include <DHT.h>

// Replace the next variables with your SSID/Password combination
const char* ssid = "THAMONKEE";
const char* password = "Monkeeee";

// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.1.144";
const char* mqtt_server = "172.31.200.150";

#define DHT_SENSOR_PIN  21
#define DHT_SENSOR_TYPE DHT11

DHT dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

String message_humi = "";
String message_temp = "";
String output_humi; 
String output_temp;

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
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
}

void readDHTsensorTemp() {
  // read humidity
  float humi  = dht_sensor.readHumidity();
  // read temperature in Celsius
  float tempC = dht_sensor.readTemperature();

  // check whether the reading is successful or not
  if ( isnan(tempC) || isnan(humi)) {
    // Serial.println("Failed to read from DHT sensor!");
    tempC = 187;
    humi = 187;
  } else {
    //message_humi = "test";
    // "Temperature: " + std::to_string(tempC) + "°C  |   Humidity: " + std::to_string(humi) + "%";
    Serial.print("Humidity: ");
    Serial.print(humi);
    Serial.print("%");

    message_humi = String(humi);
    message_temp = String(tempC);

    Serial.print("  |  ");

    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.print("°C");
    Serial.println("");
  }
}

void setup() {
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  dht_sensor.begin();
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
      // Subscribe
      //client.subscribe("esp32/output");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  //client.subscribe("ranztanz");
  //client.loop();
  readDHTsensorTemp();
  output_humi = String(message_humi);
  output_temp = String(message_temp);
  client.publish("humidity", output_humi.c_str());
  client.publish("temperature", output_temp.c_str());
  delay(1000);
}