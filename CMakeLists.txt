cmake_minimum_required (VERSION 3.15)
project (margoexp C CXX)

SET(CMAKE_C_COMPILER gcc)
SET(CMAKE_CXX_COMPILER g++)

# add our cmake module directory to the path
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
     "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include (xpkg-import)
find_package (mercury REQUIRED)
include_directories(${MERCURY_INCLUDE_DIR})
xpkg_import_module (argobots REQUIRED argobots)
xpkg_import_module (margo REQUIRED margo)
xpkg_import_module (abtio REQUIRED abt-io)

add_executable(margoserver ./margoexo_server.c ./my-rpc.c)
target_link_libraries(margoserver margo)

add_executable(margoclient ./margoexp_client.c)
target_link_libraries(margoclient margo)