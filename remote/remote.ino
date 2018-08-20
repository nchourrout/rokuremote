#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Keypad.h>

// TODO:
// - Scan network for Roku
// - Low Battery indicator
// - OTA?

const char* ssid     = "YOUR_WIFI";
const char* password = "WIFI_PASSWD";
const char* rokuHost = "192.168.0.18";

HTTPClient http;

const int ledPin =  13;
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {A3, A2, A1, A0}; //connect to the row pinouts of the keypad - four in a row
byte colPins[COLS] = {MOSI, SCK, A5, A4}; //connect to the column pinouts of the keypad - four in a row

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

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
  char key = keypad.getKey();

  if (key != NO_KEY){
    Serial.println(key);
    // TODO: Does Arduino support switches?
    if (key == "2") {
      sendCommand("VolumeUp");
    } else if (key == "8") {
      sendCommand("VolumeDown");
    }
  }
}

void sendCommand(String command) {
   Serial.println("Sending command " + command);
   digitalWrite(ledPin, HIGH);
   delay(100);
   digitalWrite(ledPin, LOW);
   
   String url = "http://" + rokuHost + ":8060/keypress/" + command;
   http.begin(url);
   http.POST("");
   http.end();
}
