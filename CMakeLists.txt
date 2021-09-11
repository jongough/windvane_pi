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
    if(POLICY CMP0076)
        cmake_policy(SET CMP0076 NEW)
    endif(POLICY CMP0076)
    if(POLICY CMP0077)
        cmake_policy(SET CMP0077 NEW)
    endif(POLICY CMP0077)
    if(POLICY CMP0079)
        cmake_policy(SET CMP0079 NEW)
    endif(POLICY CMP0079)
endif(COMMAND cmake_policy)

set(VERBOSE_NAME "Windvane_Autopilot")
set(COMMON_NAME "Windvane")
set(TITLE_NAME "windvane")
set(PACKAGE_CONTACT "Jon Gough")
set(PACKAGE "windvane")

set(VERSION_MAJOR "1")
set(VERSION_MINOR "0")
set(VERSION_PATCH "22")
set(VERSION_TWEAK "0")
set(VERSION_DATE "05/09/2021") # DD/MM/YYYY format
set(OCPN_MIN_VERSION "ov50")
set(OCPN_API_VERSION_MAJOR "1")
set(OCPN_API_VERSION_MINOR "16")
set(TP_COMMENT "  * Release for O5 using CI")

set(PARENT "opencpn")

# The next line allows for setting up your own local webserver with git on it for testing purposes, the default is github.com
#set(GIT_REPOSITORY_SERVER "github.com")

# Specifies cloudsmith repository sufficies
set(PROD "prod")    # Standard Repos
set(BETA "beta")    # Standard Repos
set(ALPHA "alpha")  # Standard Repos

# set this if your cloudsmith repository name does not match your git repository name
#set(CLOUDSMITH_BASE_REPOSITORY "testplugin_pi")
set(CLOUDSMITH_BASE_REPOSITORY "windvane")
# Set the following to use OpenCPN cloudsmith repositories instead of own repositories
set(CLOUDSMITH_USER "opencpn")
#set(CLOUDSMITH_USER "jon-gough")

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

# Use local version of GLU library
set(USE_LOCAL_GLU FALSE)
# Prefer libGL.so to libOpenGL.so, see CMP0072
set(OpenGL_GL_PREFERENCE "LEGACY")
# Don't use local version of GLU library
#set(USE_LOCAL_GLU FALSE)

option(USE_GL "Enable OpenGL support" OFF)
message(STATUS "${CMLOC}USE_GL: ${USE_GL}")

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build" FORCE)
endif("${CMAKE_BUILD_TYPE}" STREQUAL "")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "" "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
message(STATUS "${CMLOC}Build type: ${CMAKE_BUILD_TYPE}")

##
## ----- The statements below are used to setup standard variables that are required by the CMAKE process - do not remove ----- ##
##

project(${PACKAGE})

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include("PluginSetup")

##
## ----- The above is used to setup standard variables that are required by the CMAKE process - do not remove ----- ##
##

##
## ----- Modify section below if there are special requirements for the plugin ----- ##
##

set(CMAKE_VERBOSE_MAKEFILE "Activate verbose mode for make files" ON)

option(Plugin_CXX11 "Use c++11" OFF)

##
## ----- Modify section above if there are special requirements for the plugin ----- ##
##

##
## ----- do not change next section - needed to configure build process ----- ##
##

include("PluginConfigure")

##
## ----- do not change section above - needed to configure build process ----- ##
##

##
## ----- Change below to match project requirements ----- ##
##

ADD_DEFINITIONS(-DUSE_S57)

SET(SRCS
        src/windvane_pi.cpp
        src/WVdc.cpp
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
        include/WVdc.h
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
        ocpnsrc/wxJSON/jsonreader.cpp
        ocpnsrc/wxJSON/jsonval.cpp
        ocpnsrc/wxJSON/jsonwriter.cpp

)
SET(OCPNHDRS
        ocpninclude/ocpn_plugin.h
        ocpninclude/TexFont.h
        ocpninclude/wx28compat.h
        ocpninclude/wx/json_defs.h
        ocpninclude/wx/jsonreader.h
        ocpninclude/wx/jsonval.h
        ocpninclude/wx/jsonwriter.h
        libs/ocpn-api/ocpn_plugin.h
)
SET(EXTSRC
)
SET(EXTINCLUDE
)

OPTION(WINDVANE_USE_SVG "Use SVG graphics" ON)
IF(WINDVANE_USE_SVG)
    ADD_DEFINITIONS(-DTESTPLUGIN_USE_SVG)
ENDIF(WINDVANE_USE_SVG)

set(EXTINCLUDE_DIR ${EXTINCLUDE_DIR} libs/ocpn-api/)

include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/gdal/include)
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/iso8211/include)
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/ocpninclude)
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/ocpnsrc/lz4)
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/ocpnsrc/bitmaps)
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/${EXTINCLUDE_DIR})
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/nmea0183/src)
if(QT_ANDROID)
    include_directories(BEFORE ${qt_android_include})
    include_directories( ${CMAKE_SOURCE_DIR}/extinclude/GLES2)

    #    #include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/glshim/include/GLES)
endif(QT_ANDROID)

target_sources(${PACKAGE_NAME} PUBLIC ${SRCS} ${HDRS} ${OCPNSRC} ${EXTSRC})

if(NOT OCPN_FLATPAK_CONFIG)
    # Build environment not available when flatpak is being configured so following statements will not work
    message(STATUS "${CMLOC}Adding target link libraries to ${PACKAGE_NAME}")

    add_subdirectory(libs/nmea0183)

    target_link_libraries(${PACKAGE_NAME} ocpn::nmea0183)
endif(NOT OCPN_FLATPAK_CONFIG)

##
## ----- Change above to match project requirements ----- ##
##

##
## ----- do not change next section - needed to configure build process ----- ##
##

include("PluginInstall")
include("PluginLocalization")
include("PluginPackage")

##
## ----- do not change section above - needed to configure build process ----- ##
##
message(STATUS "${CMLOC}include directories: ")
get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
    message(STATUS "${CMLOC}dir='${dir}'")
endforeach()
