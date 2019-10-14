cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

#file(GLOB SOURCES "tensor_net/*.h")
#add_library(native-lib SHARED ${SOURCES})
add_library(nativelib STATIC tensor_net/tensor_net.cpp)
set_property(TARGET nativelib PROPERTY CXX_STANDARD 17)
add_executable(3DSmoothNet main.cpp core/core.cpp core/core.h)
set_property(TARGET 3DSmoothNet PROPERTY CXX_STANDARD 17)
# try the shared tensorflow library first
find_package(TensorflowCC COMPONENTS Shared)
if(TensorflowCC_Shared_FOUND)
  message("Tensor shared")
  target_link_libraries(3DSmoothNet TensorflowCC::Shared)
  target_link_libraries(nativelib TensorflowCC::Shared)
# fallback to the static library
else()
  message("Tensor static")
  find_package(TensorflowCC REQUIRED COMPONENTS Static)
  target_link_libraries(3DSmoothNet TensorflowCC::Static)
  target_link_libraries(nativelib TensorflowCC::Static)
endif()

find_package( PCL 1.8 REQUIRED)
# PATHS /usr/lib/x86_64-linux-gnu/cmake/pcl
if( PCL_FOUND )
  # [C/C++]>[General]>[Additional Include Directories]
  include_directories( ${PCL_INCLUDE_DIRS} )
  message(${PCL_INCLUDE_DIRS})
  # [C/C++]>[Preprocessor]>[Preprocessor Definitions]
  add_definitions( ${PCL_DEFINITIONS} )
  
  # For Use Not PreCompiled Features 
  #add_definitions( -DPCL_NO_PRECOMPILE )

  # [Linker]>[General]>[Additional Library Directories]
  link_directories( ${PCL_LIBRARY_DIRS} )

  # [Linker]>[Input]>[Additional Dependencies]
endif()

# link cuda if it is available
find_package(CUDA)
if(CUDA_FOUND)
  target_link_libraries(3DSmoothNet ${CUDA_LIBRARIES} ${PCL_LIBRARIES} )
  # "/cnpy/build/libcnpy.so"
endif()

FIND_PACKAGE(Boost COMPONENTS program_options REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

target_link_libraries(3DSmoothNet nativelib ${Boost_LIBRARIES} )


FIND_PACKAGE( OpenMP REQUIRED)
if(OPENMP_FOUND)
  message("OPENMP FOUND")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} -lboost_program_options")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

