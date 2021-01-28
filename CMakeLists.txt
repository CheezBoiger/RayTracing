# 
cmake_minimum_required(VERSION 3.0)
project("RayTracer")

set(RAY_TRACER_NAME "RayTracer")
set(RAY_TRACER_EXE "RayTracer")
set(RAY_TRACER_FILES )

set(RAY_TRACER_INCLUDES
    source
)

include(cmake/Main.cmake)
include(cmake/Common.cmake)
include(cmake/Math.cmake)
include(cmake/Scene.cmake)
include(cmake/FrameBuffer.cmake)
include(cmake/Acceleration.cmake)
include(cmake/Geometry.cmake)

include_directories(${RAY_TRACER_INCLUDES})
add_executable(${RAY_TRACER_EXE} ${RAY_TRACER_FILES})

if (MSVC)
  foreach(source IN LISTS RAY_TRACER_FILES)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
  endforeach()
endif()

# Visual Studio organization. For Good purposes.
if (WIN32)
  set_property(TARGET ${RAY_TRACER_EXE} 
    PROPERTY 
    FOLDER ${RAY_TRACER_EXE})
endif()