#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h> // Use <<<!nav>>WiFiServer<<!/nav>> for older Arduino IDE versions
#include "webscripts.h"

const byte DNS_PORT = 53;
const char* ssid = "ESP32_AP"; // Your network name
const char* password = ""; // Your network password
IPAddress apIP(192, 168, 1, 1);

#define LED_BUILTIN 2 // common for led are 2 & 4

DNSServer dnsServer;
WebServer server(80); // Web server on port 80

// HTML content to display on the web page
String getHtml() {
  /*
  String ptr = "<!DOCTYPE html> <html> <head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\"> <title>ESP32 AP</title>";
  ptr += "<style> html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}";
  ptr += "body{padding-top: 50px;} h1 {color: #444444;margin: 50px auto 20px;} h3 {color: #444444;margin-bottom: 50px;}";
  ptr += ".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 80px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}";
  ptr += ".button-red {background-color: #e74c3c;}";
  ptr += "</style> </head>";
  ptr += "<body> <h1>ESP32 Access Point</h1>";
  ptr += "<h3><a href=\"/led/on\"><button class=\"button\">ON</button></a></h3>";
  ptr += "<h3><a href=\"/led/off\"><button class=\"button button-red\">OFF</button></a></h3>";
  ptr += "</body></html>";
  return ptr;
  */
  return response001;
}

void handleRoot() {
  server.send(200, "text/html", getHtml());
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  delay(100);

  Serial.println();
  Serial.print("Setting up AP...");

  // Set up Access Point mode
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssid, password);

  Serial.println(" AP setup complete.");
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP()); // Print the IP address of the ESP32 AP

  // Start the DNS server to redirect all requests
  dnsServer.start(DNS_PORT, "*", apIP); // Redirect all domains to this IP

  // Define web server routes
  server.on("/", handleRoot); // Handle requests to the root path
  server.on("/led/on", []() { // Handle requests to turn LED on
    digitalWrite(LED_BUILTIN, HIGH);
    server.send(200, "text/html", getHtml());
  });
  server.on("/led/off", []() { // Handle requests to turn LED off
    digitalWrite(LED_BUILTIN, LOW);
    server.send(200, "text/html", getHtml());
  });
  server.onNotFound([]() {
    server.send(200, "text/html", getHtml());
  });

  server.begin(); // Start the web server
  Serial.println("HTTP server started");

  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  dnsServer.processNextRequest(); // Handle DNS requests
  server.handleClient(); // Handle incoming client connections
}