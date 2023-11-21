#pragma once

// DO NOT CHANGE THIS ENUM
enum class ChannelOrder {
	BGR,
	BRG,
	GBR,
	GRB,
	RBG,
	RGB
};

template <typename T>
class Pixel {
    public:
        // Default constructor
        Pixel()
            : blue_channel(0), green_channel(0), red_channel(0){};

        // Constructor with values
        Pixel(T blue, T green, T red)
            : blue_channel(blue), green_channel(green), red_channel(red) {};

        // Public methods to get blue channel intensities
        [[nodiscard]] T get_blue_channel() const noexcept {
            return blue_channel;
        }

        // Public methods to get green channel intensities
        [[nodiscard]] T get_green_channel() const noexcept {
            return green_channel;
        }

        // Public methods to get red channel intensities
        [[nodiscard]] T get_red_channel() const noexcept {
            return red_channel;
        }

        // Equality operator 
        [[nodiscard]] bool operator==(const Pixel<T>& other) const noexcept {
            return (blue_channel == other.blue_channel)
                && (green_channel == other.green_channel)
                && (red_channel == other.red_channel);
        }
    private:
        // Private attributes
        T blue_channel;
        T green_channel;
        T red_channel;
        ChannelOrder channel_order = ChannelOrder::BGR;
};
