// ---------------------------------------- //
// created by: Kacper Kaczmarek
// created date: 28-05-2023
// ---------------------------------------- //

#include <iostream>
#include <cstdlib>
#include <math.h>
#include <fstream>

#include "wire_geometry.h"
#include "physical_calculations.h"

using namespace std;

// -------------------------------------------------------------------------------- //

void init_antenna_geometry(antenna_geometry_t *geometry){
    for(int i = 0; i < WIRE_COUNT; i++){
        geometry->wires[i].start.x = 0.0;
        geometry->wires[i].start.y = 0.0;
        geometry->wires[i].start.z = 0.0;

        geometry->wires[i].end.x = 0.0;
        geometry->wires[i].end.y = 0.0;
        geometry->wires[i].end.z = 0.0;

        geometry->geometry[i].length = 0.0;
        geometry->geometry[i].angle_xy = 0.0;
        geometry->geometry[i].angle_xz = 0.0;
    }
}

// -------------------------------------------------------------------------------- //

void print_antenna_geometry(antenna_geometry_t *geometry){
    cout << "\nAntenna geometry: " << endl;
    for(int i = 0; i < WIRE_COUNT; i++){
        cout << "----------------------------------------" << endl;
        cout << "Wire " << i+1 << " start: " << geometry->wires[i].start.x << " " << geometry->wires[i].start.y << " " << geometry->wires[i].start.z << endl;
        cout << "Wire " << i+1 << " end: " << geometry->wires[i].end.x << " " << geometry->wires[i].end.y << " " << geometry->wires[i].end.z << endl;
        cout << "Wire " << i+1 << " length: " << geometry->geometry[i].length << endl;
        cout << "Wire " << i+1 << " angle_xy: " << geometry->geometry[i].angle_xy << endl;
        cout << "Wire " << i+1 << " angle_xz: " << geometry->geometry[i].angle_xz << endl;
    }
    cout << "----------------------------------------\n" << endl;
}

void print_antenna_wires(antenna_geometry_t *geometry){
    cout << "\nAntenna wires: " << endl;
    cout << "----------------------------------------" << endl;
    for(int i = 0; i < WIRE_COUNT; i++){
        printf("wire %d \t->  start -> x: %.3f y: %.3f  z: %.3f \t", i+1, geometry->wires[i].start.x, geometry->wires[i].start.y, geometry->wires[i].start.z);
        printf("end -> x: %.3f  y: %.3f  z: %.3f\n", geometry->wires[i].end.x, geometry->wires[i].end.y, geometry->wires[i].end.z);
    }
    cout << "----------------------------------------\n" << endl;
}

// -------------------------------------------------------------------------------- //

void calculate_wire_vectors(antenna_geometry_t *geometry){
    for(int i = 0; i < WIRE_COUNT; i++){
        geometry->wires[i].end.x = geometry->wires[i].start.x + geometry->geometry[i].length * cos(geometry->geometry[i].angle_xy) * cos(geometry->geometry[i].angle_xz);
        geometry->wires[i].end.y = geometry->wires[i].start.y + geometry->geometry[i].length * sin(geometry->geometry[i].angle_xy) * cos(geometry->geometry[i].angle_xz);
        geometry->wires[i].end.z = geometry->wires[i].start.z + geometry->geometry[i].length * sin(geometry->geometry[i].angle_xz);

        if(i < WIRE_COUNT-1){
            geometry->wires[i+1].start.x = geometry->wires[i].end.x;
            geometry->wires[i+1].start.y = geometry->wires[i].end.y;
            geometry->wires[i+1].start.z = geometry->wires[i].end.z;
        }

    }
}


// -------------------------------------------------------------------------------- //

