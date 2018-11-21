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
#ifndef RETROSPYCOMMON_HELPER_H
#define RETROSPYCOMMON_HELPER_H

#include "Defines.h"
#include <MDK/Database.h>

#include <string>

// Base64
void B64Decode(const char *input, char *output, size_t inlen, size_t * outlen, int encodingType);
void B64Encode(const char *input, char *output, size_t inlen, int encodingType);
size_t B64DecodeLen(const char *input, int encodingType);

// Random
void RandSeed(unsigned long seed);
int RandInt(int low, int high);

// Helper
void gs_pass_decode(const char *buf, char *out);
void gs_pass_decode(std::string& string);
void gs_make_valid(char *name);
void gs_do_proof(char *out, const char *password, const char *token, const char *serverch, const char *clientch);
void hash_md5(const char *what, size_t len, char *out);

// Database
bool GetProfileIDFromNickEmailAndPass(CDatabase* c, const char *nick, const char *email, const char* pass, unsigned int* id);
bool GetProfileIDFromNickEmail(CDatabase* c, const char *nick, const char *email, unsigned int* pid);
void GetUniqueNickFromProfileID(CDatabase* c, unsigned int pid, char *unick, int size);
bool GetProfileIDFromUniqueNick(CDatabase* db, const char *unick, unsigned int* out);
void GetPasswordFromUserID(CDatabase* c, char *out, int out_len, unsigned int id);
unsigned int GetUserIDFromProfileID(CDatabase* db, unsigned int id, unsigned int* out);
int AssignSessionKeyFromProfileID(CDatabase* c, unsigned int id);
void FreeSessionKey(CDatabase* c, unsigned int profileid);
bool GetProfileInfo(CDatabase* c, unsigned int pid, GPIInfoCache *out, unsigned int *id_out);
bool GetProfileIDFromSessKey(CDatabase* db, unsigned int seeskey, unsigned int* out);
bool GetProfileIDFromAuthToken(CDatabase* db, const char* authtoken, unsigned int *out);
bool GetUserIDFromEmail(CDatabase* db, const char* email, unsigned int* id);
bool RegisterUser(CDatabase* db, const char* email, const char* nick, const char* pass, unsigned int* userid);

//String
bool user_to_emailnick(const char *buffer, char *lpEmail, size_t email_size, char *lpNick, size_t nick_size);
bool is_gs_valid(const char *base);
size_t get_gs_req(const char *base, char *out, size_t max_size);
char* get_gs_data(const char *base, const char *what, char *out, size_t max_size);
bool get_gs_data(std::string base, std::string& out, const char* what);

// Mask
bool match_mask_with_name(const char *mask, const char *name);
bool match_mask_with_name2(const char *mask, const char *name);

#endif
