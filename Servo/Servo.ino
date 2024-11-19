// Test ran and notes written by Nex_isDumb
#include <Servo.h> // Including the servomotors' lib

Servo motor;
int i;

void setup() {
  motor.attach(9); // Using Pin 9 to control the servomotor(s)
}

void loop() {
  delay(1000); // Pauses for 1000 ms Arduino's program

  for(i = 180; i > 65; i--) {
    motor.write(i); // Makes the servomotors' shafts rotate (on standard types this usually sets the shafts' angle)
    delay(25);  // Stops for 25 ms or else it'd rotate too fast
  }


  // This cycle does the same but in the opposite direction because of i incrementing instead of decrementing
  for(i = 65; i < 180; i++) {
    motor.write(i);
    delay(25);
  }
}