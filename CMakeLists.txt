cmake_minimum_required(VERSION 3.14)

project (htjs LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "-std=c++11 -O3 -fexceptions -DOJPH_DISABLE_INTEL_SIMD")

file(GLOB OPENJPH_SOURCES "${PROJECT_SOURCE_DIR}/external/OpenJPH/src/core/coding/*.cpp"
                    "${PROJECT_SOURCE_DIR}/external/OpenJPH/src/core/others/*.cpp"
                    "${PROJECT_SOURCE_DIR}/external/OpenJPH/src/core/codestream/*.cpp"
                    "${PROJECT_SOURCE_DIR}/external/OpenJPH/src/core/transform/ojph_colour.cpp"
                    "${PROJECT_SOURCE_DIR}/external/OpenJPH/src/core/transform/ojph_transform.cpp")

include_directories(src external/OpenJPH/src/core/common)

add_executable(HT_internal ${OPENJPH_SOURCES} src/cpp/HT_internal.cpp)

set_target_properties(HT_internal PROPERTIES LINK_FLAGS "--bind -s EXPORT_ES6=1 -s ALLOW_MEMORY_GROWTH=1 -s MODULARIZE=1 -s EXPORT_NAME=\"'makeHTCodec'\"")
