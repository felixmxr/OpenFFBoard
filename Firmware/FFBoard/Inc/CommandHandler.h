/*
 * CommandHandler.h
 *
 *  Created on: 03.04.2020
 *      Author: Yannick
 */

#ifndef COMMANDHANDLER_H_
#define COMMANDHANDLER_H_
#include <CmdParser.h>
#include "ClassChooser.h"
#include <set>
#include "mutex.hpp"

enum class ParseStatus : uint8_t {NOT_FOUND,OK,ERR,OK_CONTINUE,NO_REPLY};

/**
 * Implements an interface for parsed command handlers.
 *
 * Adds itself to a global vector of handlers that can be called from the main class when a command gets parsed.
 *
 * For example motor drivers and button sources can implement this to easily get serial commands
 */
class CommandHandler {
public:
	static std::vector<CommandHandler*> cmdHandlers;
	static std::set<uint16_t> cmdHandlerIDs;

	CommandHandler();
	virtual ~CommandHandler();
	virtual bool hasCommands();
	virtual void setCommandsEnabled(bool enable);
	virtual ParseStatus command(ParsedCommand* cmd,std::string* reply);
	virtual const ClassIdentifier getInfo() = 0; // Command handlers always have class infos. Works well with ChoosableClass
	virtual std::string getHelpstring(); // Returns a help string if "help" command is sent
	static void sendSerial(std::string cmd,std::string string,char prefix = 0); // Send a command reply formatted sequence
	static void logSerial(std::string string);	// Send a log formatted sequence

	static bool logEnabled;
	static bool logsEnabled();
	static void setLogsEnabled(bool enabled);
	uint16_t getCommandHandlerID(){return this->commandHandlerID;}

protected:


	bool commandsEnabled = true;
	virtual void addCommandHandler();
	virtual void removeCommandHandler();

	template<typename TVal>
 	bool handleGetSet(ParsedCommand* cmd, std::string *reply, TVal& val) {
 		if (cmd->type == CMDtype::set) {
 			val = TVal(cmd->val);
			return true;
 		} else if (cmd->type == CMDtype::get) {
 			*reply += std::to_string(val);
 		}

		return false;
 	}

	uint16_t commandHandlerID = 0;
	cpp_freertos::MutexStandard cmdHandlerListMutex;

private:

};

#endif /* COMMANDHANDLER_H_ */
