#include <ESP8266WiFi.h>
#include <WiFiClient.h>

const char* ssid = "SSID";
const char* password = "PASS";

const char* server = "api.thinkspeak.com";
const char* link = "http://api.thingspeak.com/update?api_key=2LO1QR75U9GGX4DH";

unsigned long lastTime = 0;
unsigned long timeDelay = 1000;

const int analogIn = A0;

WiFiClient client;

void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println('\n');
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.println(" ...");
  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(++i); Serial.print(' ');
  }
  Serial.println('\n');
  Serial.println("Connection established!");

  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
}

void loop() {
  int val1 = 0, val2 = 0, val3 = 0, val4 = 0;

  digitalWrite(4, HIGH);
  digitalWrite(5, LOW);
  delay(2000);
  int data = analogRead(analogIn);
  int out = map(data, 0, 1023, 0, 255);
  val1 = out * 2.3 + 20;
  val2 = out * 1.2 + 10;

  digitalWrite(4, LOW);
  digitalWrite(5, HIGH);
  delay(2000);
  data = analogRead(analogIn);
  out = map(data, 0, 1023, 0, 255);
  val3 = out * 4.5 + 40;
  val4 = out * 1.2 + 50;
  
  
  if (client.connect(server, 80)) {
    String getStr = String(link) + "&field1=" + String(val1) + "&field2=" + String(val2) + "&field3=" + String(val3) + "&field4=" + String(val4);
    
    client.print("GET " + getStr + "\n");
    client.print("HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n\n\n");

    
    while(client.available()) {
      char c = client.read();
      Serial.print(c);
    }

    Serial.println("");
  }

  client.stop();
  Serial.println("Waiting...");
  delay(5000);
}
