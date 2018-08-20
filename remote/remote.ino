
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// TODO:
// - Scan network for Roku
// - Support 4x4 Matrix Keyboard
// - Low Battery indicator
// - OTA?

const char* ssid     = "YOUR_WIFI";
const char* password = "WIFI_PASSWD";

HTTPClient http;
const int ledPin =  13;
const int buttonUpPin = 2;
const int buttonDownPin = 4;

int buttonUpState = 0;
int buttonDownState = 0;

void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(ledPin, OUTPUT);
  pinMode(buttonUpPin, INPUT_PULLUP);
  pinMode(buttonDownPin, INPUT_PULLUP);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  digitalWrite(ledPin, HIGH);
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

int value = 0;

void loop() {
  buttonUpState = digitalRead(buttonUpPin);
  buttonDownState = digitalRead(buttonDownPin);
  
  if (buttonUpState == LOW) {
    sendCommand("VolumeUp");
  } else if (buttonDownState == LOW) {
    sendCommand("VolumeDown");
  }
}

void sendCommand(String command) {
   Serial.println("Sending command " + command);
   digitalWrite(ledPin, HIGH);
   delay(100);
   digitalWrite(ledPin, LOW);
   
   String url = "http://192.168.0.18:8060/keypress/" + command;
   http.begin(url);
   http.POST("");
   http.end();
}
