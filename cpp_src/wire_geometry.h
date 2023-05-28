#ifndef GENETIC_ANTENNA_WIRE_GEOMETRY_H
#define GENETIC_ANTENNA_WIRE_GEOMETRY_H

#include "main.h"
#include "physical_calculations.h"

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
    wire_vector_t wires[WIRE_COUNT];
    wire_parameters_t geometry[WIRE_COUNT];
};

// -------------------------------------------------------------------------------- //

antenna_geometry_t generate_random_antenna();
void save_geometry_to_file(antenna_geometry_t *geometry, const char *file_name);


#endif //GENETIC_ANTENNA_WIRE_GEOMETRY_H
