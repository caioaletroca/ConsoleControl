#include "ConsoleControl.h"

ConsoleControl::ConsoleControl(HardwareSerial &hs, uint32_t baud, String pw) : serial(&hs) {
	serial->begin(baud);
	serial->setTimeout(1000);
	this->password = pw;
}

/**
 * The main console control loop
 */
void ConsoleControl::loop(void) {
	if(this->serial->available()) {
		String command = this->serial->readString();
		this->serial->println(String("[INFO] Command: ") + command);

		this->compile(command);
	}
}

/**
 * Compile and run a single command
 * @param command The command as a string
 */
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

/**
 * Registers a new command to the console
 * @param command  The command string
 * @param function The action to execute for that command
 */
void ConsoleControl::on(String command, CommandFunction function) {
	_commands[command] = function;
}

/**
 * Removes a command from the console
 * @param command The command as string
 */
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

/**
 * The login action methods
 * @param cmd  The command
 * @param args The arguments
 */
void ConsoleControl::login(String cmd, CommandArgs args) {
	if(args.size() > 1) {
		if(args[1] == this->password) {
			this->serial->println("[INFO] You have developer rights");
			this->authorized = true;
		}
		else
			this->serial->println("[ERROR] Authentication failed...");
	}
}

/**
 * The logout action method
 * @param cmd The command
 */
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

/**
 * Explodes a string in different params
 * @param  string The string to explode
 * @return        [description]
 */
ConsoleControl::CommandArgs ConsoleControl::explode(String string) {
	
	// Usable variables
	int pos = 0;
	int spacePos = 0;
	int quotePos = 0;
	String current = "";
	CommandArgs exploded;

	// Stop when not found any special chars
	while(spacePos >= 0 || quotePos >= 0) {

		// Get the new chars positions
		spacePos = string.indexOf(' ', pos);
		quotePos = string.indexOf('\"', pos);

		// Get quote message mode
		if(quotePos >= 0 && (quotePos < spacePos || spacePos < 0)) {

			// Get the second double quote on the string
			int secondQuotePos = string.indexOf('\"', quotePos + 1);

			// If not find the next quote, throw error
			if(secondQuotePos < 0) {
				Serial.println("[ERROR] Could not found the proper double quote, error on arguments formation.");
				return exploded;
			}

			// Get the string
			current = string.substring(quotePos + 1, secondQuotePos);

			// Add to the array
			exploded.push_back(current);

			// Jump to the next position
			pos = secondQuotePos + 2;

		// Space mode
		} else if(spacePos >= 0) {

			// Get the string
			current = string.substring(pos, spacePos);

			// Add to the array
			exploded.push_back(current);

			// Jump to the next position
			pos = spacePos + 1;
		}

		// Serial.println(spacePos);
		// Serial.println(quotePos);
	}

	// Add the final of the string
	if(pos < string.length())
		exploded.push_back(string.substring(pos));

	for(int i = 0; i < exploded.size(); i++) {
      	log(exploded[i]);
    }

	return exploded;
}