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

CModuleManager::CModuleManager()
{

}

CModuleManager::~CModuleManager()
{
	size_t i = 0;

	// Free memory of the modules
	for (; i < m_vModules.size(); i++)
		delete m_vModules.at(i);

	m_vModules.clear();
}

bool CModuleManager::LoadRetroSpyModule(const char *name)
{
	CModule *module = NULL;

	// Add the pointer into the vector
	m_vModules.push_back(new CModule());

	// Get the module
	module = m_vModules.at(m_vModules.size() - 1);

	// Try to load the module
	if (!module->Load(name))
	{
		delete module;
		// Remove the module from the array
		m_vModules.erase(m_vModules.begin()+(m_vModules.size()-1));
		return false;
	}

	return true;
}


bool CModuleManager::LoadRetroSpyModule(const char *name, ModuleConfigMap cfg)
{
	CModule *module = NULL;

	// Add the pointer into the vector
	m_vModules.push_back(new CModule());

	// Get the module
	module = m_vModules.at(m_vModules.size() - 1);

	// Try to load the module
	if (!module->Load(name, cfg))
	{
		delete module;
		// Remove the module from the array
		m_vModules.erase(m_vModules.begin()+(m_vModules.size()-1));
		return false;
	}

	return true;
}

void CModuleManager::Start()
{
	size_t i = 0;

	for (; i < m_vModules.size(); i++)
	{
		printf("[ModuleManager] Starting module %s\n", m_vModules.at(i)->GetName());
		m_vModules.at(i)->Start();
	}
}

void CModuleManager::Stop()
{
	size_t i = 0;

	for (; i < m_vModules.size(); i++)
	{
		m_vModules.at(i)->Stop();
	}
}

size_t CModuleManager::GetModuleSize()
{
	return m_vModules.size();
}

CModule* CModuleManager::GetModule(size_t index)
{
	if (GetModuleSize() < index)
		return NULL;

	return m_vModules.at(index);
}
