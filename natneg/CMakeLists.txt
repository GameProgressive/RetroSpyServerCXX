set(SOURCES
	${CMAKE_CURRENT_LIST_DIR}/NatNeg.cpp
	${CMAKE_CURRENT_LIST_DIR}/NNServer.cpp
	${CMAKE_CURRENT_LIST_DIR}/NNServer.h
	${CMAKE_CURRENT_LIST_DIR}/ClientManager.h
	${CMAKE_CURRENT_LIST_DIR}/NNTypes.h
	${CMAKE_CURRENT_LIST_DIR}/ClientManager.cpp
	${CMAKE_CURRENT_LIST_DIR}/Client.h
	${CMAKE_CURRENT_LIST_DIR}/Client.cpp
)

if (WIN32)
	SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} /DEF:natneg.def" )
endif()

include_directories("${CMAKE_CURRENT_LIST_DIR}/../common")

add_library(NatNeg SHARED ${SOURCES})
target_link_libraries(NatNeg RetroSpyCommon)
