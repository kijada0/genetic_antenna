// ---------------------------------------- //
// created by: Kacper Kaczmarek
// created date: 28-05-2023
// ---------------------------------------- //

#include <iostream>
#include <cstdlib>
#include <math.h>
#include <fstream>
#include <cstring>
#include <iomanip>

#include "wire_geometry.h"
#include "utils.h"

using namespace std;

// -------------------------------------------------------------------------------- //

void init_antenna_geometry(antenna_geometry_t *geometry){
    for(int i = 0; i < WIRE_COUNT; i++){
        geometry->geometry[i].start.x = 0.0;
        geometry->geometry[i].start.y = 0.0;
        geometry->geometry[i].start.z = 0.0;

        geometry->geometry[i].end.x = 0.0;
        geometry->geometry[i].end.y = 0.0;
        geometry->geometry[i].end.z = 0.0;

        geometry->active_elements[i].length = 0.0;
        geometry->active_elements[i].angle_xy = 0.0;
        geometry->active_elements[i].angle_xz = 0.0;
    }
}

// -------------------------------------------------------------------------------- //

void print_antenna_geometry(antenna_geometry_t *geometry){
    cout << "\nAntenna active_elements: " << endl;
    for(int i = 0; i < TOTAL_WIRE_COUNT; i++){
        cout << "----------------------------------------" << endl;
        cout << "Wire " << i+1 << " start: " << geometry->geometry[i].start.x << " " << geometry->geometry[i].start.y << " " << geometry->geometry[i].start.z << endl;
        cout << "Wire " << i+1 << " end: " << geometry->geometry[i].end.x << " " << geometry->geometry[i].end.y << " " << geometry->geometry[i].end.z << endl;
        cout << "Wire " << i+1 << " length: " << geometry->active_elements[i].length << endl;
        cout << "Wire " << i+1 << " angle_xy: " << geometry->active_elements[i].angle_xy << endl;
        cout << "Wire " << i+1 << " angle_xz: " << geometry->active_elements[i].angle_xz << endl;
    }
    cout << "----------------------------------------\n" << endl;
}

void print_antenna_wires(antenna_geometry_t *geometry){
    cout << "\nAntenna geometry: " << endl;
    cout << "----------------------------------------" << endl;
    for(int i = 0; i < TOTAL_WIRE_COUNT; i++){
        printf("wire %d \t->  start -> x: %.3f y: %.3f  z: %.3f \t", i+1, geometry->geometry[i].start.x, geometry->geometry[i].start.y, geometry->geometry[i].start.z);
        printf("end -> x: %.3f  y: %.3f  z: %.3f\n", geometry->geometry[i].end.x, geometry->geometry[i].end.y, geometry->geometry[i].end.z);
    }
    cout << "----------------------------------------\n" << endl;
}

void print_antenna_wires_parameters(antenna_geometry_t *geometry){
    int i, j;
    for(i = 0; i < WIRE_COUNT; i++){
        printf("wire a  %d \t->  length: %.3f \t", i+1, geometry->active_elements[i].length);
        printf("angle_xy: %.3f \t", geometry->active_elements[i].angle_xy);
        printf("angle_xz: %.3f\n", geometry->active_elements[i].angle_xz);
    }
    for(j = 0; j < GROUND_PLANE_ELEMENT_COUNT; j++){
        printf("wire gp %d \t->  length: %.3f \t", i+j+1, geometry->ground_plane[j].length);
        printf("angle_xy: %.3f \t", geometry->ground_plane[j].angle_xy);
        printf("angle_xz: %.3f\n", geometry->ground_plane[j].angle_xz);
    }
}

// -------------------------------------------------------------------------------- //

