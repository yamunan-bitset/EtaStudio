cmake_minimum_required(VERSION 3.5)

project(EtaStudio)

set(SOURCE_FILES eta.c)
set_source_files_properties(${SOURCE_FILES} PROPERTIES LANGUAGE C)

add_library(eta STATIC ${SOURCE_FILES})
SET_TARGET_PROPERTIES(eta PROPERTIES LINKER_LANGUAGE C)
