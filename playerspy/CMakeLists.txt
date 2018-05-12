set(SOURCES
	${CMAKE_CURRENT_LIST_DIR}/Client.cpp
	${CMAKE_CURRENT_LIST_DIR}/Client.h
	${CMAKE_CURRENT_LIST_DIR}/ClientManager.cpp
	${CMAKE_CURRENT_LIST_DIR}/ClientManager.h
	${CMAKE_CURRENT_LIST_DIR}/PlayerSpy.cpp
	${CMAKE_CURRENT_LIST_DIR}/PYServer.cpp
	${CMAKE_CURRENT_LIST_DIR}/PYServer.h
	${CMAKE_CURRENT_LIST_DIR}/Types.h
)

if (WIN32)
	SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} /DEF:PlayerSpy.def" )
endif()

include_directories("${CMAKE_CURRENT_LIST_DIR}/../common")

add_library(PlayerSpy SHARED ${SOURCES})
target_link_libraries(PlayerSpy RetroSpyCommon)
