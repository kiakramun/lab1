#pragma once

#include <cstdint>
#include <stdexcept>
#include <vector>

#include "pixel.h"

// Define BitmapPixel as the dependent type
using BitmapPixel = Pixel<std::uint8_t>;
// Define index_type as the dependent type
using index_type = int;

class BitmapImage {
    public:
        // Constructor with height and width parameters
        BitmapImage(int img_height, int img_width) 
            : height(img_height), width(img_width) {
            // Check if height and width are within the valid range
            if (height <= 0 || height > 8192 || width <= 0 || width > 8192) {
                throw std::exception("Invalid height or width for BitmapImage");
            }

            // Initialize the pixel data with a default pixel
            pixel_data.resize(height * width, BitmapPixel());
        }
        
        // Public method to get the height of the image
        [[nodiscard]] int get_height() const noexcept {
            return height;
        }
        
        // Public method to get the width of the image
        [[nodiscard]] int get_width() const noexcept {
            return width;
        }
        
        // Set Pixel at the specified position
        void set_pixel(index_type x, index_type y, const BitmapPixel& new_pixel) {
            if (x <= 0 || x > width || y <= 0 || y > height) {
                throw std::exception("Pixel position is outside the image bounds");
            }
            pixel_data[y * width + x] = new_pixel;
        }

        // Get Pixel at the specified position
        [[nodiscard]] BitmapPixel get_pixel(index_type x, index_type y) const {
            if (x <= 0 || x > width || y <= 0 || y > height) {
                throw std::exception("Pixel position is outside the image bounds");
            }
            return pixel_data[y * width + x];
        }

        // Returns a new image
        [[nodiscard]] BitmapImage transpose() const {
            // Create a new BitmapImage with swapped height and width
            BitmapImage transposedImage(width, height);

            // Loop through each pixel in the original image and copy to transposed image
            for (index_type x = 0; x < width; ++x) {
                for (index_type y = 0; y < height; ++y) {
                    transposedImage.set_pixel(y, x, get_pixel(x, y));
                }
            }
            return transposedImage;
        }
    private:
        // Private attributes
        int height;
        int width;
        std::vector<BitmapPixel>pixel_data; // Represent the pixel data of the image
};
