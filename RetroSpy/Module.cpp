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
#include "Module.h"
#include "Config.h"

#include <stdio.h>

CModule::CModule()
{
	m_bRunning = false;

	m_threadID = 0;
	m_lpThread = NULL;
	
	m_cbConfig = NULL;
	m_cbMain = NULL;

#ifdef _WIN32
	m_handle = NULL;
#endif
}

CModule::~CModule()
{
	Stop();
	
	// Free Dynamic Library
	if (m_lpThread)
	{
#ifdef _WIN32
		FreeLibrary(m_lpThread);
#else
		dlclose(m_lpThread);
#endif
	}
}

bool CModule::Load(const char *name, ModuleConfigMap cfg)
{
	m_module.cfg = cfg;

	return Load(name);
}


bool CModule::Load(const char *name)
{
	std::string dllName = name;

#ifdef _WIN32
	dllName += ".dll";
#else
	dllName += ".so";
#endif
	
	// Save the name
	strcpy_s(m_szName, MAX_MODULENAME, name);

	// Load the Dynamic library
#ifdef _WIN32
	m_lpThread = LoadLibrary(dllName.c_str());
#else
	m_lpThread = dlopen(dllName.c_str(), RTLD_LAZY);
#endif

	if (!m_lpThread)
	{
		printf("[Module] Cannot find library %s\n", m_szName);
		return false;
	}

	// Load the functions
#ifdef _WIN32
	m_cbMain = (Module_EntryPoint)GetProcAddress(m_lpThread, "RetroSpyMain");
#else
	m_cbMain = (Module_EntryPoint)dlsym(m_lpThread, "RetroSpyMain");
#endif

	if (!m_cbMain)
	{
		printf("[Module] Invalid library %s\n", m_szName);
		return false;
	}

	return true;
}

void CModule::Start()
{
	m_module.default_ip = (char*)CConfig::GetDefaultIP();

	// Create the module thread
#ifdef _WIN32
	m_handle = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)m_cbMain, (void*)&m_module, 0, &m_threadID);

	if (m_handle)
		m_bRunning = true;
#else
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	
	if (!pthread_create(&m_threadID, &attr, (start_routine)m_cbMain, (void*)&m_module))
		m_bRunning = true;
#endif
}

void CModule::Stop()
{
	if (!m_bRunning)
		return;

	m_bRunning = false;

	// Terminate the thread
#ifdef _WIN32
	TerminateThread(m_handle, 0);
	m_handle = NULL; //SET THIS TO NULL OTHERWISE THE APP WILL CRASH
#else
	pthread_kill(m_threadID, SIGKILL);
#endif
	m_threadID = 0;
}

#ifdef _WIN32
DWORD CModule::getThreadID()
#else
pthread_t CModule::getThreadID()
#endif
{
	return m_threadID;
}

bool CModule::IsRunning()
{
	return m_bRunning;
}

const char *CModule::GetName()
{
	return m_szName;
}
