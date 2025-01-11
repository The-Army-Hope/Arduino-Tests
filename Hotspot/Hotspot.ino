// Test ran by Nex_isDumb
// NOTE: Based a bit on Arduino Docs examples and most of all on the Serial test I made
#include "WiFiS3.h"
#include "font8x8_basic.h"
#include "Arduino_LED_Matrix.h"

inline void BLOCK() {
  while (true);
}

const char SSID[] = "UNO R4 Test";  // The network's name
const char PASSWORD[] = "This is just a Test!";  // The network's password (use for WPA, or use as key for WEP)

int status = WL_IDLE_STATUS;
WiFiServer server(80);  // Configuring the Web Server on port 80

ArduinoLEDMatrix matrix;
const float WAIT_TIME = 50;
byte frame[8][12] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

void setup() {
  Serial.begin(9600);  // Starting the Serial Port
  delay(3000);

  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    BLOCK();
  }

  if (WiFi.firmwareVersion() < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("You should upgrade your Firmware...");
    Serial.println();
  }

  WiFi.config(IPAddress(192, 48, 56, 2));  // Overriding the default IP (192.168.4.1) with another one just for testing since its possible

  Serial.print("Creating access point for Web Server named ");
  Serial.println(SSID);

  if ((status = WiFi.beginAP(SSID, PASSWORD)) != WL_AP_LISTENING) {  // Creating the network with the password and blocking if an error occurs
    Serial.println("Creating access point failed");
    BLOCK();
  }

  server.begin();  // Starting the Web Server on port 80
  printWiFiStatus();

  matrix.begin();
}

const String _prefix = "GET /value=";  // Internal stuff

void loop() {
  if (status != WiFi.status())  // Checking if the status has changed
    Serial.println((status = WiFi.status()) == WL_AP_CONNECTED ? "Device connected to AP" : "Device disconnected from AP");  // Checks for connected or disconnected devices

  WiFiClient client = server.available();  // Listening for incoming clients
  String received = "";
  if (client) {
    Serial.println("New client!");
    String currentLine = "";  // Holds incoming data from the client
    while (client.connected()) {
      delayMicroseconds(10);  // This is required for the Arduino Nano RP2040 Connect - otherwise it will loop so fast that SPI will never be served.
      if (client.available()) {  // If there are bytes to read from the client
        char c = client.read();
        Serial.write(c);
        if (c == '\n') {
          if (currentLine.length() == 0) {  // If the current line is blank, you got two newline characters in a row. That's the end of the client HTTP request, and so we gotta send a response
            client.println("HTTP/1.1 200 OK");  // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            client.println("Content-type:text/html");  // And a content-type so the client knows what's coming, then a blank line
            client.println();

            client.print("<p>Type in this box what you want to display:<br><input type='text' id='thebox' size='10'> <input type='button' value='Display this!' onclick='window.location.href=\"value=\"+document.getElementById(\"thebox\").value.replace(\" \", \"_\");'></p>");  // The HTTP response (basically the site's body)

            client.println();  // The HTTP response ends with another blank line
            break;
          } else {  // If you got a newline...
            if (currentLine.startsWith(_prefix))
              Serial.println("Value is " + (received = currentLine.substring(_prefix.length(), currentLine.lastIndexOf(" HT"))));  // Here's the received text to display!
            currentLine = "";  // Clears the currentLine
          }
        } else if (c != '\r')
          currentLine += c;
      }
    }

    client.stop();  // Stops the client
    Serial.println("Client disconnected!");

    // Displaying:
    if (received.length() > 0) {
      for (int i = 0; i < received.length(); i++)
        for (int row = 0; row < 8; row++) {
          for (int col = 0; col < 8; col++)
            frame[col][11] = (font8x8_basic[received[i]][col] >> row) & 1;

          left_scroll();
        }

      while (true) {
        bool shouldContinue = false;
        for (int row = 0; row < 12 && !shouldContinue; row++)
          for (int col = 0; col < 8 && !shouldContinue; col++)
            if (frame[col][row] == 1) {
              left_scroll();
              shouldContinue = true;
            }

        if (!shouldContinue) break;  // Continues to left scroll till every LED is off
      }
    }
  }
}

void printWiFiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}

void left_scroll() {
  for(int col = 0; col < 11; col++)
    for(int row = 0; row < 8; row++)
      frame[row][col] = frame[row][col+1];

  matrix.renderBitmap(frame, 8, 12);
  delay(WAIT_TIME);
}
