// ---------------------------------------- //
// created by: Kacper Kaczmarek
// created date: 28-05-2023
// ---------------------------------------- //


#include <math.h>

#include "physical_calculations.h"

// -------------------------------------------------------------------------------- //

double calculate_wavelength(double freq){
    return LIGHT_SPEED / freq;
}

double random_angle_in_radina(){
    return (((double)rand() / RAND_MAX) * 2 * M_PI);
}
