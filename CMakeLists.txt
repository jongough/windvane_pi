##---------------------------------------------------------------------------
# Author:      jongough aka. Jon Gough
## Copyright:
## License:     wxWidgets License
##---------------------------------------------------------------------------

# define minimum cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(windvane_pi)

SET(PACKAGE_NAME windvane_pi)
SET(VERBOSE_NAME windvane)
SET(TITLE_NAME WINDVANE)
SET(CPACK_PACKAGE_CONTACT "Jon Gough")

SET(VERSION_MAJOR "1")
SET(VERSION_MINOR "0")
SET(VERSION_PATCH "4")
SET(VERSION_DATE "04/07/2019")

SET(BUNDLE_DATA FALSE)
IF( NOT CMAKE_BUILD_TYPE )
  SET( CMAKE_BUILD_TYPE Release CACHE STRING
       "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
       FORCE )
ENDIF( NOT CMAKE_BUILD_TYPE )

MESSAGE (STATUS "Build type: ${CMAKE_BUILD_TYPE}")

SET(CMAKE_VERBOSE_MAKEFILE ON)

#SET_DIRECTORY_PROPERTIES(dummy ADDITIONAL_MAKE_CLEAN_FILES ${PROJECT_SOURCE_DIR}/include/version.h)

INCLUDE("cmake/PluginConfigure.cmake")

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


INCLUDE("cmake/PluginInstall.cmake")
INCLUDE("cmake/PluginLocalization.cmake")
INCLUDE("cmake/PluginPackage.cmake")
