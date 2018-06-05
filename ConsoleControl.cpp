#include "ConsoleControl.h"

ConsoleControl::ConsoleControl(HardwareSerial &hs, uint32_t baud, String pw) : serial(&hs) {
	serial->begin(baud);
	serial->setTimeout(1000);
	this->password = pw;
}

void ConsoleControl::loop(void) {
	if(this->serial->available()) {
		String command = this->serial->readString();
		this->serial->println(String("[INFO] Command: ") + command);

		this->compile(command);
	}
}

void ConsoleControl::compile(String command) {
	String cmd = command.substring(0, command.indexOf(" "));
	CommandArgs args = this->explode(command);

	if(cmd.indexOf(LOGIN_COMMAND) >= 0)
		this->login(cmd, args);
	else if(cmd.indexOf(LOGOUT_COMMAND) >= 0)
		this->logout(cmd);
	else if(cmd.indexOf("reset") >= 0)
		this->reset(cmd);
	else {
		if(this->authorized) {
			auto e = _commands.find(cmd);

			if(e != _commands.end()) {
				e->second(cmd, args);
			} else {
				Serial.println("[ERROR] Unknow command");
			}
		}
		else {
			Serial.println("[ERROR] You have not developer rights");
		}
	}
}

void ConsoleControl::on(String command, CommandFunction function) {
	_commands[command] = function;
}

void ConsoleControl::remove(String command) {
	auto e = _commands.find(command);

	if(e != _commands.end()) {
		_commands.erase(e);
	} else {
		Serial.println(String("[ERROR] Command ") + command + String(" can not be removed"));
	}
}

void ConsoleControl::log(String line) {
	this->serial->println(String("[INFO] ") + line);
}

//Internal Commands
void ConsoleControl::login(String cmd, CommandArgs args) {
	if(args.size() > 1) {
		if(args[1] == this->password) {
			this->serial->println("[INFO] You have developer rights");
			this->authorized = true;
		}
		else
			this->serial->println("[ERROR] Failed...");
	}
}

void ConsoleControl::logout(String cmd) {
	if(this->authorized) {
		Serial.println("[INFO] You logout successful");
		this->authorized = false;
	}
}

void ConsoleControl::reset(String cmd) {
	if(this->authorized) {
		Serial.println("[WARNING] Reseting...");
		void(*resetFunction)(void) = 0;
		resetFunction();
	}
}

ConsoleControl::CommandArgs ConsoleControl::explode(String string) {
	CommandArgs exploded;

	char *data = const_cast<char*>(string.c_str());

	char *buffer = strtok(data, " ");
	while(buffer != 0)
	{
		exploded.push_back(String(buffer));
		buffer = strtok(0, " ");
	}

	return exploded;
}