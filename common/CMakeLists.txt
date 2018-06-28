set(SOURCES
	${CMAKE_CURRENT_LIST_DIR}/chc_endian.cpp
	${CMAKE_CURRENT_LIST_DIR}/chc_endian.h
	${CMAKE_CURRENT_LIST_DIR}/Helper.cpp
	${CMAKE_CURRENT_LIST_DIR}/Helper.h
	${CMAKE_CURRENT_LIST_DIR}/Helper_Base64.cpp
	${CMAKE_CURRENT_LIST_DIR}/Helper_Query.cpp
	${CMAKE_CURRENT_LIST_DIR}/Helper_Random.cpp
	${CMAKE_CURRENT_LIST_DIR}/md5.c
	${CMAKE_CURRENT_LIST_DIR}/md5.h
	${CMAKE_CURRENT_LIST_DIR}/StringServer.cpp
	${CMAKE_CURRENT_LIST_DIR}/StringServer.h
)

add_library(RetroSpyCommon STATIC ${SOURCES})
target_link_libraries(RetroSpyCommon MDK)

