cmake_minimum_required(VERSION 3.22)
project(serde CXX)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_BUILD_TYPE Debug)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE
    "include"
)

if(NOT TARGET Boost::pfr)
	message(STATUS "Boost.PFR missing target")
	find_package(Boost 1.84.0)
	if(Boost_FOUND)
		target_include_directories(${PROJECT_NAME} INTERFACE ${Boost_INCLUDE_DIRS})
	else()
		message(STATUS "Using FetchContent Boost.PFR GIT_TAG: boost-1.86.0!")
		include(FetchContent)
		FetchContent_Declare(
			boost_pfr
			GIT_REPOSITORY https://github.com/boostorg/pfr.git
			GIT_TAG        boost-1.86.0
		)
		FetchContent_MakeAvailable(boost_pfr)
		target_link_libraries(${PROJECT_NAME} INTERFACE Boost::pfr)
	endif()
else()
	target_link_libraries(${PROJECT_NAME} INTERFACE Boost::pfr)
endif()

option(SERDE_TEST "Enable Serde Tests" OFF)
if(SERDE_TEST)
	enable_testing()
	add_library("${PROJECT_NAME}_tests" STATIC
		test/tests.cpp
	)

	target_link_libraries("${PROJECT_NAME}_tests" PRIVATE serde)
	target_include_directories("${PROJECT_NAME}_tests" INTERFACE include)

	if(WIN32)
		target_compile_definitions("${PROJECT_NAME}_tests" PRIVATE _WIN32_WINNT=0x0A00)
	endif()

	function(serde_add_test SERDE_TEST_NAME)
		add_executable("${PROJECT_NAME}_tests_${SERDE_TEST_NAME}"
			"test/${SERDE_TEST_NAME}.cpp"
		)

		if(WIN32)
			target_compile_definitions("${PROJECT_NAME}_tests_${SERDE_TEST_NAME}" PRIVATE _WIN32_WINNT=0x0A00)
		endif()

		target_link_libraries("${PROJECT_NAME}_tests_${SERDE_TEST_NAME}" PRIVATE
			"${PROJECT_NAME}_tests"
		)	

		add_test(
			NAME "Serde::Tests::${SERDE_TEST_NAME}"
			COMMAND "${PROJECT_NAME}_tests_${SERDE_TEST_NAME}"
		)
	endfunction()

	serde_add_test(object_super)
	serde_add_test(object_super_iterable)
	serde_add_test(object_super_iterable_empty)
endif()
