// ---------------------------------------- //
// created by: Kacper Kaczmarek
// created date: 28-05-2023
// ---------------------------------------- //

#include <iostream>
#include <random>
#include <ctime>
#include <chrono>
#include <thread>

#include "print.h"

#include "wire_geometry.h"
#include "fitness.h"
#include "evolution.h"

using namespace std;
using namespace std::chrono;

int cycle_count = 0;

// -------------------------------------------------------------------------------- //

void process_antenna(int individual_number, antenna_t *sub_population, int process_id) {
    for(int i = 0; i < individual_number; i++) {
        string file_name = "out/geometry_" + to_string(i) + "_" + to_string(cycle_count) + + "_" + to_string(process_id) + ".txt";
        //print_antenna_wires_parameters(&sub_population[i].geometry);
        //print_antenna_geometry(&sub_population[i].geometry);
        sub_population[i].parameters = calculate_antenna_parameters(&sub_population[i].geometry);
        //print_antenna_parameters(&sub_population[i].parameters);
        sub_population[i].fitness = calculate_antenna_fitness(&sub_population[i].parameters);
    }
}

void distributes_computations_across_threads(antenna_t *population) {
    pr_info("Distributing computations across threads...");
    thread threads[MAX_THREAD_COUNT];
    int individuals_per_thread = POPULATION_SIZE / MAX_THREAD_COUNT;
    int remaining_individuals = POPULATION_SIZE % MAX_THREAD_COUNT;

    int index_of_first_individual_in_current_sub_population = 0;
    int i;

    remaining_individuals = POPULATION_SIZE % MAX_THREAD_COUNT;
    for (i = 0; i < MAX_THREAD_COUNT; i++) {
        int number_of_individuals_for_thread = individuals_per_thread;
        if(remaining_individuals > 0) {
            number_of_individuals_for_thread++;
            remaining_individuals--;
        }
        pr_debug("creating thread, %d, individuals %d, index %d", i, number_of_individuals_for_thread, index_of_first_individual_in_current_sub_population);
        threads[i] = thread(process_antenna, number_of_individuals_for_thread, &population[index_of_first_individual_in_current_sub_population], i);
        index_of_first_individual_in_current_sub_population += number_of_individuals_for_thread;
    }

    pr_info("Waiting for threads to finish...");

    for (i = 0; i < MAX_THREAD_COUNT; i++) {
        threads[i].join();
    }
}

// -------------------------------------------------------------------------------- //

void save_what_is_to_be_saved(antenna_t *population, int *ranking, int size, int cycle_number, double population_time) {
    pr_debug("Saving what is to be saved...");
    save_telemetry(population, ranking, size, cycle_number, TELEMETRY_RANGE, population_time);
    if(cycle_number % SAVE_POPULATION_PERIOD == 0){
        save_sorted_population_to_file(&population[0], &ranking[0], size);
    }
    pr_info("Cycle %d calculation time: %.3f", cycle_number, population_time);
}

// -------------------------------------------------------------------------------- //

double get_duration_in_s(std::chrono::high_resolution_clock::time_point start) {
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    double duration_in_s = duration.count() / 1'000'000.0;
    pr_debug("Czas: %.3f s", duration_in_s);
    return duration_in_s;
}



// -------------------------------------------------------------------------------- //

int main() {
    auto start = high_resolution_clock::now();

    static antenna_t population[POPULATION_SIZE];
    static int ranking[POPULATION_SIZE];

    printf("Starting evolution...\n");
    pr_always("------------------------------------------------------------");
    pr_always("Starting evolution...");
    pr_always("------------------------------------------------------------");

    if(LOAD_POPULATION_FROM_FILE == false){
        pr_info("Generation generated randomly.");
        printf("Generation generated randomly.\n");
        create_generation_zero(&population[0], POPULATION_SIZE);
    }
    else{
        if(load_population_from_file(&population[0], POPULATION_SIZE) != 0){
            pr_error("Failed to load population from file.");
            printf("Failed to load population from file.\n");
            create_generation_zero(&population[0], POPULATION_SIZE);
            pr_info("Generation generated randomly.");
            printf("Generation generated randomly.\n");
        }
        else{
            pr_info("Generation loaded from file.");
            printf("Generation loaded from file.\n");
        }
    }

    while(get_duration_in_s(start) < (24*60*60) && cycle_count < 25000){
        auto time0 = high_resolution_clock::now();
        distributes_computations_across_threads(&population[0]);
        sort_antennas_by_fitness(&population[0], &ranking[0], POPULATION_SIZE);
        save_sorted_population_to_file(&population[0], &ranking[0], POPULATION_SIZE);
        save_what_is_to_be_saved(&population[0], &ranking[0], POPULATION_SIZE, cycle_count, get_duration_in_s(time0));
        create_next_generation(&population[0], &population[0], &ranking[0], POPULATION_SIZE, PARENT_COUNT);
        cycle_count++;
    }

    printf("----------------------------------------\n");
    printf("Evolution finished.\n");
    printf("----------------------------------------\n");
    printf("Generation count: %d\n", cycle_count);

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    printf("Time taken by function: %.3f\n", (double)duration.count()/1000000);

    return 0;
}


