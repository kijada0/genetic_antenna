// ---------------------------------------- //
// created by: Kacper Kaczmarek
// created date: 28-05-2023
// ---------------------------------------- //

#ifndef GENETIC_ANTENNA_UTILS_H
#define GENETIC_ANTENNA_UTILS_H

#include <string>

#define LIGHT_SPEED 299792458

double calculate_wavelength(double freq);

std::string get_current_time();
long long get_current_timestamp();
void delay(int milliseconds);

int random_int_in_range(int min, int max);
double random_double_in_range(double min, double max);
void random_int_pair_without_repetition(int *a, int *b, int min, int max);
double random_angle_in_radina();


void create_folder_if_not_exist(const std::string &folderPath);

#endif //GENETIC_ANTENNA_UTILS_H
