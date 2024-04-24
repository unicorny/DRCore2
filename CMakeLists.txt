# Setup inspired from https://alexreinking.com/blog/building-a-dual-shared-and-static-library-with-cmake.html
cmake_minimum_required(VERSION 3.19)
project(DRCore2 VERSION 1.0.0)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

if (NOT DEFINED CMAKE_CXX_VISIBILITY_PRESET AND
    NOT DEFINED CMAKE_VISIBILITY_INLINES_HIDDEN)
  set(CMAKE_CXX_VISIBILITY_PRESET hidden)
  set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)
endif ()

if (DEFINED DRCore2_SHARED_LIBS)
    set(BUILD_SHARED_LIBS "${DRCore2_SHARED_LIBS}")
endif ()


FILE(GLOB ALGORITHM "include/DRCore2/Algorithm/*.h" "src/Algorithm/*.cpp")
FILE(GLOB FOUNDATION "include/DRCore2/Foundation/*.h" "src/Foundation/*.cpp")
FILE(GLOB MANAGER "include/DRCore2/Manager/*.h" "src/Manager/*.cpp")
FILE(GLOB UTILS "include/DRCore2/Utils/*.h" "src/Utils/*.cpp")
FILE(GLOB THREADING "include/DRCore2/Threading/*.h" "src/Threading/*.cpp")
FILE(GLOB SRC "include/DRCore2/*.h" "src/*.cpp")

if (MSVC)
  source_group("Algorithm" FILES ${ALGORITHM})
  source_group("Foundation" FILES ${FOUNDATION})
  source_group("Manager" FILES ${MANAGER})
  source_group("Threading" FILES ${THREADING})
  source_group("Utils" FILES ${UTILS})
endif()

add_library(DRCore2 ${ALGORITHM} ${FOUNDATION} ${MANAGER} ${THREADING} ${SRC} ${UTILS})
add_library(DRCore2::DRCore2 ALIAS DRCore2)
set_target_properties(DRCore2 PROPERTIES
                      VERSION ${DRCore2_VERSION}
                      SOVERSION ${DRCore2_VERSION_MAJOR})
target_include_directories(
    DRCore2 PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>")
target_compile_features(DRCore2 PUBLIC cxx_std_17)

#IF(UNIX)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -mavx")
#ENDIF(UNIX)
#if(MSVC)
    #set(CMAKE_CXX_FLAGS "/arch:AVX2")  
#endif(MSVC)

include(GenerateExportHeader)
generate_export_header(DRCore2 EXPORT_FILE_NAME include/DRCore2/export.h)
target_compile_definitions(
    DRCore2 PUBLIC "$<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:DRCORE2_STATIC_DEFINE>")
target_include_directories(
    DRCore2 PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>")

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)
option(DRCore2_INCLUDE_PACKAGING "Include packaging rules for DRCore2" "${is_top_level}")
if (DRCore2_INCLUDE_PACKAGING)
    add_subdirectory(packaging)
endif ()


