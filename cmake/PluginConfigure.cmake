##---------------------------------------------------------------------------
## Author:      Pavel Kalian (Based on the work of Sean D'Epagnier)
## Copyright:   2014
## License:     GPLv3+
##---------------------------------------------------------------------------

SET(PLUGIN_SOURCE_DIR .)

MESSAGE (STATUS "*** Staging to build ${PACKAGE_NAME} ***")

#  Do the version.h & wxWTranslateCatalog configuration into the build output directory,
#  thereby allowing building from a read-only source tree.
# The removes are to get rid of old copies - this can be removed at next release version, i.e. > 1.1.0
FILE(REMOVE ${PROJECT_SOURCE_DIR}/include/version.h)
FILE(REMOVE ${PROJECT_SOURCE_DIR}/include/wxWTranslateCatalog.h)
IF(NOT SKIP_VERSION_CONFIG)
    SET(BUILD_INCLUDE_PATH ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY})
    configure_file(cmake/version.h.in ${BUILD_INCLUDE_PATH}/include/version.h)
    configure_file(cmake/wxWTranslateCatalog.h.in ${BUILD_INCLUDE_PATH}/include/wxWTranslateCatalog.h)
    INCLUDE_DIRECTORIES(${BUILD_INCLUDE_PATH}/include)
ENDIF(NOT SKIP_VERSION_CONFIG)

SET(PACKAGE_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}" )

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/src)

# SET(PROFILING 1)
IF(CMAKE_BUILD_TYPE STREQUAL Debug)
  ADD_DEFINITIONS( "-DDEBUG_BUILD" )
  MESSAGE (STATUS "DEBUG available")
ENDIF(CMAKE_BUILD_TYPE STREQUAL Debug)

