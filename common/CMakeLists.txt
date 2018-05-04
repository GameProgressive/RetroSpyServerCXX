set(SOURCES
	${CMAKE_CURRENT_LIST_DIR}/chc_endian.cpp
	${CMAKE_CURRENT_LIST_DIR}/chc_endian.h
	${CMAKE_CURRENT_LIST_DIR}/Defines.h
	${CMAKE_CURRENT_LIST_DIR}/Helper.cpp
	${CMAKE_CURRENT_LIST_DIR}/Helper.h
	${CMAKE_CURRENT_LIST_DIR}/Helper_Base64.cpp
	${CMAKE_CURRENT_LIST_DIR}/Helper_Query.cpp
	${CMAKE_CURRENT_LIST_DIR}/Helper_Random.cpp
	${CMAKE_CURRENT_LIST_DIR}/Loop.cpp
	${CMAKE_CURRENT_LIST_DIR}/Loop.h
	${CMAKE_CURRENT_LIST_DIR}/md5.c
	${CMAKE_CURRENT_LIST_DIR}/md5.h
	${CMAKE_CURRENT_LIST_DIR}/ModuleDef.h
	${CMAKE_CURRENT_LIST_DIR}/Query.cpp
	${CMAKE_CURRENT_LIST_DIR}/Query.h
	${CMAKE_CURRENT_LIST_DIR}/RSString.cpp
	${CMAKE_CURRENT_LIST_DIR}/RSString.h
	${CMAKE_CURRENT_LIST_DIR}/Server.cpp
	${CMAKE_CURRENT_LIST_DIR}/Server.h
	${CMAKE_CURRENT_LIST_DIR}/StringServer.cpp
	${CMAKE_CURRENT_LIST_DIR}/StringServer.h
	${CMAKE_CURRENT_LIST_DIR}/w_stdint.h
)

if (WIN32)
	set(SOURCES
		${SOURCES}
		${CMAKE_CURRENT_LIST_DIR}/resource.h
	)
	set_source_files_properties(commom.rc PROPERTIES LANGUAGE RC)
endif()

add_library(RetroSpyCommon SHARED ${SOURCES})
target_link_libraries(RetroSpyCommon uv connector)

