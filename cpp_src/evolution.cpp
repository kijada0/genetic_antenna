// ---------------------------------------- //
// created by: Kacper Kaczmarek
// created date: 12-06-2023
// ---------------------------------------- //

#include <cstring>
#include <string>
#include <filesystem>

#include "print.h"

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
    pr_info("Creating generation zero...");
    for(int i = 0; i < population_size; i++){
        population[i].geometry = generate_random_antenna();
    }
    pr_debug("Generation zero created.");
}

// -------------------------------------------------------------------------------- //

void antenna_crossing_over(antenna_t *child, antenna_t *parent_A, antenna_t *parent_B){
    pr_debug("Crossing over antennas...");
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
    pr_debug("Mutating antenna...");
    wire_parameters_t *child_active_wire = child->geometry.active_elements;
    wire_parameters_t *child_ground_plane = child->geometry.ground_plane;

    double mutation_multiplier;
    for(int i = 0; i < WIRE_COUNT; i++){
        mutation_multiplier = 1.0 + random_normal_double_in_range(-1 * mutation_rate, mutation_rate);
        child_active_wire[i].length *= mutation_multiplier;

        mutation_multiplier = 1.0 + random_normal_double_in_range(-1 * mutation_rate, mutation_rate);
        child_active_wire[i].angle_xy *= mutation_multiplier;

        mutation_multiplier = 1.0 + random_normal_double_in_range(-1 * mutation_rate, mutation_rate);
        child_active_wire[i].angle_xz *= mutation_multiplier;
    }

    mutation_multiplier = 1.0 + random_normal_double_in_range(-1 * mutation_rate, mutation_rate);
    child_ground_plane[0].length *= mutation_multiplier;

    mutation_multiplier = 1.0 + random_normal_double_in_range(-1 * mutation_rate, mutation_rate);
    child_ground_plane[0].angle_xz *= mutation_multiplier;

    for(int i = 1; i < GROUND_PLANE_ELEMENT_COUNT; i++) {
        child_ground_plane[i].length = child_ground_plane[0].length;
        child_ground_plane[i].angle_xz = child_ground_plane[0].angle_xz;
    }

}

// -------------------------------------------------------------------------------- //

void create_next_generation(antenna_t *population, antenna_t *parents, int *ranking, int population_size, int parent_count){
    pr_info("Creating next generation...");
    antenna_t new_population[population_size];
    inti_population(&new_population[0], population_size);
    for(int i = 0; i < population_size; i++){
        int parent_A_ranking_index, parent_B_ranking_index;
        //random_int_pair_without_repetition(&parent_A_ranking_index, &parent_B_ranking_index, 0, parent_count-1);
        random_int_pair_without_repetition_nonlinear_distribution(&parent_A_ranking_index, &parent_B_ranking_index, 0, parent_count - 1);

        int parent_A_index = ranking[parent_A_ranking_index];
        int parent_B_index = ranking[parent_B_ranking_index];
        pr_debug("Parent A: %d (%d), Parent B: %d(%d)", parent_A_index, parent_A_ranking_index, parent_B_index, parent_B_ranking_index);

        antenna_crossing_over(&new_population[i], &parents[parent_A_index], &parents[parent_B_index]);
        antenna_mutation(&new_population[i], MUTATION_RATE);
        calculate_wire_geometry(&new_population[i].geometry);
    }

    memcpy(population, &new_population, sizeof(antenna_t) * population_size);
    pr_debug("Next generation created.");
}

// -------------------------------------------------------------------------------- //

void save_sorted_population_to_file(antenna_t *population, int *ranking, int population_size){
    pr_debug("Saving population to file...");
    create_folder_if_not_exist(string(OUTPUT_FILE_DIRECTORY));

    string file_name = string(OUTPUT_FILE_DIRECTORY) + "population_" + get_current_time() + ".csv";
    FILE *file = fopen(file_name.c_str(), "w");

    string file_name_last = string(OUTPUT_FILE_DIRECTORY) + "population_last.csv";
    FILE *file_last = fopen(file_name_last.c_str(), "w");

    if(file == nullptr || file_last == nullptr){
        printf("Error opening file!\n");
        pr_error("Error opening file!");
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
        fprintf(file_last, "%s\n", line.c_str());
    }

    fclose(file);
    fclose(file_last);

    pr_info("Population saved to file... %s", file_name.c_str());
}

