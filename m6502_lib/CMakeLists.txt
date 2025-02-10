cmake_minimum_required(VERSION 3.7)

project(M6502Lib)

if(MSVC)
    add_compile_options(/MP)    # use multiple preprocessors when building
    add_compile_options(/W4 /wd4201 /WX)    # warnings level 4, all warnings treated as errors
else()
    add_compile_options((-W -Wall -Werror)) # all warnins to be treated as errors
endif()

set (M6502_SOURCES
    "src/public/main_6502.h"
    "src/private/main_6502.cpp")

source_group("src" FILES ${M6502_SOURCES})

add_library(M6502Lib ${M6502_SOURCES})

target_include_directories(M6502Lib PUBLIC "${PROJECT_SOURCE_DIR}/src/pulic")
target_include_directories(M6502Lib PRIVATE "${PROJECT_SOURCE_DIR}/src/private")

# set_target_properties(M6502Lib PROPERTIES FOLDER "M6502Lib")