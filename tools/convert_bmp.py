import sys

def get_bytes_per_pixel(bmp_file):
    bmp_file.seek(28)

    return int.from_bytes(bmp_file.read(2)[::-1]) // 8

def get_pixel_array_offset(bmp_file):
    bmp_file.seek(10)

    return int.from_bytes(bmp_file.read(4)[::-1])

def uses_color_pallet(bmp_file):
    bmp_file.seek(46)
    
    return int.from_bytes(bmp_file.read(4)[::-1])

def calculate_padding(bmp_file):
    width, _ = get_image_dimensions(bmp_file)
    bytes_per_pixel = get_bytes_per_pixel(bmp_file)

    padding = (bytes_per_pixel * width) % 4

    if padding == 0:
        return 0
    
    padding = 4 - padding 

    return padding

def get_image_dimensions(bmp_file):
    out = [0, 0]

    bmp_file.seek(18)
    out[0] = int.from_bytes(bmp_file.read(4)[::-1])

    bmp_file.seek(22)
    out[1] = int.from_bytes(bmp_file.read(4)[::-1])

    return tuple(out)

def check_DIB_header(bmp_file):
    bmp_file.seek(14)

    header_size = int.from_bytes(bmp_file.read(4)[::-1])

    if (header_size != 124):
        print("This script only allows BITMAPV5HEADER.")
        exit(1)

def parse_bmp_pixel_data(bmp_f):
    bmp_pixel_rows = []

    width, height = get_image_dimensions(bmp_f)
    bytes_per_px = get_bytes_per_pixel(bmp_f)
    padding = calculate_padding(bmp_f)

    bmp_f.seek(get_pixel_array_offset(bmp_f))

    for _ in range(height):
        row = bytearray()

        for _ in range(width):
            pixel = bmp_f.read(bytes_per_px)

            row.extend(pixel)
            row.extend(b'\x00')
        
        bmp_f.seek(padding, 1)
        bmp_pixel_rows.append(row)
    
    return bmp_pixel_rows

def main(args):
    if (len(args) != 2):
        print("Need a file to convert.")

    output_filename = f'{args[1].split('.')[0]}.raw'

    reverse_rows = None

    with open(args[1], 'rb') as bmp_f:
        check_DIB_header(bmp_f)

        if (get_bytes_per_pixel(bmp_f) != 3):
            print("BMP encoding needs 24 bits per pixel.")
            exit(1)

        if (uses_color_pallet(bmp_f)):
            print("BMP color pallet not supported.")
            exit(1)

        reverse_rows = parse_bmp_pixel_data(bmp_f)
    
    with open(output_filename, 'wb') as out_f:
        for row in reversed(reverse_rows):
            out_f.write(row)

if __name__ == '__main__':
    main(sys.argv)
