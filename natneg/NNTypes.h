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
#ifndef NNTYPES_H
#define NNTYPES_H

#define NN_PT_GP  0
#define NN_PT_NN1 1
#define NN_PT_NN2 2
#define NN_PT_NN3 3

#define MATCHUP_PORT 27901

/* 
NAT Negotiation Packet Magic Bytes
These bytes will start each incoming packet that is part of the NAT Negotiation SDK.
If you are sharing a game socket with the SDK, you can use these bytes to determine when to
pass a packet to NNProcessData
*/
#define NATNEG_MAGIC_LEN 6
#define NN_MAGIC_0 0xFD
#define NN_MAGIC_1 0xFC
#define NN_MAGIC_2 0x1E
#define NN_MAGIC_3 0x66
#define NN_MAGIC_4 0x6A
#define NN_MAGIC_5 0xB2

#define NN_INIT	0
#define NN_INITACK 1
#define NN_ERTTEST 2
#define NN_ERTACK 3
#define NN_STATEUPDATE 4
#define NN_CONNECT 5
#define NN_CONNECT_ACK 6
#define NN_CONNECT_PING 7
#define NN_BACKUP_TEST 8
#define NN_BACKUP_ACK 9
#define NN_ADDRESS_CHECK 10
#define NN_ADDRESS_REPLY 11
#define NN_NATIFY_REQUEST 12
#define NN_REPORT 13
#define NN_REPORT_ACK 14

#define FINISHED_NOERROR 0
#define FINISHED_ERROR_DEADBEAT_PARTNER 1
#define FINISHED_ERROR_INIT_PACKETS_TIMEDOUT 2

typedef enum { no_nat, firewall_only, full_cone, restricted_cone, port_restricted_cone, symmetric, unknown, NUM_NAT_TYPES } NatType;
typedef enum { unrecognized, private_as_public, consistent_port, incremental, mixed, NUM_MAPPING_SCHEMES } NatMappingScheme;

#define INITPACKET_SIZE BASEPACKET_SIZE + 9
#define INITPACKET_ADDRESS_OFFSET BASEPACKET_SIZE + 3
typedef struct _InitPacket
{
	unsigned char porttype;
	unsigned char clientindex;
	unsigned char usegameport;
	unsigned int localip;
	unsigned short localport;
} InitPacket;

#define REPORTPACKET_SIZE BASEPACKET_SIZE + 61
typedef struct _ReportPacket
{
	unsigned char porttype;
	unsigned char clientindex;
	unsigned char negResult;
	NatType natType;
	NatMappingScheme natMappingScheme;
	char gamename[50];
} ReportPacket;

#define CONNECTPACKET_SIZE BASEPACKET_SIZE + 8
typedef struct _ConnectPacket
{
	unsigned int remoteIP;
	unsigned short remotePort;
	unsigned char gotyourdata;
	unsigned char finished;
} ConnectPacket;

#define BASEPACKET_SIZE 12
#define BASEPACKET_TYPE_OFFSET 7
typedef struct _NatNegPacket {
	// Base members:
	unsigned char magic[NATNEG_MAGIC_LEN];
	unsigned char version;
	unsigned char packettype;
	int cookie;	

	union 
	{
		InitPacket Init;
		ConnectPacket Connect;
		ReportPacket Report;
	} Packet;

} NatNegPacket;

#endif