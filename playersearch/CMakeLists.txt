set(SOURCES
${CMAKE_CURRENT_LIST_DIR}/PlayerSearch.cpp
${CMAKE_CURRENT_LIST_DIR}/PSServer.cpp
${CMAKE_CURRENT_LIST_DIR}/PSServer.h
)

if (WIN32)
	SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} /DEF:PlayerSearch.def" )
endif()

include_directories("${CMAKE_CURRENT_LIST_DIR}/../common")

add_library(PlayerSearch SHARED ${SOURCES})
target_link_libraries(PlayerSearch RetroSpyCommon)
