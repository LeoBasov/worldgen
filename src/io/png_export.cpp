#include "png_export.hpp"

namespace wg {

Image::Image(std::size_t width, std::size_t height)
    : m_data(width * height), m_width{static_cast<std::int32_t>(width)}, m_height{static_cast<std::int32_t>(height)} {}

void Image::set(std::int32_t x, std::int32_t y, const RGB& color) {
    if (not inBounds(y, x)) {
        return;
    }

    m_data[static_cast<std::size_t>(y) * m_width + x] = color;
}

std::int32_t Image::width() const noexcept { return m_width; }

std::int32_t Image::height() const noexcept { return m_height; }

bool Image::saveBMP(const std::string& path) {
    const std::int32_t rowSize = m_width * 3 + m_width % 4;
    const std::uint32_t bmpsize = rowSize * m_height;
    const BMPHeader header = {0x4d42,
                              static_cast<std::uint32_t>(bmpsize + sizeof(BMPHeader)),
                              0,
                              0,
                              sizeof(BMPHeader),
                              40,
                              m_width,
                              m_height,
                              1,
                              24,
                              0,
                              bmpsize,
                              0,
                              0,
                              0,
                              0};

    if (std::ofstream ofs{path, std::ios_base::binary}) {
        ofs.write(reinterpret_cast<const char*>(&header), sizeof(header));

        std::vector<std::uint8_t> line(rowSize);

        for (std::int32_t y = m_height - 1; -1 < y; --y) {
            size_t pos = 0;

            for (std::int32_t x = 0; x < m_width; ++x) {
                const RGB& col = m_data[static_cast<std::size_t>(y) * m_width + x];
                line[pos++] = ToUint8(col.b);
                line[pos++] = ToUint8(col.g);
                line[pos++] = ToUint8(col.r);
            }

            ofs.write(reinterpret_cast<const char*>(line.data()), line.size());
        }

        return true;
    } else {
        return false;
    }
}

bool Image::inBounds(std::int32_t y, std::int32_t x) const noexcept {
    return (0 <= y) && (y < m_height) && (0 <= x) && (x < m_width);
}

}  // namespace wg
