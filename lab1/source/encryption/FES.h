#pragma once

#include "Key.h"
#include "../image/bitmap_image.h"
#include "../image/pixel.h"
#include "../io/image_parser.h"


class FES{
    // public field
public:
    // Constructor
    FES(){};

    // Define dependent types
    using key_type = Key::key_type;
    using row_type = std::array<BitmapImage::BitmapPixel, 16>;
    using block_type = std::array<std::array<row_type, 3>, 3>;



};
