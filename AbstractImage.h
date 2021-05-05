#pragma once

#include <iostream>

enum class Reflections{
    UpDown,
    Left,
    Right,
    LeftRight
};

class Pixel {
public:
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char alpha;

    Pixel(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0, unsigned char a = 0);
    Pixel(const Pixel& other);

    Pixel& operator=(const Pixel& other);
    Pixel operator+(const Pixel& other);

    void applyFilter(double redRed, double redGreen, double redBlue,
                     double greenRed, double greenGreen, double greenBlue,
                     double blueRed, double blueGreen, double blueBlue);

    void setRed(int colour);
    void setGreen(int colour);
    void setBlue(int colour);
    void setAlpha(int colour);

    unsigned char colourToAverage() const;
};

class PixelMatrix {
public:
    PixelMatrix(size_t height = 0, size_t width = 0);
    PixelMatrix(Pixel* data, size_t width = 0, size_t height = 0);
    PixelMatrix(const PixelMatrix& other);
    PixelMatrix(PixelMatrix&& other) noexcept;

    PixelMatrix& operator=(const PixelMatrix& other);
    PixelMatrix& operator=(PixelMatrix&& other) noexcept;

    Pixel* operator[](size_t i) const;

    size_t height() const;
    size_t width() const;
    Pixel* data() const;

    ~PixelMatrix();

private:
    Pixel* _data;
    size_t _height;
    size_t _width;
};

class AbstractImage {
public:
    AbstractImage() = default;
    AbstractImage(const AbstractImage& other);
    AbstractImage(AbstractImage&& other) noexcept;

    AbstractImage& operator=(const AbstractImage& other);
    AbstractImage& operator=(AbstractImage&& other) noexcept;

    virtual ~AbstractImage() = default;

    void inverse();                             //Инверсия цветов
    void scale(double percent);                 //Изменение размеров с сохранением пропорции
    void whiteBlack();                          //Черно-белый
    void reflect(Reflections reflection);       //Симметрия и поворот на 90 градусов
    void sepia();                               //Сепия
    void greyscale();                           //Оттенки серого
    void brightness(double percent);            //Яркость
    void toAverage();                           //Усреднение цвета пикселей

    void gammaRed(double percent);              //Контраст красного
    void gammaBlue(double percent);             //Контраст синего
    void gammaGreen(double percent);            //Контраст зеленого

    void resizeWidth(size_t width);             //Изменить ширину
    void resizeHeight(size_t height);           //Изменить высоту

    void changeName(const std::string& name);   //Сменить имя изображения
    void changePath(const std::string& path);   //Сменить путь изображения

    virtual void save() = 0;
    size_t height() const;
    size_t width() const;

protected:
    std::string _path;
    std::string _name;
    PixelMatrix _matrix;

    unsigned char* rawData();

    Pixel* pixelLine(size_t i);
    Pixel& pixelAt(size_t i, size_t j);
};
