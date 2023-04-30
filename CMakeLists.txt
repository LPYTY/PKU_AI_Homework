cmake_minimum_required(VERSION 3.0.0)
project(hw7)
cmake_policy(SET CMP0054 NEW)
aux_source_directory(./src SOURCE_FILES)
include_directories(./includes)
if(MSVC)
add_compile_options(/MT)
elseif(MINGW)
add_compile_options(-lws2_32)
endif(MSVC)
link_directories(./lib)
add_executable(hw7 ${SOURCE_FILES})
target_link_libraries(hw7 PUBLIC rjsjai)
#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set_target_properties(
	hw7 
	PROPERTIES 
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED True
)
if(MSVC)
set_target_properties(hw7 PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin)
set_target_properties(hw7 PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin)
endif(MSVC)
#install(TARGETS hw7 RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/bin)