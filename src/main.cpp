#include <WiFi.h>
#include <FS.h>
#include <WebServer.h>
#include <esp_wifi.h>
#include <tcpip_adapter.h>

const char *ssid = "ESP32-AP";
const char *password = "manuel123";

WebServer server(80);

String client1IP = "";
String client2IP = "";

void handleRoot()
{
  server.send(200, "text/plain", "Hello, this is ESP32 server!");
}

void sendToClient(String clientIP, String message)
{
  WiFiClient client;
  if (client.connect(clientIP.c_str(), 80))
  {
    String postRequest = "POST / HTTP/1.1\r\n";
    postRequest += "Host: " + clientIP + "\r\n";
    postRequest += "Content-Type: text/plain\r\n";
    postRequest += "Content-Length: " + String(message.length()) + "\r\n";
    postRequest += "\r\n";
    postRequest += message;

    client.print(postRequest);
    client.stop();
  }
  else
  {
    Serial.println("Failed to connect to client " + clientIP);
  }
}

void handlePost()
{
  if (server.method() == HTTP_POST)
  {
    String message = server.arg("plain");
    String senderIP = server.client().remoteIP().toString();
    Serial.println("Received message from " + senderIP + ": " + message);

    // Relay the message to the other client
    if (senderIP == client1IP && client2IP != "")
    {
      sendToClient(client2IP, message);
    }
    else if (senderIP == client2IP && client1IP != "")
    {
      sendToClient(client1IP, message);
    }

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

  // Update the list of connected clients
  int numStations = WiFi.softAPgetStationNum();
  wifi_sta_list_t stationList;
  tcpip_adapter_sta_list_t adapterList;

  if (esp_wifi_ap_get_sta_list(&stationList) == ESP_OK)
  {
    if (tcpip_adapter_get_sta_list(&stationList, &adapterList) == ESP_OK)
    {
      for (int i = 0; i < adapterList.num; i++)
      {
        String clientIP = ip4addr_ntoa((const ip4_addr_t *)&adapterList.sta[i].ip); // Cast to ip4_addr_t
        if (client1IP == "")
        {
          client1IP = clientIP;
        }
        else if (client2IP == "" && clientIP != client1IP)
        {
          client2IP = clientIP;
        }
      }
    }
  }

  Serial.print("Number of stations connected to AP: ");
  Serial.println(numStations);
  delay(1000);
}