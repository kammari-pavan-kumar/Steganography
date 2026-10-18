# Image Steganography in C

## Project Description

This project implements Image Steganography using the Least Significant Bit (LSB) technique in C. It hides secret file data inside a BMP image and retrieves the hidden data through decoding.

The project demonstrates how data can be concealed within an image while maintaining its visual quality.

## Features

- Encode secret file data into a BMP image.
- Decode hidden data from a stego image.
- Validate command-line arguments and file extensions.
- Check image capacity before encoding.
- Store and retrieve secret file data.
- Support default output filenames.

## Technologies Used

- C Programming
- File Handling
- Bitwise Operations
- BMP Image Processing
- LSB (Least Significant Bit) Technique

## How to Compile and Run

Compile the project using GCC and the required source files.

### Encoding

Command:

    ./a.out -e beautiful.bmp secret.txt

This command encodes the contents of `secret.txt` into the input image `beautiful.bmp`. If an output image is not specified, the program uses its default output filename.

### Encoding with Output Filename

    ./a.out -e beautiful.bmp secret.txt New.bmp

This command specifies `New.bmp` as the output image filename.

### Decoding

Command:

    ./a.out -d stego.bmp

This command extracts the hidden data from `stego.bmp`. If an output filename is not specified, the program uses its default output filename.

### Decoding with Output Filename

    ./a.out -d stego.bmp Output.txt

This command specifies `Output.txt` as the decoded output filename.

## Project Structure

- `encode.c` - Encoding implementation.
- `encode.h` - Encoding function declarations.
- `decode.c` - Decoding implementation.
- `decode.h` - Decoding function declarations.
- `common.h` - Common declarations.
- `types.h` - Type definitions.
- `test_encode.c` - Encoding test code.
- `beautiful.bmp` - Input BMP image.
- `stego.bmp` - Image containing hidden data.
- `doc.txt` - Project documentation and sample outputs.

## Encoding Process

1. Validate the command-line arguments.
2. Open the input BMP image and secret file.
3. Check whether the image can hold the secret data.
4. Copy the BMP image header.
5. Encode the magic string signature.
6. Encode the secret file extension and file size.
7. Encode the secret file data into the image using LSB.
8. Copy the remaining image data and complete encoding.

## Decoding Process

1. Validate the command-line arguments.
2. Open the stego image.
3. Skip the BMP image header.
4. Decode and verify the magic string signature.
5. Decode the secret file extension and file size.
6. Extract the hidden file data.
7. Complete the decoding process.

## Validation

The project checks command-line arguments and file extensions for encoding and decoding operations. It also checks whether the input image and secret file are valid and whether the image has sufficient capacity for encoding.

Detailed sample inputs, outputs, and validation cases are available in `doc.txt`.

## Learning Outcomes

- Understanding LSB-based image steganography.
- Working with file operations in C.
- Using bitwise operations to encode and decode data.
- Handling command-line arguments and file validation.
- Understanding how data can be hidden inside BMP images.

## Author

K. Pavan Kumar
