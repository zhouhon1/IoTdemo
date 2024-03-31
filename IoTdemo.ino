#include <DHTStable.h>

#include <ArduinoHttpClient.h>

#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>
#include <WiFiGeneric.h>
#include <WiFiMulti.h>
#include <WiFiScan.h>
#include <WiFiServer.h>
#include <WiFiSTA.h>
#include <WiFiType.h>
#include <WiFiUdp.h>

#define DHTPIN 13 // the GPIO pin that receives signals from DHT11
#define DHTTYPE DHT11

char serverAddr[] = "thingsboard.cloud";
int port = 80;

DHTStable dht;

WiFiClient wifi;
HttpClient client = HttpClient(wifi, serverAddr, port);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
	
	// connect to WiFi
  WiFi.begin("WiFi_network_name", "WiFi_password");
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(".");
  }
  Serial.println("Connected!");
	// show IP address of ESP32 board
  IPAddress ip;
  ip = WiFi.localIP();
  Serial.println(ip);
}

void loop() {
  // put your main code here, to run repeatedly:
	// get temperature reading
  int chk = dht.read11(DHTPIN);
  float t = dht.getTemperature();
  Serial.print("Temp: ");
  Serial.println(t);

  String contentType = "application/json";
  String data = "{\"temperature\":" + String(t) + "}";
  Serial.print("Uploading data: ");
  Serial.println(data);
  client.post("/api/v1/<access_token>/telemetry", contentType, data);

  unsigned int statusCode = client.responseStatusCode();
  Serial.print("Status code: ");
  Serial.println(statusCode);
  String response = client.responseBody();
  Serial.print("Response: ");
  Serial.println(response);
  delay(3000);
}
