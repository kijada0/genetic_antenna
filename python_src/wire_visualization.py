import matplotlib.pyplot as plt
from datetime import datetime

file_path = "D:\Studia\CDV\Semetr2\Sztuczna Inteligencja\projekt_zaliczeniowy\genetic_antenna\cpp_src\cmake-build-debug"
source_file = "test.txt"


def main():
    print("Antenna visualization")

    path = file_path + "\\" + source_file
    geometry = read_file(path)
    print("-"*40)
    show_geometry(geometry)
    print("-"*40)
    print(geometry)


def read_file(file_name):
    geometry = []

    with open(file_name, "r") as file:
        lines = file.readlines()
        for line in lines:
            geometry.append(line[:-2].split(";"))
            print(line[:-2].split(";"))

    return geometry


def show_geometry(geometry):
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

    plt.show()
    plt.savefig("out/geometry" + str(time) + ".png")


main()
