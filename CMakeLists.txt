cmake_minimum_required(VERSION 3.10)

project(TWRay CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcolor-diagnostics -fansi-escape-codes")
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcolor-diagnostics -fansi-escape-codes -stdlib=libstdc++ -std=c++17 -fPIC")
endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
  # macOS-specific commands go here
  find_package(glog REQUIRED)

  include_directories(
    /usr/local/include/
    /usr/local/Cellar/glog/0.5.0/include/
    /usr/local/Cellar/openexr/3.1.3/include/OpenEXR/
    /usr/local/Cellar/imath/3.1.3/include/Imath/
    /usr/local/Cellar/gflags/2.2.2/include/
    src
    src/core
  )

  link_directories(
    /usr/local/lib/
    /usr/local/Cellar/glog/0.5.0/lib/
    /usr/local/Cellar/openexr/3.1.3/lib/
    /usr/local/Cellar/imath/3.1.3/lib/
    /usr/local/Cellar/gflags/2.2.2/lib/
  )

elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
  # Linux-specific commands go here
  find_package(GLOG REQUIRED) 
  find_package( Threads )

  include_directories(
    src
    src/core
    /usr/include/OpenEXR
    /usr/include/x86_64-linux-gnu/qt5
    /usr/include/x86_64-linux-gnu/qt5/QtCore
    /usr/include/x86_64-linux-gnu/qt5/QtGui
    /usr/include/x86_64-linux-gnu/qt5/QtWidgets
    /usr/local/include/glog/
    /usr/local/include/gflags/
  )

  link_directories(
    /usr/local/lib/
    /usr/local/lib/cmake/gflags/
    /usr/local/lib/cmake/glog/
  )
endif()

add_library(pbrt STATIC
  src/core/bssrdf.cpp
  src/core/bvh.cpp
  src/core/camera.cpp
  src/core/efloat.cpp
  src/core/error.cpp
  src/core/imageio.cpp
  src/core/interaction.cpp
  src/core/interpolation.cpp
  src/core/integrator.cpp
  src/core/fileutil.cpp
  src/core/film.cpp
  src/core/filter.cpp
  src/core/floatfile.cpp
  src/core/geometry.cpp
  src/core/light.cpp
  src/core/lightdistrib.cpp
  src/core/lowdiscrepancy.cpp
  src/core/material.cpp
  src/core/medium.cpp
  src/core/memory.cpp
  src/core/microfacet.cpp
  src/core/parallel.cpp
  src/core/paramset.cpp
  src/core/primitive.cpp
  src/core/progressreporter.cpp
  src/core/quaternion.cpp
  src/core/reflection.cpp
  src/core/sampling.cpp
  src/core/sampler.cpp
  src/core/scene.cpp
  src/core/shape.cpp
  src/core/sobolmatrices.cpp
  src/core/spectrum.cpp
  src/core/stats.cpp
  src/core/texture.cpp
  src/core/transform.cpp
  src/core/twray.cpp
  src/cameras/perspective.cpp
  src/ext/rply.cpp
  src/ext/lodepng.cpp
  src/ext/targa.cpp
  src/integrators/directlighting.cpp
  src/integrators/path.cpp
  src/integrators/volpath.cpp
  src/integrators/bdpt.cpp
  src/integrators/sppm.cpp
  src/filters/box.cpp
  src/lights/diffuse.cpp
  src/lights/distant.cpp
  src/lights/point.cpp
  src/lights/spot.cpp
  src/lights/infinite.cpp
  src/materials/matte.cpp
  src/materials/glass.cpp
  src/materials/disney.cpp
  src/materials/subsurface.cpp
  src/materials/uber.cpp
  src/media/homogeneous.cpp
  src/samplers/random.cpp
  src/samplers/zerotwosequence.cpp
  src/samplers/halton.cpp
  src/shapes/sphere.cpp
  src/shapes/disk.cpp
  src/shapes/plymesh.cpp
  src/shapes/triangle.cpp
  src/shapes/heightfield.cpp
  src/textures/constant.cpp
  src/textures/checkerboard.cpp
  src/qt/qt.cpp
)

FILE ( GLOB PBRT_SOURCE
  src/ext/*
  src/accelerators/*
  src/cameras/*
  src/filters/*
  src/integrators/*
  src/lights/*
  src/materials/*
  src/samplers/*
  src/shapes/*
  src/textures/*
  src/media/*
  src/qt/*
)

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
  # macOS-specific commands go here
  set(ALL_PBRT_LIBS
    pbrt
    gflags
    glog
    openexr
    Imath
  )
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
  # Linux-specific commands go here

  set(ALL_PBRT_LIBS
    pthread
    Qt5::Core
    Qt5::Widgets
    pbrt
    glog
    IlmImf
    Imath
    Half
  )
endif()

add_executable(TWRay
  src/core/main.cc
)

target_link_libraries(TWRay
  ${ALL_PBRT_LIBS}
)