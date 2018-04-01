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
#define RSC_EXPORT 1 //Export the methods
#include "Loop.h"

CLoop::CLoop()
{
	m_loop = uv_default_loop();
}

CLoop::~CLoop()
{
	if (m_loop)
		uv_loop_close(m_loop);
}

DLLAPI void CLoop::Run()
{
	uv_run(m_loop, UV_RUN_DEFAULT);
}

DLLAPI uv_loop_t* CLoop::GetLoopPtr()
{
	return m_loop;
}
