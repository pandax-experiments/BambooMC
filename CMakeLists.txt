cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(BambooMC)

# check for cxx compiler flag
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-std=c++14" COMPILER_SUPPORTS_CXX14)

if(COMPILER_SUPPORTS_CXX14)
  set(CMAKE_CXX_STANDARD 14)
else()
  message(FATAL_ERROR "You need a compiler with at least standard c++11 support")
endif()

# geant4
find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})
include_directories(${Geant4_INCLUDE_DIRS})

# root
#find_package(ROOT)
#include_directories(${ROOT_INCLUDE_DIRS})

# find Qt
find_package(Qt5Core QUIET)

set(QTLIB "")
if(Qt5Core_FOUND)
  message("Found Qt5Core")
  include_directories(${Qt5Core_INCLUDE_DIRS})
  set(QTLIB Qt5::Core)
else()
  unset(Qt5Core_DIR CACHE)
  find_package(Qt4 REQUIRED COMPONENTS QtCore)
  include_directories(${QT_QTCORE_INCLUDE_DIR})
  set(QTLIB Qt4::QtCore)
endif()

# set include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
#file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
#file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
#file(GLOB Detector ${PROJECT_SOURCE_DIR}/src/detector/*.cc)
#file(GLOB Physics ${PROJECT_SOURCE_DIR}/src/physics/*.cc)
#file(GLOB Generator ${PROJECT_SOURCE_DIR}/src/generator/*.cc)
#file(GLOB Analysis ${PROJECT_SOURCE_DIR}/src/analysis/*.cc)


# enable detector sets
set(ENABLE_DETECTOR_SETS "example" CACHE STRING "The detector sets to be enabled")
if(ENABLE_DETECTOR_SETS)
  string(REPLACE "," ";" DETECTOR_SET_LIST ${ENABLE_DETECTOR_SETS})
  foreach(DETECTOR_SET ${DETECTOR_SET_LIST})
    include(${PROJECT_SOURCE_DIR}/user/detectors/${DETECTOR_SET}/sources.cmake)
    include_directories(${PROJECT_SOURCE_DIR}/user/detectors/${DETECTOR_SET})
  endforeach(DETECTOR_SET)
  add_library(UserDetector ${Detector})
  target_link_libraries(UserDetector ${Geant4_LIBRARIES})
endif(ENABLE_DETECTOR_SETS)


add_library(BambooCore src/BambooControl.cc src/BambooUtils.cc src/BambooDetectorConstruction.cc)
target_link_libraries(BambooCore ${QTLIB} ${Geant4_LIBRARIES})

add_library(PandaXComponets src/PandaXPhysics.cc src/SimpleGPSGenerator.cc)
target_link_libraries(PandaXComponets ${Geant4_LIBRARIES})

add_executable(test_detector_parameters test/test_detector_parameters.cc)
target_link_libraries(test_detector_parameters BambooCore)

add_executable(test_control test/test_control.cc)
target_link_libraries(test_control "-Wl,--whole-archive" BambooCore PandaXComponets UserDetector "-Wl,--no-whole-archive")

add_executable(test_set test/test_set.cc)


add_executable(BambooMC BambooMC.cc)
target_link_libraries(BambooMC ${Geant4_LIBRARIES} BambooCore)
target_link_libraries(BambooMC "-Wl,--whole-archive" PandaXComponets "-Wl,--no-whole-archive")

if(ENABLE_DETECTOR_SETS)
  target_link_libraries(BambooMC "-Wl,--whole-archive" UserDetector "-Wl,--no-whole-archive")
endif(ENABLE_DETECTOR_SETS)
# target_link_libraries(BambooMC ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} BambooDetector BambooPhysics BambooGenerator BambooAnalysis BambooCore)

add_executable(parser src/parser.cc)
target_link_libraries(parser Qt5::Core)


#--- test related ---#
#SET(BUILD_TEST OFF CACHE BOOL "whether the test programs will be built")
#IF(BUILD_TEST)
#ENABLE_TESTING()
#FILE(GLOB TEST_SRC ${PROJECT_SOURCE_DIR}/test/*.cxx)
#FILE(COPY ${PROJECT_SOURCE_DIR}/test/test.xml DESTINATION ${PROJECT_BINARY_DIR})
#ADD_EXECUTABLE(bambooTest ${TEST_SRC})
#TARGET_LINK_LIBRARIES(bambooTest CppUTest CppUTestExt BambooDetector BambooCore BambooAnalysis ${ROOT_LIBRARIES})
#ADD_TEST(allTest bambooTest)
#ENDIF()
