// ---------------------------------------- //
// created by: Kacper Kaczmarek
// created date: 28-05-2023
// ---------------------------------------- //

#ifndef GENETIC_ANTENNA_MAIN_H
#define GENETIC_ANTENNA_MAIN_H

// ---------------------------------------- //
// Antenna parameters //
#define WIRE_COUNT 8
#define GROUND_PLANE_ELEMENT_COUNT 4
#define TOTAL_WIRE_COUNT (WIRE_COUNT+GROUND_PLANE_ELEMENT_COUNT)
#define CUBE_EDGE_LENGTH_RELATIVE_TO_WAVELENGTH 0.5

// ---------------------------------------- //
// Target parameters //
#define FREQ 144000000

// ---------------------------------------- //
// Population parameters //
#define POPULATION_SIZE 512
#define PARENT_COUNT 128
#define MUTATION_RATE 0.08


// ---------------------------------------- //
// System parameters //
#define MAX_THREAD_COUNT 24

#endif //GENETIC_ANTENNA_MAIN_H
