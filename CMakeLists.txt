cmake_minimum_required(VERSION	2.8)

project(capp)

#set(EXECUTABLE_OUTPUT_PATH	${CMAKE_SOURCE_DIR}/bin)
#set(LIBRARY_OUTPUT_PATH	${CMAKE_BINARY_DIR})

include_directories(
	PRIVATE
		${PROJECT_SOURCE_DIR}
)

add_executable(app	
		${PROJECT_SOURCE_DIR}/coins.cpp
)

