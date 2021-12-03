include(FindPackageHandleStandardArgs)

# find emdw parent dir for include dirs
find_path(
    EMDW_SOURCE_DIR
    NAMES emdw-base
    PATHS $ENV{HOME}/devel/emdw/src
    )

message(${EMDW_SOURCE_DIR})

# finally the library itself
find_library(
    EMDW_LIBRARY
    NAMES emdw
    PATHS $ENV{HOME}/devel/emdw/build/src
    )

message(${EMDW_LIBRARY})

set(EMDW_INCLUDE_DIRS
  ${EMDW_SOURCE_DIR}/emdw-base
  ${EMDW_SOURCE_DIR}/emdw-beliefprop
  ${EMDW_SOURCE_DIR}/emdw-clustering
  ${EMDW_SOURCE_DIR}/emdw-factors
  ${EMDW_SOURCE_DIR}/emdw-graphing
  ${EMDW_SOURCE_DIR}/utils-dependancyglue
  ${EMDW_SOURCE_DIR}/utils-informationtheory
  ${EMDW_SOURCE_DIR}/utils-matrix
  ${EMDW_SOURCE_DIR}/utils-misc
  ${EMDW_SOURCE_DIR}/utils-sigmapoints
  ${EMDW_SOURCE_DIR}/utils-vector
  )

find_package_handle_standard_args(
  emdw DEFAULT_MSG
  EMDW_LIBRARY EMDW_INCLUDE_DIRS
  )

set(EMDW_LIBRARIES ${EMDW_LIBRARY})

mark_as_advanced(EMDW_LIBRARY EMDW_INCLUDE_DIRS)
