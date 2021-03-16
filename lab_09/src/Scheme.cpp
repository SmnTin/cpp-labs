#include "Scheme.hpp"

#include <algorithm>

Scheme::Scheme(std::size_t capacity) {
    figures.reserve(capacity);
}

Scheme::~Scheme() = default;

void Scheme::push_back_figure(std::unique_ptr<Figure> fg) {
    figures.emplace_back(std::move(fg));
}

void Scheme::push_back_figure(Figure *fg) {
    figures.emplace_back(fg);
}

void Scheme::print_all_figures() {
    for (const auto &fg : figures)
        fg->print();
}

Scheme::Container::iterator Scheme::find_by_id(int id) {
    return std::find_if(figures.begin(), figures.end(), [id](const auto &fg) {
        return fg->id == id;
    });
}

void Scheme::remove_figure(int id) {
    figures.erase(find_by_id(id));
}

void Scheme::move(int id, int new_x, int new_y) {
    auto it = find_by_id(id);
    if (it != figures.end())
        (*it)->move(new_x, new_y);
}

void Scheme::zoom_figure(int id, int factor) {
    auto it = find_by_id(id);
    if (it != figures.end())
        (*it)->zoom(factor);
}

Figure *Scheme::is_inside_figure(int x, int y) {
    for (const auto &fg : figures)
        if (fg->is_inside(x, y))
            return fg.get();
    return nullptr;
}
