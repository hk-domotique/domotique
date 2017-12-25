#include <dht.h>

#include <ArduinoHttpClient.h>

#include <ESP8266WiFi.h>

#define dht_apin A0 // Analog Pin sensor is connected to
 
dht DHT;

// Web socket server address
char serverAddress[] =  "192.168.2.102"; // "echo.websocket.org";  // server address  //
int port = 8080;

// web socket client
WiFiClient wifi;
WebSocketClient socketClient = WebSocketClient(wifi, serverAddress, port);
int status = WL_IDLE_STATUS;
int count = 0;

// wifi setup
//const char* ssid = "InterVenture-MOB";
//const char* password = "iv11000mob";
const char* ssid = "pjolep";
const char* password = "222bmw22";

const char* type = "light";

int ledPin = D5;
WiFiServer server(80);


void setup() {
  // Serial.begin(115200);
   Serial.begin(9600);
  delay(1000);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.print("Use this URL : ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

  connectWebSocket();
}

// connect to websocket server
void connectWebSocket(){
  Serial.println("starting WebSocket client");
  socketClient.begin();
  if(socketClient.connected()){
    Serial.println("Socket client connected.");
    initialMsg();
  }
}

// send initial msg
void initialMsg(){
    Serial.println("Sending initial message.");
    Serial.println(TYPE_TEXT);
    socketClient.beginMessage(TYPE_TEXT);
    socketClient.print(type);
    socketClient.print("|");
    socketClient.print(WiFi.localIP());    
    socketClient.endMessage();
    digitalWrite(ledPin, HIGH);
}

  //led light status
  int value = LOW;

void loop() {
  
  // Check if a client has connected
  WiFiClient client = server.available();
//  if (!client) {
//    return;
//  }

  while (!socketClient.connected()) {
    Serial.print("Websocket client not conenct. Trying to reconnect ");
    Serial.print(".");delay(1000);Serial.print(".");delay(1000);Serial.print(".");delay(1000);
    digitalWrite(ledPin, LOW);
    connectWebSocket();
  }

//
  while (socketClient.connected()) {
    Serial.print("Sending hello ");
    socketClient.beginMessage(TYPE_TEXT);
    socketClient.print(WiFi.localIP());
    socketClient.endMessage();

    // check if a message is available to be received
    int messageSize = socketClient.parseMessage();
//    Serial.print("Received a message size: ");
//    Serial.println(messageSize);
    if (messageSize > 0) {
      Serial.println("Received a message:");
      Serial.println(socketClient.readString());
    }
    DHT.read11(dht_apin);

    Serial.print("Current humidity = ");
    Serial.print(DHT.humidity);
    Serial.print("%  ");
    Serial.print("temperature = ");
    Serial.print(DHT.temperature); 
    Serial.println("C  ");
 
//
//    // wait 5 seconds
    delay(5000);
  }

//  // Wait until the client sends some data
//  Serial.println("new client");
////  while (!client.available()) {
////    delay(1);
////  }
//
//  // Read the first line of the request
//  String request = client.readStringUntil('\r');
//  Serial.println(request);
//  client.flush();
//
//  // Match the request
//
  int value = LOW;
//  if (request.indexOf("/LED=ON") != -1) {
//    digitalWrite(ledPin, HIGH);
//    value = HIGH;
//  }
//  if (request.indexOf("/LED=OFF") != -1) {
//    digitalWrite(ledPin, LOW);
//    value = LOW;
//  }
//


  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");

  client.print("Led pin is now: ");

  if (value == HIGH) {
    client.print("On");
  } else {
    client.print("Off");
  }
  client.println("<br><br>");
  client.println("Click <a href=\"/LED=ON\">here</a> turn the LED on pin 5 ON<br>");
  client.println("Click <a href=\"/LED=OFF\">here</a> turn the LED on pin 5 OFF<br>");
  client.println("</html>");

  delay(1);
  Serial.println("Client disconnected");
  Serial.println("");

}
