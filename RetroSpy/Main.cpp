/*
    This file is part of RetroSpy Server.

    RetroSpy Server is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    RetroSpy Server is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with RetroSpy Server.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "ModuleManager.h"
#include "RConfig.h"
#include "Database.h"

#include "../common/Helper.h"

#include <iostream>
#include <string>

#include <stdlib.h>
#include <time.h>

#define CONFIG_NAME "RetroSpy.ini"

// Help defines
#define MAX_CMDSTR 100
#define MAX_CMDDSC 300

typedef struct THelp
{
	char name[MAX_CMDSTR];
	char desc[MAX_CMDDSC];
} SHelp;

void ConsolePause();

// Commands
void _PrintHelp();
void _PrintModuleInfo(CModuleManager mngr);

bool Main_Running = true;

// Help Command
const SHelp HelpArray[] = {
	{ "quit",				"Close the Server and exit" },
	{ "help",				"Print this information screen" },
	{ "moduleinfo",			"Print the information about the modules" }
};
const int HelpArray_Len = 3;

/*
	Function: Main
	Description: Entry Point of the Server
*/
int main()
{
	// Create the main instances
	CModuleManager mm;
	CDatabase db;
	
	// We use this variable for console input
	// I defined this here, so we won't create
	// a variable each call of the loop
	std::string consoleInput = "";

	Util_RandSeed((unsigned int)time(NULL));

	printf("RetroSpy Server By GameProgressive\n");
	
	// Load the configuration
	if (!CConfig::Load(&mm, CONFIG_NAME))
	{
		LOG_ERROR("RetroSpy", "Cannot load %s\n", CONFIG_NAME);
		ConsolePause();
		return 0;
	}

/*	if (!db.Connect())
	{
		ConsolePause();
		return 0;
	}

	LOG_INFO("RetroSpy", "Connected to Database!\n");*/

	// Start our modules
	mm.Start();

	LOG_INFO("RetroSpy", "Server Started!");
	LOG_INFO("RetroSpy", "Type \"help\" for the list of commands avaiable.");

	// Application loop
	while (Main_Running)
	{
		consoleInput.clear();

		// Read the input and process that
		std::cin >> consoleInput;

		if (strcmp(consoleInput.c_str(), "quit") == 0)
			Main_Running = false;
		else if (strcmp(consoleInput.c_str(), "help") == 0)
			_PrintHelp();
		else if (strcmp(consoleInput.c_str(), "moduleinfo") == 0)
			_PrintModuleInfo(mm);
		else
			LOG_WARN("RetroSpy", "Unknown command \"%s\"", consoleInput.c_str());
	}

	// Stop the modules
	mm.Stop();

	ConsolePause();
	return 0;
}

/*
	Function: _PrintHelp
	Description: Prints the command list
*/
void _PrintHelp()
{
	int i = 0;

	LOG_INFO("RetroSpy", "List of Avaiable Commands:");
	
	for (; i < HelpArray_Len; i++)
	{
		LOG_INFO("RetroSpy", "%s\t\t%s", HelpArray[i].name, HelpArray[i].desc);
	}
}

/*
	Function: _PrintModuleInfo
	Description: Print the status of the modules
*/
void _PrintModuleInfo(CModuleManager mngr)
{
	LOG_INFO("RetroSpy", "Loaded modules: %u", mngr.GetModuleSize());

	if (mngr.GetModuleSize() > 0)
	{
		size_t i = 0;
		for (; i < mngr.GetModuleSize(); i++)
		{
			CModule *m = mngr.GetModule(i);
			LOG_INFO("RetroSpy", "Module %u: %s\tStatus: %s (Code: %d)\t\tDatabase Status: %s", i, m->GetName(), m->IsRunning() ? "Running" : "Stopped", m->GetExitCode(), m->GetDatabaseStatus());
		}
	}
}

/*
	Function: ConsolePause
	Descrption: Simulate "pause" command of Windows
*/
void ConsolePause()
{
	char k = 0;

	printf("Press ENTER to exit...");

#if _WIN32 && _MSC_VER >= 1400
	scanf_s("%1c", &k, 1);
#else
	scanf("%1c", &k);
#endif

	getchar();

}
