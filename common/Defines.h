#ifndef _RETROSPY_DEFINES_HEADER_
#define _RETROSPY_DEFINES_HEADER_

#include <MDK/MDK_Definitions.h>

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
	GP_NEW_STATUS_INFO_NOT_SUPPORTED = 0xC01,
	
	// Error codes that can occur while creating a new user.
	GP_NEWUSER						= 512, // 0x200, There was an error creating a new user.
	GP_NEWUSER_BAD_NICK				= 513, // 0x201, A profile with that nick already exists.
	GP_NEWUSER_BAD_PASSWORD			= 514, // 0x202, The password does not match the email address.
	GP_NEWUSER_UNIQUENICK_INVALID	= 515, // 0x203, The uniquenick is invalid.
	GP_NEWUSER_UNIQUENICK_INUSE = 516, // 0x204, The uniquenick is already in use.

} GPEnum;

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
#define GP_PLACE_LEN                128

#define MD5_BUFFER_LEN				33

typedef struct
{
  char nick[GP_NICK_LEN];
  char uniquenick[GP_UNIQUENICK_LEN];
  char email[GP_EMAIL_LEN];
  char firstname[GP_FIRSTNAME_LEN];
  char lastname[GP_LASTNAME_LEN];
  char homepage[GP_HOMEPAGE_LEN];
  int icquin;
  char zipcode[GP_ZIPCODE_LEN];
  char countrycode[GP_COUNTRYCODE_LEN];
  float longitude; // negative is west, positive is east.  (0, 0) means unknown.
  float latitude;  // negative is south, positive is north.  (0, 0) means unknown.
  char place[GP_PLACE_LEN];  // e.g., "USA|California|Irvine", "South Korea|Seoul", "Turkey"
  int birthday;
  int birthmonth;
  int birthyear;
  GPEnum sex;
  int publicmask;
  char aimname[GP_AIMNAME_LEN];
  int pic;
  int occupationid;
  int industryid;
  int incomeid;
  int marriedid;
  int childcount;
  int interests1;
  int ownership1;
  int conntypeid;
} GPIInfoCache;

#if _WIN32 && _MSC_VER
#include <Windows.h>
#ifdef __cplusplus
#define RetroSpyModuleStart extern "C" { __declspec(dllexport) int MDKModule(void* data) {
#else
#define RetroSpyModuleStart __declspec(dllexport) int MDKModule(void* data) {
#endif
#else
#ifdef __cplusplus
#define RetroSpyModuleStart extern "C" { int MDKModule(void* data) {
#else
#define RetroSpyModuleStart int MDKModule(void* data) {
#endif
#endif
#ifdef __cplusplus
#define RetroSpyModuleEnd Server->Run(); return ERROR_NONE; }}
#else
#define RetroSpyModuleEnd return ERROR_NONE; }
#endif
#endif
