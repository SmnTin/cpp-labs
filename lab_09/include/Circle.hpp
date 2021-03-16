#pragma once

#include "Figure.hpp"

#include <string>

class Circle : public Figure {
public:
    Circle(int id, int x, int y, int radius, std::string label);

    void print() const override;
    bool is_inside(int x, int y) const override;
    void zoom(int factor) override;

private:
    int radius;
    std::string label;
};
