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

add_library(CompositeFilter CompositeFilter.cpp)
target_link_libraries( CompositeFilter ${Glue} ${VTK_LIBRARIES} ${ITK_LIBRARIES} )
target_include_directories (CompositeFilter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
