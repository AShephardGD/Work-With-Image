#include "LodePNG\lodepng.h"
#include "PNG.h"

#include <stdexcept>

PNG::PNG(const std::string& path, const std::string& name) {
    unsigned char** buf = new unsigned char*(nullptr);
    unsigned width, height, dataSize;
    _name = name;
    _path = path;
    std::string fileName = _path + "/" + _name;
    unsigned err = lodepng::decode(buf, width, height, dataSize, fileName, LCT_RGBA, 8u);
    PixelMatrix buffer(reinterpret_cast<Pixel*>(*buf), width, height);

    _matrix = std::move(buffer);
    if (*buf) {
        delete[] *buf;
    }
    delete buf;

    if (err) {
        throw std::runtime_error("Called PNG's constructor");
    }
}

void PNG::save() {
    std::string fileName = _path + "/" + _name;
    lodepng::encode(fileName, rawData(), width(), height());
}
