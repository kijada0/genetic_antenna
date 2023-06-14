// ---------------------------------------- //
// created by: Kacper Kaczmarek
// created date: 28-05-2023
// ---------------------------------------- //

#include <iostream>
#include <random>
#include <ctime>
#include <chrono>
#include <thread>

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
        save_geometry_to_file(&sub_population[i].geometry, file_name);
        sub_population[i].parameters = calculate_antenna_parameters(&sub_population[i].geometry);
        sub_population[i].fitness = calculate_antenna_fitness(&sub_population[i].parameters);
    }
}

void distributes_computations_across_threads(antenna_t *population) {
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
        printf("creating thread, %d, individuals %d, index %d\n", i, number_of_individuals_for_thread, index_of_first_individual_in_current_sub_population);
        threads[i] = thread(process_antenna, number_of_individuals_for_thread, &population[index_of_first_individual_in_current_sub_population], i);
        index_of_first_individual_in_current_sub_population += number_of_individuals_for_thread;
    }

    for (i = 0; i < MAX_THREAD_COUNT; i++) {
        threads[i].join();
    }
}


// -------------------------------------------------------------------------------- //

double get_duration_in_s(high_resolution_clock::time_point start){
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    double duration_in_s = duration.count()/1000000;
    printf("Time: %.1f s\n", duration_in_s);
    return duration_in_s;
}

// -------------------------------------------------------------------------------- //

int main() {
    auto start = high_resolution_clock::now();

    static antenna_t population[POPULATION_SIZE];
    static int ranking[POPULATION_SIZE];

    create_generation_zero(&population[0], POPULATION_SIZE);

    while(get_duration_in_s(start) < 10 && cycle_count < 5){
        distributes_computations_across_threads(&population[0]);
        sort_antennas_by_fitness(&population[0], &ranking[0], POPULATION_SIZE);
        create_next_generation(&population[0], &population[0], &ranking[0], POPULATION_SIZE, POPULATION_SIZE);
        cycle_count++;
    }

    sort_antennas_by_fitness(&population[0], &ranking[0], POPULATION_SIZE);
    for(int i = 0; i < POPULATION_SIZE; i++) {
        printf("%d fitness: %d \n", i+1, population[i].fitness);
    }

    printf("----------------------------------------\n");

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    printf("Time taken by function: %.3f", (double)duration.count()/1000000);

    return 0;
}


