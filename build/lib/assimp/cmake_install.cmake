# Install script for directory: /Users/juancarlossanchezruiz/Documents/Proyects/Nitro/lib/assimp

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibassimp5.0.1-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/assimp-5.0" TYPE FILE FILES
    "/Users/juancarlossanchezruiz/Documents/Proyects/Nitro/build/lib/assimp/assimp-config.cmake"
    "/Users/juancarlossanchezruiz/Documents/Proyects/Nitro/build/lib/assimp/assimp-config-version.cmake"
    "/Users/juancarlossanchezruiz/Documents/Proyects/Nitro/build/lib/assimp/assimpTargets.cmake"
    "/Users/juancarlossanchezruiz/Documents/Proyects/Nitro/build/lib/assimp/assimpTargets-release.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibassimp5.0.1-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/Users/juancarlossanchezruiz/Documents/Proyects/Nitro/build/lib/assimp/assimp.pc")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/Users/juancarlossanchezruiz/Documents/Proyects/Nitro/build/lib/assimp/contrib/cmake_install.cmake")
  include("/Users/juancarlossanchezruiz/Documents/Proyects/Nitro/build/lib/assimp/code/cmake_install.cmake")
  include("/Users/juancarlossanchezruiz/Documents/Proyects/Nitro/build/lib/assimp/tools/assimp_cmd/cmake_install.cmake")
  include("/Users/juancarlossanchezruiz/Documents/Proyects/Nitro/build/lib/assimp/test/cmake_install.cmake")

endif()

