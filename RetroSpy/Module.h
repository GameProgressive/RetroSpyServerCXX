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
#ifndef RETROSPY_MODULE_H
#define RETROSPY_MODULE_H

#include "../common/ModuleDef.h"

// Include pthreads/WINAPI threading
#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
#else
	#include <pthread.h>
#endif

#include <string>
#include <map>

// MAX_PATH in Windows
#define MAX_MODULENAME 255

// Callbacks and typedef
typedef int (* Module_EntryPoint)(void*);
typedef bool (* Module_ConfigFunction)(ModuleConfigMap);

/*
	This class rappresents a dynamic module that will
	be loaded into RetroSpy Main Server
*/
class CModule
{
public:
	CModule();
	~CModule();

	/*
		Function: Load
		Descrption: Load a dynamic module
		Parameters: 
			name => Name of the module to be loaded (without .dll/.so)
		
		Return: true if the library is loaded
	*/
	bool Load(const char *name);
	
	/*
		Function: Load
		Descrption: Load a dynamic module
		Parameters: 
			name => Name of the module to be loaded (without .dll/.so)
			cfg => A map to the configuration parameters
		
		Return: true if the library is loaded
	*/
	bool Load(const char *name, ModuleConfigMap cfg);

	/*
		Function: Start
		Description: Starts the module
	*/
	void Start();

	/*
		Function: Stop
		Description: Stop the module
	*/
	void Stop();

	/*
		Function: IsRunning
		Descrption: Checks if the module is running
		Return: true if the module is running, otherwise false
	*/
	bool IsRunning();

	/*
		Function: GetThreadID
		Description: Get the thread ID
		Return: The thread ID
	*/
#ifdef _WIN32
	DWORD getThreadID();
#else
	pthread_t getThreadID();
#endif

	/*
		Function: GetName
		Description: Get the thread name
		Return: the thread name
	*/
	const char *GetName();
	
	const char *GetDatabaseStatus();

private:
	char m_szName[MAX_MODULENAME+1];

	Module_EntryPoint m_cbMain;
	Module_ConfigFunction m_cbConfig;

	ModuleMain m_module;

	bool m_bRunning;

#ifdef _WIN32
	HANDLE m_handle;
	DWORD m_threadID;
	HMODULE m_lpThread;
#else
	pthread_t m_threadID;
	void *m_lpThread;
#endif

	MYSQL m_connection;
	bool m_database_disabled;
};

#endif
