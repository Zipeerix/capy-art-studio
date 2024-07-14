import random
import sys


def generate_random_list(n, lower_bound, upper_bound):
    return [random.randint(lower_bound, upper_bound) for _ in range(n)]


def generate_random_layers_data(width, height, layers):
    layers_bytes = []

    # Metadata
    layers_bytes += width.to_bytes(4, byteorder='little')
    layers_bytes += height.to_bytes(4, byteorder='little')
    layers_bytes += layers.to_bytes(4, byteorder='little')

    # Names
    layer_names_bytes = []
    for layer_index in range(0, layers):
        layer_name = "Layer " + str(layer_index)
        layer_names_bytes += list(layer_name.encode())
        layer_names_bytes += [0x00]

    layers_bytes += layer_names_bytes
    layers_bytes += [0xFF]

    # Data
    for layer_index_for_data_gen in range(0, layers):
        layers_bytes += generate_random_list(width * height * 4, 0, 255)

    return layers_bytes


def file_to_byte_list(file_path):
    with open(file_path, 'rb') as file:
        byte_data = file.read()
    return list(byte_data)


def save_byte_list_to_file(byte_list, file_path):
    with open(file_path, 'wb') as file:
        bytes_data = bytes(byte_list)
        file.write(bytes_data)


def main():
    if len(sys.argv) != 6:
        print("Usage: python3 create_empty_project.py [WIDTH] [HEIGHT] [LAYERS] [MINIATURE_PATH] [OUTPUT_PATH]")
        exit()

    # Magic string CAPY
    file_bytes = [0x43, 0x41, 0x50, 0x59, 0x00]

    width = int(sys.argv[1])
    height = int(sys.argv[2])
    layers = int(sys.argv[3])
    miniature_path = sys.argv[4]
    output_path = sys.argv[5]
    miniature_bytes = file_to_byte_list(miniature_path)
    miniature_len_bytes = len(miniature_bytes).to_bytes(4, byteorder='little')

    assert len(miniature_len_bytes) == 4

    layers_data_bytes = generate_random_layers_data(width, height, layers)

    full_file_bytes = file_bytes + list(miniature_len_bytes) + miniature_bytes + layers_data_bytes

    save_byte_list_to_file(full_file_bytes, output_path)


if __name__ == "__main__":
    main()
