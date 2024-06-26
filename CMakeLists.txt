cmake_minimum_required(VERSION 3.23)
project(StudyGuide)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)

set(STUDYGUIDE_VERSION_MAJOR 0)
set(STUDYGUIDE_VERSION_MINOR 1)
set(STUDYGUIDE_VERSION_PATCH 0)
set(STUDYGUIDE_VERSION ${STUDYGUIDE_VERSION_MAJOR}.${STUDYGUIDE_VERSION_MINOR}.${STUDYGUIDE_VERSION_PATCH})

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets LinguistTools)

set(EXECUTABLE_NAME "StudyGuide")

if (LINUX)
    set(EXECUTABLE_NAME "studyguide") #Linux is case-sensitive, so this should make it possible to call from command line like every regular app. (Without using shift)
endif ()

set(CMAKE_WIN32_EXECUTABLE ON)


#Get Git commit and tag
execute_process(
        COMMAND git describe
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_TAG
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
        COMMAND git log -1 --format=%h
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_COMMIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
        COMMAND git rev-list --count HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_COMMIT_NUMBER
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
        COMMAND git rev-parse --abbrev-ref HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_BRANCH
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

message(STATUS "Git tag: ${GIT_TAG}")
message(STATUS "Git commit hash: ${GIT_COMMIT_HASH}")
message(STATUS "Git Branch: ${GIT_BRANCH}")
message(STATUS "Git Branch Commit: ${GIT_COMMIT_NUMBER}")

string(TIMESTAMP TODAY "%Y-%m-%d")
set(STUDYGUIDE_BUILD_DATE "${TODAY}")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(STUDYGUIDE_VERSION "${STUDYGUIDE_VERSION}-r${GIT_COMMIT_NUMBER}.${GIT_COMMIT_HASH}")
endif ()

# Cpack stuff
set(CPACK_PACKAGE_NAME "StudyGuide")
set(CPACK_NSIS_PACKAGE_NAME ${CPACK_PACKAGE_NAME})
set(CPACK_PACKAGE_VENDOR "JesseRobot01")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "StudyGuide")
set(CPACK_NSIS_MODIFY_PATH "YES")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")

set(CPACK_PACKAGE_EXECUTABLES "..\\\\${EXECUTABLE_NAME}" "StudyGuide") #The ..\ is because CPack looks in the BIN directory, and not in the root as where the executable is.
set(CPACK_CREATE_DESKTOP_LINKS "..\\\\${EXECUTABLE_NAME}" "StudyGuide")

set(CPACK_PACKAGE_VERSION ${STUDYGUIDE_VERSION})
set(CPACK_PACKAGE_VERSION_MAJOR ${STUDYGUIDE_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${STUDYGUIDE_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${STUDYGUIDE_VERSION_PATCH})

set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt6core6 (>= 6.4.2)")
set(CPACK_PACKAGE_CONTACT "JesseRobot01")

if (UNIX)
    set(CPACK_PACKAGE_NAME "StudyGuide")
endif ()
include(InstallRequiredSystemLibraries)
include(CPack)

if (UNIX)
    set(BINARY_DEST_DIR "bin")
    set(LIBRARY_DEST_DIR "lib")
endif ()

add_subdirectory(buildconfig)
add_subdirectory(StudyGuide)

