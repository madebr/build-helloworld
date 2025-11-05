cmake_minimum_required(VERSION 3.0...4.0)
project(helloworld C)

add_library(library_static STATIC src_lib_static.c src_lib_static.h)

add_library(library_shared SHARED src_lib_shared.c)
set_property(TARGET library_shared PROPERTY DEFINE_SYMBOL "BUILDING_SHARED_LIB")

add_executable(exe_cli src_cli.c)
target_link_libraries(exe_cli PRIVATE library_static library_shared)

add_executable(exe_gui WIN32 src_gui.c src_gui.rc)
target_link_libraries(exe_gui PRIVATE library_static library_shared)

