cmake_minimum_required(VERSION 3.0.2)
project(minimal_ogre)

find_package(PkgConfig REQUIRED)
find_package(Boost REQUIRED
  COMPONENTS
  system
)

pkg_check_modules(OGRE OGRE OGRE-Overlay)
pkg_check_modules(OIS OIS)

message("OGRE_INCLUDE_DIRS: ${OGRE_INCLUDE_DIRS}")
message("OGRE_LIBRARIES: ${OGRE_LIBRARIES}")
include_directories(${OGRE_INCLUDE_DIRS})
include_directories(${OIS_INCLUDE_DIRS})

# Add executable
add_executable(MinimalOgre MinimalOgre.cpp)
target_link_libraries(MinimalOgre ${OGRE_LIBRARIES} ${OIS_LIBRARIES} ${Boost_LIBRARIES})
