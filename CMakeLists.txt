##---------------------------------------------------------------------------
# Author:      jongough aka. Jon Gough
## Copyright:
## License:     wxWidgets License
##---------------------------------------------------------------------------

##
## ----- Modify section below to include all the details for your plugin ----- ##
##

# define minimum cmake version
cmake_minimum_required(VERSION 3.1.1)
if(COMMAND cmake_policy)
    if(POLICY CMP0043)
        cmake_policy(SET CMP0043 NEW)
    endif(POLICY CMP0043)
    cmake_policy(SET CMP0048 NEW)
    if(POLICY CMP0077)
        cmake_policy(SET CMP0077 NEW)
    endif(POLICY CMP0077)
endif(COMMAND cmake_policy)

set(VERBOSE_NAME "Windvane Autopilot")
set(COMMON_NAME "windvane")
set(TITLE_NAME "windvane")
set(PACKAGE_CONTACT "Jon Gough")
set(PACKAGE "windvane")

set(VERSION_MAJOR "1")
set(VERSION_MINOR "0")
set(VERSION_PATCH "5")
set(VERSION_TWEAK "0")
set(VERSION_DATE "25/04/2020")
set(OCPN_MIN_VERSION "ov50")
set(OCPN_API_VERSION_MAJOR "1")
set(OCPN_API_VERSION_MINOR "16")
set(TP_COMMENT "  * Release for O5 using CI")


set(PARENT "opencpn")
set(GIT_USER "jongough")
set(GIT_REPOSITORY_NAME "windvane_pi")
message(STATUS "CIRCLECI: ${CIRCLECLI}, Env CIRCLECI: $ENV{CIRCLECI}")
message(STATUS "TRAVIS: ${TRAVIS}, Env TRAVIS: $ENV{TRAVIS}")
# This if allows for setting up your own local webserver with git on it for testing purposes
if($ENV{CIRCLECI})
    set(GIT_REPOSITORY_HOST "github.com")
    set(GIT_REPOSITORY_DIR "${GIT_USER}/")
    set(GIT_REPOSITORY_BRANCH "$ENV{CIRCLE_BRANCH}")
elseif($ENV{TRAVIS})
    set(GIT_REPOSITORY_HOST "github.com")
    set(GIT_REPOSITORY_DIR "${GIT_USER}/")
    set(GIT_REPOSITORY_BRANCH "$ENV{TRAVIS_BRANCH}")
elseif($ENV{APPVEYOR})
    set(GIT_REPOSITORY_HOST "github.com")
    set(GIT_REPOSITORY_DIR "${GIT_USER}/")
    set(GIT_REPOSITORY_BRANCH "$ENV{APPVEYOR_REPO_BRANCH}")
else()
    set(GIT_REPOSITORY_HOST "github.com")
    set(GIT_REPOSITORY_DIR "${GIT_USER}/")
    # Get the current working branch
    execute_process(
        COMMAND git rev-parse --abbrev-ref HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_REPOSITORY_BRANCH
        OUTPUT_STRIP_TRAILING_WHITESPACE
      )
    if(${GIT_REPOSITORY_BRANCH} EQUAL "")
        message(STATUS "Setting default GIT repository branch - master")
        set(GIT_REPOSITORY_BRANCH "master")
    endif()
endif()
message(STATUS "Git Branch: ${GIT_REPOSITORY_BRANCH}")

# Builds repository sufficies
set(PROD "prod")    #Standard Repos
set(BETA "beta")    #Standard Repos
set(ALPHA "alpha")  #Standard Repos

set(CLOUDSMITH_USER "jon-gough")
set(CLOUDSMITH_BASE_REPOSITORY "${GIT_REPOSITORY_NAME}")
set(XML_INFO_URL "https://opencpn.org/wiki/dokuwiki/doku.php?id=opencpn:opencpn_user_manual:plugins:sailing:windvane")
set(XML_SUMMARY "Plugin to provide windvane like autopilot")
set(XML_DESCRIPTION "Windvane Autopilot is designed to use the wind instruments and autopilot on board the yatch to act like a standard windvane autopilot and allow the yatch to follow the wind.")

##
## ----- Modify section above to include all the details for your plugin ----- ##
##
##
## ----- Modify section below if there are special requirements for the plugin ----- ##
##

option(PLUGIN_USE_SVG "Use SVG graphics" ON)

set(CMAKE_CXX_STANDARD 11)

if(NOT ${PACKAGE} MATCHES "(.*)_pi")
    set(PACKAGE_NAME ${PACKAGE}_pi)
    set(PACKAGE_FILE_NAME "${PACKAGE}_pi")
