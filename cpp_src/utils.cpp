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
#include <thread>
#include <filesystem>

#include "utils.h"

#include "print.h"

using namespace std;

// -------------------------------------------------------------------------------- //

double calculate_wavelength(double freq){
    return LIGHT_SPEED / freq;
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

long long get_current_timestamp() {
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::seconds>(duration).count();
}

void delay(int milliseconds){
    this_thread::sleep_for(chrono::milliseconds(milliseconds));
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

double random_normal_double_in_range(double min, double max){
    double mean = (max + min) / 2;
    double std = (max - mean)/2 * sqrt(2)/2;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> dist(mean, std);

    int i = 0;
    while(i < 100){
        double val = dist(gen);
        if(val >= min && val <= max){
            return val;
        }
        i++;
    }
    return dist(gen);
}

double random_half_normal_double_in_range(double min, double max){
    double min0 = min - (max - min);
    double mean = (max + min0) / 2;
    double std = (max - mean)/2 * sqrt(2)/2;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> dist(mean, std);

    int i = 0;
    while(i < 100){
        double val = dist(gen);
        if(val >= min && val <= max){
            return val;
        }
        i++;
    }
    return dist(gen);
}


double random_lognormal_double_in_range(double min, double max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::lognormal_distribution<double> dist(1, 1);

    int i = 0;
    while (i < 100) {
        double val = dist(gen);
        if (val >= 0 && val <= 25) {
            double scaled_val = val / 25;
            double mapped_val = scaled_val * (max - min) + min;
            return mapped_val;
        }
        i++;
    }
    return dist(gen);
}

double random_chi_squared_double_in_range(double min, double max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::chi_squared_distribution<double> dist(10);

    int i = 0;
    while (i < 100) {
        double val = dist(gen);
        if (val >= 0 && val <= 25) {
            double scaled_val = val / 25;
            double mapped_val = scaled_val * (max - min) + min;
            return mapped_val;
        }
        i++;
    }
    return dist(gen);
}


void random_int_pair_without_repetition(int *a, int *b, int min, int max){
    pr_debug("Random int pair without repetition...");
    *a = random_int_in_range(min, max);
    *b = random_int_in_range(min, max);

    while(*a == *b){
        *b = random_int_in_range(min, max);
    }
}

void random_int_pair_without_repetition_nonlinear_distribution(int *a, int *b, int min, int max){
    pr_debug("Random int pair without repetition non linear distribution...");
    int a0 = (int)random_half_normal_double_in_range(min, max);
    int b0 = (int)random_chi_squared_double_in_range(min, max);
    pr_debug("a0: %d, b0: %d", a0, b0);

    while(a0 == b0){
        b0 = (int)random_chi_squared_double_in_range(min, max);
    }

    *a = a0;
    *b = b0;
}

double random_angle_in_radina(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0.0, 2.0 * M_PI);

    return dist(gen);
}

// -------------------------------------------------------------------------------- //

void create_folder_if_not_exist(const string &folderPath) {
    if (!filesystem::exists(folderPath)){
        if (std::filesystem::create_directory(folderPath)) {
            pr_info("Utworzono folder: %s", folderPath.c_str());
        }
        else{
            pr_error("Nie udało się utworzyć folderu: %s", folderPath.c_str());
        }
    }
    else{
        pr_debug("Folder %s już istnieje", folderPath.c_str());
    }
}

// -------------------------------------------------------------------------------- //


