#pragma once

#include "Figure.hpp"

#include <vector>
#include <memory>

class Scheme {
public:
    explicit Scheme(std::size_t capacity);
    ~Scheme();

    void push_back_figure(std::unique_ptr<Figure> fg);
    void push_back_figure(Figure *fg); // implicitly takes ownership of *fg
    void remove_figure(int id); // элементы смещаются влево

    void print_all_figures();
    void zoom_figure(int id, int factor);
    Figure *is_inside_figure(int x, int y); // если внутри нескольких фигур, то возвращается любая из них
    void move(int id, int new_x, int new_y);

private:
    typedef std::vector<std::unique_ptr<Figure>> Container;
    Container::iterator find_by_id(int id);
    Container figures;
};
