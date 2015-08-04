###############################################
set (PROJECT_CONTACT biser)
project(${PROJECT_CONTACT})
cmake_minimum_required(VERSION 2.4) 
###############################################
set(CMAKE_MODULE_PATH 
${CMAKE_SOURCE_DIR}/cmake
${CMAKE_SOURCE_DIR}/cmake/externModules
)
message(${CMAKE_MODULE_PATH})
#Preven warn
if(COMMAND cmake_policy)
   cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
###############################################
include_directories(${CMAKE_SOURCE_DIR}/src_lib/)
###############################################
set(LIBRARIES_CONTACT
lib_raps
lib_dcm
lib_sk
)
###############################################
INCLUDE(FindThreads)
INCLUDE(IncludeLibPq)
include_lib_pq()
###############################################

set(LIBRARIES_CONTACT_NAME
_raps
_dcm
_sk
pthread
)
###############################################
message("Libraries will link whith biser")
foreach(LIBRARY ${LIBRARIES_CONTACT})
   include_directories(${CMAKE_SOURCE_DIR}/src_lib/${LIBRARY}/ ${CMAKE_SOURCE_DIR}/src_lib/${LIBRARY}/include)
   message(${CMAKE_SOURCE_DIR}/src_lib/${LIBRARY}/)
endforeach(LIBRARY)
###############################################
set(HEADERS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SOURCES_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src)
#set(RPM_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/rpm)
###############################################
include_directories( ${HEADERS_DIRECTORY} ${SOURCES_DIRECTORY})
###############################################
set(HEADERS
${HEADERS_DIRECTORY}/BDReadConf.h
${HEADERS_DIRECTORY}/BDState.h
${HEADERS_DIRECTORY}/UiTcp.h
${HEADERS_DIRECTORY}/nsdManage.h
${HEADERS_DIRECTORY}/DcmNsd.h
${HEADERS_DIRECTORY}/DcmUps.h
${HEADERS_DIRECTORY}/BDStateNsd.h
${HEADERS_DIRECTORY}/BDStateUps.h
)
##############################################
set(SOURCES
${SOURCES_DIRECTORY}/main.cpp
${SOURCES_DIRECTORY}/BDReadConf.cpp
${SOURCES_DIRECTORY}/BDState.cpp
${SOURCES_DIRECTORY}/UiTcp.cpp
${SOURCES_DIRECTORY}/nsdManage.cpp
${SOURCES_DIRECTORY}/DcmNsd.cpp
${SOURCES_DIRECTORY}/DcmUps.cpp
${SOURCES_DIRECTORY}/BDStateNsd.cpp
${SOURCES_DIRECTORY}/BDStateUps.cpp
)
###############################################
#Включение в зависимости только h-ов, чтобы при 
#изменении cpp'ов в библиотеках не пересобиралось все подряд
set(ALLHEADERS ${HEADERS})
INCLUDE(IncludeOnlyHeaders)
include_only_headers(${ALLHEADERS})

 add_definitions(-pipe -O2 -Wall -W -Wunused-parameter -g -pg -ggdb -fPIC -D_REENTRANT) #для профилировщика gprof
add_executable(${PROJECT_CONTACT} ${HEADERS} ${SOURCES})

###############################################
target_link_libraries(${PROJECT_CONTACT} ${LIBRARIES_CONTACT_NAME})
###############################################
install(TARGETS ${PROJECT_CONTACT} DESTINATION ${RUNTIME_OUTPUT_DIRECTORY})
###############################################
