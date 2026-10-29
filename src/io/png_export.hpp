#pragma once

#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

namespace wg {

struct BMPHeader {
    std::uint16_t bfType;
    std::uint32_t bfSize;
    std::uint16_t bfReserved1;
    std::uint16_t bfReserved2;
    std::uint32_t bfOffBits;
    std::uint32_t biSize;
    std::int32_t biWidth;
    std::int32_t biHeight;
    std::uint16_t biPlanes;
    std::uint16_t biBitCount;
    std::uint32_t biCompression;
    std::uint32_t biSizeImage;
    std::int32_t biXPelsPerMeter;
    std::int32_t biYPelsPerMeter;
    std::uint32_t biClrUsed;
    std::uint32_t biClrImportant;
};

struct RGB {
    double r = 0.0;
    double g = 0.0;
    double b = 0.0;
    constexpr RGB() = default;
    explicit constexpr RGB(double _rgb) noexcept : r{_rgb}, g{_rgb}, b{_rgb} {}
    constexpr RGB(double _r, double _g, double _b) noexcept : r{_r}, g{_g}, b{_b} {}
};

class Image {
   public:
    Image() = default;
    Image(std::size_t width, std::size_t height);

    void set(std::int32_t x, std::int32_t y, const RGB& color);
    std::int32_t width() const noexcept;
    std::int32_t height() const noexcept;
    bool saveBMP(const std::string& path);

   private:
    std::vector<RGB> m_data;
    std::int32_t m_width = 0, m_height = 0;

    bool inBounds(std::int32_t y, std::int32_t x) const noexcept;

    static constexpr std::uint8_t ToUint8(double x) noexcept {
        return (x <= 0.0) ? 0 : (1.0 <= x) ? 255 : static_cast<std::uint8_t>(x * 255.0 + 0.5);
    }
};

}  // namespace wg
