// Test ran by Nex_isDumb
// NOTE: Based a bit on Arduino Docs examples
#include <WiFiS3.h>

inline void BLOCK() {  // Inline will make the compiler replace directly the call of this function with what's inside this function making runtime really fast, basically like a constant!!
  while (true);        // Makes the Arduino be stuck in an infinite loop so it won't continue
}

int status = WL_IDLE_STATUS;

void setup() {
  Serial.begin(9600);  // Starting the Serial Port
  delay(3000);

  if (WiFi.status() == WL_NO_MODULE) {  // In case the communication with the WiFi module failed
    Serial.println("Communication with WiFi module failed!");
    BLOCK();
  }

  if (WiFi.firmwareVersion() < WIFI_FIRMWARE_LATEST_VERSION) {  // Just a reminder to update
    Serial.println("You should upgrade your Firmware...");
    Serial.println();
  }

  scanNetworks();
}

void loop() {
  if (Serial.available() > 0) {
    String msg = getCoolMessage();

    if (msg == "scan") {
      Serial.println();
      scanNetworks();
    } else {
      int cur = msg.toInt() - 1;
      if (cur > -1) {
        String name = WiFi.SSID(cur);
        Serial.println();
        Serial.print("Please insert the password for the network ");
        Serial.println(name);

        while (Serial.available() <= 0);  // Waits for the password message

        String password = Serial.readStringUntil('\n');
        Serial.print("Inserted password: ");
        Serial.println(password);
        Serial.println("Please wait at least 10s...");
        Serial.println();

        status = WiFi.begin(name.c_str(), password.c_str());  // Tries to connect...
        delay(10000);  // Waits 10 seconds for connection

        if (status == WL_CONNECTED) {  // Connection established!!
          Serial.println("Connection established!");
          printCurrentNet();
          printWiFiData();

          Serial.println();
          Serial.println("Type Disconnect whenever you're ready to disconnect from the network and restart the test.");

          while (true) {
            if (Serial.available() <= 0 || getCoolMessage() != "disconnect") continue;

            Serial.println("Disconnected, restarting the test.");
            Serial.println();
            disonnectNscan();
            break;
          }
        } else {
          Serial.println("Failed to connect, please try again...");  // In case it fails to connect.. wrong password maybe?? check again!
          Serial.println();
          disonnectNscan();
        }
      }
    }
  }
}

String getCoolMessage() {
  String msg = Serial.readStringUntil('\n');
  msg.toLowerCase();
  msg.trim();
  return msg;
}

void disonnectNscan() {
  WiFi.disconnect();
  status = WL_IDLE_STATUS;
  scanNetworks();
}

void scanNetworks() {
  byte mac[6];
  listNetworks();
  WiFi.macAddress(mac);
  Serial.print("Your MAC Address is: ");
  printMacAddress(mac);
  Serial.println("Write the number of the router you'd like connecting to or the word Scan for scanning again the nearby networks.");
}

void listNetworks() {
  Serial.println("Scanning available networks...");
  int numSsid = WiFi.scanNetworks();
  if (numSsid == -1) {  // Bad error check with the module
    Serial.println("Couldn't get a WiFi connection");
    BLOCK();
  }

  Serial.print("Number of available networks: ");
  Serial.println(numSsid);

  for (int thisNet = 0; thisNet < numSsid; thisNet++) {  // Printing every networks and their infos
    Serial.print(thisNet + 1);
    Serial.print(") ");
    Serial.print(WiFi.SSID(thisNet));
    Serial.print("   Signal: ");
    Serial.print(WiFi.RSSI(thisNet));
    Serial.print(" dBm");
    Serial.print("   Encryption: ");
    printEncryptionType(WiFi.encryptionType(thisNet));
  }
}

void printEncryptionType(int thisType) {  // For printing the encryption type
  switch (thisType) {
    case ENC_TYPE_WEP:
      Serial.println("WEP");
      break;
    case ENC_TYPE_WPA:
      Serial.println("WPA");
      break;
    case ENC_TYPE_WPA2:
      Serial.println("WPA2");
      break;
    case ENC_TYPE_WPA3:
      Serial.print("WPA3");
      break;   
    case ENC_TYPE_NONE:
      Serial.println("None");
      break;
    case ENC_TYPE_AUTO:
      Serial.println("Auto");
      break;
    case ENC_TYPE_UNKNOWN:
    default:
      Serial.println("Unknown");
      break;
  }
}

void printCurrentNet() {  // For printing the Network data
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());  // Prints the Network's SSID

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("BSSID: ");
  printMacAddress(bssid);  // Prints the Network's BSSID

  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI): ");
  Serial.print(rssi);  // Prints the Network's RSSI
  Serial.println(" dBm");

  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type: ");
  Serial.println(encryption, HEX); // Prints the Network's Encryption Type
}

void printWiFiData() {  // For printing WiFi infos
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());  // Prints the board's IP address

  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  printMacAddress(mac);  // Prints the board's MAC address
}

void printMacAddress(byte mac[]) {  // For printing the MAC Address
  for (int i = 0; i < 6; i++) {
    if (i > 0) {
      Serial.print(":");
    }
    if (mac[i] < 16) {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
  }
  Serial.println();
}
