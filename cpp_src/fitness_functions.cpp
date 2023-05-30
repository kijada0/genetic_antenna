// ---------------------------------------- //
// created by: Kacper Kaczmarek
// created date: 29-05-2023
// ---------------------------------------- //

#include <iostream>
#include <cstdlib>
#include <math.h>
#include <fstream>

#include "c_geometry.h"
#include "nec_context.h"
#include "nec_exception.h"
#include "nec_radiation_pattern.h"

#include "fitness_functions.h"
#include "physical_calculations.h"
#include "wire_geometry.h"

using namespace std;

// -------------------------------------------------------------------------------- //

void print_antenna_parameters(antenna_parameters_t *parameters){
    cout << "Antenna parameters:" << endl;
    cout << "----------------------------------------" << endl;
    cout << "Gain:       \t" << parameters->gain << " dBi" << endl;
    cout << "Efficiency: \t" << parameters->efficiency << " %" << endl;
    cout << "Bandwidth:  \t" << parameters->bandwidth << " MHz" << endl;
    cout << "Impedance:  \t" << parameters->impedance << " Ohm" << endl;
    cout << "SWR:        \t" << parameters->SWR << endl;
    cout << "----------------------------------------" << endl;
}

// -------------------------------------------------------------------------------- //

void create_and_fill_nec_geometry(nec_context *nec, wire_vector_t *wires, double frequency){
    c_geometry *geometry = nec->get_geometry();

    for(int i = 0; i < WIRE_COUNT; i++){
        int segment_count = (int)((calculate_wire_length(&wires[i]) * WIRE_PER_WAVELENGTH)/calculate_wavelength(frequency));
        geometry->wire(0, segment_count, wires[i].start.x, wires[i].start.y, wires[i].start.z,wires[i].end.x, wires[i].end.y, wires[i].end.z, WIRE_RADIUS, 1, 1);
    }

    nec->geometry_complete(0);
}

void setup_nec_cards(nec_context *nec, double frequency){
    nec->gn_card(-1 ,0, 0, 0, 0, 0, 0, 0);
    nec->ex_card(EXCITATION_LINEAR, 1, 1, 0, 0, 0, 0, 0, 0, 0);
    nec->fr_card(0, 1, FREQ, 0);
    nec->rp_card(0, 18, 18, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0);
    //nec->xq_card(0);
}

// -------------------------------------------------------------------------------- //

antenna_parameters_t calculate_antenna_parameters(antenna_geometry_t *geometry, double frequency){
    antenna_parameters_t parameters;

    nec_context nec;
    nec.initialize();

    create_and_fill_nec_geometry(&nec, geometry->wires, frequency);
    setup_nec_cards(&nec, frequency);

    nec_radiation_pattern* rp = nec.get_radiation_pattern(0);

    parameters.gain = nec.get_gain_mean(0);
    parameters.efficiency = 0;
    parameters.bandwidth = 0;
    parameters.impedance = 0;
    parameters.SWR = 0;

    return parameters;
}


