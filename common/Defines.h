/*
	BSD 3-Clause License
	
	Copyright (c) 2017, GameProgressive Team
	All rights reserved.

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:

	* Redistributions of source code must retain the above copyright notice, this
	  list of conditions and the following disclaimer.

	* Redistributions in binary form must reproduce the above copyright notice,
	  this list of conditions and the following disclaimer in the documentation
	  and/or other materials provided with the distribution.

	* Neither the name of the copyright holder nor the names of its
	  contributors may be used to endorse or promote products derived from
	  this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
	AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
	IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
	DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
	FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
	DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
	SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
	CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
	OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef RETROSPYCOMMON_DLLAPI_H
#define RETROSPYCOMMON_DLLAPI_H

// DLLAPI exports for W32
#if _WIN32 && _MSC_VER
	#if RSC_EXPORT
		#define DLLAPI __declspec(dllexport)
	#else
		#define DLLAPI __declspec(dllimport)
	#endif
#else
	#define DLLAPI 
#endif

#define GS_REQUEST_LEN				20
#define GS_GAMENAME_LEN				64
#define GP_EMAIL_LEN				51
#define GP_PASSWORD_LEN				31
#define GP_PASSWORDENC_LEN			((((GP_PASSWORD_LEN+2)*4)/3)+1)
#define GP_PARTNERID_LEN			sizeof(int)
#define GP_NAMESPACEID_LEN			sizeof(int)
#define GP_NICK_LEN                 31
#define GP_UNIQUENICK_LEN           21
#define GP_XOR_SEED                 0x79707367 // "gspy"
#define GP_STATUS_STRING_LEN        256
#define GP_FIRSTNAME_LEN            31
#define GP_LASTNAME_LEN             31
#define GP_HOMEPAGE_LEN             76
#define GP_ZIPCODE_LEN              11
#define GP_COUNTRYCODE_LEN          3
#define GP_AIMNAME_LEN              51
#define GP_LOGIN_TICKET_LEN         25
#define GP_LOCATION_STRING_LEN      256
#define GS_PARTNER_ID				0
#define GP_SERVERCHALL_LEN			11
#define GP_AUTHTOKEN_LEN			256
#define GP_PARTNERCHALLENGE_LEN     256
#define GP_CLIENTCHALL_LEN			64

#define MD5_BUFFER_LEN				33

#endif
