vcpkg_from_github(
  OUT_SOURCE_PATH
  SOURCE_PATH
  REPO
  daleglass/qwebdavlib
  REF
  master
  SHA512
  6dd39a3b5802d2ed297d03f2d82f5234e2cef746d1ec9b6c799b1c150a0c560d0f062d8f6f2b29c71373b07616e51a4d314f6ed0d9cd2c1905485b57de08fa0c
  HEAD_REF
  master
)


vcpkg_configure_cmake(
    SOURCE_PATH ${SOURCE_PATH}
    PREFER_NINJA
    OPTIONS -DBUILD_SAMPLES=OFF
)

vcpkg_install_cmake()
vcpkg_copy_pdbs()

# vcpkg requires the specific file for copyright
file(INSTALL ${SOURCE_PATH}/LICENSE DESTINATION ${CURRENT_PACKAGES_DIR}/share/qwebdav RENAME copyright)

# remove the unneeded files from the debug build, library is kept
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/share")

