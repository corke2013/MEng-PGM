include(FindPackageHandleStandardArgs)

# find ptools parent dir for include dirs
find_path(
    PTOOLS_SOURCE_DIR
    NAMES opencontainers_1_8_5
    PATHS $ENV{HOME}/devel/ptools
    )

message(${PTOOLS_SOURCE_DIR})

# finally the library itself
find_library(
    PTOOLS_LIBRARY
    NAMES ptools
    PATHS $ENV{HOME}/devel/ptools/build
    )

message(${PTOOLS_LIBRARY})

set(PTOOLS_INCLUDE_DIRS
    ${PTOOLS_SOURCE_DIR}/src
    ${PTOOLS_SOURCE_DIR}/opencontainers_1_8_5
    )

find_package_handle_standard_args(
    ptools DEFAULT_MSG
    PTOOLS_LIBRARY PTOOLS_INCLUDE_DIRS
    )

set(PTOOLS_LIBRARIES ${PTOOLS_LIBRARY})

mark_as_advanced(PTOOLS_LIBRARY PTOOLS_INCLUDE_DIRS)