#[=======================================================================[.rst:
FindNX
-----------

Find NX CAD for building NXOpen User Function

Hints
^^^^^

The following variables may be set to control search behavior:

``NX_ROOT_DIR``
  Set to the root directory of an NX installation.

#]=======================================================================]

if(NOT MSVC)
    message(FATAL_ERROR "NX only avaliable on Windows MSVC")
endif()

message(STATUS "MSVC_TOOLSET_VERSION=${MSVC_TOOLSET_VERSION}")

if(NOT ${MSVC_TOOLSET_VERSION} STREQUAL "140")
    message(FATAL_ERROR "NX User Function requires MSVC 2015 build tools (toolset v140)")
endif()

set(NX_VERSION )

find_path(NX_INCLUDE_DIR
    NAMES
      uf.h
    HINTS
      ${NX_ROOT_DIR}
    PATHS
      "C:\\Program Files\\Siemens\\NX1926"
      "C:\\Program Files\\Siemens\\NX1953"
      "C:\\Program Files\\Siemens\\NX1980"
      "C:\\Program Files\\Siemens\\NX2007"
      "C:\\Program Files\\Siemens\\NX2206"
      "C:\\Program Files\\Siemens\\NX2212"
    PATH_SUFFIXES
        "UGOPEN"
)

message(STATUS "Found NX_INCLUDE_DIR=${NX_INCLUDE_DIR}")

macro(_FIND_NX_LIBRARY _varname _targetname _libname )

find_library(${_varname}
  NAMES ${_libname}
  HINTS
    ${NX_ROOT_DIR}
  PATHS
    "C:\\Program Files\\Siemens\\NX1926"
    "C:\\Program Files\\Siemens\\NX1953"
    "C:\\Program Files\\Siemens\\NX1980"
    "C:\\Program Files\\Siemens\\NX2007"
    "C:\\Program Files\\Siemens\\NX2206"
    "C:\\Program Files\\Siemens\\NX2212"
  PATH_SUFFIXES
    "UGOPEN"

    )
message(STATUS "Found ${_varname}=${${_varname}}")

if(${_varname})
  if(NOT TARGET NX::${_targetname})
    add_library(NX::${_targetname} UNKNOWN IMPORTED)
    set_target_properties(NX::${_targetname} PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES "${NX_INCLUDE_DIR}")
    if(${_varname})
      set_target_properties(NX::${_targetname} PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES "CPP"
        IMPORTED_LOCATION "${${_varname}}"
        )
    endif()
  endif()
endif()


endmacro()

_FIND_NX_LIBRARY(NX_OPENCPP_LIBRARY nxopencpp libnxopencpp)
_FIND_NX_LIBRARY(NX_UFUN_LIBRARY ufun libufun)
_FIND_NX_LIBRARY(NX_UGOPENINT_LIBRARY ugopenint libugopenint)

unset(_FIND_NX_LIBRARY)

cmake_path(CONVERT ${NX_INCLUDE_DIR} TO_CMAKE_PATH_LIST NX_INCLUDE_DIR_N)

message(STATUS "NX_INCLUDE_DIR_N=${NX_INCLUDE_DIR_N}")

string(REGEX MATCH "NX([0-9][0-9][0-9][0-9])/UGOPEN" NX_INCLUDE_DIR_STUB ${NX_INCLUDE_DIR_N})
string(REGEX REPLACE "NX([0-9][0-9][0-9][0-9])/UGOPEN" "\\1" NX_VERSION ${NX_INCLUDE_DIR_STUB})

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(NX  REQUIRED_VARS  NX_INCLUDE_DIR  
  NX_OPENCPP_LIBRARY NX_UFUN_LIBRARY NX_UGOPENINT_LIBRARY
  VERSION_VAR NX_VERSION )
