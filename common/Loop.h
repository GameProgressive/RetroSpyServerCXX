/*
    This file is part of RetroSpy Server.

    RetroSpy Server is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef RETROSPYCOMMON_LOOP_H
#define RETROSPYCOMMON_LOOP_H

#include "Defines.h"

#include <uv.h>

/*
	This class rappresents an application loop
	based from libuv
*/
class CLoop
{
public:
	DLLAPI CLoop();
	DLLAPI ~CLoop();

	/*
		Function: Run
		Description: Run the loop
	*/
	void DLLAPI Run();

	/*
		Function: GetLoopPtr
		Description: Get the loop pointer
		Return: The loop pointer
	*/
	DLLAPI uv_loop_t* GetLoopPtr();

private:
	uv_loop_t *m_loop;
};

#endif
