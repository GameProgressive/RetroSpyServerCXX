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
#ifndef RETROSPYCOMMON_STRING_H
#define RETROSPYCOMMON_STRING_H

#include "DllApi.h"

bool DLLAPI strendswith(const char *buffer, const char *what);
bool DLLAPI strdelete(char *buffer, int buffersize, int number);
void DLLAPI strrnd(char *buffer, int buffersize, const char *table);
bool DLLAPI strfind(char *buffer, const char what);

bool DLLAPI user_to_emailnick(const char *buffer, char *lpEmail, int email_size, char *lpNick, int nick_size);

bool DLLAPI is_gs_valid(const char *base);

int DLLAPI get_gs_req(const char *base, char *out, int max_size);
DLLAPI char* get_gs_data(const char *base, const char *what, char *out, int max_size);

#endif
