#include <string>

#include "JPG.h"
#include "JpegCompressor\jpgd.h"
#include "JpegCompressor\jpge.h"

JPG::JPG(const std::string& path, const std::string& name) {
    int width, height, actualComps;
    _path = path;
    _name = name;
    std::string fileName = path + "/" + name;
    unsigned char* data = jpgd::decompress_jpeg_image_from_file(fileName.c_str(), &width, &height, &actualComps, 4);
    PixelMatrix matrix(reinterpret_cast<Pixel*>(data), width, height);
    _matrix = std::move(matrix);
    delete[] data;
}

void JPG::save() {
    std::string name = _path + "/" + _name;
    jpge::compress_image_to_jpeg_file(name.c_str(), width(), height(), 4, rawData());
}
