cmake_minimum_required(VERSION 3.9.6)

set(APP_NAME MiningSimulatorGameServer)
project(${APP_NAME})

add_definitions("-std=c++11 -lpq")

include_directories(${PROJECT_SOURCE_DIR}/ThirdFunc)
include_directories(${PROJECT_SOURCE_DIR}/ThirdFunc/json/single_include)
include_directories("${PROJECT_SOURCE_DIR}/include/")

set(THIRDFUNC_HEADERS
        ${PROJECT_SOURCE_DIR}/ThirdFunc/json/single_include/nlohmann/json.hpp
        ${PROJECT_SOURCE_DIR}/ThirdFunc/tinyformat/tinyformat.h
        ${PROJECT_SOURCE_DIR}/ThirdFunc/fast-cpp-csv-parser/csv.h
        )

set(THIRDFUNC_SOURCES

        )

file(GLOB HEADER_FILES
        "include/ServerMS/*.h"
        "include/ServerMS/Network/*.h"
        "include/ServerMS/Time/*.h"
        "include/ServerMS/Log/*.h"
        "include/ServerMS/DB/*.h"
        )
file(GLOB SOURCE_FILES
        "src/ServerMS/*.cpp"
        "src/ServerMS/Network/*.cpp"
        "src/ServerMS/Time/*.cpp"
        "src/ServerMS/Log/*.cpp"
        "src/ServerMS/DB/*.cpp"
        )

include_directories("${PROJECT_SOURCE_DIR}/src/" "${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/ThirdFunc/MsgLists/")


set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)

find_package(Boost REQUIRED COMPONENTS date_time atomic system thread chrono regex signals)

if (Boost_FOUND)
    message(STATUS "include: ${Boost_INCLUDE_DIRS}")
    message(STATUS "lib: ${Boost_LIBRARIES}")
endif ()
include_directories(${Boost_INCLUDE_DIRS})
message(STATUS "${PROTOBUF_SOURCES}")

set(Protobuf_IMPORT_DIRS "${PROJECT_SOURCE_DIR}/ThirdFunc/ProtoFile")


find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})
SET_SOURCE_FILES_PROPERTIES(${PROTO_SRCS} ${PROTO_HDRS} PROPERTIES GENERATED TRUE)
set(PROTO_FILE
        ${Protobuf_IMPORT_DIRS}/Computer.proto
        ${Protobuf_IMPORT_DIRS}/Item.proto
        ${Protobuf_IMPORT_DIRS}/Shop.proto
        ${Protobuf_IMPORT_DIRS}/DBToGS.proto
        ${Protobuf_IMPORT_DIRS}/GCToGS.proto
        ${Protobuf_IMPORT_DIRS}/GSToDB.proto
        ${Protobuf_IMPORT_DIRS}/GSToGC.proto
        )

EXEC_PROGRAM(protoc ARGS --cpp_out ${PROJECT_SOURCE_DIR}/ThirdFunc/MsgLists --proto_path ${Protobuf_IMPORT_DIRS}
        ${PROTO_FILE})

file(GLOB PROTOBUF_HEADERS "${PROJECT_SOURCE_DIR}/ThirdFunc/MsgLists/*.pb.h")
file(GLOB PROTOBUF_SOURCES "${PROJECT_SOURCE_DIR}/ThirdFunc/MsgLists/*.pb.cc")

find_package(PostgreSQL REQUIRED)

include_directories(${PostgreSQL_INCLUDE_DIRS})
add_executable(${APP_NAME} ${HEADER_FILES} ${SOURCE_FILES} ${PROTOBUF_SOURCES} ${PROTOBUF_HEADERS}
        ${THIRDFUNC_HEADERS} ${THIRDFUNC_SOURCES} ${PROTO_HDRS} ${PROTO_SRCS}
        )

get_filename_component(full_path_stdafx ${CMAKE_CURRENT_SOURCE_DIR}/include/ServerMS/stdafx.h ABSOLUTE)
list(REMOVE_ITEM HEADER_FILES "${full_path_stdafx}")
message("${HEADER_FILES}")

ADD_LIBRARY(PCH STATIC "src/ServerMS/stdafx.cpp" "include/ServerMS/stdafx.h")

ADD_CUSTOM_COMMAND(TARGET PCH
        PRE_BUILD
        COMMAND ${CMAKE_CXX_COMMAND} -x c++-header ${CMAKE_CURRENT_SOURCE_DIR}/include/ServerMS/stdafx.h -o stdafx.h.pch)
SET_SOURCE_FILES_PROPERTIES(${SOURCE_FILES} ${HEADER_FILES}
        PROPERTIES
        COMPILE_FLAGS "-include ${CMAKE_CURRENT_SOURCE_DIR}/include/ServerMS/stdafx.h")

target_link_libraries(${APP_NAME} ${Boost_LIBRARIES} ${PROTOBUF_LIBRARIES} ${PostgreSQL_LIBRARIES} PCH -lpthread)