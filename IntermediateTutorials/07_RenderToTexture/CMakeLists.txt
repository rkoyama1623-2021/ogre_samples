cmake_minimum_required(VERSION 3.0.2)
project(minimal_ogre)

find_package(PkgConfig REQUIRED)
find_package(Boost REQUIRED
  COMPONENTS
  system
)

pkg_check_modules(OGRE OGRE OGRE-Overlay OGRE-RTShaderSystem)
pkg_check_modules(OIS OIS)
# pkg_check_modules(CEGUI CEGUI-0 CEGUI-0-OGRE)

message("OGRE_INCLUDE_DIRS: ${OGRE_INCLUDE_DIRS}")
message("OGRE_LIBRARIES: ${OGRE_LIBRARIES}")
include_directories(
  ../Common
  ${OGRE_INCLUDE_DIRS}
  ${OIS_INCLUDE_DIRS}
  ${CEGUI_INCLUDE_DIRS}
)

# Add executable
add_executable(BasicApp BasicApp.cpp)
target_link_libraries(
  BasicApp
  ${OGRE_LIBRARIES}
  ${OIS_LIBRARIES}
  ${Boost_LIBRARIES}
  # ${CEGUI_LIBRARIES}
)
