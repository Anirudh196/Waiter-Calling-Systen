//Code for 1st Hut
#include <ESP8266WiFi.h>
#include <PubSubClient.h>     

#define switchPin1 D6
#define switchPin2 D7

const char* ssid = "GARDEN";
const char* password = "9842201111@gard";

const char* mqtt_server = "broker.hivemq.com"; 
const int mqtt_port = 1883; 
const char* mqtt_topic = "hut";

const char* messageSwitchPin1 = "o1"; 
const char* messageSwitchPin2 = "f11"; 

WiFiClient espClient1;
PubSubClient client(espClient1); 

void setup_wifi() 
{ 
    delay(10);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.mode(WIFI_STA); 
    WiFi.begin(ssid, password);
    while (WiFi.status()!= WL_CONNECTED) 
    {
        delay(500);
        Serial.print(".");
    }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() 
{
    while (!client.connected()) 
    {
        Serial.print("Attempting MQTT connection...");
        String clientId = "ESP8266Client-";
        clientId += String(random(0xffff), HEX);
        if (client.connect(clientId.c_str())) 
        {
             Serial.println("connected");
        } 
      else 
        {
             Serial.print("failed, rc=");
             Serial.print(client.state());
             Serial.println(" try again in 0.1 seconds");
             delay(100);
        }
    }
}

void setup() 
{
    Serial.begin(115200);
    setup_wifi();
    client.setServer(mqtt_server, mqtt_port);

    pinMode(switchPin1, INPUT_PULLUP);
    pinMode(switchPin2, INPUT_PULLUP);
}

void loop() 
{
    if (!client.connected()) 
    {
        reconnect();
    }
    client.loop();

    if (digitalRead(switchPin1) == LOW) 
    {
        client.publish(mqtt_topic, messageSwitchPin1);
        delay(100);
    }

    if (digitalRead(switchPin2) == LOW) 
    {
        client.publish(mqtt_topic, messageSwitchPin2);
        delay(100);
    }
}
