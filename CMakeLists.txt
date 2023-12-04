cmake_minimum_required(VERSION 3.10)

set(CMAKE_TOOLCHAIN_FILE $ENV{PS2SDK}/ps2dev.cmake)

project(library)

enable_language(C ASM)

add_executable(library library.c library.h l2handler.s)

# add_executable(library_test library_test.c)

target_link_libraries(library
	kernel
	)

#target_link_libraries(library_test
#	library
#	kernel
#	)

