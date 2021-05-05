#include "AbstractImage.h"

#include <cstring>
#include <fstream>

Pixel::Pixel(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    : red(r), green(g), blue(b), alpha(a) {}

Pixel::Pixel(const Pixel& other) {
    red = other.red;
    green = other.green;
    blue = other.blue;
    alpha = other.alpha;
}

Pixel& Pixel::operator=(const Pixel& other) {
    red = other.red;
    green = other.green;
    blue = other.blue;
    alpha = other.alpha;
    return *this;
}

Pixel Pixel::operator+(const Pixel& other) {
    return Pixel((red + other.red) / 2,
                 (green + other.green) / 2,
                 (blue + other.blue) / 2,
                 (alpha + other.alpha) / 2);
}

void Pixel::applyFilter(double redRed, double redGreen, double redBlue,
                        double greenRed, double greenGreen, double greenBlue,
                        double blueRed, double blueGreen, double blueBlue){
    setRed(red * redRed + green * redGreen + blue * redBlue);
    setGreen(red * greenRed + green * greenGreen + blue * greenBlue);
    setBlue(red * blueRed + green * blueGreen + blue * blueBlue);
}

void Pixel::setRed(int colour) {
    if (colour > 255) {
        red = 255;
    }
    else if (colour < 0) {
        red = 0;
    }
    else {
        red = colour;
    }
}

void Pixel::setGreen(int colour) {
    if (colour > 255) {
        green = 255;
    }
    else if (colour < 0) {
        green = 0;
    }
    else {
        green = colour;
    }
}

void Pixel::setBlue(int colour) {
    if (colour > 255) {
        blue = 255;
    }
    else if (colour < 0) {
        blue = 0;
    }
    else {
        blue = colour;
    }
}
void Pixel::setAlpha(int colour) {
    if (colour > 255) {
        alpha = 255;
    }
    else if (colour < 0) {
        alpha = 0;
    }
    else {
        alpha = colour;
    }
}

unsigned char Pixel::colourToAverage() const {
    return (red + blue + green) / 3;
}



PixelMatrix::PixelMatrix(size_t height, size_t width)
    : _height(height), _width(width) {
    if (!height || !width) {
        _data = nullptr;
    }
    else {
        _data = new Pixel[width * height];
    }
}

PixelMatrix::PixelMatrix(Pixel* data, size_t width, size_t height) {
    _width = width;
    _height = height;
    if (!height || !width) {
        _data = nullptr;
    }
    else {
        size_t arraySize = width * height;
        _data = new Pixel[width * height];
        for (size_t i = 0; i < arraySize; ++i) {
            _data[i] = data[i];
        }
    }
}

PixelMatrix::PixelMatrix(const PixelMatrix& other) {
    _width = other._width;
    _height = other._height;
    if (other._data) {
        size_t arraySize = _width * _height;
        for (size_t i = 0; i < arraySize; ++i) {
            _data[i] = other._data[i];
        }
    }
}

PixelMatrix::PixelMatrix(PixelMatrix&& other) noexcept {
    std::swap(_height, other._height);
    std::swap(_width, other._width);
    std::swap(_data, other._data);
    other._data = nullptr;
    other._width = 0;
    other._height = 0;
}

PixelMatrix& PixelMatrix::operator=(const PixelMatrix& other) {
    _width = other._width;
    _height = other._height;
    delete[] _data;
    size_t arraySize = _width * _height;
    if (arraySize) {
        _data = new Pixel[arraySize];
        for (size_t i = 0; i < arraySize; ++i) {
            _data[i] = other._data[i];
        }
    }
    else {
        _data = nullptr;
    }
    return *this;
}

PixelMatrix& PixelMatrix::operator=(PixelMatrix&& other) noexcept {
    std::swap(_height, other._height);
    std::swap(_width, other._width);
    std::swap(_data, other._data);
    delete[] other._data;
    other._data = nullptr;
    other._width = 0;
    other._height = 0;
    return *this;
}

Pixel* PixelMatrix::operator[](size_t i) const {
    return &_data[i * _width];
}

size_t PixelMatrix::height() const {
    return _height;
}

size_t PixelMatrix::width() const {
    return _width;
}

Pixel* PixelMatrix::data() const {
    return _data;
}

PixelMatrix::~PixelMatrix() {
    delete[] _data;
}



AbstractImage::AbstractImage(const AbstractImage& other) {
    _path = other._path;
    _name = other._name;
    _matrix = other._matrix;
}

AbstractImage::AbstractImage(AbstractImage&& other) noexcept {
    std::swap(_path, other._path);
    std::swap(_name, other._name);
    std::swap(_matrix, other._matrix);
    other._path = "";
    other._name = "";
    other._matrix = PixelMatrix();
}

AbstractImage& AbstractImage::operator=(const AbstractImage& other) {
    _path = other._path;
    _name = other._name;
    _matrix = other._matrix;
    return *this;
}

AbstractImage& AbstractImage::operator=(AbstractImage&& other) noexcept {
    std::swap(_path, other._path);
    std::swap(_name, other._name);
    std::swap(_matrix, other._matrix);
    other._path = "";
    other._name = "";
    other._matrix = PixelMatrix();
    return *this;
}

void AbstractImage::inverse() {
    for (size_t i = 0; i < height(); ++i) {
        for (size_t j = 0; j < width(); ++j) {
            pixelAt(i, j).red =   255 - pixelAt(i, j).red;
            pixelAt(i, j).green = 255 - pixelAt(i, j).green;
            pixelAt(i, j).blue =  255 - pixelAt(i, j).blue;
            pixelAt(i, j).alpha = 255 - pixelAt(i, j).alpha;
        }
    }
}

void AbstractImage::scale(double k) {
    size_t newWidth = k * width(), newHeight = k * height();
    resizeWidth(newWidth);
    resizeHeight(newHeight);
}

void AbstractImage::whiteBlack() {
    for (size_t i = 0; i < height(); ++i) {
        for (size_t j = 0; j < width(); ++j) {
            if (pixelAt(i, j).colourToAverage() > 128) {
                pixelAt(i, j).red   = 255;
                pixelAt(i, j).green = 255;
                pixelAt(i, j).blue  = 255;
            }
            else {
                pixelAt(i, j).red   = 0;
                pixelAt(i, j).green = 0;
                pixelAt(i, j).blue  = 0;
            }
        }
    }
}

void AbstractImage::reflect(Reflections reflection) {
    switch (reflection) {
        case Reflections::Left:      //Поворот налево на 90 градусов
            {
                PixelMatrix newMatrix(width(), height());
                for (size_t i = 0; i < height(); ++i) {
                    for (size_t j = 0; j < width(); ++j) {
                        newMatrix[j][i] = pixelAt(i, width() - j - 1);
                    }
                }
                _matrix = newMatrix;
            }
            break;
        case Reflections::LeftRight: //Вертикальная симметрия
            for (size_t i = 0; i < height(); ++i) {
                for (size_t j = 0; j < width()/2; ++j) {
                    std::swap(pixelAt(i, j), pixelAt(i, width() - j - 1));
                }
            }
            break;
        case Reflections::Right:     //Поворот направо на 90 градусов
            {
                PixelMatrix newMatrix(width(), height());
                for (size_t i = 0; i < height(); ++i) {
                    for (size_t j = 0; j < width(); ++j) {
                        newMatrix[j][i] = pixelAt(height() - i - 1, j);
                    }
                }
                _matrix = newMatrix;
            }
            break;
        case Reflections::UpDown:    //Горизонтальная симметрия
            for (size_t i = 0; i < height() / 2; ++i) {
                for (size_t j = 0; j < width(); ++j) {
                    std::swap(pixelAt(i, j), pixelAt(height() - i - 1, j));
                }
            }
            break;
    }
}

void AbstractImage::sepia() {
    for (size_t i = 0; i < height(); ++i) {
        for (size_t j = 0; j < width(); ++j) {
            pixelAt(i, j).applyFilter(0.393, 0.769, 0.189,
                                      0.349, 0.686, 0.168,
                                      0.272, 0.534, 0.131);
        }
    }

}

void AbstractImage::greyscale() {
    for (size_t i = 0; i < height(); ++i) {
        for (size_t j = 0; j < width(); ++j) {
            pixelAt(i, j).applyFilter(0.299, 0.587, 0.144,
                                      0.299, 0.587, 0.144,
                                      0.299, 0.587, 0.144);
        }
    }

}

void AbstractImage::brightness(double k) {
    k /= 100;
    for (size_t i = 0; i < height(); ++i) {
        for (size_t j = 0; j < width(); ++j) {
            pixelAt(i, j).applyFilter(k, 0, 0,
                                      0, k, 0,
                                      0, 0, k);
        }
    }
}
void AbstractImage::toAverage() {
    for (size_t i = 0; i < height(); ++i) {
        for (size_t j = 0; j < width(); ++j) {
            unsigned char avg = pixelAt(i, j).colourToAverage();
            pixelAt(i, j).red = avg;
            pixelAt(i, j).green = avg;
            pixelAt(i, j).blue = avg;
        }
    }
}

void AbstractImage::extractRed() {
    for (size_t i = 0; i < height(); ++i) {
        for (size_t j = 0; j < width(); ++j) {
            pixelAt(i, j).applyFilter(0, 0, 0,
                                      0, 1, 0,
                                      0, 0, 1);
        }
    }
}

void AbstractImage::extractBlue() {
    for (size_t i = 0; i < height(); ++i) {
        for (size_t j = 0; j < width(); ++j) {
            pixelAt(i, j).applyFilter(1, 0, 0,
                                      0, 1, 0,
                                      0, 0, 0);
        }
    }
}

void AbstractImage::extractGreen() {
    for (size_t i = 0; i < height(); ++i) {
        for (size_t j = 0; j < width(); ++j) {
            pixelAt(i, j).applyFilter(1, 0, 0,
                                      0, 0, 0,
                                      0, 0, 1);
        }
    }
}

void AbstractImage::resizeWidth(size_t width) {
    if (width == 0) {
        _matrix = PixelMatrix(height(), 0);
        return;
    }
    double k = (double)width / (double)this->width();
    PixelMatrix newMatrix(height(), width);
    if (k > 1) {
        for (size_t i = 0; i < height(); ++i) {
            for (size_t j = 0; j < width; ++j) {
                newMatrix[i][j] = pixelAt(i, j / k);
            }
        }
        _matrix = newMatrix;
    }
    else if (k < 1) {
        for (size_t i = 0; i < height(); ++i) {
            for (size_t j = 0; j < width; ++j) {
                Pixel pixel = pixelAt(i, j / k);
                size_t start = (j / k) + 1, end = ((j + 1) / k);
                for (size_t p = start; p < end; ++p) {
                    pixel = pixel + pixelAt(i, p);
                }
                newMatrix[i][j] = pixel;
            }
        }
        _matrix = newMatrix;
    }
}

void AbstractImage::resizeHeight(size_t height) {
    if (height == 0) {
        _matrix = PixelMatrix((size_t)0, width());
        return;
    }
    double k = (double) height / (double) this->height();
    PixelMatrix newMatrix(height, width());
    if (k > 1) {
        for (size_t i = 0; i < height; ++i) {
            for (size_t j = 0; j < width(); ++j) {
                newMatrix[i][j] = pixelAt(i / k, j);
            }
        }
        _matrix = newMatrix;
    }
    else if (k < 1) {
        for (size_t i = 0; i < height; ++i) {
            for (size_t j = 0; j < width(); ++j) {
                Pixel pixel = pixelAt(i / k, j);
                size_t start = (i / k) + 1, end = ((i + 1) / k);
                for (size_t p = start; p < end; ++p) {
                    pixel = pixel + pixelAt(p, j);
                }
                newMatrix[i][j] = pixel;
            }
        }
        _matrix = newMatrix;
    }
}

void AbstractImage::changeName(const std::string& name) {
    _name = name;
}

void AbstractImage::changePath(const std::string& path) {
    _path = path;
}

size_t AbstractImage::height() const {
    return _matrix.height();
}

size_t AbstractImage::width() const {
    return _matrix.width();
}

unsigned char* AbstractImage::rawData() {
    return reinterpret_cast<unsigned char*>(_matrix.data());
}

Pixel* AbstractImage::pixelLine(size_t i) {
    return _matrix[i];
}

Pixel& AbstractImage::pixelAt(size_t i, size_t j) {
    return pixelLine(i)[j];
}
