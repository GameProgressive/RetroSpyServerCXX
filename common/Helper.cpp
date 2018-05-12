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

#include "md5.h"

#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

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

DLLAPI void hash_md5(const char *what, int len, char *out)
{
	unsigned char md5h[16];
	static const char hex[] = "0123456789abcdef";
	md5_context         md5t;
	int i = 0;

    md5_starts(&md5t);
    md5_update(&md5t, (unsigned char *)what, len);
    md5_finish(&md5t, md5h);

	for (i = 0; i < 16; i++) {
		*out++ = hex[md5h[i] >> 4];
		*out++ = hex[md5h[i] & 15];
	}
	*out = 0;
}

DLLAPI void gs_do_proof(char *out, const char *password, const char *token, const char *serverch, const char *clientch)
{
	char passmd5[MD5_BUFFER_LEN], buffer[512];
	passmd5[0] = buffer[0] = 0;

	/* Hash the decrypted password */
	hash_md5(password, strlen(password), passmd5);

	/* Generate the buffer */
	_snprintf_s(buffer, sizeof(buffer), sizeof(buffer) - 1, "%s%s%s%s%s%s",
		passmd5,
		"                                                ",
		token,
		serverch,
		clientch,
		passmd5);

	/* Hash the buffer */
	hash_md5(buffer, strlen(buffer), out);
}

#ifndef _WIN32
DLLAPI void fopen_s(FILE**fp, const char *path, const char *mode)
{
	*fp = fopen(path, mode);
}
#endif
