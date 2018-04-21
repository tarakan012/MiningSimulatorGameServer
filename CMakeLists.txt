cmake_minimum_required(VERSION 3.9.6)

set(APP_NAME MiningSimulatorGameServer)
project (${APP_NAME})

add_definitions("-std=c++11 -lpq")

set(TinyFormat_DIRS ${PROJECT_SOURCE_DIR}/ThirdFunc/TinyFormat)
set(JsonCocos_DIRS ${PROJECT_SOURCE_DIR}/ThirdFunc/JsonCocos)

include_directories(${TinyFormat_DIRS} ${JsonCocos_DIRS})

set(THIRDFUNC_HEADERS
	${JsonCocos_DIRS}/Json.h
    ${TinyFormat_DIRS}/tinyformat.h
    ${JsonCocos_DIRS}/extension.c
        )

set(THIRDFUNC_SOURCES
    ${JsonCocos_DIRS}/Json.c
    ${JsonCocos_DIRS}/extension.c
	)

file(GLOB HEADER_FILES "include/*.h")
file(GLOB SOURCE_FILES "src/*.cpp")

include_directories("${PROJECT_SOURCE_DIR}/src/" "${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/ThirdFunc/MsgLists/")


set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)

find_package(Boost REQUIRED COMPONENTS date_time atomic system thread chrono regex signals)

if(Boost_FOUND)
    message(STATUS "include: ${Boost_INCLUDE_DIRS}")
    message(STATUS "lib: ${Boost_LIBRARIES}")
endif()
include_directories(${Boost_INCLUDE_DIRS})
message(STATUS "${PROTOBUF_SOURCES}")

set(Protobuf_IMPORT_DIRS "${PROJECT_SOURCE_DIR}/ThirdFunc/ProtoFile")


find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})
SET_SOURCE_FILES_PROPERTIES(${PROTO_SRCS} ${PROTO_HDRS} PROPERTIES GENERATED TRUE)
set(PROTO_FILE
        ${Protobuf_IMPORT_DIRS}/util.proto
        ${Protobuf_IMPORT_DIRS}/DBToGS.proto
        ${Protobuf_IMPORT_DIRS}/GCToGS.proto
        ${Protobuf_IMPORT_DIRS}/GSToDB.proto
        ${Protobuf_IMPORT_DIRS}/GSToGC.proto
)

EXEC_PROGRAM(protoc ARGS --cpp_out ${PROJECT_SOURCE_DIR}/ThirdFunc/MsgLists --proto_path ${Protobuf_IMPORT_DIRS}
        ${PROTO_FILE})

file(GLOB PROTOBUF_HEADERS "${PROJECT_SOURCE_DIR}/ThirdFunc/MsgLists/*.pb.h")
file(GLOB PROTOBUF_SOURCES "${PROJECT_SOURCE_DIR}/ThirdFunc/MsgLists/*.pb.cc")

find_package (PostgreSQL REQUIRED)

include_directories(${PostgreSQL_INCLUDE_DIRS})
add_executable(${APP_NAME} ${HEADER_FILES} ${SOURCE_FILES} ${PROTOBUF_SOURCES} ${PROTOBUF_HEADERS}
        ${THIRDFUNC_HEADERS}  ${THIRDFUNC_SOURCES} ${PROTO_HDRS} ${PROTO_SRCS}
        )

target_link_libraries(${APP_NAME}  ${Boost_LIBRARIES} ${PROTOBUF_LIBRARIES} ${PostgreSQL_LIBRARIES} -lpthread)