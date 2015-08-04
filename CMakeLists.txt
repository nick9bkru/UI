###############################################
set (PROJECT_CONTACT UIControler)
project(${PROJECT_CONTACT})
cmake_minimum_required(VERSION 2.4)
###############################################
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
###############################################
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)
################################################
set(CMAKE_MODULE_PATH 
${CMAKE_SOURCE_DIR}/cmake
${CMAKE_SOURCE_DIR}/cmake/externModules
)
###############################################
option(POSTGRESQL_REQUIRED "set POSTGRESQL_REQUIRED option ON if class TpostgreSQL required" ON)
if( NOT POSTGRESQL_REQUIRED)
    message("ERROR !!!!!!! POSTGRESQL is not installed")
   return()
endif(NOT POSTGRESQL_REQUIRED)
###############################################
set(LIBRARIES				# Не переносите LIBRARIES на другую строку, пожалуйста, - не будет работать автодобавление библиотек
  libsk
  libdcm
  libraps
)
###############################################
set(RUNTIME_PROGRAMMS
  contact
 biser
)
###############################################
message(${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_VERSION})
###############################################
foreach(LIBRARY ${LIBRARIES})
   add_subdirectory(${CMAKE_SOURCE_DIR}/src_lib/${LIBRARY} ${CMAKE_SOURCE_DIR}/build/${LIBRARY})
   include_directories(${CMAKE_SOURCE_DIR}/src_lib/${LIBRARY}/include/ ${CMAKE_SOURCE_DIR}/src_lib/${LIBRARY})
endforeach(LIBRARY)
###############################################
foreach(PROGRAMM ${RUNTIME_PROGRAMMS})
   add_subdirectory(${CMAKE_SOURCE_DIR}/src/${PROGRAMM} ${CMAKE_SOURCE_DIR}/build/${PROGRAMM})
   include_directories(${CMAKE_SOURCE_DIR}/src/${PROGRAMM}/include/)   
endforeach(PROGRAMM)
###############################################
