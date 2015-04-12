#
# This file sets up include directories, link directories, and
# compiler settings for a project to use ItkVtkGlue.  It should not be
# included directly, but rather through the ItkVtkGlue_USE_FILE setting
# obtained from ItkVtkGlue.cmake.
#

# The hints point the find_package to the proper VTK and ITK
find_package(VTK REQUIRED HINTS /Users/matthew/Documents/18-791/MIIA/VTKBin)
include(${VTK_USE_FILE})

find_package(ITK REQUIRED HINTS /Users/matthew/Documents/18-791/MIIA/SITKBin/ITK-build)
include(${ITK_USE_FILE})

# Add include directories needed to use ItkVtkGlue.
include_directories(/Users/matthew/Documents/18-791/local_projects/ItkVtkGlueBin;/Users/matthew/Documents/18-791/local_projects/ItkVtkGlue)

# Add link directories needed to use ItkVtkGlue.
link_directories(/Users/matthew/Documents/18-791/local_projects/ItkVtkGlueBin)

# Define VTK_LIBRARIES if VTK does not define it
if(NOT VTK_LIBRARIES)
  set(VTK_LIBRARIES vtkHybrid vtkVolumeRendering)
endif()
