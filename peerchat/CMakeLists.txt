set(SOURCES
	${CMAKE_CURRENT_LIST_DIR}/PeerChatServer.h
	${CMAKE_CURRENT_LIST_DIR}/PeerChatServer.cpp
)

include_directories("${CMAKE_CURRENT_LIST_DIR}/../common")

add_library(PeerChat SHARED ${SOURCES})
target_link_libraries(PeerChat RetroSpyCommon)
