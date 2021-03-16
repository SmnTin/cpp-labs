#include "Rectangle.hpp"

#include <iostream>

Rectangle::Rectangle(int id, int x, int y, int width, int height)
        : Figure(id, x, y), width(width), height(height) {}

void Rectangle::print() const {
    std::cout << "Rectangle " << id << ": x = " << x << " y = " << y
              << " width = " << width << " height = " << height << std::endl;
}

bool Rectangle::is_inside(int x_, int y_) const {
    int dx = x - x_;
    int dy = y - y_;
    return 2 * dx <= width && 2 * dy <= height;
}

void Rectangle::zoom(int factor) {
    width *= factor;
    height *= factor;
}