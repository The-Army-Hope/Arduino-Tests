// Test ran and notes written by Nex_isDumb
// NOTE: Some of the base testing code was taken from here https://github.com/misan/UNOR4text/ but it could have been also done easily through the ArduinoGraphics lib

#include "font8x8_basic.h"
#include "Arduino_LED_Matrix.h"

const float WAIT_TIME = 50;
ArduinoLEDMatrix matrix;
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
  Serial.begin(9600); // This will make the arduino start the serial port connection on the 9600 baud (You'll have to select 9600 baud in Arduino's Serial Monitor too!)
  delay(3000); // Since it takes a little for the serial connection to start lets make the arduino wait 3 seconds before sending the first message
  Serial.println("Hey ho are you there??");

  matrix.begin();
}

// Lets make the arduino display what we send as a message!
void loop() {
  if (Serial.available() > 0) { // Checking if there's any incoming bits from the serial port
    String text = " " + Serial.readStringUntil('\n'); + " ";  // Reading until there's a new line and placing two spaces at the start and at the end for better visualization
    Serial.println("Writing:" + text);

    for (int i = 0; i < text.length(); i++)
      for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++)
          frame[col][11] = (font8x8_basic[text[i]][col] >> row) & 1; // Displaying the font: I need to rotate colums as font is given as rows instead of columns

        left_scroll();
      }
  }

  left_scroll();
}

void left_scroll() { // Moves the message on the left by one pixel and then waits [WAIT_TIME] ms
  for(int col = 0; col < 11; col++)
    for(int row = 0; row < 8; row++)
      frame[row][col] = frame[row][col+1];

  matrix.renderBitmap(frame, 8, 12);
  delay(WAIT_TIME);
}