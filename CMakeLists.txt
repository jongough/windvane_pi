##---------------------------------------------------------------------------
# Author:      jongough aka. Jon Gough
## Copyright:
## License:     wxWidgets License
##---------------------------------------------------------------------------

# define minimum cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(ocpn_draw_pi)

SET(PACKAGE_NAME windvane_pi)
SET(VERBOSE_NAME windvane)
SET(TITLE_NAME WINDVANE)
SET(CPACK_PACKAGE_CONTACT "Jon Gough")

SET(VERSION_MAJOR "0")
SET(VERSION_MINOR "0")
SET(VERSION_PATCH "1")
SET(VERSION_DATE "03/12/2016")

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
     )

SET(HDRS
        include/windvane_pi.h
)

SET(OCPNSRC
        src/nmea0183/apb.cpp
        src/nmea0183/rmc.cpp
        src/nmea0183/xte.cpp
        src/nmea0183/lat.cpp
        src/nmea0183/latlong.cpp
        src/nmea0183/long.cpp
        src/nmea0183/nmea0183.cpp
        src/nmea0183/response.cpp
        src/nmea0183/sentence.cpp
        src/nmea0183/talkerid.cpp
)
SET(OCPNHDRS
        ocpninclude/ocpn_plugin.h
        src/nmea0183/apb.hpp
        src/nmea0183/RMC.HPP
        src/nmea0183/xte.hpp
        src/nmea0183/LatLong.hpp
        src/nmea0183/nmea0183.h
        src/nmea0183/nmea0183.hpp
        src/nmea0183/Response.hpp
        src/nmea0183/Sentence.hpp
)

OPTION(OD_USE_SVG "Use SVG graphics" ON)

IF(OD_USE_SVG)
  MESSAGE(STATUS "SVG support enabled...")
  IF(NOT QT_ANDROID)

    IF(NOT WIN32)
      #Find Cairo
      IF( CAIRO_INCLUDE_DIR )
        # Already in cache, be silent
        SET( CAIRO_FIND_QUIETLY TRUE )
      ENDIF( CAIRO_INCLUDE_DIR )

      FIND_PATH( CAIRO_INCLUDE_DIR cairo.h
              PATHS /usr/local/include /usr/include
              PATH_SUFFIXES cairo/ libcairo/ cairo/libcairo/ )

      SET( CAIRO_NAMES libcairo cairo )
      FIND_LIBRARY( CAIRO_LIBRARY
              NAMES ${CAIRO_NAMES}
              PATHS /usr/lib /usr/local/lib /usr/lib/i386-linux-gnu)

      GET_FILENAME_COMPONENT( CAIRO_LIBRARY_DIR ${CAIRO_LIBRARY} PATH )

      # handle the QUIETLY and REQUIRED arguments and set CAIRO_FOUND to TRUE if
      # all listed variables are TRUE
      INCLUDE( "FindPackageHandleStandardArgs" )
      FIND_PACKAGE_HANDLE_STANDARD_ARGS( "CAIRO" DEFAULT_MSG CAIRO_INCLUDE_DIR CAIRO_LIBRARY )

      MARK_AS_ADVANCED( CAIRO_INCLUDE_DIR CAIRO_LIBRARY )
      if ( NOT CAIRO_FOUND )
        message(FATAL_ERROR "Cairo component required, but not found!")
      endif()

      IF(NOT WIN32 AND NOT APPLE)
        OPTION(OCPN_FORCE_GTK3 "Force the build to use GTK3" OFF)

        IF(NOT OCPN_FORCE_GTK3)
          FIND_PACKAGE(GTK2)
        ENDIF(NOT OCPN_FORCE_GTK3)

        IF(GTK2_FOUND)
          INCLUDE_DIRECTORIES(${GTK2_INCLUDE_DIRS})
          SET(GTK_LIBRARIES ${GTK2_LIBRARIES})
          MESSAGE(STATUS "Building against GTK2...")
        ELSE(GTK2_FOUND)
          FIND_PACKAGE(GTK3)
          INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS})
          SET(GTK_LIBRARIES ${GTK3_LIBRARIES})
          MESSAGE(STATUS "Building against GTK3...")
        ENDIF(GTK2_FOUND)
      ENDIF(NOT WIN32 AND NOT APPLE)

       FIND_PACKAGE(EXPAT REQUIRED)
    ELSE(NOT WIN32)
      #On Windows, we have our own expat
      SET(EXPAT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/buildwin/expat-2.1.0/include)
      INCLUDE_DIRECTORIES(${EXPAT_INCLUDE_DIR})
      #And cairo
      SET(CAIRO_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/buildwin/gtk/include)
    ENDIF(NOT WIN32)

    INCLUDE_DIRECTORIES(${CAIRO_INCLUDE_DIR})
    IF(APPLE)
      INCLUDE_DIRECTORIES(${CAIRO_INCLUDE_DIR}/..)
    ENDIF(APPLE)
    #INCLUDE_DIRECTORIES(${FFMPEG_INCLUDE_DIRS})

    ADD_DEFINITIONS(-DUSE_RENDER_CAIRO)

    ADD_DEFINITIONS(-DODraw_USE_SVG)
    IF(WIN32)
      SET(SVG_LIBS
        ${CMAKE_SOURCE_DIR}/buildwin/gtk/cairo.lib
        ${CMAKE_SOURCE_DIR}/buildwin/expat-2.1.0/expat.lib
      )
    ELSE(WIN32)
      SET(SVG_LIBS ${CAIRO_LIBRARY} ${EXPAT_LIBRARIES} ${GTK_LIBRARIES})
    ENDIF(WIN32)

    INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/ocpninclude/wxsvg)
    INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/ocpninclude/wxSVGXML)
    IF(WIN32)
        SET(EXTRA_LIBS ${EXTRA_LIBS} ${SVG_LIBS} ${CMAKE_BINARY_DIR}/wxSVG.lib)
    ENDIF(WIN32)
  ENDIF(NOT QT_ANDROID)
ENDIF(OD_USE_SVG)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/mygdal  )
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/ocpninclude)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/ocpninclude/wxcurl)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/ocpninclude/wxcurl/include)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/ocpnsrc/lz4)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/ocpnsrc/bitmaps)


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
