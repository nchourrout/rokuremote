#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Keypad.h>

// TODO:
// - Implement modem sleep or deep sleep
// - Scan network for Roku
// - Detect Roku power status

const char* ssid     = "SSID";
const char* password = "PASSWORD";
String rokuHost = "192.168.0.X";

HTTPClient http;

const int ledPin = 0;
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {13, 12, 14, 16}; // connect to the row pinouts of the keypad - four in a row
byte colPins[COLS] = {15, 2, 4, 5}; // connect to the column pinouts of the keypad - four in a row

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
int on = false;

void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(ledPin, OUTPUT);

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

  blink();
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void blink() {
  digitalWrite(ledPin, LOW); // High and Low and inverted for some reason
  delay(100);
  digitalWrite(ledPin, HIGH);
}

void loop() {
  char key = keypad.getKey();

  if (key != NO_KEY){
    Serial.println("Key pressed"+ key);
    if (key == '1') {
      sendCommand("Back");
    } else if (key == '2') {
      sendCommand("Up");
    } else if (key == '3') {
      sendCommand("Home");
    } else if (key == 'A') {
      if (on) { // TODO: Get Roku power status from query/device-info
        sendCommand("PowerOff");  
      } else {
        sendCommand("PowerOn");
      }
      on = !on;
    } else if (key == '4') {
      sendCommand("Left");
    } else if (key == '5') {
      sendCommand("Select");
    } else if (key == '6') {
      sendCommand("Right");
    } else if (key == 'B') {
      sendCommand("VolumeUp");
    } else if (key == '7') {
      sendCommand("InstantReplay");
    } else if (key == '8') {
      sendCommand("Down");
    } else if (key == '9') {
      sendCommand("Info");
    } else if (key == 'C') {
      sendCommand("VolumeDown");
    } else if (key == '*') {
      sendCommand("Rev");
    } else if (key == '0') {
      sendCommand("Play");
    } else if (key == '#') {
      sendCommand("Fwd");
    } else if (key == 'D') {
      sendCommand("VolumeMute");
    }
  }
}

void sendCommand(String command) {
   Serial.println("Sending command " + command);
   blink();
   
   String url = "http://" + rokuHost + ":8060/keypress/" + command;
   http.begin(url);
   http.POST("");
   http.end();
}
