#Формируем regexp для include_regular_expression - чтобы оганичить число зависимостей для проекта temp_subproject
#Перед использовнием должны быть установлены переменные HEADERS_DIRECTORY
#В качестве параметра передается список всех заголовочных файлов для включения в проект
MACRO(include_only_headers)
   string(REPLACE ${HEADERS_DIRECTORY}/ "|" ALLHEADERS_INCLUDE ${ARGV})                #Отбрасываем абсолютный путь до хедеров
   string(REPLACE ".hpp" "" ALLHEADERS_INCLUDE ${ALLHEADERS_INCLUDE})
   string(REPLACE ".h" "" ALLHEADERS_INCLUDE ${ALLHEADERS_INCLUDE})                    #Отбрасываем расширения
   string(REGEX REPLACE "^\\|" "" ALLHEADERS_INCLUDE ${ALLHEADERS_INCLUDE})            #Убираем в самом начале лишний символ |
   set(ALLHEADERS_INCLUDE "${ALLHEADERS_INCLUDE}\\.(h|hpp)$")                                #Последний штрих в regexp - означает - все строки, заканчивающиеся на .h
#    message(ALLHEADERS_INCLUDE = ${ALLHEADERS_INCLUDE})
   include_regular_expression(${ALLHEADERS_INCLUDE})                                   #Имеем file1|file2|file3\.h$ - означает file1.h или file2.h или file3.h
ENDMACRO(include_only_headers IN_HEADERS)