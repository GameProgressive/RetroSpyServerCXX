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
#ifndef RETROSPYCOMMON_DLLAPI_H
#define RETROSPYCOMMON_DLLAPI_H

/*
	This file exports the DLLAPI for Windows
*/

#if _WIN32 && _MSC_VER
	#if RSC_EXPORT
		#define DLLAPI __declspec(dllexport)
	#else
		#define DLLAPI __declspec(dllimport)
	#endif
#else
	#define DLLAPI 
#endif

#endif
