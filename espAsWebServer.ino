#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char*  espName = "esp_sfm";
const char *ssid = "___";
const char *password = "___";
const int redPin = 16;
const int yellowPin = 4;
const int greenPin = 2;
const int redPin2 = 14;
const int yellowPin2 = 12;
const int greenPin2 = 13;
int currentLedTag = 0;

ESP8266WebServer server(80);
const char* nodeServer = "192.168.8.104";

void updateTrafficLightState() {
  if (currentLedTag == 1) {
    sendLEDState("RED","GREEN");
    digitalWrite(redPin, HIGH);
    digitalWrite(greenPin, LOW);
    digitalWrite(yellowPin, LOW);

    digitalWrite(redPin2, LOW);
    digitalWrite(greenPin2, HIGH);
    digitalWrite(yellowPin2, LOW);
    delay(10000);

    sendLEDState("RED","YELLOW");
    digitalWrite(greenPin2, LOW);
    digitalWrite(yellowPin2, HIGH);
    delay(5000);


  } else if (currentLedTag == 2) {

    sendLEDState("GREEN","RED");
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, HIGH);
    digitalWrite(yellowPin, LOW);

    digitalWrite(redPin2, HIGH);
    digitalWrite(greenPin2, LOW);
    digitalWrite(yellowPin2, LOW);
    delay(10000);
  } else if (currentLedTag == 0) {
    sendLEDState("YELLOW","RED");
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, LOW);
    digitalWrite(yellowPin, HIGH);

    digitalWrite(redPin2, HIGH);
    digitalWrite(greenPin2, LOW);
    digitalWrite(yellowPin2, LOW);    
    delay(5000);
  }
}

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(redPin2, OUTPUT);
  pinMode(yellowPin2, OUTPUT);
  pinMode(greenPin2, OUTPUT);

  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();
  if(currentLedTag >= -1){
    currentLedTag = (currentLedTag + 1) % 3;
    updateTrafficLightState();
  }else{
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, LOW);
    digitalWrite(yellowPin, LOW);
    digitalWrite(redPin2, LOW);
    digitalWrite(greenPin2, LOW);
    digitalWrite(yellowPin2, LOW);
  }
}

void handleRoot() {
  String message = "Hello from ESP8266!";
  if(server.hasArg("turn")){
    server.send(200, "text/plain", "tfi o ch3al");
    Serial.println("tfi o ch3al wslatt");
   if(server.arg("turn") == "OFF") {
      currentLedTag = -10;
    }else if(server.arg("turn") == "ON"){
      currentLedTag = 0;
    }
  }else{
    server.send(200, "text/plain", "flipo...");
    currentLedTag == 1 ? currentLedTag = -1 : currentLedTag = 0;
  }
}

void sendLEDState(const char* state1, const char* state2) {
    String url = "http://"+String(nodeServer)+"/get-led-state/"+String(espName)+"?ledState=" + String(state1)+";"+String(state2);
    
    WiFiClient client;
    if (client.connect(nodeServer, 3000)) {
        client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                     "Host: "+String(nodeServer)+"\r\n" +
                     "Connection: close\r\n\r\n");
        client.stop();
    }
}


