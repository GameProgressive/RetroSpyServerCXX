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
#include "Helper.h"

#include <string.h>
#include <stdlib.h>
#include <time.h>

const char cvdallowed[] = "1234567890#"
		"_-`()$-=;/[]"
		"@+&%"
		"abcdefghijklmnopqrstuvwxyz"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ";


DLLAPI void gs_pass_decode(const char *in, char *out)
{
	int pass_len = strlen(in), out_len = B64DecodeLen(in, 1), i = 0;
	
	Util_RandSeed(GP_XOR_SEED);
	B64Decode(in, out, pass_len, &out_len, 1);

	for (; i < out_len; i++)
		out[i] ^= (char)(Util_RandInt(0, 0xFF));

	out[out_len] = '\0';

	/* Restore the seed */
	Util_RandSeed((unsigned int)time(NULL));
}

bool charValid(char ch)
{
	int i = 0;
	for (; i < sizeof(cvdallowed); i++)
	{
		if (cvdallowed[i] == ch)
			return true;
	}

	return false;
}

DLLAPI void gs_make_valid(char *name)
{
	size_t i = 0;
	for (; i < strlen(name); i++)
	{
		if (!charValid(name[i]))
			name[i] = '_';
	}
}
