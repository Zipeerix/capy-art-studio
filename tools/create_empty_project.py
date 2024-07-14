import sys


def file_to_byte_list(file_path):
    with open(file_path, 'rb') as file:
        byte_data = file.read()
    return list(byte_data)


def save_byte_list_to_file(byte_list, file_path):
    with open(file_path, 'wb') as file:
        bytes_data = bytes(byte_list)
        file.write(bytes_data)


def main():
    if len(sys.argv) != 3:
        print("Usage: python3 create_empty_project.py [MINIATURE_PATH] [OUTPUT_PATH]")

    # Magic string
    file_bytes = [0x43, 0x41, 0x50, 0x59, 0x00]

    miniature_path = sys.argv[1]
    output_path = sys.argv[2]
    miniature_bytes = file_to_byte_list(miniature_path)
    miniature_len_bytes = len(miniature_bytes).to_bytes(4, byteorder='little')

    assert len(miniature_len_bytes) == 4

    # TODO: Empty layers data

    full_file_bytes = file_bytes + list(miniature_len_bytes) + miniature_bytes

    save_byte_list_to_file(full_file_bytes, output_path)


if __name__ == "__main__":
    main()
