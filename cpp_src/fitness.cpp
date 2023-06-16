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

#include "print.h"

#include "fitness.h"
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
    printf("Bandwidth:       \t%d MHz\n", parameters->band.bandwidth/1000000);
    printf("Band start:      \t%d MHz\n", parameters->band.start/1000000);
    printf("Band end:        \t%d MHz\n", parameters->band.end/1000000);
    printf("----------------------------------------\n");
}

// -------------------------------------------------------------------------------- //

void create_and_fill_nec_geometry(nec_context *nec, wire_vector_t *wires, double frequency){
    c_geometry *geometry = nec->get_geometry();

    for(int i = 0; i < TOTAL_WIRE_COUNT; i++){
        int segment_count = (int)((calculate_wire_length(&wires[i]) * WIRE_PER_WAVELENGTH)/calculate_wavelength(frequency)) + 1;
        if(segment_count < 1) segment_count = 1;
        pr_junk("wire len: %f \t segments: %d", calculate_wire_length(&wires[i]), segment_count);
        geometry->wire(i, segment_count, wires[i].start.x, wires[i].start.y, wires[i].start.z,wires[i].end.x, wires[i].end.y, wires[i].end.z, WIRE_RADIUS, 1, 1);
    }

    nec->geometry_complete(0);
}

void setup_nec_cards(nec_context *nec, int freq){
    pr_debug("Setting up NEC cards...");
    nec->gn_card(-1 ,0, 0, 0, 0, 0, 0, 0);
    nec->ex_card(EXCITATION_VOLTAGE, 0, 1, 0, 1, 0, 0, 0, 0, 0);
    nec->fr_card(0, 1, freq/1000000, 1);
    nec->rp_card(0, MES_GAIN_STEP_COUNT,MES_GAIN_STEP_COUNT, 0, 5, 0, 0, 0, 0, 360/MES_GAIN_STEP_COUNT, 360/MES_GAIN_STEP_COUNT, 0, 0);
}

void setup_nec_cards_for_sweep(nec_context *nec, int middle_frequency, int frequency_step, int step_count){
    pr_debug("Setting up NEC cards for sweep ...");
    double start_frequency = (double)middle_frequency - ((double)step_count/2 * frequency_step);

    nec->gn_card(-1 ,0, 0, 0, 0, 0, 0, 0);
    nec->ex_card(EXCITATION_VOLTAGE, 0, 1, 0, 1, 0, 0, 0, 0, 0);
    nec->fr_card(0, step_count, start_frequency/1000000, (double)frequency_step/1000000);
    nec->rp_card(0, 1,1, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0);
}

// -------------------------------------------------------------------------------- //

double get_gamma(antenna_parameters_t *parameters){
    double gamma = 0;
    double zrpro2 = parameters->impedance.real + 50;
    zrpro2 *= zrpro2;

    double zrmro2 = parameters->impedance.real  - 50;
    zrmro2 *= zrmro2;

    double zimag2 = parameters->impedance.imag * parameters->impedance.imag;
    gamma = sqrt( (zrmro2 + zimag2) / (zrpro2 + zimag2) );
    return gamma;
}

double get_gamma_from_sweep(antenna_parameters_freq_sweep_t *parameters, int index){
    double gamma = 0;
    double zrpro2 = parameters->impedance[index].real + 50;
    zrpro2 *= zrpro2;

    double zrmro2 = parameters->impedance[index].real  - 50;
    zrmro2 *= zrmro2;

    double zimag2 = parameters->impedance[index].imag * parameters->impedance[index].imag;
    gamma = sqrt( (zrmro2 + zimag2) / (zrpro2 + zimag2) );
    return gamma;
}


// -------------------------------------------------------------------------------- //

void get_parameters_from_nec(nec_context *nec, antenna_parameters_t *parameters){
    pr_trash("Getting parameters for middle frequency from nec");
    parameters->gain.mean = nec->get_gain_mean(0);
    parameters->gain.max = nec->get_gain_max(0);
    parameters->gain.min = nec->get_gain_min(0);

    parameters->RHCP_gain.mean = nec->get_gain_rhcp_mean(0);
    parameters->RHCP_gain.max = nec->get_gain_rhcp_max(0);
    parameters->RHCP_gain.min = nec->get_gain_rhcp_min(0);

    parameters->LHCP_gain.mean = nec->get_gain_lhcp_mean(0);
    parameters->LHCP_gain.max = nec->get_gain_lhcp_max(0);
    parameters->LHCP_gain.min = nec->get_gain_lhcp_min(0);

    parameters->impedance.imag = nec->get_impedance_imag(0);
    parameters->impedance.real = nec->get_impedance_real(0);
    parameters->impedance.Z = sqrt(pow(parameters->impedance.imag, 2) + pow(parameters->impedance.real, 2));

    double gamma = get_gamma(parameters);
    parameters->SWR.SWR = parameters->impedance.Z/50;
    parameters->SWR.VSWR = (1 + gamma) / (1 - gamma);
    parameters->SWR.S11 = 20 * log10(gamma);
}

void get_parameters_of_frequency_sweep_from_nec(nec_context *nec, antenna_parameters_freq_sweep_t *parameters){
    pr_trash("Getting parameters of frequency sweep from nec");
    for(int i=0; i<MES_STEP_COUNT; i++) {
        parameters->impedance[i].imag = nec->get_impedance_imag(i);
        parameters->impedance[i].real = nec->get_impedance_real(i);
        parameters->impedance[i].Z = sqrt(pow(parameters->impedance[i].imag, 2) + pow(parameters->impedance[i].real, 2));

        double gamma = get_gamma_from_sweep(parameters, i);
        parameters->SWR[i].SWR = parameters->impedance[i].Z / 50;
        parameters->SWR[i].VSWR = (1 + gamma) / (1 - gamma);
        parameters->SWR[i].S11 = 20 * log10(gamma);
    }
}

