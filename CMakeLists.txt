cmake_minimum_required( VERSION 3.1.0...3.27.0 )

project( raytracing_weekend LANGUAGES CXX )

# Source
set ( SOURCE_ONE_WEEKEND
      main.cpp )

# Executables
add_executable( in_one_weekend ${SOURCE_ONE_WEEKEND} )
