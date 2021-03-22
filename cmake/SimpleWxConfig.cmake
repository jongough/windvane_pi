macro(SimpleWxConfig)
# if(NOT QT_ANDROID)
#    set( wxWidgets_USE_DEBUG OFF)
#    set( wxWidgets_USE_UNICODE ON)
#    set( wxWidgets_USE_UNIVERSAL OFF)
#    set( wxWidgets_USE_STATIC OFF)
#    if(WXWIDGETS_FORCE_VERSION)
#        set (wxWidgets_CONFIG_OPTIONS --version=${WXWIDGETS_FORCE_VERSION})
#    endif()
#    if(MSVC)
#        # Exclude wxexpat.lib, since we use our own version.
#        # Other things are excluded as well, but we don't need them
#        SET(wxWidgets_EXCLUDE_COMMON_LIBRARIES TRUE)
#    endif(MSVC)
#    set(wxWidgets_USE_LIBS base core xml html)
#    find_package(wxWidgets REQUIRED COMPONENTS ${wxWidgets_USE_LIBS})
#    INCLUDE(${wxWidgets_USE_FILE})
#
#    find_package(GTK2)
#    if(GTK2_FOUND AND NOT "$ENV{BUILD_GTK3}" STREQUAL "TRUE")
#        set(wxWidgets_CONFIG_OPTIONS
#            ${wxWidgets_CONFIG_OPTIONS} --toolkit=gtk2)
#    else ()
#        find_package(GTK3)
#        if(GTK3_FOUND)
#            set(wxWidgets_CONFIG_OPTIONS
#                ${wxWidgets_CONFIG_OPTIONS} --toolkit=gtk3)
#        endif ()
#    endif ()
# endif(NOT QT_ANDROID)
#endmacro(SimpleWxConfig)
if(NOT QT_ANDROID)
    # Find wxWidgets here, and the setting get inherited by all plugins. These options can be used to set the linux widgets build type
    set(wxWidgets_USE_DEBUG OFF)
    set(wxWidgets_USE_UNICODE ON)
    set(wxWidgets_USE_UNIVERSAL OFF)
    set(wxWidgets_USE_STATIC OFF)

    set(WXWIDGETS_FORCE_VERSION CACHE STRING "Force usage of a specific wxWidgets version.")
    if(WXWIDGETS_FORCE_VERSION)
        set(wxWidgets_CONFIG_OPTIONS --version=${WXWIDGETS_FORCE_VERSION})
    endif()

    set(wxWidgets_USE_LIBS base core xml html)
    message(STATUS "${CMLOC}wxWidgets components: ${wxWidgets_USE_LIBS}")
    find_package(wxWidgets REQUIRED COMPONENTS ${wxWidgets_USE_LIBS})

    if(MSVC)
        # Exclude wxexpat.lib, since we use our own version. Other things are excluded as well, but we don't need them
        set(wxWidgets_EXCLUDE_COMMON_LIBRARIES TRUE)
    endif(MSVC)

    if(WIN32 OR APPLE OR QT_ANDROID)
        if(MSYS)
            # this is to convert msys to windows paths, and handle the missing /usr
            string(REGEX REPLACE "/usr/local" ";C:/MinGW/msys/1.0/local" wxWidgets_INCLUDE_DIRS ${wxWidgets_INCLUDE_DIRS})
        endif(MSYS)
    endif(WIN32 OR APPLE OR QT_ANDROID)

    include(${wxWidgets_USE_FILE})

    message(STATUS "${CMLOC}Found wxWidgets...")
    message(STATUS "${CMLOC} wxWidgets Include: ${wxWidgets_INCLUDE_DIRS}")
    message(STATUS "${CMLOC} wxWidgets Libraries: ${wxWidgets_LIBRARIES}")

    # We need to remove GLU from the wxWidgets_LIBRARIES list It only appears to get on the list for MSW...
    foreach(_currentLibFile ${wxWidgets_LIBRARIES})
        set(UCNAME ${_currentLibFile})
        string(TOUPPER ${UCNAME} UCNAME)
        if(NOT ${UCNAME} MATCHES "(.*)GLU(.*)")
            set(REVISED_wxWidgets_LIBRARIES ${REVISED_wxWidgets_LIBRARIES} ${_currentLibFile})
        endif()
    endforeach(_currentLibFile)
    set(wxWidgets_LIBRARIES ${REVISED_wxWidgets_LIBRARIES})

    message(STATUS "${CMLOC} Revised wxWidgets Libraries: ${wxWidgets_LIBRARIES}")
endif(NOT QT_ANDROID)
endmacro(SimpleWxConfig)