void get_bandwidth_from_frequency_sweep(antenna_parameters_t *parameters, antenna_parameters_freq_sweep_t *freq_sweep_parameters){
    pr_trash("Get bandwidth from frequency sweep... ");
    int i, start_freq, current_freq;

    start_freq = FREQ - (MES_STEP_COUNT/2 * MES_FREQ_STEP);

    for(i=MES_STEP_COUNT/2; i>=0; i--){
        current_freq = start_freq + (i * MES_FREQ_STEP);
        if(freq_sweep_parameters->SWR[i].VSWR < MAX_SWR){
            parameters->band.start = current_freq;
        }
        else{
            break;
        }
    }

    for(i=MES_STEP_COUNT/2; i<MES_STEP_COUNT; i++){
        current_freq = start_freq + (i * MES_FREQ_STEP);
        if(freq_sweep_parameters->SWR[i].VSWR < MAX_SWR){
            parameters->band.end = current_freq;
        }
        else{
            break;
        }
    }

    parameters->band.bandwidth = parameters->band.end - parameters->band.start;
}

// -------------------------------------------------------------------------------- //

antenna_parameters_t calculate_antenna_parameters(antenna_geometry_t *geometry){
    pr_trash("Calculating antenna parameters...");
    antenna_parameters_t parameters{};
    antenna_parameters_freq_sweep_t freq_sweep_parameters{};

    try {
        nec_context nec;
        nec.initialize();

        create_and_fill_nec_geometry(&nec, geometry->geometry, FREQ);
        nec.calc_prepare();

        setup_nec_cards(&nec, FREQ);
        get_parameters_from_nec(&nec, &parameters);

        setup_nec_cards_for_sweep(&nec, FREQ, MES_FREQ_STEP, MES_STEP_COUNT);
        get_parameters_of_frequency_sweep_from_nec(&nec, &freq_sweep_parameters);
        get_bandwidth_from_frequency_sweep(&parameters, &freq_sweep_parameters);

        parameters.error_flag = 0;
    }
    catch (nec_exception* e) {
        //cout << e->get_message() << endl;
        pr_debug("Error in NEC calculation!\n");
        parameters.error_flag = 1;
    }

    return parameters;
}

// -------------------------------------------------------------------------------- //

int calculate_antenna_fitness(antenna_parameters_t *parameters){
    pr_debug("Calculating antenna fitness...");
    int fitness = 0;
    if(parameters->error_flag == 1){
        fitness = -999999999;
        return fitness;
    }

    // Gain
    int gain_fitness = parameters->gain.max * GAIN_WEIGHT;
    int gain_RHCP_fitness = parameters->RHCP_gain.max * GAIN_RHCP_WEIGHT;
    int gain_LHCP_fitness = parameters->LHCP_gain.max * GAIN_LHCP_WEIGHT;
    pr_junk("Gain: %d \nRHCP %d \nLHCP %d \n", gain_fitness, gain_RHCP_fitness, gain_LHCP_fitness);

    // SWR
    int VSWR_fitness = VSWR_WEIGHT - (abs(1 - parameters->SWR.VSWR) * VSWR_WEIGHT);
    pr_junk("VSWR: %d\n", VSWR_fitness);
    int S11_fitness = -1 * parameters->SWR.S11 * S11_WEIGHT;
    pr_junk("S11: %d\n", S11_fitness);

    // Bandwidth
    int bandwidth_fitness = parameters->band.bandwidth/1000000 * BANDWIDTH_WEIGHT;
    pr_junk("Bandwidth: %d\n", bandwidth_fitness);

    fitness = gain_fitness + gain_RHCP_fitness + gain_LHCP_fitness + VSWR_fitness + S11_fitness + bandwidth_fitness;
    pr_trash("Fitness: %d\n", fitness);

    return fitness;
}

// -------------------------------------------------------------------------------- //

void sort_antennas_by_fitness(antenna_t *population, int *ranking, int population_size){
    pr_debug("Sort antennas by fitness...");
    int i, j, temp;
    for(i = 0; i < population_size; i++){
        ranking[i] = i;
    }

    for(i = 0; i < population_size; i++){
        for(j = i+1; j < population_size; j++){
            if(population[ranking[i]].fitness < population[ranking[j]].fitness){
                temp = ranking[i];
                ranking[i] = ranking[j];
                ranking[j] = temp;
            }
        }
    }

    for(i = 0; i < population_size; i++){
        pr_junk("%d. antenna: %d  \t fitness: %d\n", i, ranking[i], population[ranking[i]].fitness);
    }
}

// -------------------------------------------------------------------------------- //

void clear_antenna_parameters(antenna_parameters_t *parameters){
    parameters->gain.max = 0;
    parameters->gain.min = 0;
    parameters->gain.mean = 0;

    parameters->RHCP_gain.max = 0;
    parameters->RHCP_gain.min = 0;
    parameters->RHCP_gain.mean = 0;

    parameters->LHCP_gain.max = 0;
    parameters->LHCP_gain.min = 0;
    parameters->LHCP_gain.mean = 0;

    parameters->impedance.real = 0;
    parameters->impedance.imag = 0;
    parameters->impedance.Z = 0;

    parameters->SWR.SWR = 0;
    parameters->SWR.VSWR = 0;
    parameters->SWR.S11 = 0;

    parameters->band.start = 0;
    parameters->band.end = 0;
    parameters->band.bandwidth = 0;

    parameters->error_flag = 0;
}

// -------------------------------------------------------------------------------- //
