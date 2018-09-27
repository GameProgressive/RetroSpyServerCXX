set(SOURCES
	${CMAKE_CURRENT_LIST_DIR}/PSServer.cpp
	${CMAKE_CURRENT_LIST_DIR}/PSServer.h
)

include_directories("${CMAKE_CURRENT_LIST_DIR}/../common")

add_library(PlayerSearch SHARED ${SOURCES})
target_link_libraries(PlayerSearch RetroSpyCommon)
