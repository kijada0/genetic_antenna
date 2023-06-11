// ---------------------------------------- //
// created by: Kacper Kaczmarek
// created date: 28-05-2023
// ---------------------------------------- //


#include <math.h>
#include <chrono>
#include <ctime>
#include <string>
#include <iomanip>


#include "utils.h"

using namespace std;

// -------------------------------------------------------------------------------- //

double calculate_wavelength(double freq){
    return LIGHT_SPEED / freq;
}

double random_angle_in_radina(){
    return (((double)rand() / RAND_MAX) * 2 * M_PI);
}

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
