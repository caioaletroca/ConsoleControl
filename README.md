# ConsoleControl &middot; [![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)

## Installation

### If you are using boards Arduino Nano, Uno or Mega series, you must install first the [Stardard C++ Arduino](https://github.com/maniacbug/StandardCplusplus) to use this library!!!

Just download both libraries with "download as zip" from GitHub and install on Arduino IDE.

## Quick Usage
To quick usage ConsoleControl you must install Stardard C++ Arduino, initialize the console with a user's password, register the required commands and call *ConsoleControl::loop* function in arduino's loop as the example above. You may use inline C++ functions as you wish:
```c
ConsoleControl console(Serial, 9600, "password")

void setup() {
	Serial.begin(9600);
	console.on("example", [] (String cmd, ConsoleControl::CommandArgs args) -> void {
		//do stuff
	});
}

void loop() {
	console.loop();
}
```

ConsoleControl::CommandArgs is a std::vector and his methods is identical as stardard C++.

## Functions

### ConsoleControl(Serial, int baud_rate, String password)
Starts the console's functionality and defines a security password.
### ConsoleControl::loop()
Is required to call that function in arduino's loop. This function waits for data on Serial port and run the command.
### ConsoleControl::on(command, callback)
binds a function to an command call. The callback receives a String containing the command name, and a [std::vector](http://www.cplusplus.com/reference/vector/vector/) with the arguments.
##### Parameter
```command``` name of the command
```callback``` callback function
##### Callback
Function signature must be
```c
void (const String, std::vector<String>)
```
##### Example
```c
void callback(const String cmd, std::vector<String> args) {
	//do stuff
}
console.on("example", callback);
```
##### Default commands:
* `login` - takes password as parameter. After login, gives user authorization to run other commands
* `logout` - logout user and removes rights
* `reset` - software reset arduino
### ConsoleControl::remove(command)
removes command from commands list. Useful if you want to disable any command at runtime.

## Example
* See the [SimpleExample](examples/SimpleExample/SimpleExample.ino) for more information.

## Supported and Tested boards
* NANO
* UNO
* MEGA 1280 and MEGA 2560
* NODEMCU and ESP8266 series
