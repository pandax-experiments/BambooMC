cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(BambooMC VERSION 1.99)

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
find_package(ROOT)

# find Qt
find_package(Qt5Core QUIET)

set(QTLIB "")
if(Qt5Core_FOUND)
  message("Found Qt5Core version ${Qt5Core_VERSION}")
  include_directories(${Qt5Core_INCLUDE_DIRS})
  set(QTLIB Qt5::Core)
else()
  unset(Qt5Core_DIR CACHE)
  find_package(Qt4 REQUIRED COMPONENTS QtCore)
  include_directories(${QT_QTCORE_INCLUDE_DIR})
  set(QTLIB Qt4::QtCore)
endif()

# find yaml-cpp
find_package(yaml-cpp 0.6 NO_MODULE)
if(yaml-cpp_FOUND)
  message("found yaml-cpp version ${yaml-cpp_VERSION}")
  add_compile_definitions(WITH_YAML_CPP)
  include_directories(${YAML_CPP_INCLUDE_DIR})
endif()

# set include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

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

# enable user mc
set(ENABLE_USER_MC "" CACHE STRING "The User MC to be enabled")
if(ENABLE_USER_MC)
  string(REPLACE "," ";" USER_MC_LIST ${ENABLE_USER_MC})
  foreach(USER_MC ${USER_MC_LIST})
    include(${PROJECT_SOURCE_DIR}/user/mc/${USER_MC}/sources.cmake)
    include_directories(${PROJECT_SOURCE_DIR}/user/mc/${USER_MC})
  endforeach(USER_MC)
  add_library(UserMC ${UserMC})
  target_include_directories(UserMC PRIVATE ${ROOT_INCLUDE_DIRS})
  target_link_libraries(UserMC ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})
endif(ENABLE_USER_MC)

add_library(BambooCore src/BambooControl.cc src/BambooUtils.cc src/BambooDetectorConstruction.cc)
target_link_libraries(BambooCore ${QTLIB} ${Geant4_LIBRARIES})
if(yaml-cpp_FOUND)
  target_link_libraries(BambooCore ${YAML_CPP_LIBRARIES})
endif()

set(PandaXAnalysis src/PandaXEnergyDepositionHit.cc src/PandaXSurfaceFluxHit.cc src/PandaXSensitiveDetector.cc src/PandaXTrackingAction.cc src/PandaXDataManager.cc src/PandaXRunAction.cc src/PandaXEventAction.cc src/PandaXStackingAction.cc src/PandaXSteppingAction.cc src/PandaXAnalysis.cc)

add_library(PandaXMC src/PandaXPhysics.cc src/PandaXOpticalPhysics.cc src/SimpleGPSGenerator.cc src/HEPEvtLoader.cc src/GdmlLoader.cc ${PandaXAnalysis})
target_include_directories(PandaXMC PRIVATE ${ROOT_INCLUDE_DIRS})
target_link_libraries(PandaXMC ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

add_executable(BambooMC BambooMC.cc)
target_link_libraries(BambooMC ${Geant4_LIBRARIES} BambooCore)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_link_libraries(BambooMC "-Wl,--whole-archive" PandaXMC "-Wl,--no-whole-archive")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  target_link_libraries(BambooMC "-Wl,-all_load" PandaXMC)
endif()

if(ENABLE_DETECTOR_SETS)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_link_libraries(BambooMC "-Wl,--whole-archive" UserDetector "-Wl,--no-whole-archive")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  target_link_libraries(BambooMC "-Wl,-all_load" UserDetector)
endif()
endif(ENABLE_DETECTOR_SETS)

if(ENABLE_USER_MC)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_link_libraries(BambooMC "-Wl,--whole-archive" UserMC "-Wl,--no-whole-archive")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  target_link_libraries(BambooMC "-Wl,-all_load" UserMC)
endif()
endif(ENABLE_USER_MC)

install(TARGETS BambooMC RUNTIME DESTINATION bin)
install(DIRECTORY config/ DESTINATION share/bamboo-mc/config)
install(DIRECTORY macro/ DESTINATION share/bamboo-mc/macro)
install(DIRECTORY data/ DESTINATION share/bamboo-mc/data)
