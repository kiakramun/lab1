#pragma once

#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <wingdi.h>

#include "../image/bitmap_image.h"  // Include the BitmapImage class



class ImageParser {
public:
    // Static method to read a BitmapImage from a file
    static BitmapImage read_bitmap(const std::filesystem::path& filepath) {
        // Check if the file exists
        if (!std::filesystem::exists(filepath)) {
            throw std::exception();
        }

        // Open the file in binary mode
        std::ifstream file(filepath, std::ios::binary);

        // Check if the file is open
        if (!file.is_open()) {
            throw std::exception();
        }

        // Read the BITMAPFILEHEADER
        BITMAPFILEHEADER fileHeader;
        file.read(reinterpret_cast<char*>(&fileHeader), sizeof(BITMAPFILEHEADER));

        // Read the BITMAPINFOHEADER
        BITMAPINFOHEADER infoHeader;
        file.read(reinterpret_cast<char*>(&infoHeader), sizeof(BITMAPINFOHEADER));

        // Check if the file is a BMP image
        if (fileHeader.bfType != 0x4D42) {
            throw std::exception();
        }

        // Check if the image is uncompressed
        if (infoHeader.biCompression != 0) {
            throw std::exception();
        }

        // Read the pixel data
        BitmapImage image(infoHeader.biHeight, infoHeader.biWidth);

        int padding = calculate_padding(infoHeader.biWidth);

        for (int y = 0; y < infoHeader.biHeight; ++y) {
            for (int x = 0; x < infoHeader.biWidth; ++x) {
                BitmapImage::BitmapPixel pixel;
                file.read(reinterpret_cast<char*>(&pixel), sizeof(BitmapImage::BitmapPixel));
                image.set_pixel(x, y, pixel);
            }
            // Skip padding bytes
            file.seekg(padding, std::ios::cur);
        }

        // Close the file
        file.close();

        return image;
    }

    // Static method to write a BitmapImage to a file
    static void write_bitmap(const std::filesystem::path& filepath, const BitmapImage& image) {
        // Open the file in binary mode for writing
        std::ofstream file(filepath, std::ios::binary);

        // Check if the file is open
        if (!file.is_open()) {
            throw std::exception();
        }

        // Write the BITMAPFILEHEADER
        BITMAPFILEHEADER fileHeader;
        fileHeader.bfType = 0x4D42; // BM in ASCII
        fileHeader.bfSize = calculate_file_size(image); //Total size of the BMP file
        fileHeader.bfReserved1 = 0; // Reserved, set to 0
        fileHeader.bfReserved2 = 0; // Reserved, set to 0
        fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER); // Offset to the start of pixel data

        file.write(reinterpret_cast<const char*>(&fileHeader), sizeof (BITMAPFILEHEADER));

        // Write the BITMAPINFOHEADER
        BITMAPINFOHEADER infoHeader;
        infoHeader.biSize = sizeof(BITMAPINFOHEADER); // Size of the BITMAPINFOHEADER structure
        infoHeader.biWidth = image.get_width(); // Width of the image in pixels
        infoHeader.biHeight = image.get_height(); // Height of the image in pixels
        infoHeader.biPlanes = 1; // Number of color planes, must be 1
        infoHeader.biBitCount = sizeof(BitmapImage::BitmapPixel) * 8; // Bits per pixel
        infoHeader.biCompression = 0; // Uncompressed
        infoHeader.biSizeImage = calculate_image_size(image); // Size of the image data in bytes
        infoHeader.biXPelsPerMeter = 0; // Horizontal resolution in pixels per meter
        infoHeader.biYPelsPerMeter = 0; // Vertical resolution in pixels per meter
        infoHeader.biClrUsed = 0; // Number of colors in the color palette (0 for full color)
        infoHeader.biClrImportant = 0; // Number of important colors (0 for all)

        file.write(reinterpret_cast<const char*>(&infoHeader),sizeof(BITMAPINFOHEADER));

        // Write the pixel data
        int padding = calculate_padding(image.get_width());

        for (int y = 0; y < image.get_height(); ++y) {
            for (int x = 0; x < image.get_width(); ++x) {
                BitmapImage::BitmapPixel pixel = image.get_pixel(x,y);
                file.write(reinterpret_cast<const char*>(&pixel), sizeof(BitmapImage::BitmapPixel));
            }
            // Write padding bytes
            for (int p = 0; p < padding; p++) {
                char padByte = 0;
                file.write(&padByte, sizeof(char));
            }
        }

        // Close the file
        file.close();
    }

private:
    // Helper method to calculate the padding for each row
    static int calculate_padding(int width) {
        return (4 - (width * sizeof(BitmapImage::BitmapPixel) % 4)) % 4;
    }

    // Helper method to calculate the total file size
    static int calculate_file_size(const BitmapImage& image) {
        return sizeof(BITMAPFILEHEADER) + sizeof (BITMAPINFOHEADER) + calculate_image_size(image);
    }

    // Helper method to calculate the total image size
    static int calculate_image_size(const BitmapImage& image) {
        return image.get_height() * image.get_width() * sizeof(BitmapImage::BitmapPixel);
    }
};