else(NOT ${PACKAGE} MATCHES "(.*)_pi")
    set(PACKAGE_NAME ${PACKAGE})
    set(PACKAGE_FILE_NAME "${PACKAGE}")
endif(NOT ${PACKAGE} MATCHES "(.*)_pi")
string(TOUPPER "${PACKAGE}" TITLE_NAME)

project(${PACKAGE_NAME} VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.${VERSION_TWEAK})
message(STATUS "PROJECT_VERSION: ${PROJECT_VERSION}")

set(PACKAGE_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

message(STATUS "${VERBOSE_NAME} Version: ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.${VERSION_TWEAK}")
message(STATUS "OPCN_FLATPAK: ${OCPN_FLATPAK}")

# Prefer libGL.so to libOpenGL.so, see CMP0072
set(OpenGL_GL_PREFERENCE "LEGACY")
# Don't use local version of GLU library
set(USE_LOCAL_GLU FALSE)
option(USE_GL "Enable OpenGL support" OFF)
message(STATUS "USE_GL: ${USE_GL}")

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build" FORCE)
endif("${CMAKE_BUILD_TYPE}" STREQUAL "")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "" "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# set(PKG_NVR ${PACKAGE_NAME}-${PROJECT_VERSION}-${OCPN_MIN_VERSION}-${OCPN_API_VERSION_MAJOR}.${OCPN_API_VERSION_MINOR})
set(PKG_NVR ${PACKAGE_NAME}-${PROJECT_VERSION})
set(PKG_URL "https://dl.cloudsmith.io/public/--pkg_repo--/raw/names/--name--/versions/--version--/--filename--")

include("cmake/PluginSetup.cmake")

if(DEFINED ENV{OCPN_TARGET})
    set(PACKAGING_NAME "${PKG_NVR}_${PKG_TARGET}-${PKG_TARGET_VERSION}-$ENV{OCPN_TARGET}")
else(DEFINED ENV{OCPN_TARGET})
    set(PACKAGING_NAME "${PKG_NVR}_${PKG_TARGET}-${PKG_TARGET_VERSION}")
endif(DEFINED ENV{OCPN_TARGET})
message(STATUS "PACKAGING_NAME: ${PACKAGING_NAME}")

set(PLUGIN_NAME ${PACKAGE}-plugin-${PKG_TARGET}-${PKG_TARGET_VERSION})

set(CMAKE_VERBOSE_MAKEFILE ON)

option(Plugin_CXX11 "Use c++11" OFF)

if(Plugin_CXX11)
    message(STATUS "Attempting to use c++11")
    include(CheckCXXCompilerFlag)
    check_cxx_compiler_flag("-std=c++11" COMPILER_SUPPORTS_CXX11)
    check_cxx_compiler_flag("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
    if(COMPILER_SUPPORTS_CXX11)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
        set(Plugin_USE_CXX11 ON)
    elseif(COMPILER_SUPPORTS_CXX0X)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
        set(Plugin_USE_CXX11 ON)
    else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
        set(Plugin_USE_CXX11 OFF)
    endif()
else()
    set(Plugin_USE_CXX11 OFF)
endif()

if(Plugin_USE_CXX11)
    message(STATUS "Allowing use of c++11")
endif()

message(STATUS "CMAKE version: ${CMAKE_VERSION}")
if(CMAKE_VERSION VERSION_GREATER 3.4)
    set(ENABLE_CLANG_TIDY
    OFF
    CACHE BOOL "Add clang-tidy automatically to builds")
    if(ENABLE_CLANG_TIDY)
        find_program(
            CLANG_TIDY_EXE
            NAMES "clang-tidy"
            PATHS /usr/local/opt/llvm/bin)
            if(CLANG_TIDY_EXE)
                message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
                # For more, see http://clang.llvm.org/extra/clang-tidy/ set(CLANG_TIDY_CHECKS "-*,modernize-*")
                set(CLANG_TIDY_CHECKS "-*,performance-*")
                set(CMAKE_CXX_CLANG_TIDY
                "${CLANG_TIDY_EXE};-checks=${CLANG_TIDY_CHECKS};-header-filter='${CMAKE_SOURCE_DIR}/*'"
                CACHE STRING "" FORCE)
            else()
                message(AUTHOR_WARNING "clang-tidy not found!")
                set(CMAKE_CXX_CLANG_TIDY
                ""
                CACHE STRING "" FORCE) # delete it
            endif()
    endif()
endif()

if(CMAKE_VERSION VERSION_GREATER 3.9)
    set(ENABLE_CPPCHECK
    OFF
    CACHE BOOL "Add cppcheck automatically to builds")
    if(ENABLE_CPPCHECK)
        find_program(CPPCHECK_EXECUTABLE NAMES "cppcheck")
        set(CMAKE_CXX_CPPCHECK ${CPPCHECK_EXECUTABLE})
    endif()
endif()

##
## ----- Modify section above if there are special requirements for the plugin ----- ##
##


include("cmake/PluginConfigure.cmake")

##
## ----- Change below to match project requirements ----- ##
##

ADD_DEFINITIONS(-DUSE_S57)


SET(SRCS
        src/windvane_pi.cpp
        src/WVDialFrame.cpp
        src/WVEventHandler.cpp
        src/WVPropertiesDialogDef.cpp
        src/WVPropertiesDialogImpl.cpp
        src/WVicons.cpp
        src/WVJSON.cpp
        src/WVUtils.cpp
        src/dial.cpp
        src/instrument.cpp
        src/wind.cpp
     )

SET(HDRS
        include/windvane_pi.h
        include/WVDialFrame.h
        include/WVEventHandler.h
        include/WVPropertiesDialogDef.h
        include/WVPropertiesDialogImpl.h
        include/WVicons.h
        include/WVJSON.h
        include/WVUtils.h
        include/dial.h
        include/instrument.h
        include/wind.h

)

SET(OCPNSRC
        src/nmea0183/apb.cpp
        src/nmea0183/mwv.cpp
        src/nmea0183/rmb.cpp
        src/nmea0183/xte.cpp
        src/nmea0183/lat.cpp
        src/nmea0183/latlong.cpp
        src/nmea0183/long.cpp
        src/nmea0183/nmea0183.cpp
        src/nmea0183/expid.cpp
        src/nmea0183/hexvalue.cpp
        src/nmea0183/response.cpp
        src/nmea0183/sentence.cpp
        src/nmea0183/talkerid.cpp
        src/nmea0183/vwr.cpp
        ocpnsrc/wxJSON/jsonreader.cpp
        ocpnsrc/wxJSON/jsonval.cpp
        ocpnsrc/wxJSON/jsonwriter.cpp

)
SET(OCPNHDRS
        ocpninclude/ocpn_plugin.h
        ocpninclude/TexFont.h
        ocpninclude/wx28compat.h
        src/nmea0183/apb.hpp
        src/nmea0183/mwv.hpp
        src/nmea0183/RMB.HPP
        src/nmea0183/xte.hpp
        src/nmea0183/LatLong.hpp
        src/nmea0183/nmea0183.h
        src/nmea0183/nmea0183.hpp
        src/nmea0183/Response.hpp
        src/nmea0183/Sentence.hpp
        src/nmea0183/vwr.hpp
        ocpninclude/wx/json_defs.h
        ocpninclude/wx/jsonreader.h
        ocpninclude/wx/jsonval.h
        ocpninclude/wx/jsonwriter.h
        
)

OPTION(WINDVANE_USE_SVG "Use SVG graphics" ON)
IF(WINDVANE_USE_SVG)
    ADD_DEFINITIONS(-DTESTPLUGIN_USE_SVG)
ENDIF(WINDVANE_USE_SVG)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/mygdal  )
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/ocpninclude)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/ocpninclude/wxcurl)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/ocpninclude/wxcurl/include)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/ocpnsrc/lz4)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/ocpnsrc/bitmaps)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/nmea0183)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/ocpninclude/wxsvg)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/ocpninclude/wxSVGXML)


