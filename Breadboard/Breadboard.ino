// Test ran by Nex_isDumb
// NOTE: If you place one MORE resistance the electricity is gonna be low and the LED won't light that much, if you REMOVE one you'll generate a short circuit!!

const int LED_PIN = 2;     // The LED's pin
const int BUTTON_PIN = 3;  // The Button's Pin

void setup() {
  pinMode(LED_PIN, OUTPUT);    // LED pin as output
  pinMode(BUTTON_PIN, INPUT);  // Button Pin as an input
}

void loop() {
  digitalWrite(LED_PIN, digitalRead(BUTTON_PIN));  // Setting the value of the LED's Pin the same as the Button one
}