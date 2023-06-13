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

// -------------------------------------------------------------------------------- //

void process_antenna(int individual_number, antenna_t *sub_population) {
    for(int i = 0; i < individual_number; i++) {
        sub_population[i].geometry = generate_random_antenna();
        sub_population[i].parameters = calculate_antenna_parameters(&sub_population[i].geometry);
        //print_antenna_parameters(&sub_population->parameters);
        sub_population[i].fitness = calculate_antenna_fitness(&sub_population[i].parameters);

        //string filename = "test_" + get_current_time() + "_" + to_string(rand()%1000) + ".txt";
        //save_geometry_to_file(&sub_population->geometry, "test.txt");
    }
}

double get_duration_in_s(high_resolution_clock::time_point start){
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    double duration_in_s = duration.count()/1000000;
    printf("Time: %.1f s\n", duration_in_s);
    return duration_in_s;
}


int main() {
    auto start = high_resolution_clock::now();

    thread threads[MAX_THREAD_COUNT];
    int individuals_per_thread = POPULATION_SIZE / MAX_THREAD_COUNT;
    int remaining_individuals = POPULATION_SIZE % MAX_THREAD_COUNT;
    int counter = 0;

    antenna_t population[POPULATION_SIZE];
    int index_of_first_individual_in_current_sub_population = 0;

    //while(get_duration_in_s(start) < 300) {
        remaining_individuals = POPULATION_SIZE % MAX_THREAD_COUNT;
        printf("--------------------------------------------------------------------------------\n");
        for (int i = 0; i < MAX_THREAD_COUNT; i++) {
            int number_of_individuals_for_thread = individuals_per_thread;
            if(remaining_individuals > 0) {
                number_of_individuals_for_thread++;
                remaining_individuals--;
            }
            printf("creating thread, %d, individuals %d, index %d\n", i, number_of_individuals_for_thread, index_of_first_individual_in_current_sub_population);
            threads[i] = thread(process_antenna, number_of_individuals_for_thread, &population[index_of_first_individual_in_current_sub_population]);
            index_of_first_individual_in_current_sub_population += number_of_individuals_for_thread;
        }

        for (int i = 0; i < MAX_THREAD_COUNT; i++) {
            threads[i].join();
        }
        counter++;
    //}

    int ranking[POPULATION_SIZE];
    sort_antennas_by_fitness(&population[0], &ranking[0]);

    antenna_t sub_population{};
    process_antenna(1, &sub_population);


    printf("--------------------------------------------------------------------------------\n");
    printf("Cycle count: %d\n", counter);
    printf("Avg cycle time: %.3f s\n", get_duration_in_s(start)/(float)counter);
    printf("--------------------------------------------------------------------------------\n");

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by function: " << duration.count() << " microseconds" << endl;
    cout << "Time taken by function: " << duration.count()/1000 << " ms" << endl;
    cout << "Time taken by function: " << duration.count()/1000000 << " s" << endl;

    return 0;
}


