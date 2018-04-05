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

// Extended message support
#define GPI_NEW_AUTH_NOTIFICATION	(1<<0)
#define GPI_NEW_REVOKE_NOTIFICATION (1<<1)

// New Status Info support
#define GPI_NEW_STATUS_NOTIFICATION (1<<2)

// Buddy List + Block List retrieval on login
#define GPI_NEW_LIST_RETRIEVAL_ON_LOGIN (1<<3)

#endif
