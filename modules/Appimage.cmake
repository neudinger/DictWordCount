# This isn't meant to be a fully featured implementation. It's just a small playground project.
# Use `cmake -Bbuild -S. -GNinja` to generate the project
# Then use `cmake --build build --target package` to generate a .appimage file ready to go
# This project DOES do dangerous things and tries to download the linuxdeploy tool directly.
# When this makes it into IXM, it will be expected that users have the linuxdeploy tool installed to *some* location.
# It will NOT download and execute something from the internet.
cmake_minimum_required(VERSION 3.16)
project(summon-test LANGUAGES CXX VERSION 0.1.0)

add_executable(app)
set_property(TARGET app PROPERTY APPIMAGE_DESKTOP_FILE_TERMINAL YES)
set_property(TARGET app PROPERTY APPIMAGE_DESKTOP_FILE "${PROJECT_BINARY_DIR}/appimage/app.desktop")
set_property(TARGET app PROPERTY APPIMAGE_ICON_FILE "${PROJECT_BINARY_DIR}/appimage/app.summon.icon.svg")

install(TARGETS app
  RUNTIME DESTINATION bin)

file(GENERATE OUTPUT "${PROJECT_BINARY_DIR}/src/test.cxx"
  CONTENT [[
#include <cstdio>
int main () { std::puts("Hello, World!"); }
]])

target_sources(app PRIVATE ${PROJECT_BINARY_DIR}/src/test.cxx)

file(GENERATE
  OUTPUT "${PROJECT_BINARY_DIR}/appimage/app.summon.icon.svg"
  CONTENT [[<svg></svg>]])
file(GENERATE
  OUTPUT "${PROJECT_BINARY_DIR}/appimage/app.desktop"
  CONTENT [[
[Desktop Entry]
Name=AppImage And CPack External Generator Example
Exec=$<TARGET_FILE_NAME:app>
Icon=app.summon.icon
Type=Application
Categories=$<IF:$<BOOL:$<TARGET_PROPERTY:app,APPIMAGE_DESKTOP_FILE_CATEGORIES>>,$<TARGET_PROPERTY:app,APPIMAGE_DESKTOP_FILE_CATEGORIES>,Utility>$<SEMICOLON>
$<$<BOOL:$<TARGET_PROPERTY:app,APPIMAGE_DESKTOP_FILE_TERMINAL>>:Terminal=$<IF:$<BOOL:$<TARGET_PROPERTY:app,APPIMAGE_DESKTOP_FILE_TERMINAL>>,true,false>>
]])
file(GENERATE
  OUTPUT "${PROJECT_BINARY_DIR}/appimage-generate.cmake"
  CONTENT [[
include(CMakePrintHelpers)
cmake_print_variables(CPACK_TEMPORARY_DIRECTORY)
cmake_print_variables(CPACK_TOPLEVEL_DIRECTORY)
cmake_print_variables(CPACK_PACKAGE_DIRECTORY)
cmake_print_variables(CPACK_PACKAGE_FILE_NAME)

find_program(LINUXDEPLOY_EXECUTABLE
  NAMES linuxdeploy linuxdeploy-x86_64.AppImage
  PATHS ${CPACK_PACKAGE_DIRECTORY}/linuxdeploy)

if (NOT LINUXDEPLOY_EXECUTABLE)
  message(STATUS "Downloading linuxdeploy")
  set(LINUXDEPLOY_EXECUTABLE ${CPACK_PACKAGE_DIRECTORY}/linuxdeploy/linuxdeploy)
  file(DOWNLOAD 
      https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
      ${LINUXDEPLOY_EXECUTABLE}
      INACTIVITY_TIMEOUT 10
      LOG ${CPACK_PACKAGE_DIRECTORY}/linuxdeploy/download.log
      STATUS LINUXDEPLOY_DOWNLOAD)
  execute_process(COMMAND chmod +x ${LINUXDEPLOY_EXECUTABLE} COMMAND_ECHO STDOUT)
endif()

execute_process(
  COMMAND
    ${CMAKE_COMMAND} -E env
      OUTPUT=${CPACK_PACKAGE_FILE_NAME}.appimage
      VERSION=$<IF:$<BOOL:${CPACK_PACKAGE_VERSION}>,${CPACK_PACKAGE_VERSION},0.1.0>
    ${LINUXDEPLOY_EXECUTABLE}
    --appdir=${CPACK_TEMPORARY_DIRECTORY}
    --executable=$<TARGET_FILE:app>
    $<$<BOOL:$<TARGET_PROPERTY:app,APPIMAGE_DESKTOP_FILE>>:--desktop-file=$<TARGET_PROPERTY:app,APPIMAGE_DESKTOP_FILE>>
    $<$<BOOL:$<TARGET_PROPERTY:app,APPIMAGE_ICON_FILE>>:--icon-file=$<TARGET_PROPERTY:app,APPIMAGE_ICON_FILE>>
    --output=appimage
    #    --verbosity=2
)
]])

list(APPEND CPACK_GENERATOR External)

set(CPACK_EXTERNAL_PACKAGE_SCRIPT "${PROJECT_BINARY_DIR}/appimage-generate.cmake")
set(CPACK_EXTERNAL_ENABLE_STAGING YES)
set(CPACK_PACKAGE_FILE_NAME ${PROJECT_NAME}-${PROJECT_VERSION})
include(CPack)