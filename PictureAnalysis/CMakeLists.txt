# This is the root ITK CMakeLists file.
cmake_minimum_required(VERSION 2.4)
if(COMMAND CMAKE_POLICY)
  cmake_policy(SET CMP0003 NEW)
endif()


# This project is designed to be built outside the Insight source tree.
project(pictureAnalysis)

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

include_directories(“${PROJECT_SOURCE_DIR}/composite_registration”)
add_subdirectory (composite_registration)


#find_package(composite_registration REQUIRED)
#include(${composite_registration_USE_FILE})


add_executable(PictureAnalysis PictureAnalysis.cxx)

target_link_libraries(PictureAnalysis ${Glue} ${VTK_LIBRARIES} ${ITK_LIBRARIES} composite_registration)
