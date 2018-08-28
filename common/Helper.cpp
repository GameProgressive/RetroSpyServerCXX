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


void gs_pass_decode(const char *in, char *out)
{
	int pass_len = strlen(in), out_len = B64DecodeLen(in, 1), i = 0;
	
	RandSeed(GP_XOR_SEED);
	B64Decode(in, out, pass_len, &out_len, 1);

	for (; i < out_len; i++)
		out[i] ^= (char)(RandInt(0, 0xFF));

	out[out_len] = '\0';
}

void gs_pass_decode(std::string& str)
{
	int pass_len = str.length(), out_len = B64DecodeLen(str.c_str(), 1), i = 0;
	char* out = (char*)malloc(out_len + 1);

	RandSeed(GP_XOR_SEED);
	B64Decode(str.c_str(), out, pass_len, &out_len, 1);

	for (; i < out_len; i++)
		out[i] ^= (char)(RandInt(0, 0xFF));

	out[out_len] = '\0';

	str = out;
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

void gs_make_valid(char *name)
{
	size_t i = 0;
	for (; i < strlen(name); i++)
	{
		if (!charValid(name[i]))
			name[i] = '_';
	}
}

void hash_md5(const char *what, int len, char *out)
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

void gs_do_proof(char *out, const char *password, const char *token, const char *serverch, const char *clientch)
{
	char passmd5[MD5_BUFFER_LEN], buffer[512];
	passmd5[0] = buffer[0] = 0;

	/* Hash the decrypted password */
	hash_md5(password, strlen(password), passmd5);

	/* Generate the buffer */
	snprintf(buffer, sizeof(buffer), "%s%s%s%s%s%s",
		passmd5,
		"                                                ",
		token,
		serverch,
		clientch,
		passmd5);

	/* Hash the buffer */
	hash_md5(buffer, strlen(buffer), out);
}

bool user_to_emailnick(const char *buffer, char *lpEmail, int email_size, char *lpNick, int nick_size)
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

	strncpy(lpNick, buffer, pos);
	lpNick[pos] = '\0';
	strncpy(lpEmail, &buffer[pos + 1], buffersize-pos-1);
	lpEmail[buffersize - pos] = '\0';
	
	return true;
}

bool is_gs_valid(const char *base)
{
	int len = strlen(base);
	
	if (base[0] != '\\')
		return false;

	if (strcmp(&base[len - 7], "\\final\\") != 0)
		return false;
	
	return true;
}

int get_gs_req(const char *base, char *out, int max_size)
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

	strncpy(out, in, pos);
	out[pos] = '\0';

	pos += 1; /* First \\ */

	if (in[pos] == '\\')
		return pos + 2; /* Double slash handling */

	return pos + 1;
}

bool get_gs_data(std::string input, std::string& out, const char* what)
{
	char possible[1024];
	if (!get_gs_data(input.c_str(), what, possible, 1024))
		return false;

	out = possible;
	return true;
}

char* get_gs_data(const char *base, const char *what, char *out, int max_size)
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

		strncpy(tmp, bx, i);
		tmp[i] = '\0';

		if (bfound)
		{
			if (i < max_size)
			{
				strncpy(out, tmp, i);
				out[i] = '\0';
				
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
