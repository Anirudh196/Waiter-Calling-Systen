#ifdef ESP8266
 #include <ESP8266WiFi.h>
 #else
 #include <WiFi.h>
#endif

#include <PubSubClient.h>
#include <WiFiClientSecure.h>    

#define switchPin1 D4
#define switchPin2 D6

const char* ssid = "GARDEN";
const char* password = "9842201111@gard";

const char* mqtt_server = "d55860b1ebb94d229518a106cad61eb9.s1.eu.hivemq.cloud";
const char* mqtt_username = "GardenRestaurant";
const char* mqtt_password = "Garden12@";
const int mqtt_port = 8883;

const char* messageSwitchPin1 = "o1"; //Change number
const char* messageSwitchPin2 = "f1"; //Change number

WiFiClientSecure espClient1; //Change number
PubSubClient client(espClient1); //Change number

void setup_wifi() {
  delay(10);
  Serial.print("\nConnecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("\nWiFi connected\nIP address: ");
  Serial.println(WiFi.localIP());
} 

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";   // Create a random client ID
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("connected");
    } 
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 2 seconds");   // Wait 5 seconds before retrying
      delay(2000);
    }
  }
}

void publishMessage(const char* topic, String payload , boolean retained){
  if (client.publish(topic, payload.c_str(), true))
      Serial.println("Message publised ["+String(topic)+"]: "+payload);
}

void setup() {
  pinMode(switchPin1, INPUT_PULLUP);
  pinMode(switchPin2, INPUT_PULLUP);
  
  Serial.begin(9600);
  while (!Serial) delay(1);
  setup_wifi();

  #ifdef ESP8266
    espClient1.setInsecure();
  #else
    espClient1.setCACert(root_ca);      // enable this line and the the "certificate" code for secure connection
  #endif

  client.setServer(mqtt_server, mqtt_port);
}

void loop() { 

  if (!client.connected()) reconnect(); // check if client is connected
  client.loop();
  

  if (digitalRead(switchPin1) == LOW) {
    publishMessage("hut", messageSwitchPin1, true);
  }

  if (digitalRead(switchPin2) == LOW) {
    publishMessage("hut", messageSwitchPin2, true);
  }

  delay(1000);
}
