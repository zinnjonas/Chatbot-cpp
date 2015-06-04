cmake_minimum_required(VERSION 3.1)
project(Chatbot-cpp)

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${CMAKE_SOURCE_DIR}/plugins)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -MMD -std=gnu++11")

set( PLUGIN_LIB
    dl)

set( CLOCK_FILES
     src/plugin/clock/clock.cpp)
add_library(clock MODULE ${CLOCK_FILES})

set( SPEAK_FILES
     src/plugin/speak/speak.cpp)
add_library(speak MODULE ${SPEAK_FILES})

set( SYSTEM_FILES
     src/plugin/system/system.cpp)
add_library(system MODULE ${SYSTEM_FILES})

set( ECHO_FILES
    src/plugin/echo/echo.cpp)

add_library(echo MODULE ${ECHO_FILES})

set( LIBS_BOT
     gloox
     sqlite3
     dl
     jsoncpp
     curl
    )

set(CHATBOT_FILES
    src/plugin/botplugin.h
    src/chatbot.cpp
    src/chatbot.h
    src/connectionlistener.cpp
    src/connectionlistener.h
    src/database.cpp
    src/database.h
    src/interface.h
    src/jsonwrapper.cpp
    src/jsonwrapper.h
    src/katarina.cpp
    src/katarina.h
    src/loadplugin.cpp
    src/loadplugin.h
    src/main.cpp
    src/plugin.h
    src/sendsocket.cpp
    src/sendsocket.h
    src/user.cpp
    src/user.h)

set( LIBS
    pthread
    Sockets
    ssl
    crypto
    jsoncpp
    iniReader)

if( WIN32)
    list( APPEND LIBS ws2_32 wsock32 gdi32)
endif()

add_executable(chatbot_cpp ${CHATBOT_FILES} src/defaultplugin.cpp src/defaultplugin.h)
target_link_libraries(chatbot_cpp LINK_PUBLIC ${LIBS_BOT} ${LIBS})
target_include_directories(chatbot_cpp PUBLIC iniReader)

set( SERVER_FILES
    Server/database.cpp
    Server/database.h
    Server/DisplaySocket.cpp
    Server/DisplaySocket.h
    Server/main.cpp)

add_executable(server ${SERVER_FILES})
target_link_libraries(server LINK_PUBLIC sqlite3 ${LIBS})

set( INIREADER_FILES
    iniReader/iniReader.cpp
    iniReader/iniReader.h)

add_library(iniReader ${INIREADER_FILES})