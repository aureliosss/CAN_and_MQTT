#include "CAN_receive.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <mcp_can.h>
#include <SPI.h>

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];

MCP_CAN CAN0(5); // Set CS to pin 5

// WiFi
const char *ssid = ""; // Enter your WiFi network name
const char *password = "pwd";  // Enter WiFi password

// MQTT Broker
const char *mqtt_broker = "your_host_name";
const char *topic = "CAN/node_C";
const char *mqtt_username = "your_user_name";
const char *mqtt_password = "pwd";
const int mqtt_port = 1883;
char textMessage[] = "Hello, I am NODE C, and I receive only messages for ID 0x102!";

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char *topic, byte *payload, unsigned int length);

void setup() {
  Serial.begin(115200);

  // Connecting to a WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to the Wi-Fi network");
  
  // Connecting to an MQTT broker
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  while (!client.connected()) {
    String client_id = "esp32-client-";
    client_id += String(WiFi.macAddress());
    Serial.printf("The client %s is connecting to the public MQTT broker\n", client_id.c_str());
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Connected to the public MQTT broker");
    } else {
      Serial.print("Failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }

  if (CAN0.begin(MCP_STDEXT, CAN_500KBPS, MCP_8MHZ) == CAN_OK) {
    Serial.println("MCP2515 Initialization OK!!");
  } else {
    Serial.println("MCP2515 Initialization Failed!!");
  }
  pinMode(21, INPUT); // Setting pin 21 for /INT input

  CAN0.init_Mask(0, 0, 0x01FF0000); // Initializing the mask to accept messages with 0x101 in the lower 11 bits
  CAN0.init_Filt(0, 0, 0x01010000); // Initializing the filter to match only 0x101
  Serial.println("MCP2515 Library - Filter Example...");
  CAN0.setMode(MCP_NORMAL); // Switch to normal mode to allow message transmission

  // Publish the text message when initialization is complete
  if (client.publish(topic, textMessage)) {
    Serial.println("Initialization message has been published on MQTT.");
    Serial.println(textMessage);
  } else {
    Serial.println("Error publishing the initialization message.");
  }
}

void loop() {
  if (!digitalRead(21)) // If pin 21 is in a LOW state, read the receive buffer
  {
    CAN0.readMsgBuf(&rxId, &len, rxBuf); // Read data: len = data length, rxBuf = data bytes
    if (rxId == 0x102) {
      Serial.print("ID: 0x102 Data: ");
      for (int i = 0; i < len; i++) {
        if (rxBuf[i] < 0x10) {
          Serial.print("0");
        }
        Serial.print(rxBuf[i], HEX);
        Serial.print(" ");
      }
      Serial.println();
      
      // Publish the message on MQTT when receiving ID 0x102
      if (client.publish(topic, textMessage)) {
        Serial.println("Message has been published on MQTT.");
      } else {
        Serial.println("Error publishing the message on MQTT.");
      }
    }
  }
}

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message received in topic: ");
  Serial.print(topic);
  Serial.print(" / ID: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
}
