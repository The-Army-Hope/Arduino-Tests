// Test ran and notes written by Nex_isDumb
#include "Arduino_LED_Matrix.h" // Including the board's 12x8 LED Matrix's lib

const float WAIT_TIME = 500;
ArduinoLEDMatrix matrix; // Declaring the matrix obj

// METHOD N1: HARDEST ONE
// This constant declares an array of 32-bit integers that will translate as a drawing of a Happy face
// This is the hardest method to understand which leds are turning on, to understand which arduino left a code snippet online to translate METHOD N1 to METHOD N3
const uint32_t happy[] = {
  0x19819,
  0x80000001,
  0x81f8000
};

// METHOD N2: UNOPTIMIZED YET EASIEST ONE
// This is another way to draw, cleaner and way easier to understand yet less optimized as there are more stored bits (it's a drawing of 11 in roman numbers btw)
// 1 = Red Pixel, 0 = Turned off pixel
byte eleven[8][12] = {
  { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
  { 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0 },
  { 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0 },
  { 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0 },
  { 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0 },
  { 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0 },
  { 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0 },
  { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 }
};

// METHOD N3: BEST ONE
// This method is optimized like METHOD N1 and easy to understand like or maybe even more than METHOD N2 (this specific example draws a heart)
// 1 = Red Pixel, 0 = Turned off pixel here too
// JUST REMEMBER THAT every 12 bits theres a new line that starts with 0b, so for example:
/*
  001100011000
  010010100100
  010001000100
  001000001000
  000100010000
  000010100000
  000001000000
  000000000000
*/
// Becomes this:
const uint32_t heart[] = {
  0b00110001100001001010010001000100,
  0b01000010000010000001000100000000,
  0b10100000000001000000000000000000
};


void setup() {
  matrix.begin(); // Starting the matrix
  // This Arduino offers the possibility to also type stuff on the leds through the ArduinoGraphics library!
}

void loop() {
  // Draws the matrices I made through two functions I made based on the type of those drawings' variables or constants
  drawNwait(happy);
  renderNwait(eleven);
  drawNwait(heart);

  // Arduino offers some already drawn frames! Few examples out of many (check for others in their docs!):
  drawNwait(LEDMATRIX_DANGER);
  drawNwait(LEDMATRIX_EMOJI_BASIC);
  drawNwait(LEDMATRIX_CLOUD_WIFI);
  drawNwait(LEDMATRIX_BLUETOOTH);
  drawNwait(LEDMATRIX_BLUETOOTH);

  // It even offers the possibility to have animations!! Here's an example of one already made by arduino (you can also make custom ones through advanced methods, check the docs):
  matrix.loadSequence(LEDMATRIX_ANIMATION_STARTUP);
  matrix.play(false); // False cause I don't wanna loop it
  while (!matrix.sequenceDone()) yield(); // Makes the program wait for the animation to be completed
}

// Below here the two functions I made for drawing the two different types of matrices (draws and then waits for [WAIT_TIME] ms)

void drawNwait(const uint32_t * buffer) {
  matrix.loadFrame(buffer);
  delay(WAIT_TIME);
}

void renderNwait(byte bitmap[8][12]) {  // Only works with METHOD N2 and not METHOD N1 or METHOD N3
  matrix.renderBitmap(bitmap, 8, 12);  // Wanted to make the argument and the two variables as constants but the lib api wont allow constants goddammit
  delay(WAIT_TIME);
}