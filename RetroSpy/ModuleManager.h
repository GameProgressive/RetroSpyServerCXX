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
#ifndef RETROSPY_MODULEMANAGER_H
#define RETROSPY_MODULEMANAGER_H

#include "Module.h"

#include <vector>

/*
	This class manage all the modules that will be loaded
	with the Server
*/
class CModuleManager
{
public:
	CModuleManager();
	~CModuleManager();

	/*
		Function: LoadRetroSpyModule
		Description: Load a module and store it into the array
		Return: true if the module is loaded, otherwise false
		Parameters:
			name => The name of the module to be loaded
	*/
	bool LoadRetroSpyModule(const char *name);

	/*
		Function: LoadRetroSpyModule
		Description: Load a module and store it into the array
		Return: true if the module is loaded, otherwise false
		Parameters:
			name => The name of the module to be loaded
			cfg => A map to the configuration
	*/
	bool LoadRetroSpyModule(const char *name, ModuleConfigMap cfg);

	/*
		Function: Start
		Description: Start all the modules
	*/
	void Start();

	/*
		Function: Stop
		Description: Stop all the modules
	*/
	void Stop();

	/*
		Function: GetModuleSize
		Description: Get the count of the modules
	*/
	size_t GetModuleSize();

	/*
		Function: GetModule
		Description: Get a module
		Return: A pointer to the module or NULL if the module dosen't exists at the specified index
		Parametres:
			index => The number of the module to get
	*/
	CModule *GetModule(size_t index);

protected:
	std::vector<CModule*> m_vModules;
};

#endif
