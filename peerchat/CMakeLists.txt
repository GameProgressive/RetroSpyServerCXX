set(SOURCES
	${CMAKE_CURRENT_LIST_DIR}/PeerChatServer.h
	${CMAKE_CURRENT_LIST_DIR}/PeerChatServer.cpp
	${CMAKE_CURRENT_LIST_DIR}/Cache.h
	${CMAKE_CURRENT_LIST_DIR}/Cache.cpp
	${CMAKE_CURRENT_LIST_DIR}/IRCClient.h
	${CMAKE_CURRENT_LIST_DIR}/IRCChannel.cpp
	${CMAKE_CURRENT_LIST_DIR}/IRCClient.h
	${CMAKE_CURRENT_LIST_DIR}/IRCClient.cpp
	${CMAKE_CURRENT_LIST_DIR}/Structs.h
)

include_directories("${CMAKE_CURRENT_LIST_DIR}/../common")

add_library(PeerChat SHARED ${SOURCES})
target_link_libraries(PeerChat RetroSpyCommon)
