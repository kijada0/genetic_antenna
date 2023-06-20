// ---------------------------------------- //
// created by: Kacper Kaczmarek
// created date: 29-05-2023
// ---------------------------------------- //

#ifndef GENETIC_ANTENNA_FITNESS_H
#define GENETIC_ANTENNA_FITNESS_H

#define WIRE_RADIUS 0.003           // 3mm in meters
#define WIRE_PER_WAVELENGTH 128     // number of wires per wavelength
#define WIRE_SEGMENT_COUNT 4      // number of segments of wire

#define MES_FREQ_STEP 500000        // 500kHz
#define MES_BANDWIDTH 25000000      // 25MHz
#define MES_STEP_COUNT (int)(MES_BANDWIDTH/MES_FREQ_STEP)
#define MES_GAIN_STEP_COUNT 24      // 24 steps of gain

#define MAX_SWR 1.0

#define GAIN_WEIGHT 200
#define GAIN_RHCP_WEIGHT 100
#define GAIN_LHCP_WEIGHT 25
#define VSWR_WEIGHT 800
#define S11_WEIGHT 4
#define BANDWIDTH_WEIGHT 50

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
    int start;
    int end;
    int bandwidth;
};

struct antenna_parameters_freq_sweep_t{
    antenna_impedance_t impedance[MES_STEP_COUNT];
    antenna_SWR_t SWR[MES_STEP_COUNT];
};

struct antenna_parameters_t{
    antenna_gain_t gain;
    antenna_gain_t RHCP_gain;
    antenna_gain_t LHCP_gain;
    antenna_impedance_t impedance;
    antenna_SWR_t SWR;
    antenna_band_t band;
    int error_flag;
    int death_flag;
};

struct antenna_t{
    antenna_geometry_t geometry;
    antenna_parameters_t parameters;
    int fitness;
};

// -------------------------------------------------------------------------------- //

antenna_parameters_t calculate_antenna_parameters(antenna_geometry_t *geometry);
void print_antenna_parameters(antenna_parameters_t *parameters);

int calculate_antenna_fitness(antenna_parameters_t *parameters);
void sort_antennas_by_fitness(antenna_t *population, int *ranking, int population_size);

void clear_antenna_parameters(antenna_parameters_t *parameters);


#endif //GENETIC_ANTENNA_FITNESS_H