// -------------------------------------------------------------------------------- //

void save_telemetry(antenna_t *population, int *ranking, int population_size, int generation_number, int range, double generation_duration){
    create_folder_if_not_exist(string(TELEMETRY_FILE_DIRECTORY));
    string file_path = string(TELEMETRY_FILE_DIRECTORY) + "telemetry.csv";

    FILE *file = fopen(file_path.c_str(), "a");

    if(file == nullptr){
        printf("File does not exist. Creating new file.\n");
        pr_error("File does not exist. Creating new file.");
        file = fopen(file_path.c_str(), "w");
        if(file == nullptr){
            printf("Error creating file!\n");
            pr_error("Error creating file!");
            exit(1);
        }
    }

    string line = "";

    // line += get_current_time() + "; ";
    line += to_string(get_current_timestamp()) + "; ";
    line += to_string(generation_number) + "; ";
    line += to_string(generation_duration) + "; ";

    for(int i = 0; i < range; i++){
        line += to_string(population[ranking[i]].fitness) + "; ";
    }

    fprintf(file, "%s\n", line.c_str());
    fclose(file);

    pr_info("Saving telemetry... %s", file_path.c_str());
}

// -------------------------------------------------------------------------------- //

string getLatestFileName(const std::string& folder_path) {
    string latestFileName;
    filesystem::file_time_type latestTime;

    for (const auto& entry : filesystem::directory_iterator(folder_path)) {
        if (entry.is_regular_file()) {
            const auto& fileTime = filesystem::last_write_time(entry);
            if (fileTime > latestTime) {
                latestTime = fileTime;
                latestFileName = entry.path().filename().string();
            }
        }
    }

    return latestFileName;
}

int load_population_from_file(antenna_t *population, int population_size){
    pr_debug("Loading population from file...");
    string population_dir = string(OUTPUT_FILE_DIRECTORY);
    if(!filesystem::is_directory(population_dir)){
        printf("Population directory does not exist!\n");
        pr_error("Population directory does not exist!");
        return -1;
    }

    string file_name = population_dir + "population_last.csv";
    FILE *file = fopen(file_name.c_str(), "r");

    if(file == nullptr){
        printf("Error opening file!\n");
        pr_error("Error opening file!");
        return -1;
    }

    char line[5+3*WIRE_COUNT+3*GROUND_PLANE_ELEMENT_COUNT*16];
    string line_tab[5+3*WIRE_COUNT+3*GROUND_PLANE_ELEMENT_COUNT];
    int individual_count = 0;

    while(fgets(line, sizeof(line), file) != nullptr){
        if (individual_count >= 0){
            individual_count++;
            continue;
        }
        individual_count++;

        int i = 0;
        char *token = strtok(line, ";");
        while(token != nullptr){
            line_tab[i] = string(token);
            token = strtok(nullptr, ";");
            i++;
        }

        int index = stoi(line_tab[0]);
        int ranking = stoi(line_tab[1]);
        double fitness = stod(line_tab[2]);

        population[ranking].fitness = fitness;

        int active_element_count = stoi(line_tab[3]);
        for(int j = 0; j < active_element_count; j++){
            population[ranking].geometry.active_elements[j].length = stod(line_tab[4+j*3]);
            population[ranking].geometry.active_elements[j].angle_xy = stod(line_tab[5+j*3]);
            population[ranking].geometry.active_elements[j].angle_xz = stod(line_tab[6+j*3]);
        }

        int ground_plane_element_count = stoi(line_tab[4+3*WIRE_COUNT]);
        for(int j = 0; j < ground_plane_element_count; j++){
            population[ranking].geometry.ground_plane[j].length = stod(line_tab[5+3*WIRE_COUNT+j*3]);
            population[ranking].geometry.ground_plane[j].angle_xy = stod(line_tab[6+3*WIRE_COUNT+j*3]);
            population[ranking].geometry.ground_plane[j].angle_xz = stod(line_tab[7+3*WIRE_COUNT+j*3]);
        }

        calculate_wire_geometry(&population[ranking].geometry);
        clear_antenna_geometry(&population[ranking].geometry);
    }

    fclose(file);

    pr_info("Population loaded from file... %s", file_name.c_str());
    printf("Population loaded from file... %s\n", file_name.c_str());

    return 0;
}

// -------------------------------------------------------------------------------- //