IF(UNIX)
 INCLUDE("cmake/FindTinyXML.cmake")
 FIND_PACKAGE(TinyXML QUIET)
ENDIF(UNIX)

IF(TINYXML_FOUND)
 message (STATUS "Building with system tinyxml")
 INCLUDE_DIRECTORIES(${TINYXML_INCLUDE_DIR})
 ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRCS} ${HDRS} ${OCPNSRC} ${EXTSRC})
 TARGET_LINK_LIBRARIES(${PACKAGE_NAME} ${TINYXML_LIBRARIES})
ELSE(TINYXML_FOUND)
 message (STATUS "Building with embedded tinyxml")
 INCLUDE_DIRECTORIES(src/tinyxml/)
 SET(SRC_LTINYXML
            src/tinyxml/tinyxml.cpp
            src/tinyxml/tinyxmlparser.cpp
            src/tinyxml/tinyxmlerror.cpp
 )
 ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRCS} ${HDRS} ${OCPNSRC} ${EXTSRC} ${SRC_LTINYXML})
ENDIF(TINYXML_FOUND)
ADD_DEFINITIONS(-DTIXML_USE_STL)

##
## ----- Change above to match project requirements ----- ##
##

##
## ----- do not change - needed to build app ----- ##
##

include("cmake/PluginInstall.cmake")
include("cmake/PluginLocalization.cmake")
include("cmake/PluginPackage.cmake")
