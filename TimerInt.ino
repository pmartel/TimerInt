// Phil Martel noodling around with timer interrupts

#include <Streaming.h> // This lets Serial use the C++ '<<' operator
void setup() {
  // put your setup code here, to run once:
  pinMode( LED_BUILTIN, OUTPUT );
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite( LED_BUILTIN, !digitalRead( LED_BUILTIN ) );
  delay(500);
}
