cmake_minimum_required(VERSION 3.0.2)
project(minimal_ogre)

find_package(PkgConfig REQUIRED)
find_package(Boost REQUIRED
  COMPONENTS
  system
)

pkg_check_modules(OGRE OGRE OGRE-Overlay OGRE-RTShaderSystem)
pkg_check_modules(OIS OIS)

message("OGRE_INCLUDE_DIRS: ${OGRE_INCLUDE_DIRS}")
message("OGRE_LIBRARIES: ${OGRE_LIBRARIES}")
include_directories(
  ../Common
  ${OGRE_INCLUDE_DIRS}
  ${OIS_INCLUDE_DIRS}
)

# Add executable
add_executable(BasicTutorial4 TutorialApplication.cpp ../Common/BaseApplication.cpp)
target_link_libraries(BasicTutorial4 ${OGRE_LIBRARIES} ${OIS_LIBRARIES} ${Boost_LIBRARIES})
