set(SOURCES
	${CMAKE_CURRENT_LIST_DIR}/QueryReport2.cpp
	${CMAKE_CURRENT_LIST_DIR}/QR2Server.h
	${CMAKE_CURRENT_LIST_DIR}/QR2Server.cpp
)
	

include_directories("${CMAKE_CURRENT_LIST_DIR}/../common")

add_library(QueryReport2 SHARED ${SOURCES})
target_link_libraries(QueryReport2 RetroSpyCommon)
