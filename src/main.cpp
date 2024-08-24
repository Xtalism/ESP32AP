#include <WiFi.h>
#include <WebServer.h>

const char *ssid = "ESP32-AP";
const char *password = "manuel123";

uint8_t stationNum = WiFi.softAPgetStationNum();

WebServer server(80);

void handleRoot()
{
  server.send(200, "text/plain", "Hello, this is ESP32 server!");
}

void handlePost()
{
  if (server.method() == HTTP_POST)
  {
    String message = server.arg("plain");
    Serial.println(message);
    server.send(200, "text/plain", "Message received");
  }
  else
  {
    server.send(405, "text/plain", "Method Not Allowed");
  }
}

void handleNotFound()
{
  server.send(404, "text/plain", "404: Not found");
}

void setup()
{
  Serial.begin(115200);
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.on("/", HTTP_GET, handleRoot);
  server.on("/", HTTP_POST, handlePost);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop()
{
  server.handleClient();

  while (stationNum != WiFi.softAPgetStationNum())
  {
    stationNum = WiFi.softAPgetStationNum();
    Serial.print("Number of stations connected to AP: ");
    Serial.println(stationNum);
  }
}