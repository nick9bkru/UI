set (PROJECT_LIB_CONTACT _sk)
project(${PROJECT_CONTACT})
cmake_minimum_required(VERSION 2.4)
# option(SNMP_REQUIRED "set SNMP_REQUIRED option ON if class TSnmp required" OFF)                     
###############################################
set(LIB_INSTALL_PATH ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/externModules)
message(${CMAKE_MODULE_PATH}) 
###############################################
foreach(LIB_INC ${LIBRARIES_INCLUDE})
   include_directories(${LIB_INC})
endforeach(LIB_INC)
###############################################
set(HEADERS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SOURCES_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories( ${HEADERS_DIRECTORY} ${SOURCES_DIRECTORY})
###############################################
set(HEADERS
${HEADERS_DIRECTORY}/DataInterface.h
${HEADERS_DIRECTORY}/InterfaceSet.h
${HEADERS_DIRECTORY}/DeviceInterface.h
${HEADERS_DIRECTORY}/Log.h 
${HEADERS_DIRECTORY}/LogSingleton.h 
${HEADERS_DIRECTORY}/TCPSocketInterface.h
${HEADERS_DIRECTORY}/util.h
${HEADERS_DIRECTORY}/PMutexLocker.h
${HEADERS_DIRECTORY}/Singleton.h
)
###############################################
set(SOURCES
${SOURCES_DIRECTORY}/DataInterface.cpp
${SOURCES_DIRECTORY}/InterfaceSet.cpp
${SOURCES_DIRECTORY}/TCPSocketInterface.cpp
${SOURCES_DIRECTORY}/util.cpp
${SOURCES_DIRECTORY}/DeviceInterface.cpp
${SOURCES_DIRECTORY}/Log.cpp
${SOURCES_DIRECTORY}/LogSingleton.cpp
${SOURCES_DIRECTORY}/PMutexLocker.cpp
)
###############################################
INCLUDE(IncludeOnlyHeaders)
include_only_headers(${HEADERS})
###############################################
set(CMAKE_EXE_LINKER_FLAGS "-pg")
add_definitions(-pipe -O2 -unused-result -Wall -W -ggdb -g -pg -fPIC -D_REENTRANT) #для профилировщика gprof
#add_definitions(-pipe -O2 -Werror -Wall -W -ggdb -fPIC -D_REENTRANT)
###############################################

#Preven warn
if(COMMAND cmake_policy)
   cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)
###############################################
add_library (${PROJECT_LIB_CONTACT} SHARED ${HEADERS} ${SOURCES})
target_link_libraries(${PROJECT_LIB_CONTACT} ${LIBRARIES_LIB_CONTACT} ${CLANG_LIBS})
###############################################

###############################################
install(TARGETS ${PROJECT_LIB_CONTACT} LIBRARY DESTINATION ${LIB_INSTALL_PATH})
###############################################