void calculate_wire_geometry_of_active_elements(antenna_geometry_t *geometry){
    for(int i = 0; i < WIRE_COUNT; i++){
        geometry->geometry[i].end.x = geometry->geometry[i].start.x + geometry->active_elements[i].length * cos(geometry->active_elements[i].angle_xy) * cos(geometry->active_elements[i].angle_xz);
        geometry->geometry[i].end.y = geometry->geometry[i].start.y + geometry->active_elements[i].length * sin(geometry->active_elements[i].angle_xy) * cos(geometry->active_elements[i].angle_xz);
        geometry->geometry[i].end.z = geometry->geometry[i].start.z + geometry->active_elements[i].length * sin(geometry->active_elements[i].angle_xz);

        if(i < WIRE_COUNT-1){
            geometry->geometry[i + 1].start.x = geometry->geometry[i].end.x;
            geometry->geometry[i + 1].start.y = geometry->geometry[i].end.y;
            geometry->geometry[i + 1].start.z = geometry->geometry[i].end.z;
        }
    }
}

void calculate_wire_geometry_of_ground_plane(antenna_geometry_t *geometry){
    for(int i = 0; i < GROUND_PLANE_ELEMENT_COUNT; i++){
        geometry->geometry[i + WIRE_COUNT].start.x = 0;
        geometry->geometry[i + WIRE_COUNT].start.y = 0;
        geometry->geometry[i + WIRE_COUNT].start.z = 0;

        geometry->geometry[i + WIRE_COUNT].end.x = geometry->ground_plane[i].length * cos(geometry->ground_plane[i].angle_xy) * cos(geometry->ground_plane[i].angle_xz);
        geometry->geometry[i + WIRE_COUNT].end.y = geometry->ground_plane[i].length * sin(geometry->ground_plane[i].angle_xy) * cos(geometry->ground_plane[i].angle_xz);
        geometry->geometry[i + WIRE_COUNT].end.z = geometry->ground_plane[i].length * sin(geometry->ground_plane[i].angle_xz);
    }
}

void calculate_wire_geometry(antenna_geometry_t *geometry){
    calculate_wire_geometry_of_active_elements(geometry);
    calculate_wire_geometry_of_ground_plane(geometry);
}

// -------------------------------------------------------------------------------- //

bool check_if_geometry_fits_in_cube(antenna_geometry_t *geometry, double cube_edge_length){
    antenna_geometry_t temp_geometry{};
    memcpy(&temp_geometry, geometry, sizeof(antenna_geometry_t));

    calculate_wire_geometry(&temp_geometry);

    for(int i = 0; i < WIRE_COUNT; i++){
        if(temp_geometry.geometry[i].end.x > cube_edge_length / 2 || temp_geometry.geometry[i].end.x < -cube_edge_length / 2){
            return false;
        }
        if(temp_geometry.geometry[i].end.y > cube_edge_length / 2 || temp_geometry.geometry[i].end.y < -cube_edge_length / 2){
            return false;
        }
        if(temp_geometry.geometry[i].end.z > cube_edge_length || temp_geometry.geometry[i].end.z < 0){
            return false;
        }
    }

    return true;
}

// -------------------------------------------------------------------------------- //

void divide_length_into_random_length_segment(double length, int element, double *target){
    double length_sum = 0.0;
    double offset = 0.75;
    double min_length = (length/element) * (1 - offset);
    double max_length = (length/element) * (1 + offset);

    // cout << "Dividing length: " << length << " into " << element << " elements" << endl;

    for(int i = 0; i < element-1; i++){
        target[i] = random_double_in_range(min_length, max_length);
        length_sum += target[i];
    }
    target[element-1] = length - length_sum;

    // length_sum += target[element-1];
    // cout << "Length sum: " << length_sum << endl;
}


void generate_random_antenna_wires(antenna_geometry_t *geometry){
    double segments_lengths[WIRE_COUNT];
    double wavelength;

    //cout << "Generating random active_elements" << endl;

    wavelength = calculate_wavelength(FREQ);
    divide_length_into_random_length_segment(wavelength/2, WIRE_COUNT, segments_lengths);

    for(int i = 0; i < WIRE_COUNT; i++){
        geometry->active_elements[i].length = segments_lengths[i];
        do {
            geometry->active_elements[i].angle_xy = random_angle_in_radina();
            geometry->active_elements[i].angle_xz = random_angle_in_radina();
        } while(!check_if_geometry_fits_in_cube(geometry, wavelength*CUBE_EDGE_LENGTH_RELATIVE_TO_WAVELENGTH));
    }
}

