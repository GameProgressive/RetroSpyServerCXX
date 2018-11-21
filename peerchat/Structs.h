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
#ifndef _STRUCTS_H_
#define _STRUCTS_H_

#include <string.h>
#include <time.h>

#define MAX_NAME 64
#define MAX_COMMENT 256

typedef struct {
	char mask[MAX_NAME + 1];
	char comment[MAX_COMMENT + 1];
	time_t expires;
	char hostmask[MAX_NAME + 1];
	char machineid[MAX_NAME + 1];
	int modeflags;
	int profileid;
	char setbyhost[MAX_NAME + 1];
	char setbynick[MAX_NAME + 1];
	int setbypid;
	int usermodeid;
	char isGlobal; //is it temp or stored in db
	char hideNick; //only used while setting it(remove when we use the db msgs to do this)
	time_t setondate;
} SUserProps;

typedef struct {
	char key[MAX_NAME + 1];
	char mask[MAX_NAME + 1];
	char comment[MAX_COMMENT + 1];
	char entrymsg[MAX_COMMENT + 1];
	time_t expires;
	char groupname[MAX_NAME + 1];
	int limit;
	char modes[MAX_NAME + 1];
	char onlyowner;
	char setbynick[MAX_NAME + 1];
	char setbyhost[MAX_NAME + 1];
	int setbypid;
	char topic[MAX_COMMENT + 1];
	time_t setondate;
} SChannelProps;

typedef struct {
	char mask[MAX_NAME + 1];
	int gameid;//-1 = all encrypted, 0 = unencrypted or anything, otherwise specific gameid
} SGameClient;

static bool operator==(const SChannelProps& p1, const SChannelProps& p2)
{
	if (strcmp(p1.key, p2.key) != 0) return false;
	if (strcmp(p1.mask, p2.mask) != 0) return false;
	if (strcmp(p1.comment, p2.comment) != 0) return false;
	if (strcmp(p1.entrymsg, p2.entrymsg) != 0) return false;
	if (p1.expires != p2.expires) return false;
	if (strcmp(p1.groupname, p2.groupname) != 0) return false;
	if (p1.limit != p2.limit) return false;
	if (strcmp(p1.modes, p2.modes) != 0) return false;
	if (p1.onlyowner != p2.onlyowner) return false;
	if (strcmp(p1.setbynick, p2.setbynick) != 0) return false;
	if (strcmp(p1.setbyhost, p2.setbyhost) != 0) return false;
	if (p1.setbypid != p2.setbypid) return false;
	if (strcmp(p1.topic, p2.topic) != 0) return false;
	if (p1.setondate != p2.setondate) return false;
	
	return true;
}

#endif
