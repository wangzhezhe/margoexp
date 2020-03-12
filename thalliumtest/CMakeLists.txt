cmake_minimum_required (VERSION 3.14)
project (thalliumtest C CXX)
set (CMAKE_CXX_STANDARD 14)

option(ENABLE_TESTING "Enable Testing" ON)
option(ENABLE_EXAMPLE "Enable ENABLE_EXAMPLE" ON)
option(USE_TIMERS "Use profiling timers" ON)
enable_testing()
add_definitions(-DENABLE_TIMERS)

find_package(MPI REQUIRED) 

# add our cmake module directory to the path
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
     "${CMAKE_CURRENT_SOURCE_DIR}/cmake")


# link shared lib with full rpath
set (CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set (CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# setup cache variables for ccmake
if (NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE Release
         CACHE STRING "Choose the type of build." FORCE)
    set_property (CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS 
                  "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
endif ()
set (CMAKE_PREFIX_PATH "" CACHE STRING "External dependencies path")
set (BUILD_SHARED_LIBS "OFF" CACHE BOOL "Build a shared library")

# packages we depend on (load all the necessary depedency by spack)
include (xpkg-import)
find_package (mercury REQUIRED)
include_directories(${MERCURY_INCLUDE_DIR})
xpkg_import_module (argobots REQUIRED argobots)
xpkg_import_module (margo REQUIRED margo)
xpkg_import_module (abtio REQUIRED abt-io)
find_package (thallium REQUIRED)

# compile the sim-inline-adios

option(USE_GNI "use cray gni" OFF)

if(USE_GNI)
  add_definitions(-DUSE_GNI)
  find_package(DRC REQUIRED)
  include_directories(${DRC_INCLUDE_DIRS})
  message ("use cray DRC")
  message ("include drc" ${DRC_INCLUDE_DIRS})
  message ("link drc lib" ${DRC_LIBRARIES})


  add_executable(testserver 
  ./testserver.cpp
  ./client.cpp
  )
target_link_libraries(testserver thallium ${DRC_LIBRARIES})


  add_executable(testclient
  ./testclient.cpp
  ./client.cpp
  )
target_link_libraries(testclient thallium MPI::MPI_C ${DRC_LIBRARIES})

else()

  add_executable(testserver 
  ./testserver.cpp
  ./client.cpp
  )
target_link_libraries(testserver thallium)


  add_executable(testclient
  ./testclient.cpp
  ./client.cpp
  )
target_link_libraries(testclient thallium MPI::MPI_C )


endif()







