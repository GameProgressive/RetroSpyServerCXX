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
#include "RSString.h"

#include <string.h>
#include <stdlib.h>

bool DLLAPI strendswith(const char *buffer, const char *what)
{
	int endsize = strlen(what), buffsize = strlen(buffer);
	char *checkbuf = (char *)malloc(sizeof(char) * (endsize + 1));

	for (; endsize > -1; buffsize--, endsize--)
	{
		checkbuf[endsize] =  buffer[buffsize];
	}

	if (strcmp(checkbuf, what) == 0)
	{
		if (checkbuf)
			free(checkbuf);

		return true;
	}

	if (checkbuf)
		free(checkbuf);

	return false;
}

bool DLLAPI strdelete(char *buffer, int buffersize, int number)
{
	int counter = 0;

	if (buffersize < number)
		return false;

	buffersize -= number;

	for (; counter < buffersize; counter++, number++)
	{
		buffer[counter] = buffer[number];
	}

	buffer[counter] = '\0';

	return true;
}

void DLLAPI strrnd(char *buffer, int buffersize, const char *table)
{
	int table_len = strlen(table), i = 0;

	for (; i < (buffersize - 1); i++)
	{
		int randnum = rand() % (table_len - 1);
		buffer[i] = table[randnum];
	}

	buffer[i] = '\0';
}

bool DLLAPI strfind(char *buffer, const char what)
{
	while (*buffer != '\0')
	{
		if (*buffer == what)
			return true;

		buffer++;
	}

	return false;
}

bool DLLAPI user_to_emailnick(const char *buffer, char *lpEmail, int email_size, char *lpNick, int nick_size)
{
	char *pch = NULL;
	int buffersize = 0, pos = 0;

	/* Check if the parametras are NULL */
	if (buffer == NULL)
		return false;
	if (lpEmail == NULL)
		return false;
	if (lpNick == NULL)
		return false;

	buffersize = strlen(buffer);

	/* Get @ position*/
	pch = (char*)strchr(buffer, '@');

	if (pch == NULL)
		return false; /* Dosen't exists */

	pos = pch - buffer;

	if (pos > nick_size)
		return false; 	/* Email readed too big to store into the buffer */

	if ((buffersize - pos) > email_size)
		return false;	/* Nick readed too big to store into the buffer */

	strncpy_s(lpNick, nick_size, buffer, pos);
	strncpy_s(lpEmail, email_size, &buffer[pos + 1], buffersize - pos - 1);

	return true;
}

bool DLLAPI is_gs_valid(const char *base)
{
	int len = strlen(base);
	
	if (base[0] != '\\')
		return false;

	if (strcmp(&base[len - 7], "\\final\\") != 0)
		return false;
	
	return true;
}

int DLLAPI get_gs_req(const char *base, char *out, int max_size)
{
	char *pch = NULL;
	const char *in = &base[1];
	int pos = 0;

	pch = (char*)strstr(in, "\\");

	if (pch == NULL)
		return -1;

	pos = pch-in;
	
	if (max_size < pos)
		return -1;

	strncpy_s(out, max_size, in, pos);

	pos += 1; /* First \\ */

	if (in[pos] == '\\')
		return pos + 2; /* Double slash handling */

	return pos + 1;
}

DLLAPI char* get_gs_data(const char *base, const char *what, char *out, int max_size)
{
	char *pch = NULL, *bx = NULL;
	int i2 = 0, i = 0;
	bool bfound = false, bRun = true;

	out[0] = '\0';

	while (bRun)
	{
		char *tmp = NULL;
		bx = (char *)&base[i2];

		pch = strchr(bx, '\\');

		if (pch == NULL)
			break;

		i = pch - bx;

		tmp = (char*)malloc(i + 1);

		strncpy_s(tmp, i + 1, bx, i);

		if (bfound)
		{
			if (i < max_size)
			{
				strncpy_s(out, max_size, tmp, i);

				free(tmp);
			}

			bRun = false;
		}
		else
		{
			if (strcmp(tmp, what) == 0)
				bfound = true;
		}

		if (bRun)
		{
			free(tmp);
			i2 += i + 1;
		}
	}

	return out;
}
