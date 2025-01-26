#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define ledPin1 D1
#define ledPin2 D2
#define ledPin3 D3
#define ledPin4 D4
#define ledPin5 D5
#define ledPin6 D6
#define buzzerPin D7

const char* ssid = "GARDEN";
const char* password = "9842201111@gard";

const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;
const char* topic = "hut";

WiFiClient espClientkit1; //Change number for kitchen 2
PubSubClient client(espClientkit1); //Change number for kitchen 2

void setup() 
{
    pinMode(ledPin1, OUTPUT);
    pinMode(ledPin2, OUTPUT);
    pinMode(ledPin3, OUTPUT);
    pinMode(ledPin4, OUTPUT);
    pinMode(ledPin5, OUTPUT);
    pinMode(ledPin6, OUTPUT);
    pinMode(buzzerPin, OUTPUT);

    Serial.begin(9600);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");

    client.setServer(mqttServer, mqttPort);
    client.setCallback(callback);

    while (!client.connected()) 
    {
        if (client.connect("NodeMCU_Client")) 
        {
            Serial.println("Connected to MQTT broker");
            client.subscribe(topic);
        } 
        else 
        {
            Serial.print("Failed to connect to MQTT broker, retrying in 0.1 seconds...");
            delay(100);
        }
    }
}

void loop() 
{
    if (!client.connected()) 
    {
        reconnect();
    }
    client.loop();
}

void reconnect() 
{
    while (!client.connected()) 
    {
        if (client.connect("NodeMCU_Client")) 
        {
            Serial.println("Connected to MQTT broker");
            client.subscribe(topic);
        } 
        else 
        {
            Serial.print("Failed to connect to MQTT broker, retrying in 0.1 seconds...");
            delay(100);
        }
    }
}

void callback(char* topic, byte* payload, size_t length) 
{
    String message = "";
    for (size_t i = 0; i < length; i++) 
    {
        message += (char)payload[i];
    }

    Serial.println(message);

    int callmsg = message.indexOf('o');
    if (callmsg == 0 && callmsg != -1)
    { 
        Serial.println("Call messages");
        buzz();
    }

    if (message == "o7") 
    {
        digitalWrite(ledPin1, HIGH);
    } 
   else if (message == "o8") 
    {
        digitalWrite(ledPin2, HIGH);
    } 
   else if (message == "o9") 
    {
        digitalWrite(ledPin3, HIGH);
    } 
   else if (message == "o10") 
    {
        digitalWrite(ledPin4, HIGH);
    } 
   else if (message == "o11") 
    {
        digitalWrite(ledPin5, HIGH);
    } 
   else if (message == "o12") 
    {
        digitalWrite(ledPin6, HIGH);
    } 
   else if (message == "f7") 
    {
        digitalWrite(ledPin1, LOW);
    } 
    else if (message == "f8") 
    {
        digitalWrite(ledPin2, LOW);
    }
   else if (message == "f9") 
    {
        digitalWrite(ledPin3, LOW);
    }
   else if (message == "f10") 
    {
        digitalWrite(ledPin4, LOW);
    }
    else if (message == "f11") 
    {
        digitalWrite(ledPin5, LOW);
    }
    else if (message == "f12") 
    {
        digitalWrite(ledPin6, LOW);
    }
    
}

void buzz() 
{
    int melody[] = {150, 250, 500, 550, 0, 550, 500, 250, 150};//262, 294, 330, 349, 392, 440, 494, 523
    int noteDuration = 200;
    for (int i = 0; i < 9; i++) 
    {
        tone(buzzerPin, melody[i], noteDuration);
        delay(noteDuration);
    }

    noTone(buzzerPin);
}
