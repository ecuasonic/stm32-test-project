from PIL import Image
import sys
import os

def image_to_ssd1306_words(image_path):
    img = Image.open(image_path).convert("1")
    img = img.resize((128, 64))  # Force 128x64

    pixels = img.load()
    width, height = img.size

    words = []
    for page in range(height // 8):
        for x in range(width):
            byte = 0
            for bit in range(8):
                y = page * 8 + bit
                if pixels[x, y] != 0:  # white = ON
                    byte |= (1 << bit)
            words.append(byte)

    return [w & 0xFF for w in words]

def save_c_and_h(words, basename, array_name):
    # Paths
    c_path = f"../src/oled_images/{basename}.c"
    h_path = f"../include/oled_images/{basename}.h"

    # Make sure directories exist
    os.makedirs(os.path.dirname(c_path), exist_ok=True)
    os.makedirs(os.path.dirname(h_path), exist_ok=True)

    # Write C file
    with open(c_path, "w") as f:
        f.write(f'#include "oled_images/{basename}.h"\n\n')
        f.write('#include "types.h"\n\n')
        f.write(f"const uint32_t {array_name}[{len(words)}] = {{\n")
        for i in range(0, len(words), 8):
            line = ", ".join(f"0x{w:02X}" for w in words[i:i+8])
            f.write(f"    {line},\n")
        f.write("};\n")

    # Write H file
    guard = f"_{basename.upper()}_H_"
    with open(h_path, "w") as f:
        f.write(f"#ifndef {guard}\n")
        f.write(f"#define {guard}\n\n")
        f.write("#include \"types.h\"\n\n")
        f.write(f"extern const uint32_t {array_name}[{len(words)}];\n\n")
        f.write(f"#endif // {guard}\n")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python convert.py input.* output")
        sys.exit(1)

    input_file = sys.argv[1]
    basename = os.path.splitext(os.path.basename(input_file))[0]
    array_name = basename.replace("-", "_").replace(" ", "_")

    words = image_to_ssd1306_words(input_file)
    save_c_and_h(words, basename, array_name)

    print(f"Generated: ../src/oled_images/{basename}.c and ../images/oled_images/{basename}.h")

