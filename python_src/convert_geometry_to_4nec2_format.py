file_path = "D:\Studia\CDV\Semetr2\Sztuczna Inteligencja\projekt_zaliczeniowy\genetic_antenna\cpp_src\cmake-build-debug"
source_file = "test.txt"


def main():
    print("Convert geometry to 4nec2 format")

    path = file_path + "\\" + source_file
    geometry = read_file(path)
    print("-"*40)
    convert_to_4nec2_format(geometry)
    print("-"*40)
    convert_to_maa_format(geometry)
    print("-"*40)


def read_file(file_name):
    geometry = []

    with open(file_name, "r") as file:
        lines = file.readlines()
        for line in lines:
            geometry.append(line[:-2].split(";"))
            print(line[:-2].split(";"))

    return geometry

def convert_to_4nec2_format(geometry):
    for wire, num in zip(geometry, range(len(geometry))):
        line = "GW {} {} {} {} {} {} {} {} {}".format(num, 4, wire[0], wire[1], wire[2], wire[3], wire[4], wire[5], 0.003)
        print(line)


def convert_to_maa_format(geometry):
    print(len(geometry))
    for wire, num in zip(geometry, range(len(geometry))):
        line = "{}, {}, {}, {}, {}, {}, {}, {}".format(wire[0], wire[1], wire[2], wire[3], wire[4], wire[5], 0.003, -1)
        print(line)


if __name__ == '__main__':
    main()