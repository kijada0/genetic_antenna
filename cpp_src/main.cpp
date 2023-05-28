// ---------------------------------------- //
// created by: Kacper Kaczmarek
// created date: 28-05-2023
// ---------------------------------------- //

#include <iostream>
#include <random>
#include <ctime>


#include "c_geometry.h"
#include "nec_context.h"
#include "nec_exception.h"
#include "nec_radiation_pattern.h"

#include "physical_calculations.h"
#include "wire_geometry.h"

using namespace std;

// -------------------------------------------------------------------------------- //

int main() {
    srand((unsigned) time(NULL));

    cout << "Genetic Antenna generator start!" << endl;

    antenna_geometry_t antenna = generate_random_antenna();
    save_geometry_to_file(&antenna, "geometry_test.txt");

    return 0;
}
