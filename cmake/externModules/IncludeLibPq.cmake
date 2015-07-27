#Подключение библиотеки libpq с проверкой версии cmake
#проверьте, что есть переменная CMAKE_MINOR_VERSION
MACRO(include_lib_pq)
   if (${CMAKE_MINOR_VERSION} LESS 8)
      message("Check postgresql")
      find_path(LIB_POSTGRESQL_INCLUDE_DIR libpq-fe.h HINTS /usr/include/pgsql /usr/include/postgresql)
      find_library(LIB_POSTGRESQL_LIBRARIES NAMES pq PATHS /usr/lib/ /usr/local/lib)
#       set(LIB_POSTGRESQL_LIBRARIES /usr/lib/libpq.so)
      
      if(${LIB_POSTGRESQL_INCLUDE_DIR} STREQUAL "LIB_POSTGRESQL_INCLUDE_DIR-NOTFOUND")
         message("Check postgresql ... NOT FOUND")
      else(${LIB_POSTGRESQL_INCLUDE_DIR} STREQUAL "LIB_POSTGRESQL_INCLUDE_DIR-NOTFOUND")
         message("Check postgresql ... OK")                           
      endif(${LIB_POSTGRESQL_INCLUDE_DIR} STREQUAL "LIB_POSTGRESQL_INCLUDE_DIR-NOTFOUND")

      include_directories(${LIB_POSTGRESQL_INCLUDE_DIR})
   else(${CMAKE_MINOR_VERSION} LESS 8)
      message("Check postgresql")
      find_package(LibPostgresql REQUIRED)

      if(${LIB_POSTGRESQL_INCLUDE_DIR} STREQUAL "LIB_POSTGRESQL_INCLUDE_DIR-NOTFOUND")
         message("Check postgresql ... NOT FOUND")
      else(${LIB_POSTGRESQL_INCLUDE_DIR} STREQUAL "LIB_POSTGRESQL_INCLUDE_DIR-NOTFOUND")
         message("Check postgresql ... OK")                           
      endif(${LIB_POSTGRESQL_INCLUDE_DIR} STREQUAL "LIB_POSTGRESQL_INCLUDE_DIR-NOTFOUND")                          

      include_directories(${LIB_POSTGRESQL_INCLUDE_DIR})
   endif(${CMAKE_MINOR_VERSION} LESS 8)
ENDMACRO(include_lib_pq)   

