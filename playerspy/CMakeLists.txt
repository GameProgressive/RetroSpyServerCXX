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

include_directories("${CMAKE_CURRENT_LIST_DIR}/../common")

add_library(PlayerSpy SHARED ${SOURCES})
target_link_libraries(PlayerSpy RetroSpyCommon)
target_compile_definitions(PlayerSpy PRIVATE __64BIT__)
