message("[Findfreeglut.cmake] begin")

# Check if paths are set.
#if(NOT DEFINED ${VAR_NAME})

message("[Findfreeglut.cmake] CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")

if( NOT DEFINED FREEGLUT_HOME )
    message(FATAL_ERROR "[Findfreeglut.cmake] FREEGLUT_HOME  is not set.")
endif( NOT DEFINED FREEGLUT_HOME )

set( freeglut_INCLUDE_DIR ${FREEGLUT_HOME}/inc )
message("[Findfreeglut.cmake] freeglut_INCLUDE_DIR: ${freeglut_INCLUDE_DIR}")

#message("[Findfreeglut.cmake] freeglut_LIBRARY_TEMP: ${freeglut_LIBRARY_TEMP}")
set( freeglut_LIBRARY_TEMP $ENV{CUL_OUTPUT_DIR}/${CMAKE_BUILD_TYPE}/freeglut.lib )
set( freeglut_LIBRARY ${freeglut_LIBRARY_TEMP} )
set( freeglut_DIR $ENV{CUL_ROOT_DIR} )

set( freeglut_FOUND true )
message("[Findfreeglut.cmake] end")
