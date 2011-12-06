CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(madedit)
SET(BIN madedit)

# wxWidgets
IF(WXCONFIG)
    EXECUTE_PROCESS(
        COMMAND sh "${WXCONFIG}"
            --unicode=yes --cxxflags
        OUTPUT_VARIABLE WX_CXX_FLAGS
        RESULT_VARIABLE RET
        ERROR_QUIET
        OUTPUT_STRIP_TRAILING_WHITESPACE
        )
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WX_CXX_FLAGS}")

    EXECUTE_PROCESS(
        COMMAND sh "${WXCONFIG}"
            --unicode=yes --libs aui core base
        OUTPUT_VARIABLE wxWidgets_LIBRARIES
        RESULT_VARIABLE RET
        ERROR_QUIET
        OUTPUT_STRIP_TRAILING_WHITESPACE
        )
ELSE(WXCONFIG)
    #use default module
    FIND_PACKAGE(wxWidgets REQUIRED aui core base)
    INCLUDE(${wxWidgets_USE_FILE})
ENDIF(WXCONFIG)

# boost
IF(Boost_INCLUDE_DIRS)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
ELSE(Boost_INCLUDE_DIRS)
    FIND_PACKAGE(Boost)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
ENDIF(Boost_INCLUDE_DIRS)

# gtk
IF(UNIX)
    FIND_PACKAGE(GTK2)
    IF(GTK2_FOUND)
        INCLUDE_DIRECTORIES(${GTK2_INCLUDE_DIRS})
    ELSE(GTK2_FOUND)
        FIND_PACKAGE(GTK)
        INCLUDE_DIRECTORIES(${GTK_INCLUDE_DIRS})
    ENDIF(GTK2_FOUND)
ENDIF(UNIX)


FILE(GLOB madedit_comp_SRCS "${PROJECT_SOURCE_DIR}/src/MadEdit/*.cpp")
FILE(GLOB madedit_SRCS "${PROJECT_SOURCE_DIR}/src/*.cpp")
FILE(GLOB_RECURSE chardet_SRCS "${PROJECT_SOURCE_DIR}/libs/charset-detector/*.cpp")

INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}/src/MadEdit
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/libs/charset-detector/include
    ${PROJECT_SOURCE_DIR}/libs/charset-detector/src
    ${PROJECT_SOURCE_DIR}/libs/charset-detector/src/tables
    )

ADD_EXECUTABLE(${BIN}
    ${madedit_comp_SRCS}
    ${madedit_SRCS}
    ${chardet_SRCS}
    )

TARGET_LINK_LIBRARIES(${BIN} ${wxWidgets_LIBRARIES} ${GTK2_LIBRARIES} ${GTK_LIBRARIES})

#SET_TARGET_PROPERTIES(${BIN}
#    PROPERTIES LINK_FLAGS ${MYLINK_FLAGS}
#    )
