cmake_minimum_required(VERSION 3.10)
project(fibonacci_project C)

set(CMAKE_C_STANDART 11)

set(OPTIMIZATION_LEVELS O0 O1 O2 Os)

add_library(fibonacci_static STATIC src/fibonacci.c src/fibonacci_recursive.c)
target_include_directories(fibonacci_static PUBLIC include)
set_target_properties(fibonacci_static PROPERTIES OUTPUT_NAME "fibonacci")

add_library(fibonacci_shared SHARED src/fibonacci.c src/fibonacci_recursive.c)
target_include_directories(fibonacci_shared PUBLIC include)
set_target_properties(fibonacci_shared PROPERTIES OUTPUT_NAME "fibonacci")

add_executable(fibonacci_main src/main.c)
target_link_libraries(fibonacci_main fibonacci_static)

foreach(opt IN LISTS OPTIMIZATION_LEVELS)
	add_executable(speed_test_static_${opt} test/speed_test.c)
	target_compile_options(speed_test_static_${opt} PRIVATE -${opt})
	target_link_libraries(speed_test_static_${opt} fibonacci_static)

	add_executable(speed_test_shared_${opt} test/speed_test.c)
	target_compile_options(speed_test_shared_${opt} PRIVATE -${opt})
	target_link_libraries(speed_test_shared_${opt} fibonacci_shared)

	add_executable(dlopen_test_${opt} dlopen_test/dlopen_test.c)
	target_compile_options(dlopen_test_${opt} PRIVATE -${opt})
	target_link_libraries(dlopen_test_${opt} dl)
endforeach()

#add_custom_target(run_speed_tests
#	COMMAND /bin/bash -c "for opt in O0 O1 O2 Os; do
#		echo \"Testing with -\${opt} optimization:\";
#		echo \"Static linking:\"; ./speed_test_static_\${opt};
#		echo \"Dynamic linking:\"; ./speed_test_shared_\${opt};
#		echo \"Dlopen:\"; ./dlopen_test_\${opt};
#		echo \"---------------------\";
#	done"
#	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
#)