bool check_if_geometry_fits_in_cube(antenna_geometry_t *geometry, double cube_edge_length){
    antenna_geometry_t temp_geometry;
    memcpy(&temp_geometry, geometry, sizeof(antenna_geometry_t));

    calculate_wire_vectors(&temp_geometry);

    for(int i = 0; i < WIRE_COUNT; i++){
        if(temp_geometry.wires[i].end.x > cube_edge_length/2 || temp_geometry.wires[i].end.x < -cube_edge_length/2){
            return false;
        }
        if(temp_geometry.wires[i].end.y > cube_edge_length/2 || temp_geometry.wires[i].end.y < -cube_edge_length/2){
            return false;
        }
        if(temp_geometry.wires[i].end.z > cube_edge_length || temp_geometry.wires[i].end.z < 0){
            return false;
        }
    }

    return true;
}

// -------------------------------------------------------------------------------- //

void divide_length_into_random_length_segment(double length, int element, double *target){
    rand();
    double length_sum = 0.0;
    double offset = 0.75;
    double min_length = (length/element) * (1 - offset);
    double max_length = (length/element) * (1 + offset);

    // cout << "Dividing length: " << length << " into " << element << " elements" << endl;

    for(int i = 0; i < element-1; i++){
        target[i] =  min_length + ((double)rand() / RAND_MAX) * (max_length - min_length);
        length_sum += target[i];
    }
    target[element-1] = length - length_sum;

    // length_sum += target[element-1];
    // cout << "Length sum: " << length_sum << endl;
}


void generate_random_antenna_wires(antenna_geometry_t *geometry){
    double segments_lengths[WIRE_COUNT];
    double wavelength;

    cout << "Generating random geometry" << endl;

    wavelength = calculate_wavelength(FREQ);
    divide_length_into_random_length_segment(wavelength/2, WIRE_COUNT, segments_lengths);

    for(int i = 0; i < WIRE_COUNT; i++){
        geometry->geometry[i].length = segments_lengths[i];
        do {
            geometry->geometry[i].angle_xy = random_angle_in_radina();
            geometry->geometry[i].angle_xz = random_angle_in_radina();
        } while(!check_if_geometry_fits_in_cube(geometry, wavelength*CUBE_EDGE_LENGTH_RELATIVE_TO_WAVELENGTH));
    }
}

void generate_test_dipol(antenna_geometry_t *geometry){
    double wavelength = calculate_wavelength(FREQ)*0.95;
    double segments_lengths = (wavelength/2)/WIRE_COUNT;

    for(int i = 0; i < WIRE_COUNT; i++){
        geometry->geometry[i].length = segments_lengths;
        geometry->geometry[i].angle_xy = 0;
        geometry->geometry[i].angle_xz = M_PI/2;
    }
}

// -------------------------------------------------------------------------------- //


antenna_geometry_t generate_random_antenna(){
    antenna_geometry_t random_antenna{};

    init_antenna_geometry(&random_antenna);
    generate_test_dipol(&random_antenna);
    //generate_random_antenna_wires(&random_antenna);
    calculate_wire_vectors(&random_antenna);
    print_antenna_wires(&random_antenna);

    return random_antenna;
}

// -------------------------------------------------------------------------------- //

void save_geometry_to_file(antenna_geometry_t *geometry, const char *file_name){
    cout << "Saving geometry to file: " << file_name << "\n" << endl;

    ofstream file;
    file.open(file_name);

    for(int i = 0; i < WIRE_COUNT; i++){
        file << geometry->wires[i].start.x << "; " << geometry->wires[i].start.y << "; " << geometry->wires[i].start.z << "; ";
        file << geometry->wires[i].end.x << "; " << geometry->wires[i].end.y << "; " << geometry->wires[i].end.z << "; " << endl;
    }

}

// -------------------------------------------------------------------------------- //

double calculate_wire_length(wire_vector_t *wire){
    return sqrt(pow(wire->end.x - wire->start.x, 2) + pow(wire->end.y - wire->start.y, 2) + pow(wire->end.z - wire->start.z, 2));
}

// -------------------------------------------------------------------------------- //