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
#define RS_NICK_MAX					10
#define MD5_BUFFER_LEN				33
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

#endif
