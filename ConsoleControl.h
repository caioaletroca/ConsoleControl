#ifndef __CONSOLE_CONTROL__
#define __CONSOLE_CONTROL__

#include "Arduino.h"
#include "StandardCplusplus.h"
#include "map"
#include "vector"

#define LOGIN_COMMAND "login"
#define LOGOUT_COMMAND "logout"

class ConsoleControl
{
	typedef void(*CommandFunction)(const String, std::vector<String>);

	public:
		typedef std::vector<String> CommandArgs;

		ConsoleControl(HardwareSerial &hs, uint32_t baud, String pw);
		
		void loop(void);
		void on(String command, CommandFunction function);
		void remove(String command);
		void log(String line);

	private:
		//Properties
		bool authorized = false;
		HardwareSerial *serial;
		String password;
		std::map<String, CommandFunction> _commands;
		
		//Methods
		void compile(String command);
		void login(String command, CommandArgs args);
		void logout(String command);
		void reset(String command);
		std::vector<String> explode(String string);
};

#endif