cmake_minimum_required(VERSION 3.0.2)
project(tiny_ogre)

find_package(PkgConfig REQUIRED)
find_package(Boost REQUIRED
  COMPONENTS
  system
)

pkg_check_modules(OGRE OGRE OGRE-Overlay)
pkg_check_modules(OIS OIS)

message("OGRE_OV_INCLUDE_DIRS: ${OGRE_OV_INCLUDE_DIRS}")
message("ZLIB_LIBRARIES: ${ZLIB_LIBRARIES}")
include_directories(
  ${OGRE_INCLUDE_DIRS}
  ${OIS_INCLUDE_DIRS}
)

# Add executable
add_executable(TinyOgre TinyOgre.cpp)
target_link_libraries(TinyOgre ${OGRE_LIBRARIES} ${OIS_LIBRARIES} ${Boost_LIBRARIES} ${ZLIB_LIBRARIES})
