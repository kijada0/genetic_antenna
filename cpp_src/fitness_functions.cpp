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
#include "utils.h"
#include "wire_geometry.h"

using namespace std;

// -------------------------------------------------------------------------------- //

void print_antenna_parameters(antenna_parameters_t *parameters){
    printf("Antenna parameters:\n");
    printf("----------------------------------------\n");
    printf("Gain mean:       \t%.2f dBi\n", parameters->gain.mean);
    printf("Gain max:        \t%.2f dBi\n", parameters->gain.max);
    printf("Gain min:        \t%.2f dBi\n", parameters->gain.min);
    printf("--------------------\n");
    printf("Gain RHCP mean:  \t%.2f dBi\n", parameters->RHCP_gain.mean);
    printf("Gain RHCP max:   \t%.2f dBi\n", parameters->RHCP_gain.max);
    printf("Gain RHCP min:   \t%.2f dBi\n", parameters->RHCP_gain.min);
    printf("--------------------\n");
    printf("Gain LHCP mean:  \t%.2f dBi\n", parameters->LHCP_gain.mean);
    printf("Gain LHCP max:   \t%.2f dBi\n", parameters->LHCP_gain.max);
    printf("Gain LHCP min:   \t%.2f dBi\n", parameters->LHCP_gain.min);
    printf("--------------------\n");
    printf("Impedance:       \t%.2f Ohm ( %.2f + %.2fj )\n", parameters->impedance.Z, parameters->impedance.real, parameters->impedance.imag);
    printf("SWR:             \t%.2f\n", parameters->SWR.SWR);
    printf("VSWR:            \t%.2f\n", parameters->SWR.VSWR);
    printf("S11:             \t%.2f dB\n", parameters->SWR.S11);
    printf("--------------------\n");
    printf("Bandwidth:       \t%.2f MHz\n", parameters->band.bandwidth);
    printf("Band start:      \t%.2f MHz\n", parameters->band.start);
    printf("Band end:        \t%.2f MHz\n", parameters->band.end);
    printf("----------------------------------------\n");
}

// -------------------------------------------------------------------------------- //

void create_and_fill_nec_geometry(nec_context *nec, wire_vector_t *wires, double frequency){
    c_geometry *geometry = nec->get_geometry();

    for(int i = 0; i < TOTAL_WIRE_COUNT; i++){
        int segment_count = (int)((calculate_wire_length(&wires[i]) * WIRE_PER_WAVELENGTH)/calculate_wavelength(frequency));
        geometry->wire(i, segment_count, wires[i].start.x, wires[i].start.y, wires[i].start.z,wires[i].end.x, wires[i].end.y, wires[i].end.z, WIRE_RADIUS, 1, 1);
    }

    nec->geometry_complete(0);
}

void setup_nec_cards(nec_context *nec, double frequency){
    nec->gn_card(-1 ,0, 0, 0, 0, 0, 0, 0);
    nec->ex_card(EXCITATION_VOLTAGE, 0, 1, 0, 1, 0, 0, 0, 0, 0);
    nec->fr_card(0, 1, FREQ/1000000, 0);
    nec->rp_card(0, 90,90, 0, 5, 0, 0, 0, 90, 1, 1, 0, 0);
    //nec->xq_card(0);
}

// -------------------------------------------------------------------------------- //

double get_gamma(nec_context *nec, antenna_parameters_t *parameters){
    double gamma = 0;
    double zrpro2 = parameters->impedance.real + 50;
    zrpro2 *= zrpro2;

    double zrmro2 = parameters->impedance.real  - 50;
    zrmro2 *= zrmro2;

    double zimag2 = parameters->impedance.imag * parameters->impedance.imag;
    gamma = sqrt( (zrmro2 + zimag2) / (zrpro2 + zimag2) );
    return gamma;
}

// -------------------------------------------------------------------------------- //

antenna_parameters_t calculate_antenna_parameters(antenna_geometry_t *geometry, double frequency){
    antenna_parameters_t parameters;
    try {

        nec_context nec;
        nec.initialize();

        create_and_fill_nec_geometry(&nec, geometry->geometry, frequency);
        setup_nec_cards(&nec, frequency);

        //nec_radiation_pattern *rp = nec.get_radiation_pattern(0);

        parameters.gain.mean = nec.get_gain_mean(0);
        parameters.gain.max = nec.get_gain_max(0);
        parameters.gain.min = nec.get_gain_min(0);

        parameters.impedance.imag = nec.get_impedance_imag(0);
        parameters.impedance.real = nec.get_impedance_real(0);
        parameters.impedance.Z = sqrt(pow(parameters.impedance.imag, 2) + pow(parameters.impedance.real, 2));

        double gamma = get_gamma(&nec, &parameters);
        parameters.SWR.SWR = parameters.impedance.Z/50;
        parameters.SWR.VSWR = (1 + gamma) / (1 - gamma);
        parameters.SWR.S11 = 20 * log10(gamma);

    }
    catch (nec_exception* e) {
        cout << e->get_message() << endl;
    }

    return parameters;
}