void generate_test_dipol(antenna_geometry_t *geometry){
    double wavelength = calculate_wavelength(FREQ);
    double segments_lengths = (wavelength*0.25*0.9)/WIRE_COUNT;

    for(int i = 0; i < WIRE_COUNT; i++){
        geometry->active_elements[i].length = segments_lengths;
        geometry->active_elements[i].angle_xy = 0;
        geometry->active_elements[i].angle_xz = M_PI / 2;
    }
}

void generate_random_ground_plane(antenna_geometry_t *geometry){
    double wavelength = calculate_wavelength(FREQ);
    double segments_lengths = random_double_in_range(wavelength/8, wavelength/2);
    double segments_angles_xy = 2 * M_PI / GROUND_PLANE_ELEMENT_COUNT;
    double segments_angles_xz = random_angle_in_radina();

    //segments_lengths = wavelength * 0.28 * 0.9;
    //segments_angles_xz = (2*M_PI/8)*7;

    for(int i = 0; i < GROUND_PLANE_ELEMENT_COUNT; i++){
        geometry->ground_plane[i].length = segments_lengths;
        geometry->ground_plane[i].angle_xy = segments_angles_xy * i;
        geometry->ground_plane[i].angle_xz = segments_angles_xz;
    }
}

// -------------------------------------------------------------------------------- //


antenna_geometry_t generate_random_antenna(){
    antenna_geometry_t random_antenna{};

    init_antenna_geometry(&random_antenna);
    //generate_test_dipol(&random_antenna);
    generate_random_antenna_wires(&random_antenna);
    generate_random_ground_plane(&random_antenna);
    calculate_wire_geometry(&random_antenna);
    //print_antenna_wires(&random_antenna);

    return random_antenna;
}

// -------------------------------------------------------------------------------- //

void save_geometry_to_file(antenna_geometry_t *geometry, const string& file_name){
    cout << "Saving active_elements to file: " << file_name << "\n" << endl;

    ofstream file;
    file.open(file_name);

    file << std::fixed << std::setprecision(5);

    for(int i = 0; i < TOTAL_WIRE_COUNT; i++){
        file << geometry->geometry[i].start.x << "; " << geometry->geometry[i].start.y << "; " << geometry->geometry[i].start.z << "; ";
        file << geometry->geometry[i].end.x << "; " << geometry->geometry[i].end.y << "; " << geometry->geometry[i].end.z << "; " << endl;
    }

}

// -------------------------------------------------------------------------------- //

double calculate_wire_length(wire_vector_t *wire){
    return sqrt(pow(wire->end.x - wire->start.x, 2) + pow(wire->end.y - wire->start.y, 2) + pow(wire->end.z - wire->start.z, 2));
}

// -------------------------------------------------------------------------------- //

void clear_antenna_geometry(antenna_geometry_t *geometry){
    int i;
    for(i = 0; i < TOTAL_WIRE_COUNT; i++){
        geometry->geometry[i].start.x = 0;
        geometry->geometry[i].start.y = 0;
        geometry->geometry[i].start.z = 0;
        geometry->geometry[i].end.x = 0;
        geometry->geometry[i].end.y = 0;
        geometry->geometry[i].end.z = 0;
    }

    for(i = 0; i< WIRE_COUNT; i++){
        geometry->active_elements[i].length = 0;
        geometry->active_elements[i].angle_xy = 0;
        geometry->active_elements[i].angle_xz = 0;
    }

    for(i = 0; i< GROUND_PLANE_ELEMENT_COUNT; i++){
        geometry->ground_plane[i].length = 0;
        geometry->ground_plane[i].angle_xy = 0;
        geometry->ground_plane[i].angle_xz = 0;
    }
}


// -------------------------------------------------------------------------------- //