###############################################
set (PROJECT_LIB_CONTACT _raps)
project(${PROJECT_LIB_CONTACT})
cmake_minimum_required(VERSION 2.4)
# option(SNMP_REQUIRED "set SNMP_REQUIRED option ON if class TSnmp required" OFF)                     
###############################################            
set(LIB_INSTALL_PATH ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/externModules)
message(${CMAKE_MODULE_PATH}) 

INCLUDE(IncludeLibPq)
include_lib_pq()
set(LIBRARIES_INCLUDE,${LIB_POSTGRESQL_INCLUDE_DIR})

foreach(LIB_INC ${LIBRARIES_INCLUDE})
   include_directories(${LIB_INC})
endforeach(LIB_INC)

set(HEADERS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SOURCES_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories( ${HEADERS_DIRECTORY} ${SOURCES_DIRECTORY})


set(HEADERS
${HEADERS_DIRECTORY}/common_funcs.h
${HEADERS_DIRECTORY}/Database.h
${HEADERS_DIRECTORY}/defines.h
${HEADERS_DIRECTORY}/simpleclient.h 
)

set(SOURCES
${SOURCES_DIRECTORY}/common_funcs.cpp
${SOURCES_DIRECTORY}/Database.cpp
${SOURCES_DIRECTORY}/simpleclient.cpp
)


###############################################
INCLUDE(IncludeOnlyHeaders)
include_only_headers(${HEADERS})
###############################################
set(CMAKE_EXE_LINKER_FLAGS "-pg")
add_definitions(-pipe -O2 -Wall -W -ggdb -g -pg -fPIC -D_REENTRANT) #для профилировщика gprof
#add_definitions(-pipe -O2 -Werror -Wall -W -ggdb -fPIC -D_REENTRANT)

###############################################
#Preven warn
if(COMMAND cmake_policy)
   cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)
###############################################
add_library (${PROJECT_LIB_CONTACT} SHARED ${HEADERS} ${SOURCES})
#add_executable (${PROJECT_LIB_CONTACT} ${HEADERS} ${SOURCES})
target_link_libraries(${PROJECT_LIB_CONTACT} ${LIB_POSTGRESQL_LIBRARIES} ${CLANG_LIBS})

# set_target_properties(${PROJECT_LIB_CONTACT} PROPERTIES LINK_FLAGS "-g -pg") #для профилировщика gprof
#install(TARGETS ${PROJECT_LIB_DCM} DESTINATION ${LIB_INSTALL_PATH})
install(TARGETS ${PROJECT_LIB_CONTACT} LIBRARY DESTINATION ${LIB_INSTALL_PATH})