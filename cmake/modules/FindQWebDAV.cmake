#
#  FindQWebDAV.cmake
#
#  Created by Dale Glass
#  Copyright (c) 2023 Overte e.V.
#

# QWEBDAV_FOUND               - QWebDAV library was found
# QWEBDAV_INCLUDE_DIR         - Path to QWebDAV include dir
# QWEBDAV_INCLUDE_DIRS        - Path to QWebDAV
# QWEBDAV_LIBRARIES           - List of QWebDAV libraries

include("${MACRO_DIR}/HifiLibrarySearchHints.cmake")
hifi_library_search_hints("qwebdav")

if (WIN32)
    find_path(QWEBDAV_INCLUDE_DIRS qwebdav.h PATH_SUFFIXES include/qwebdavlib HINTS ${QWEBDAV_SEARCH_DIRS})
elseif (APPLE)
    find_path(QWEBDAV_INCLUDE_DIRS qwebdav.h PATH_SUFFIXES include/qwebdavlib HINTS ${QWEBDAV_SEARCH_DIRS})
else ()
    find_path(QWEBDAV_INCLUDE_DIRS qwebdav.h PATH_SUFFIXES include/qwebdavlib HINTS ${QWEBDAV_SEARCH_DIRS})
endif ()

# We have separate debug and release versions
find_library(QWEBDAV_LIBRARY_RELEASE Qt-WebDAV PATH_SUFFIXES "lib" "lib64" HINTS ${QWEBDAV_SEARCH_DIRS})
find_library(QWEBDAV_LIBRARY_DEBUG Qt-WebDAVdbg PATH_SUFFIXES "lib" "lib64" HINTS ${QWEBDAV_SEARCH_DIRS})

# This handles dealing with the DEBUG/RELEASE configurations above, and
# sets QWEBDAV_LIBRARIES appropriately.
include(SelectLibraryConfigurations)
select_library_configurations(QWEBDAV)

# This figures out whether everything has been found, and reports to the user
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(QWebDAV DEFAULT_MSG QWEBDAV_INCLUDE_DIRS QWEBDAV_LIBRARIES)

mark_as_advanced(QWEBDAV_INCLUDE_DIRS QWEBDAV_SEARCH_DIRS)