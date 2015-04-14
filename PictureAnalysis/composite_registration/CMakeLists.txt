# This is the root ITK CMakeLists file.
cmake_minimum_required(VERSION 2.4)
if(COMMAND CMAKE_POLICY)
  cmake_policy(SET CMP0003 NEW)
endif()

# Find ITK.
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

# include the VTK stuff for visualization
if (ITKVtkGlue_LOADED)
  find_package(VTK REQUIRED)
  include(${VTK_USE_FILE})
else()
  find_package(ItkVtkGlue REQUIRED)
  include(${ItkVtkGlue_USE_FILE})
  set(Glue ItkVtkGlue)
endif()

add_library(composite_registration composite_registration.cxx)

target_link_libraries(composite_registration ${Glue} ${VTK_LIBRARIES} ${ITK_LIBRARIES} composite_registration.cpp)

