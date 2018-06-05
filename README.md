# ConsoleControl

## Installation

### You must install first the [Stardard C++ Arduino](https://github.com/maniacbug/StandardCplusplus) to use this library!!!

Just download both libraries with "download as zip" from GitHub and install on Arduino IDE.

## Functions

### ConsoleControl(Serial, int baud_rate, String password)
Starts the console's functionality and defines a security password.

### ConsoleControl::on(command, callback)
binds a function to an command call. The callback receives a String containing the command name, and a [std::vector](http://www.cplusplus.com/reference/vector/vector/) with arguments.
##### Parameter
```event``` name of the command
```callback``` callback function
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

## Example
* See the [SimpleExample](examples/SimpleExample/SimpleExample.ino) for more information.
