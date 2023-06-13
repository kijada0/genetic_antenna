// ---------------------------------------- //
// created by: Kacper Kaczmarek
// created date: 12-06-2023
// ---------------------------------------- //

#include "main.h"
#include "utils.h"
#include "wire_geometry.h"
#include "fitness.h"


// -------------------------------------------------------------------------------- //

void create_population_zero(antenna_t *population, int population_size){
    for(int i = 0; i < population_size; i++){
        population[i].geometry = generate_random_antenna();
    }
}

// -------------------------------------------------------------------------------- //

void create_population_from_parents(antenna_t *population, antenna_t *parents, int *ranking, int population_size, int parent_count){

}

