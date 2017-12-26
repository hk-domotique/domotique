#include <OneWire.h>

#include <ArduinoHttpClient.h>

#include <ESP8266WiFi.h>

//#define DIMMABLE_LED_PIN 9

// Termometar
OneWire  ds(4);  // on pin 10 (a 4.7K resistor is necessary)

// Web socket server address
char serverAddress[] =  "10.241.75.59"; // "echo.websocket.org";  // server address  //
int port = 3333;

// web socket client
WiFiClient wifi;

WebSocketClient socketClient = WebSocketClient(wifi, serverAddress, port);

int status = WL_IDLE_STATUS;

// wifi setup
const char* ssid = "InterVenture-MOB";
const char* password = "iv11000mob";

const char* type = "temp";

int ledPin = D5;
int blah = A0;
//WiFiServer server(80);


void setup() {
  Serial.begin(115200);
 
  pinMode(ledPin, OUTPUT);
  pinMode(blah, OUTPUT);
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

  //led light status
  int value = LOW;
  
 void onOff(int val){
    digitalWrite(ledPin, val);
      socketClient.beginMessage(TYPE_TEXT);
      if(val == LOW) {
        socketClient.print("ledoff");
        value = LOW;
      }
      else if (val == HIGH){
        socketClient.print("ledon");
        value = HIGH;
      }
      socketClient.endMessage();
  }

// send initial msg
void initialMsg(){
    Serial.println("Sending initial message.");
    Serial.println(TYPE_TEXT);
    socketClient.beginMessage(TYPE_TEXT);
    socketClient.print(WiFi.localIP());    
    socketClient.endMessage();
    onOff(value);
}

  float readTemperatureData(){
      // Termostat
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius;

  if ( !ds.search(addr)) {
    ds.reset_search();
    delay(250);
    return 0;
  }

  if (OneWire::crc8(addr, 7) != addr[7]) {
    Serial.println("CRC is not valid!");
    return 0;
  }
 
 // the first ROM byte indicates which chip
  switch (addr[0]) {
    case 0x10:
      type_s = 1;
      break;
    case 0x28:
      type_s = 0;
      break;
    case 0x22:
      type_s = 0;
      break;
    default:
      Serial.println("Device is not a DS18x20 family device.");
      return 0;
  } 

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end
  
  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
  
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
  }
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }
  celsius = (float)raw / 16.0;
  return celsius;
 }


void loop() {
  
  while (!socketClient.connected()) {
    Serial.print("Websocket client not conenct. Trying to reconnect ");
    Serial.print(".");delay(1000);Serial.print(".");delay(1000);Serial.print(".");delay(1000);
    connectWebSocket();
  }

//  while (socketClient.connected()) {
  if(socketClient.connected()) {
    float temp = readTemperatureData();
    if(temp > 1){
      Serial.print("Current temperature: ");
      Serial.println(temp);
      socketClient.beginMessage(TYPE_TEXT);
      socketClient.print("temp");
      socketClient.print(":");
      socketClient.print(temp);
      socketClient.endMessage();
    }

    // dimmable
    analogWrite(blah, 1023);

    // check if a message is available to be received
    int messageSize = socketClient.parseMessage();
    if (messageSize > 0) {
      String text = socketClient.readString();
      Serial.println("Received a message:");
      Serial.println(text);
      
      if(text == "on") onOff(HIGH);
      else if(text == "off") onOff(LOW);
    }
  }
  
}
