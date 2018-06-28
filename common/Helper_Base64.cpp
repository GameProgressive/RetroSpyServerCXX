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
#include "Helper.h"

#include <assert.h>
#include <memory.h>
#include <stdlib.h>

#include <algorithm>

static void QuartToTrip(char *quart, char *trip, int inlen)
{
	if (inlen >= 2)
		trip[0] = (char)(quart[0] << 2 | quart[1] >> 4);
	if (inlen >= 3)
		trip[1] = (char)((quart[1] & 0x0F) << 4 | quart[2] >> 2);
	if (inlen >= 4)
		trip[2] = (char)((quart[2] & 0x3) << 6 | quart[3]);
}

static void TripToQuart(const char *trip, char *quart, int inlen)
{
	unsigned char triptemp[3];
	int i;
	for (i = 0; i < inlen; i++)
	{
		triptemp[i] = (unsigned char)trip[i];
	}
	while (i < 3) //fill the rest with 0
	{
		triptemp[i] = 0;
		i++;
	}
	quart[0] = (char)(triptemp[0] >> 2);
	quart[1] = (char)(((triptemp[0] & 3) << 4) | (triptemp[1] >> 4));
	quart[2] = (char)((triptemp[1] & 0x0F) << 2 | (triptemp[2] >> 6));
	quart[3] = (char)(triptemp[2] & 0x3F);

}

const char defaultEncoding[] = { '+','/','=' };
const char alternateEncoding[] = { '[',']','_' };
const char urlSafeEncodeing[] = { '-','_','=' };

void B64Decode(const char *input, char *output, int inlen, int * outlen, int encodingType)
{
	const char *encoding = 0;
	int readpos = 0;
	int writepos = 0;
	char block[4];

	//int outlen = -1;
	//int inlen = (int)strlen(input);

	// 10-31-2004 : Added by Saad Nader
	// now supports URL safe encoding
	////////////////////////////////////////////////
	switch (encodingType)
	{
	case 1:
		encoding = alternateEncoding;
		break;
	case 2:
		encoding = urlSafeEncodeing;
		break;
	default: encoding = defaultEncoding;
	}

	assert(inlen >= 0);
	if (inlen <= 0)
	{
		if (outlen)
			*outlen = 0;
		output[0] = '\0';
		return;
	}

	// Break at end of string or padding character
	while (readpos < inlen && input[readpos] != encoding[2])
	{
		//    'A'-'Z' maps to 0-25
		//    'a'-'z' maps to 26-51
		//    '0'-'9' maps to 52-61
		//    62 maps to encoding[0]
		//    63 maps to encoding[1]
		if (input[readpos] >= '0' && input[readpos] <= '9')
			block[readpos % 4] = (char)(input[readpos] - 48 + 52);
		else if (input[readpos] >= 'a' && input[readpos] <= 'z')
			block[readpos % 4] = (char)(input[readpos] - 71);
		else if (input[readpos] >= 'A' && input[readpos] <= 'Z')
			block[readpos % 4] = (char)(input[readpos] - 65);
		else if (input[readpos] == encoding[0])
			block[readpos % 4] = 62;
		else if (input[readpos] == encoding[1])
			block[readpos % 4] = 63;

		// padding or '\0' characters also mark end of input
		else if (input[readpos] == encoding[2])
			break;
		else if (input[readpos] == '\0')
			break;
		else
		{
			//	(assert(0)); //bad input data
			if (outlen)
				*outlen = 0;
			output[0] = '\0';
			return; //invaid data
		}

		// every 4 bytes, convert QuartToTrip into destination
		if (readpos % 4 == 3) // zero based, so (3%4) means four bytes, 0-1-2-3
		{
			QuartToTrip(block, &output[writepos], 4);
			writepos += 3;
		}
		readpos++;
	}

	// Convert any leftover characters in block
	if ((readpos != 0) && (readpos % 4 != 0))
	{
		// fill block with pad (required for QuartToTrip)
		memset(&block[readpos % 4], encoding[2], (unsigned int)4 - (readpos % 4));
		QuartToTrip(block, &output[writepos], readpos % 4);

		// output bytes depend on the number of non-pad input bytes
		if (readpos % 4 == 3)
			writepos += 2;
		else
			writepos += 1;
	}

	if (outlen)
		*outlen = writepos;
}

void B64Encode(const char *input, char *output, int inlen, int encodingType)
{
	const char *encoding;
	char *holdout = output;
	char *lastchar;
	int todo = inlen;

	// 10-31-2004 : Added by Saad Nader
	// now supports URL safe encoding
	////////////////////////////////////////////////
	switch (encodingType)
	{
	case 1:
		encoding = alternateEncoding;
		break;
	case 2:
		encoding = urlSafeEncodeing;
		break;
	default: encoding = defaultEncoding;
	}

	//assume interval of 3
	while (todo > 0)
	{
		TripToQuart(input, output, std::min(todo, 3));
		output += 4;
		input += 3;
		todo -= 3;
	}
	lastchar = output;
	if (inlen % 3 == 1)
		lastchar -= 2;
	else if (inlen % 3 == 2)
		lastchar -= 1;
	*output = 0; //null terminate!
	while (output > holdout)
	{
		output--;
		if (output >= lastchar) //pad the end
			*output = encoding[2];
		else if (*output <= 25)
			*output = (char)(*output + 65);
		else if (*output <= 51)
			*output = (char)(*output + 71);
		else if (*output <= 61)
			*output = (char)(*output + 48 - 52);
		else if (*output == 62)
			*output = encoding[0];
		else if (*output == 63)
			*output = encoding[1];
	}
}

int B64DecodeLen(const char *input, int encodingType)
{
	const char *encoding;
	const char *holdin = input;

	switch (encodingType)
	{
	case 1:
		encoding = alternateEncoding;
		break;
	case 2:
		encoding = urlSafeEncodeing;
		break;
	default: encoding = defaultEncoding;
	}

	while (*input)
	{
		if (*input == encoding[2])
			return (input - holdin) / 4 * 3 + (input - holdin - 1) % 4;
		input++;
	}

	return (input - holdin) / 4 * 3;
}
