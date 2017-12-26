
#include <ArduinoHttpClient.h>

#include <ESP8266WiFi.h>


// Web socket server address
char serverAddress[] =  "echo.websocket.org";  // server address  //
int port = 80;

// web socket client
WiFiClient wifi;

WebSocketClient socketClient = WebSocketClient(wifi, serverAddress, port);

int status = WL_IDLE_STATUS;

// wifi setup
const char* ssid = "InterVenture-MOB";
const char* password = "iv11000mob";

const char* type = "light";

int ledPin = D5;
WiFiServer server(80);


void setup() {
  Serial.begin(115200);

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
  
  while (!socketClient.connected()) {
    Serial.print("Websocket client not conenct. Trying to reconnect ");
    Serial.print(".");delay(1000);Serial.print(".");delay(1000);Serial.print(".");delay(1000);
    digitalWrite(ledPin, LOW);
    connectWebSocket();
  }

  while (socketClient.connected()) {
    Serial.print("Sending hello ");
    socketClient.beginMessage(TYPE_TEXT);
    socketClient.print(WiFi.localIP());
    socketClient.endMessage();

    // check if a message is available to be received
    int messageSize = socketClient.parseMessage();
    if (messageSize > 0) {
      Serial.println("Received a message:");
      Serial.println(socketClient.readString());
    }

    delay(2000);
  }
}
