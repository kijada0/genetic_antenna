// ---------------------------------------- //
// created by: Kacper Kaczmarek
// created date: 28-05-2023
// ---------------------------------------- //

#ifndef GENETIC_ANTENNA_WIRE_GEOMETRY_H
#define GENETIC_ANTENNA_WIRE_GEOMETRY_H

#include "main.h"
#include "utils.h"

// -------------------------------------------------------------------------------- //

struct wire_tip_t {
    double x;
    double y;
    double z;
};

struct wire_vector_t{
    wire_tip_t start;
    wire_tip_t end;
};


struct wire_parameters_t{
    double length;
    double angle_xy;
    double angle_xz;
};


struct antenna_geometry_t{
    wire_vector_t geometry[WIRE_COUNT+GROUND_PLANE_ELEMENT_COUNT];
    wire_parameters_t ground_plane[GROUND_PLANE_ELEMENT_COUNT];
    wire_parameters_t active_elements[WIRE_COUNT];
};

// -------------------------------------------------------------------------------- //

bool check_if_geometry_fits_in_cube(antenna_geometry_t *geometry, double cube_edge_length);
antenna_geometry_t generate_random_antenna();
void save_geometry_to_file(antenna_geometry_t *geometry, const std::string& file_name);
void print_antenna_geometry(antenna_geometry_t *geometry);

// -------------------------------------------------------------------------------- //

double calculate_wire_length(wire_vector_t *wire);


#endif //GENETIC_ANTENNA_WIRE_GEOMETRY_H
