// ---------------------------------------- //
// created by: Kacper Kaczmarek
// created date: 29-05-2023
// ---------------------------------------- //

#ifndef GENETIC_ANTENNA_FITNESS_FUNCTIONS_H
#define GENETIC_ANTENNA_FITNESS_FUNCTIONS_H

#define WIRE_RADIUS 0.005           // 3mm in meters
#define WIRE_PER_WAVELENGTH 16      // should be around 12-20

#define FREQ_STEP 100000            // 100kHz
#define FREQ_OFFSET 1000000         // 1MHz

#include "main.h"
#include "wire_geometry.h"
#include "utils.h"

// -------------------------------------------------------------------------------- //

struct antenna_gain_t{
    double max;
    double min;
    double mean;
};

struct antenna_impedance_t{
    double real;
    double imag;
    double Z;
};

struct antenna_SWR_t{
    double SWR;
    double VSWR;
    double S11;
};

struct antenna_band_t{
    double start;
    double end;
    double bandwidth;
};

struct antenna_parameters_t{
    antenna_gain_t gain;
    antenna_gain_t RHCP_gain;
    antenna_gain_t LHCP_gain;
    antenna_impedance_t impedance;
    antenna_SWR_t SWR;
    antenna_band_t band;
};

struct antenna_t{
    antenna_geometry_t geometry;
    antenna_parameters_t parameters;
    int fitness;
};

// -------------------------------------------------------------------------------- //

antenna_parameters_t calculate_antenna_parameters(antenna_geometry_t *geometry, double frequency);
void print_antenna_parameters(antenna_parameters_t *parameters);

int calculate_antenna_fitness(antenna_geometry_t *geometry, double frequency);
void sort_antennas_by_fitness(antenna_t *antennas, int antenna_count);



#endif //GENETIC_ANTENNA_FITNESS_FUNCTIONS_H
