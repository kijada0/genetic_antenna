// ---------------------------------------- //
// created by: Kacper Kaczmarek
// created date: 28-05-2023
// ---------------------------------------- //


#include <cmath>
#include <chrono>
#include <ctime>
#include <string>
#include <iomanip>
#include <random>

#include "utils.h"

using namespace std;

// -------------------------------------------------------------------------------- //

double calculate_wavelength(double freq){
    return LIGHT_SPEED / freq;
}

// -------------------------------------------------------------------------------- //

double random_angle_in_radina(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0.0, 2.0 * M_PI);

    return dist(gen);
}

// -------------------------------------------------------------------------------- //

string get_current_time(){
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

    auto duration = now.time_since_epoch();
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration) % 1000;

    std::stringstream ss;
    ss << std::put_time(std::localtime(&currentTime), "%y%m%d_%H%M%S") << std::setfill('0') << std::setw(3) << milliseconds.count();
    std::string timeString = ss.str();

    return timeString;
}

// -------------------------------------------------------------------------------- //

int random_int_in_range(int min, int max){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(min, max);

    return dist(gen);
}

double random_double_in_range(double min, double max){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(min, max);

    return dist(gen);
}

void random_int_pair_without_repetition(int *a, int *b, int min, int max){
    *a = random_int_in_range(min, max);
    *b = random_int_in_range(min, max);

    while(*a == *b){
        *b = random_int_in_range(min, max);
    }
}



// -------------------------------------------------------------------------------- //
