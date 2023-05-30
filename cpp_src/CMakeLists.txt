cmake_minimum_required(VERSION 3.20)
project(genetic_antenna)

set(CMAKE_CXX_STANDARD 20)

set(CPP_SRC
        main.cpp
        main.h
        wire_geometry.cpp
        wire_geometry.h
        physical_calculations.cpp
        physical_calculations.h
        fitness_functions.cpp
        fitness_functions.h
        mutation_functions.cpp
        mutation_functions.h
    )

# LINKING NECPP
set(NECPP_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/necpp/src/c_ggrid.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/necpp/src/matrix_algebra.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/necpp/src/nec_ground.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/necpp/src/c_evlcom.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/necpp/src/c_plot_card.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/necpp/src/misc.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/necpp/src/nec_context.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/necpp/src/nec_output.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/necpp/src/c_geometry.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/necpp/src/nec_exception.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/necpp/src/nec_radiation_pattern.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/necpp/src/nec_structure_currents.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/necpp/src/electromag.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/necpp/src/nec_results.cpp
        )

# ADD EXECUTABLE
add_executable(genetic_antenna ${CPP_SRC} ${NECPP_SRC})

add_library(lapack SHARED IMPORTED)
set_target_properties(lapack PROPERTIES
        IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/lapack/liblapack.dll
        IMPORTED_IMPLIB ${CMAKE_CURRENT_SOURCE_DIR}/lib/lapack/liblapack.lib
    )

target_include_directories(genetic_antenna PRIVATE D:/Documents/Library/necpp/src)
target_compile_options(genetic_antenna PRIVATE -O2 -Wall)
target_link_libraries(genetic_antenna PRIVATE stdc++ m lapack)
