import os
import math

import matplotlib.pyplot as plt
from datetime import datetime

source_file = "population_230617_053924392.csv"

antenna_count = 10


def main():
    print("Antenna visualization")

    path = source_file
    geometry = read_file(path)
    print("-"*40)
    print(len(geometry))

    count = 0
    for antenna in geometry:
        new_geo = convert_geometry(antenna)
        show_geometry(new_geo, count)
        count += 1


def read_file(file_name):
    geometry = []

    count = 0
    with open(file_name, "r") as file:
        lines = file.readlines()
        for line in lines:
            if count >= antenna_count+1:
                break
            geometry.append(line[:-2].split(";"))
            # print(line[:-2].split(";"))
            count += 1

    return geometry[1:]


def show_geometry(geometry, id):
    fig = plt.figure()
    ax = plt.axes(projection='3d')
    ax.axis('equal')

    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')

    ax.set_xlim(-0.5, 0.5)
    ax.set_ylim(-0.5, 0.5)
    ax.set_zlim(0, 1)

    color_list = ["r", "g", "b", "y", "c", "m", "k", "r", "g", "b", "y", "c", "m", "k", "w"]

    for wire, color in zip(geometry, color_list):
        print(wire)
        x = [float(wire[0]), float(wire[3])]
        y = [float(wire[1]), float(wire[4])]
        z = [float(wire[2]), float(wire[5])]
        ax.plot3D(x, y, z, color=color, linewidth=4)

    time = datetime.now().strftime("%Y%m%d_%H%M%S")

    plt.savefig("../out/geometry" + str(id) + ".png")
    plt.show()
    plt.close()


def convert_geometry(antenna):
    new_geometry = []

    element_count = int(antenna[3])
    print("element_count: " + str(element_count))
    last_index = 4 + element_count*3

    for i in range(4, last_index, 3):
        print(i)
        new_geometry.append([float(antenna[i]), float(antenna[i+1]), float(antenna[i+2])])
        print([float(antenna[i]), float(antenna[i+1]), float(antenna[i+2])])

    vectors = convert_to_vector(new_geometry)

    gp_count = int(antenna[last_index])
    print("gp_count: ", gp_count)
    last_index_gp = last_index + 1 + gp_count*3

    for i in range(last_index+1, last_index_gp, 3):
        print(i)
        geo = [float(antenna[i]), float(antenna[i+1]), float(antenna[i+2])]
        print([float(antenna[i]), float(antenna[i+1]), float(antenna[i+2])])
        vectors.append(convert_single_wire(geo))

    return vectors


def convert_to_vector(geometry):
    vector = []

    x = geometry[0][0] * math.cos(geometry[0][1]) * math.cos(geometry[0][2])
    y = geometry[0][0] * math.sin(geometry[0][1]) * math.cos(geometry[0][2])
    z = geometry[0][0] * math.sin(geometry[0][2])

    vector.append([0, 0, 0, x, y, z])

    for i in range(1, len(geometry)):
        x = vector[i-1][3] + geometry[i][0] * math.cos(geometry[i][1]) * math.cos(geometry[i][2])
        y = vector[i-1][4] + geometry[i][0] * math.sin(geometry[i][1]) * math.cos(geometry[i][2])
        z = vector[i-1][5] + geometry[i][0] * math.sin(geometry[i][1])

        vector.append([vector[i-1][3], vector[i-1][4], vector[i-1][5], x, y, z])

    # for line in vector:
    #     print(line)

    return vector


def convert_single_wire(geometry):
    print(geometry)
    x = geometry[0] * math.cos(geometry[1]) * math.cos(geometry[2])
    y = geometry[0] * math.sin(geometry[1]) * math.cos(geometry[2])
    z = geometry[0] * math.sin(geometry[2])

    print([0, 0, 0, x, y, z])
    return [0, 0, 0, x, y, z]


main()