IF(NOT WIN32 AND NOT APPLE )
  ADD_DEFINITIONS( "-Wall -Wno-unused -fexceptions -rdynamic -fvisibility=hidden" )
  ADD_DEFINITIONS( " -fno-strict-aliasing")
  MESSAGE(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
  IF(CMAKE_BUILD_TYPE STREQUAL "Debug")
    ADD_DEFINITIONS(" -O0 -g")
    MESSAGE(STATUS "Optimisation: -O0 -g")
  ELSEIF(CMAKE_BUILD_TYPE STREQUAL "Release")
    ADD_DEFINITIONS(" -O2 -march=native")
    MESSAGE(STATUS "Optimisation: -O2 -march=native")
  ELSEIF(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    ADD_DEFINITIONS(" -O2 -march=native -g")
    MESSAGE(STATUS "Optimisation: -O2 -march=native -g")
  ELSE(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")  
    ADD_DEFINITIONS(" -O2")
    MESSAGE(STATUS "Optimisation: -O2")
  ENDIF(CMAKE_BUILD_TYPE STREQUAL "Debug")
    
  
  ADD_DEFINITIONS( " -DPREFIX=\\\"${CMAKE_INSTALL_PREFIX}\\\"")
  # profiling with gprof
#    ADD_DEFINITIONS( -pg )
#    SET(CMAKE_EXE_LINKER_FLAGS -pg)
  # profiling with gcov
#    ADD_DEFINITIONS( "-fprofile-arcs -ftest-coverage" )
#    SET(EXTRA_LIBS ${EXTRA_LIBS} "gcov")
ENDIF(NOT WIN32 AND NOT APPLE)

#  IF NOT DEBUGGING CFLAGS="-O2 -march=native"
IF(NOT MSVC)
 IF(PROFILING)
  ADD_DEFINITIONS( "-Wall -g -fprofile-arcs -ftest-coverage -fexceptions" )
 ELSE(PROFILING)
  ADD_DEFINITIONS( "-Wall -fexceptions" )
#  ADD_DEFINITIONS( "-Wall -Wno-unused-result -g -O2 -fexceptions" )
 ENDIF(PROFILING)

 IF(NOT APPLE)
  SET( CMAKE_SHARED_LINKER_FLAGS "-Wl,-Bsymbolic")
 ELSE(NOT APPLE)
  SET( CMAKE_SHARED_LINKER_FLAGS "-Wl -undefined dynamic_lookup")
 ENDIF(NOT APPLE)


ENDIF(NOT MSVC)

# Add some definitions to satisfy MS
IF(MSVC)
  ADD_DEFINITIONS(-D__MSVC__)
  ADD_DEFINITIONS(-D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_DEPRECATE)
  ADD_DEFINITIONS(-D HAVE_SNPRINTF)
  MESSAGE(STATUS "Set SNPRINTF")
ELSE(MSVC)
  IF(NOT APPLE)
    SET( CMAKE_SHARED_LINKER_FLAGS "-Wl,-Bsymbolic")
  ELSE(NOT APPLE)
    SET( CMAKE_SHARED_LINKER_FLAGS "-Wl -undefined dynamic_lookup")
  ENDIF(NOT APPLE) 
ENDIF(MSVC)

SET(wxWidgets_USE_LIBS base core net xml html adv aui)
SET(BUILD_SHARED_LIBS TRUE)
# Allow multiprocess compile

IF(MSVC)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
ENDIF(MSVC)

IF(WIN32)
  IF(MSVC)
    SET(CMAKE_C_FLAGS_DEBUG               "/MP /MDd /Ob0 /Od  /D_DEBUG  /Zi /RTC1" )
    SET(CMAKE_C_FLAGS_MINSIZEREL          "/MP /MD  /O1  /Ob1 /D NDEBUG")
    SET(CMAKE_C_FLAGS_RELEASE             "/MP /MD  /O2  /Ob2 /D NDEBUG /Zi")
    SET(CMAKE_C_FLAGS_RELWITHDEBINFO      "/MP /MD  /O2  /Ob1 /D NDEBUG /Zi")
    SET(CMAKE_CXX_FLAGS_DEBUG             "/MP /MDd /Ob0 /Od  /D_DEBUG  /Zi /RTC1 /EHa")
    SET(CMAKE_CXX_FLAGS_MINSIZEREL        "/MP /MD  /O1  /Ob1 /D NDEBUG /EHa")
    SET(CMAKE_CXX_FLAGS_RELEASE           "/MP /MD  /O2  /Ob2 /D NDEBUG /Zi /EHa")
    SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO    "/MP /MD  /O2  /Ob1 /D NDEBUG /Zi /EHa" )
    SET(CMAKE_EXE_LINKER_FLAGS /DEBUG)
  ENDIF(MSVC)
ENDIF(WIN32)

OPTION (USE_GL "Enable OpenGL support" ON)

FIND_PACKAGE(wxWidgets REQUIRED)

IF(NOT QT_ANDROID)
  # Find wxWidgets here, and the setting get inherited by all plugins.
  # These options can be used to set the linux widgets build type
  SET( wxWidgets_USE_DEBUG OFF)
  SET( wxWidgets_USE_UNICODE ON)
  SET( wxWidgets_USE_UNIVERSAL OFF)
  SET( wxWidgets_USE_STATIC OFF)

  set (WXWIDGETS_FORCE_VERSION CACHE STRING "Force usage of a specific wxWidgets version.")
  if(WXWIDGETS_FORCE_VERSION)
    set (wxWidgets_CONFIG_OPTIONS --version=${WXWIDGETS_FORCE_VERSION})
  endif()
  
  FIND_PACKAGE(wxWidgets REQUIRED COMPONENTS ${wxWidgets_FIND_COMPONENTS})

  IF(MSVC)
    # Exclude wxexpat.lib, since we use our own version.
    # Other things are excluded as well, but we don't need them
    SET(wxWidgets_EXCLUDE_COMMON_LIBRARIES TRUE)
  ENDIF(MSVC)

  IF(WIN32 OR APPLE OR QT_ANDROID)
    #FIND_PACKAGE(wxWidgets REQUIRED)
    IF(MSYS)
      # this is to convert msys to windows paths, and handle the missing /usr
      STRING( REGEX REPLACE "/usr/local" ";C:/MinGW/msys/1.0/local" wxWidgets_INCLUDE_DIRS ${wxWidgets_INCLUDE_DIRS} )
    ENDIF(MSYS)
    INCLUDE(${wxWidgets_USE_FILE})
  ENDIF(WIN32 OR APPLE OR QT_ANDROID)

  MESSAGE (STATUS "Found wxWidgets..." )
  MESSAGE (STATUS " wxWidgets Include: ${wxWidgets_INCLUDE_DIRS}")
  MESSAGE (STATUS " wxWidgets Libraries: ${wxWidgets_LIBRARIES}")

  # We need to remove GLU from the wxWidgets_LIBRARIES list
  # It only appears to get on the list for MSW...
  FOREACH (_currentLibFile ${wxWidgets_LIBRARIES})
    SET(UCNAME ${_currentLibFile})
    string(TOUPPER ${UCNAME} UCNAME)
    IF(NOT ${UCNAME} MATCHES   "(.*)GLU(.*)")
      SET( REVISED_wxWidgets_LIBRARIES  ${REVISED_wxWidgets_LIBRARIES} ${_currentLibFile})
    ENDIF()
  ENDFOREACH (_currentLibFile )
  SET( wxWidgets_LIBRARIES ${REVISED_wxWidgets_LIBRARIES})

  MESSAGE (STATUS " Revised wxWidgets Libraries: ${wxWidgets_LIBRARIES}")
ENDIF(NOT QT_ANDROID)

IF(NOT WIN32 AND NOT APPLE AND NOT QT_ANDROID)
  OPTION(OCPN_FORCE_GTK3 "Force the build to use GTK3" OFF)

  IF(NOT OCPN_FORCE_GTK3)
    FIND_PACKAGE(GTK2)
  ENDIF(NOT OCPN_FORCE_GTK3)

  IF(GTK2_FOUND)
    set(wxWidgets_CONFIG_OPTIONS ${wxWidgets_CONFIG_OPTIONS} --toolkit=gtk2)
    INCLUDE_DIRECTORIES(${GTK2_INCLUDE_DIRS})
    SET(GTK_LIBRARIES ${GTK2_LIBRARIES})
    MESSAGE(STATUS "Building against GTK2...")
  ELSE(GTK2_FOUND)
    FIND_PACKAGE(GTK3)
    INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS})
    SET(GTK_LIBRARIES ${GTK3_LIBRARIES})
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__WXGTK3__")
    set(wxWidgets_CONFIG_OPTIONS ${wxWidgets_CONFIG_OPTIONS} --toolkit=gtk3)
    MESSAGE(STATUS "Building against GTK3...")
  ENDIF(GTK2_FOUND)
  FIND_PACKAGE(wxWidgets REQUIRED)
  INCLUDE(${wxWidgets_USE_FILE})
  SET(EXTRA_LIBS ${EXTRA_LIBS} ${GTK_LIBRARIES})
ENDIF(NOT WIN32 AND NOT APPLE AND NOT QT_ANDROID)

SET(BUILD_SHARED_LIBS TRUE)

FIND_PACKAGE(Gettext REQUIRED)

