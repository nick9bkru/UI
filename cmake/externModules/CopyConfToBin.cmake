#Аккуратное копирование conf'ов из ./doc'ов в ./bin'ы с проверкой есть ли
#там уже такой файл или нет
# SRC_DIR - относительный путь, например doc
# файлы должны лежать в директории ./src/projectname/${SRC_DIR}
# проверьте, что есть переменная EXECUTABLE_OUTPUT_PATH
MACRO(copy_conf_to_bin FILENAME SRC_DIR)
      message(COPY_ ${CMAKE_CURRENT_SOURCE_DIR})
      message(COPY_ ${SRC_DIR})
   if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${SRC_DIR}/${FILENAME}.conf)
      if(NOT EXISTS ${EXECUTABLE_OUTPUT_PATH}/${FILENAME}.conf)
         message("Copy ${FILENAME}.conf...")
         if (${CMAKE_MINOR_VERSION} LESS 8)
            message(<2.6)
            execute_process(COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/${SRC_DIR}/${FILENAME}.conf ${EXECUTABLE_OUTPUT_PATH})
         else(${CMAKE_MINOR_VERSION} LESS 8)
            message(>2.6)
               file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/${SRC_DIR}/${FILENAME}.conf DESTINATION ${EXECUTABLE_OUTPUT_PATH})
         endif(${CMAKE_MINOR_VERSION} LESS 8)
      endif(NOT EXISTS ${EXECUTABLE_OUTPUT_PATH}/${FILENAME}.conf)
   endif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${SRC_DIR}/${FILENAME}.conf)
ENDMACRO(copy_conf_to_bin)   
#Аналог предыдущей функции, без добавления расширения .conf к имени файла
MACRO(copy_to_bin FILENAME SRC_DIR)
   if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${SRC_DIR}/${FILENAME})
      if(NOT EXISTS ${EXECUTABLE_OUTPUT_PATH}/${FILENAME})
         message("Copy ${FILENAME}...")
         if (${CMAKE_MINOR_VERSION} LESS 8)
            message(<2.6)
            execute_process(COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/${SRC_DIR}/${FILENAME} ${EXECUTABLE_OUTPUT_PATH})
         else(${CMAKE_MINOR_VERSION} LESS 8)
            message(>2.6)
               file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/${SRC_DIR}/${FILENAME} DESTINATION ${EXECUTABLE_OUTPUT_PATH})
         endif(${CMAKE_MINOR_VERSION} LESS 8)
      endif(NOT EXISTS ${EXECUTABLE_OUTPUT_PATH}/${FILENAME})
   endif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${SRC_DIR}/${FILENAME})
ENDMACRO(copy_to_bin)   

