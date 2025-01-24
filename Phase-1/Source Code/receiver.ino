#ifdef ESP8266
 #include <ESP8266WiFi.h>
 #else
 #include <WiFi.h>
#endif

#include <PubSubClient.h>
#include <WiFiClientSecure.h>  

#define ledPin1 D1
#define ledPin2 D2
#define ledPin3 D3
#define ledPin4 D4
#define ledPin5 D5
#define ledPin6 D6
#define buzzerPin D7

const char* ssid = "Anirudh";
const char* password = "Lagoonnebula,1610";

const char* mqtt_server = "d55860b1ebb94d229518a106cad61eb9.s1.eu.hivemq.cloud";
const char* mqtt_username = "GardenRestaurant";
const char* mqtt_password = "Garden12@";
const int mqtt_port = 8883;

WiFiClientSecure espClientkitchen;
PubSubClient client(espClientkitchen);

unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];

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

      client.subscribe("led_state");   // subscribe the topics here

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 2 seconds");   // Wait 5 seconds before retrying
      delay(2000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String incommingMessage = "";
  for (int i = 0; i < length; i++) incommingMessage+=(char)payload[i];

  Serial.println("Message arrived ["+String(topic)+"]"+incommingMessage);

  //--- check the incomming message
  if (strcmp(topic,"hut") == 0){
    if (incommingMessage.equals("o1")) 
      digitalWrite(ledPin1, HIGH);   // Turn the LED on
    else if (incommingMessage.equals("f1")) 
      digitalWrite(ledPin1, LOW);  // Turn the LED off
    else if (incommingMessage.equals("o2"))
      digitalWrite(ledPin2, HIGH);
    else if (incommingMessage.equals("f2"))
      digitalWrite(ledPin2, LOW);
    else if (incommingMessage.equals("o3"))
      digitalWrite(ledPin3, HIGH);
    else if (incommingMessage.equals("f3"))
      digitalWrite(ledPin3, LOW);
    else if (incommingMessage.equals("o4"))
      digitalWrite(ledPin4, HIGH);
    else if (incommingMessage.equals("f4"))
      digitalWrite(ledPin4, LOW);
    else if (incommingMessage.equals("o5"))
      digitalWrite(ledPin5, HIGH);
    else if (incommingMessage.equals("f5"))
      digitalWrite(ledPin5, LOW);
    else if (incommingMessage.equals("o6"))
      digitalWrite(ledPin6, HIGH);
    else if (incommingMessage.equals("f6"))
      digitalWrite(ledPin6, LOW);
    
  }

}
void setup() {
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);
  pinMode(ledPin5, OUTPUT);
  pinMode(ledPin6, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  Serial.begin(9600);
  while (!Serial) delay(1);
  setup_wifi();

  espClientkitchen.setInsecure();

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) reconnect(); // check if client is connected
  client.loop();
}

void buzz() {
  int melody[] = {150, 250, 500, 550, 0, 550, 500, 250, 150};//262, 294, 330, 349, 392, 440, 494, 523
  int noteDuration = 200;

  for (int i = 0; i < 9; i++) 
    {
      tone(buzzerPin, melody[i], noteDuration);
      delay(noteDuration);
    }

  noTone(buzzerPin);
}
