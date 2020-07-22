##---------------------------------------------------------------------------
# Author:      jongough aka. Jon Gough
## Copyright:
## License:     wxWidgets License
##---------------------------------------------------------------------------

##
## ----- Modify section below to include all the details for your plugin ----- ##
##

set(CMLOC "CMakeLists: ")

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

set(VERBOSE_NAME "Windvane_Autopilot")
set(COMMON_NAME "windvane")
set(TITLE_NAME "windvane")
set(PACKAGE_CONTACT "Jon Gough")
set(PACKAGE "windvane")

set(VERSION_MAJOR "1")
set(VERSION_MINOR "0")
set(VERSION_PATCH "21")
set(VERSION_TWEAK "0")
set(VERSION_DATE "22/07/2020")
set(OCPN_MIN_VERSION "ov50")
set(OCPN_API_VERSION_MAJOR "1")
set(OCPN_API_VERSION_MINOR "16")
set(TP_COMMENT "  * Release for O5 using CI")


set(PARENT "opencpn")

# The next line allows for setting up your own local webserver with git on it for testing purposes, the default is github.com
set(GIT_REPOSITORY_SERVER "github.com")

# Builds repository sufficies
set(PROD "prod")    #Standard Repos
set(BETA "beta")    #Standard Repos
set(ALPHA "alpha")  #Standard Repos

set(CLOUDSMITH_USER "jon-gough")
# set this if your cloudsmith repository name does not match your git repository name
#set(CLOUDSMITH_BASE_REPOSITORY "${GIT_REPOSITORY_NAME}")
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

# Prefer libGL.so to libOpenGL.so, see CMP0072
set(OpenGL_GL_PREFERENCE "LEGACY")
# Don't use local version of GLU library
set(USE_LOCAL_GLU FALSE)
option(USE_GL "Enable OpenGL support" OFF)
message(STATUS "${CMLOC}USE_GL: ${USE_GL}")

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build" FORCE)
endif("${CMAKE_BUILD_TYPE}" STREQUAL "")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "" "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
message(STATUS "${CMLOC}Build type: ${CMAKE_BUILD_TYPE}")


project(${PACKAGE})

include("cmake/PluginSetup.cmake")

set(CMAKE_VERBOSE_MAKEFILE ON)

option(Plugin_CXX11 "Use c++11" OFF)

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
    message (STATUS "${CMLOC}Building with system tinyxml")
 INCLUDE_DIRECTORIES(${TINYXML_INCLUDE_DIR})
 ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRCS} ${HDRS} ${OCPNSRC} ${EXTSRC})
 TARGET_LINK_LIBRARIES(${PACKAGE_NAME} ${TINYXML_LIBRARIES})
ELSE(TINYXML_FOUND)
    message (STATUS "${CMLOC}Building with embedded tinyxml")
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
