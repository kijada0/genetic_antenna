cmake_minimum_required(VERSION 3.25)
project(genetic_antenna)

set(CMAKE_CXX_STANDARD 11)

set(CPP_SRC main.cpp)

# LINKING NECPP
set(NECPP_SRC
        "D:/Documents/Library/necpp/src/c_ggrid.cpp"
        "D:/Documents/Library/necpp/src/matrix_algebra.cpp"
        "D:/Documents/Library/necpp/src/nec_ground.cpp"
        "D:/Documents/Library/necpp/src/c_evlcom.cpp"
        "D:/Documents/Library/necpp/src/c_plot_card.cpp"
        "D:/Documents/Library/necpp/src/misc.cpp"
        "D:/Documents/Library/necpp/src/nec_context.cpp"
        "D:/Documents/Library/necpp/src/nec_output.cpp"
        "D:/Documents/Library/necpp/src/c_geometry.cpp"
        "D:/Documents/Library/necpp/src/nec_exception.cpp"
        "D:/Documents/Library/necpp/src/nec_radiation_pattern.cpp"
        "D:/Documents/Library/necpp/src/nec_structure_currents.cpp"
        "D:/Documents/Library/necpp/src/electromag.cpp"
        "D:/Documents/Library/necpp/src/nec_results.cpp"
        )

# ADD EXECUTABLE
add_executable(genetic_antenna ${CPP_SRC} ${NECPP_SRC})

target_include_directories(genetic_antenna PRIVATE D:/Documents/Library/necpp/src)
target_compile_options(genetic_antenna PRIVATE -O2 -Wall)
target_link_libraries(genetic_antenna PRIVATE stdc++ m lapack)
