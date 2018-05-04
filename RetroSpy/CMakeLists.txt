set(SOURCES
${CMAKE_CURRENT_LIST_DIR}/RConfig.cpp
${CMAKE_CURRENT_LIST_DIR}/RConfig.h
${CMAKE_CURRENT_LIST_DIR}/Database.cpp
${CMAKE_CURRENT_LIST_DIR}/Database.h
${CMAKE_CURRENT_LIST_DIR}/ini.c
${CMAKE_CURRENT_LIST_DIR}/ini.h
${CMAKE_CURRENT_LIST_DIR}/INIReader.cpp
${CMAKE_CURRENT_LIST_DIR}/INIReader.h
${CMAKE_CURRENT_LIST_DIR}/Main.cpp
${CMAKE_CURRENT_LIST_DIR}/Module.cpp
${CMAKE_CURRENT_LIST_DIR}/Module.h
${CMAKE_CURRENT_LIST_DIR}/ModuleManager.cpp
${CMAKE_CURRENT_LIST_DIR}/ModuleManager.h
)

if (WIN32)
	set(SOURCES ${SOURCES} resource.h)
	set_source_files_properties(RetroSpy.rc PROPERTIES LANGUAGE RC)
endif()

add_executable(RetroSpy ${SOURCES})
target_link_libraries(RetroSpy RetroSpyCommon)
