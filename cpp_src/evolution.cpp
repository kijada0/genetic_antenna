// ---------------------------------------- //
// created by: Kacper Kaczmarek
// created date: 12-06-2023
// ---------------------------------------- //

#include <cstring>
#include <string>

#include "main.h"
#include "utils.h"
#include "wire_geometry.h"
#include "fitness.h"

using namespace std;

// -------------------------------------------------------------------------------- //


void inti_population(antenna_t *population, int population_size){
    for(int i = 0; i < population_size; i++){
        clear_antenna_geometry(&population[i].geometry);
        clear_antenna_parameters(&population[i].parameters);
        population[i].fitness = 0;
    }
}

// -------------------------------------------------------------------------------- //

void create_generation_zero(antenna_t *population, int population_size){
    for(int i = 0; i < population_size; i++){
        population[i].geometry = generate_random_antenna();
    }
}

// -------------------------------------------------------------------------------- //

void antenna_crossing_over(antenna_t *child, antenna_t *parent_A, antenna_t *parent_B){
    wire_parameters_t *child_active_wire = child->geometry.active_elements;
    wire_parameters_t *child_ground_plane = child->geometry.ground_plane;

    wire_parameters_t *parent_A_active_wire = parent_A->geometry.active_elements;
    wire_parameters_t *parent_A_ground_plane = parent_A->geometry.ground_plane;

    wire_parameters_t *parent_B_active_wire = parent_B->geometry.active_elements;
    wire_parameters_t *parent_B_ground_plane = parent_B->geometry.ground_plane;

    int crossing_over_point;
    for(int i = 0; i < WIRE_COUNT; i++){
        crossing_over_point = random_int_in_range(0, 3);

        child_active_wire[i].length = (crossing_over_point > 1) ? parent_A_active_wire[i].length : parent_B_active_wire[i].length;
        child_active_wire[i].angle_xy = (crossing_over_point > 2) ? parent_A_active_wire[i].angle_xy : parent_B_active_wire[i].angle_xy;
        child_active_wire[i].angle_xz = (crossing_over_point > 3) ? parent_A_active_wire[i].angle_xz : parent_B_active_wire[i].angle_xz;
    }

    crossing_over_point = random_int_in_range(0, 3);

    child_ground_plane[0].length = (crossing_over_point > 1) ? parent_A_ground_plane[0].length : parent_B_ground_plane[0].length;
    child_ground_plane[0].angle_xz = (crossing_over_point > 3) ? parent_A_ground_plane[0].angle_xz : parent_B_ground_plane[0].angle_xz;

    for(int i = 1; i < GROUND_PLANE_ELEMENT_COUNT; i++) {
        child_ground_plane[i].length = child_ground_plane[0].length;
        child_ground_plane[i].angle_xz = child_ground_plane[0].angle_xz;

        child_ground_plane[i].angle_xy = parent_A_ground_plane[i].angle_xy;
    }
}

void antenna_mutation(antenna_t *child, double mutation_rate){
    wire_parameters_t *child_active_wire = child->geometry.active_elements;
    wire_parameters_t *child_ground_plane = child->geometry.ground_plane;

    double mutation_multiplier;
    for(int i = 0; i < WIRE_COUNT; i++){
        mutation_multiplier = 1.0 + random_double_in_range(-1 * mutation_rate, mutation_rate);
        child_active_wire[i].length *= mutation_multiplier;

        mutation_multiplier = 1.0 + random_double_in_range(-1 * mutation_rate, mutation_rate);
        child_active_wire[i].angle_xy *= mutation_multiplier;

        mutation_multiplier = 1.0 + random_double_in_range(-1 * mutation_rate, mutation_rate);
        child_active_wire[i].angle_xz *= mutation_multiplier;
    }

    mutation_multiplier = 1.0 + random_double_in_range(-1 * mutation_rate, mutation_rate);
    child_ground_plane[0].length *= mutation_multiplier;

    mutation_multiplier = 1.0 + random_double_in_range(-1 * mutation_rate, mutation_rate);
    child_ground_plane[0].angle_xz *= mutation_multiplier;

    for(int i = 1; i < GROUND_PLANE_ELEMENT_COUNT; i++) {
        child_ground_plane[i].length = child_ground_plane[0].length;
        child_ground_plane[i].angle_xz = child_ground_plane[0].angle_xz;
    }

}

// -------------------------------------------------------------------------------- //

void create_next_generation(antenna_t *population, antenna_t *parents, int *ranking, int population_size, int parent_count){
    printf("Creating next generation...\n");
    antenna_t new_population[population_size];
    inti_population(&new_population[0], population_size);
    for(int i = 0; i < population_size; i++){
        int parent_A_ranking_index, parent_B_ranking_index;
        random_int_pair_without_repetition(&parent_A_ranking_index, &parent_B_ranking_index, 0, parent_count-1);

        int parent_A_index = ranking[parent_A_ranking_index];
        int parent_B_index = ranking[parent_B_ranking_index];
        printf("Parent A: %d (%d), Parent B: %d(%d) \n", parent_A_index, parent_A_ranking_index, parent_B_index, parent_B_ranking_index);

        antenna_crossing_over(&new_population[i], &parents[parent_A_index], &parents[parent_B_index]);
        antenna_mutation(&new_population[i], MUTATION_RATE);
        calculate_wire_geometry(&new_population[i].geometry);
    }

    memcpy(population, &new_population, sizeof(antenna_t) * population_size);
    printf("Next generation created.\n");
    printf("--------------------------------------------------\n");
}

// -------------------------------------------------------------------------------- //

void save_sorted_population_to_file(antenna_t *population, int *ranking, int population_size){
    create_folder_if_not_exist(string(OUTPUT_FILE_DIRECTORY));
    string file_name = string(OUTPUT_FILE_DIRECTORY) + "population_" + get_current_time() + ".csv";
    FILE *file = fopen(file_name.c_str(), "w");

    if(file == nullptr){
        printf("Error opening file!\n");
        exit(1);
    }

    string line = "";

    line += "index; ranking; fitness; ";
    line += "active_element_count; length; angle_xy; angle_xz; ";
    line += "ground_plane_element_count; length; angle_xy; angle_xz; ";
    fprintf(file, "%s\n", line.c_str());

    for(int i = 0; i < population_size; i++){
        line = "";
        line += to_string(i) + "; ";
        line += to_string(ranking[i]) + "; ";
        line += to_string(population[ranking[i]].fitness) + "; ";

        line += to_string(WIRE_COUNT) + "; ";
        for(int j = 0; j < WIRE_COUNT; j++){
            line += to_string(population[ranking[i]].geometry.active_elements[j].length) + "; ";
            line += to_string(population[ranking[i]].geometry.active_elements[j].angle_xy) + "; ";
            line += to_string(population[ranking[i]].geometry.active_elements[j].angle_xz) + "; ";
        }

        line += to_string(GROUND_PLANE_ELEMENT_COUNT) + "; ";
        for(int j = 0; j < GROUND_PLANE_ELEMENT_COUNT; j++){
            line += to_string(population[ranking[i]].geometry.ground_plane[j].length) + "; ";
            line += to_string(population[ranking[i]].geometry.ground_plane[j].angle_xy) + "; ";
            line += to_string(population[ranking[i]].geometry.ground_plane[j].angle_xz) + "; ";
        }

        fprintf(file, "%s\n", line.c_str());
    }

    fclose(file);
    printf("Population saved to file: %s \n", file_name.c_str());
}

// -------------------------------------------------------------------------------- //

