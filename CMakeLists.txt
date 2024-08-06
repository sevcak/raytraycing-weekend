cmake_minimum_required( VERSION 3.1.0...3.27.0 )

project( raytracing_weekend LANGUAGES CXX )

# Source
set ( EXTERNAL src/external/stb_image.h )
set ( SOURCE_ONE_WEEKEND
      src/main.cpp )


include_directories( src )
include_directories( src/external )

# Executables
add_executable( in_one_weekend ${EXTERNAL} ${SOURCE_ONE_WEEKEND} )
