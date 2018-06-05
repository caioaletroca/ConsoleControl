#include <ConsoleControl.h>

ConsoleControl console(Serial, 9600, "password");

void startConsole(void) {
  console.on("example", [] (String cmd, ConsoleControl::CommandArgs args) -> void {
    //Do stuff here
  });
}

void setup() {
  Serial.begin(9600);
  startConsole();
}

void loop() {
  console.loop();
}
