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
#ifndef PY_TYPES_H
#define PY_TYPES_H

// Types of bm's.
/////////////////
#define GPI_BM_MESSAGE                    1
#define GPI_BM_REQUEST                    2
#define GPI_BM_REPLY                      3  // only used on the backend
#define GPI_BM_AUTH                       4
#define GPI_BM_UTM                        5
#define GPI_BM_REVOKE                     6  // remote buddy removed from local list
#define GPI_BM_STATUS                   100						
#define GPI_BM_INVITE                   101
#define GPI_BM_PING                     102
#define GPI_BM_PONG                     103
#define GPI_BM_KEYS_REQUEST             104
#define GPI_BM_KEYS_REPLY               105
#define GPI_BM_FILE_SEND_REQUEST        200
#define GPI_BM_FILE_SEND_REPLY          201
#define GPI_BM_FILE_BEGIN               202
#define GPI_BM_FILE_END                 203
#define GPI_BM_FILE_DATA                204
#define GPI_BM_FILE_SKIP                205
#define GPI_BM_FILE_TRANSFER_THROTTLE   206
#define GPI_BM_FILE_TRANSFER_CANCEL     207
#define GPI_BM_FILE_TRANSFER_KEEPALIVE  208

typedef enum _GPEnum
{
	// Callbacks
	////////////
	GP_ERROR = 0,
	GP_RECV_BUDDY_REQUEST,
	GP_RECV_BUDDY_STATUS,
	GP_RECV_BUDDY_MESSAGE,
	GP_RECV_BUDDY_UTM,
	GP_RECV_GAME_INVITE,
	GP_TRANSFER_CALLBACK,
	GP_RECV_BUDDY_AUTH,
	GP_RECV_BUDDY_REVOKE,
		
	// Global States.
	/////////////////
	GP_INFO_CACHING = 0x0100,
	GP_SIMULATION,
	GP_INFO_CACHING_BUDDY_AND_BLOCK_ONLY,

	// Blocking
	///////////
	GP_BLOCKING = 1,
	GP_NON_BLOCKING = 0,

	// Firewall
	///////////
	GP_FIREWALL = 1,
	GP_NO_FIREWALL = 0,

	// Check Cache
	//////////////
	GP_CHECK_CACHE = 1,
	GP_DONT_CHECK_CACHE = 0,

	// Is Valid Email.
	// PANTS|02.15.00
	//////////////////
	GP_VALID = 1,
	GP_INVALID = 0,

	// Fatal Error.
	///////////////
	GP_FATAL = 1,
	GP_NON_FATAL = 0,

	// Sex
	//////
	GP_MALE = 0x0500,
	GP_FEMALE,
	GP_PAT,

	// Profile Search.
	//////////////////
	GP_MORE = 0x0600,
	GP_DONE,

	// Set Info
	///////////
	GP_NICK = 0x0700,
	GP_UNIQUENICK,
	GP_EMAIL,
	GP_PASSWORD,
	GP_FIRSTNAME,
	GP_LASTNAME,
	GP_ICQUIN,
	GP_HOMEPAGE,
	GP_ZIPCODE,
	GP_COUNTRYCODE,
	GP_BIRTHDAY,
	GP_SEX,
	GP_CPUBRANDID,
	GP_CPUSPEED,
	GP_MEMORY,
	GP_VIDEOCARD1STRING,
	GP_VIDEOCARD1RAM,
	GP_VIDEOCARD2STRING,
	GP_VIDEOCARD2RAM,
	GP_CONNECTIONID,
	GP_CONNECTIONSPEED,
	GP_HASNETWORK,
	GP_OSSTRING,
	GP_AIMNAME,  // PANTS|03.20.01
	GP_PIC,
	GP_OCCUPATIONID,
	GP_INDUSTRYID,
	GP_INCOMEID,
	GP_MARRIEDID,
	GP_CHILDCOUNT,
	GP_INTERESTS1,

	// New Profile.
	///////////////
	GP_REPLACE = 1,
	GP_DONT_REPLACE = 0,

	// Is Connected.
	////////////////
	GP_CONNECTED = 1,
	GP_NOT_CONNECTED = 0,

	// Public mask.
	///////////////
	GP_MASK_NONE        = 0x00000000,
	GP_MASK_HOMEPAGE    = 0x00000001,
	GP_MASK_ZIPCODE     = 0x00000002,
	GP_MASK_COUNTRYCODE = 0x00000004,
	GP_MASK_BIRTHDAY    = 0x00000008,
	GP_MASK_SEX         = 0x00000010,
	GP_MASK_EMAIL       = 0x00000020,
	GP_MASK_ALL         = 0xFFFFFFFF,

	// Status
	/////////
	GP_OFFLINE  = 0,
	GP_ONLINE   = 1,
	GP_PLAYING  = 2,
	GP_STAGING  = 3,
	GP_CHATTING = 4,
	GP_AWAY     = 5,

	// Session flags
	/////////////////
	GP_SESS_IS_CLOSED = 0x00000001,
	GP_SESS_IS_OPEN = 0x00000002,
	GP_SESS_HAS_PASSWORD = 0x00000004,
	GP_SESS_IS_BEHIND_NAT = 0x00000008,
	GP_SESS_IS_RANKED = 0x000000010,
	

	// CPU Brand ID
	///////////////
	GP_INTEL = 1,
	GP_AMD,
	GP_CYRIX,
	GP_MOTOROLA,
	GP_ALPHA,

	// Connection ID.
	/////////////////
	GP_MODEM = 1,
	GP_ISDN,
	GP_CABLEMODEM,
	GP_DSL,
	GP_SATELLITE,
	GP_ETHERNET,
	GP_WIRELESS,

	// Transfer callback type.
	// *** the transfer is ended when these types are received
	//////////////////////////
	GP_TRANSFER_SEND_REQUEST = 0x800,  // arg->num == numFiles
	GP_TRANSFER_ACCEPTED,
	GP_TRANSFER_REJECTED,        // ***
	GP_TRANSFER_NOT_ACCEPTING,   // ***
	GP_TRANSFER_NO_CONNECTION,   // ***
	GP_TRANSFER_DONE,            // ***
	GP_TRANSFER_CANCELLED,       // ***
	GP_TRANSFER_LOST_CONNECTION, // ***
	GP_TRANSFER_ERROR,           // ***
	GP_TRANSFER_THROTTLE,  // arg->num == Bps
	GP_FILE_BEGIN,
	GP_FILE_PROGRESS,  // arg->num == numBytes
	GP_FILE_END,
	GP_FILE_DIRECTORY,
	GP_FILE_SKIP,
	GP_FILE_FAILED,  // arg->num == error

	// GP_FILE_FAILED error
	///////////////////////
	GP_FILE_READ_ERROR = 0x900,
	GP_FILE_WRITE_ERROR,
	GP_FILE_DATA_ERROR,

	// Transfer Side.
	/////////////////
	GP_TRANSFER_SENDER = 0xA00,
	GP_TRANSFER_RECEIVER,

	// UTM send options.
	////////////////////
	GP_DONT_ROUTE = 0xB00, // only send direct

	// Quiet mode flags.
	////////////////////
	GP_SILENCE_NONE       = 0x00000000,
	GP_SILENCE_MESSAGES   = 0x00000001,
	GP_SILENCE_UTMS       = 0x00000002,
	GP_SILENCE_LIST       = 0x00000004, // includes requests, auths, and revokes
	GP_SILENCE_ALL        = 0xFFFFFFFF,

	// Flags for checking if newer version of status info is supported
	GP_NEW_STATUS_INFO_SUPPORTED = 0xC00,
	GP_NEW_STATUS_INFO_NOT_SUPPORTED = 0xC01
} GPEnum;

#endif
