// ---------------------------------------- //
// created by: Kacper Kaczmarek
// created date: 12-06-2023
// ---------------------------------------- //

#ifndef GENETIC_ANTENNA_EVOLUTION_H
#define GENETIC_ANTENNA_EVOLUTION_H

#endif //GENETIC_ANTENNA_EVOLUTION_H

#include "main.h"
#include "utils.h"
#include "wire_geometry.h"
#include "fitness.h"

// -------------------------------------------------------------------------------- //

void create_population_zero(antenna_t *population, int population_size);
void create_population_from_parents(antenna_t *population, antenna_t *parents, int *ranking, int population_size, int parent_count);

void save_population_to_file(antenna_t *population);
void load_population_from_file(antenna_t *population);