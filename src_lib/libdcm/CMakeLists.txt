set (PROJECT_LIB_DCM _dcm)
project(${PROJECT_LIB_DCM})
cmake_minimum_required(VERSION 2.4)
# option(SNMP_REQUIRED "set SNMP_REQUIRED option ON if class TSnmp required" OFF)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/externModules
                      ${CMAKE_SOURCE_DIR}/cmake/externModules)
set(LIB_INSTALL_PATH ${CMAKE_SOURCE_DIR}/lib)

set(LIBRARIES_LIB_DCM
)

set(LIBRARIES_LIB_DCM_NAME
pthread
)

foreach(LIBRARY ${LIBRARIES_LIB_DCM})   
   include_directories(${CMAKE_SOURCE_DIR}/src_lib/${LIBRARY}/include/ ${CMAKE_SOURCE_DIR}/build/${LIBRARY})
endforeach(LIBRARY)

set(HEADERS_DIRECTORY ./include)
set(SOURCES_DIRECTORY ./src)
include_directories( ${HEADERS_DIRECTORY} ${SOURCES_DIRECTORY})

###############################################
message(${CMAKE_SYSTEM_VERSION} )
string(REGEX MATCH "^.\\.." SYSTEM_VERSION ${CMAKE_SYSTEM_VERSION})
message(${SYSTEM_VERSION} )
string(COMPARE GREATER ${SYSTEM_VERSION} "2.4" IS_USLEEP_ENABLED)
message(${IS_USLEEP_ENABLED} )
if(IS_USLEEP_ENABLED)
   set(USLEEP_ENABLED 1)
endif(IS_USLEEP_ENABLED)
###############################################

set(HEADERS
${HEADERS_DIRECTORY}/tdrycontactmodule.h
${HEADERS_DIRECTORY}/TDcmCli.h
)

set(SOURCES
${SOURCES_DIRECTORY}/tdrycontactmodule.cpp
${SOURCES_DIRECTORY}/TDcmCli.cpp
)

###############################################
set(ALLHEADERS ${HEADERS})
# file(GLOB HDRS ${HEADERS_DIRECTORY}/*.h*)   #Команда запихивает все файлы из директории (удовл шаблону) в строку
#Формируем regexp для include_regular_expression - чтобы оганичить число зависимостей для проекта armotu
string(REPLACE ${HEADERS_DIRECTORY}/ "|" ALLHEADERS ${ALLHEADERS})  #Отбрасываем абсолютный путь до хедеров
string(REPLACE ".h" "" ALLHEADERS ${ALLHEADERS})                    #Отбрасываем расширения
string(REGEX REPLACE "^\\|" "" ALLHEADERS ${ALLHEADERS})            #Убираем в самом начале лишний символ |
set(ALLHEADERS "${ALLHEADERS}\\.h$")                                #Последний штрих в regexp - означает - все строки, заканчивающиеся на .h
include_regular_expression(${ALLHEADERS})                           #Имеем file1|file2|file3\.h$ - означает file1.h или file2.h или file3.h

###############################################
INCLUDE(IncludeOnlyHeaders)
include_only_headers(${HEADERS})
###############################################
set(CMAKE_EXE_LINKER_FLAGS "-pg")
add_definitions(-pipe -O2 -Werror -Wall -W -ggdb -g -pg -fPIC -D_REENTRANT) #для профилировщика gprof
#add_definitions(-pipe -O2 -Werror -Wall -W -ggdb -fPIC -D_REENTRANT)

#Preven warn
if(COMMAND cmake_policy)
   cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

add_library (${PROJECT_LIB_DCM} SHARED ${HEADERS} ${SOURCES})
target_link_libraries(${PROJECT_LIB_DCM} ${LIBRARIES_LIB_DCM_NAME} ${CLANG_LIBS})
if(DCM_SNMP_EXIST)
   target_link_libraries(${PROJECT_LIB_DCM} ${LIB_DCM_SNMP_LIBRARIES})
endif(DCM_SNMP_EXIST)
###############################################
install(TARGETS ${PROJECT_LIB_DCM} LIBRARY DESTINATION lib)