cmake_minimum_required(VERSION 3.28)
project(serde C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_BUILD_TYPE Debug)

if(WIN32)
	set(Boost_NO_SYSTEM_PATHS    ON)
	set(Boost_USE_STATIC_RUNTIME ON)
	set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreadedDebug)
endif()
set(Boost_USE_STATIC_LIBS    ON)
set(Boost_USE_MULTITHREADED  ON)
find_package(Boost)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE
    "include"
    ${Boost_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} INTERFACE
    ${Boost_LIBRARIES}
)

option(SERDE_TEST "Enable Serde Tests" OFF)

if(SERDE_TEST)
	add_executable("${PROJECT_NAME}_tests_object_super"
		tests/object_super.cpp
	)

	if(WIN32)
		target_compile_definitions("${PROJECT_NAME}_tests_object_super" PRIVATE _WIN32_WINNT=0x0A00)
	endif()

	target_link_libraries("${PROJECT_NAME}_tests_object_super" PRIVATE
		serde
	)	

	enable_testing()
	add_test(NAME Serde::Tests::object_super
             COMMAND "${PROJECT_NAME}_tests_object_super")
endif()