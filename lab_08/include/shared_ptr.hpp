#pragma once

#include "matrix.hpp"

class shared_ptr {
public:
    /**
     * Получает объект по сырому указателю во владение.
     * Передача того же указателя во владение кому либо ещё — неопределённое поведение.
     */
    shared_ptr(Matrix *obj = nullptr);
    /**
     * Строит копию умного указателя, разделяя владение с оригиналом.
     */
    shared_ptr(const shared_ptr &other);
    /**
     * Присваивает умному указателю новое значение, корректно меняя владение с одного объекта на другой.
     */
    shared_ptr &operator=(const shared_ptr &other);
    ~shared_ptr();

    /**
     * Возвращает сырой указатель для несовместимого кода.
     */
    Matrix *ptr() const;
    /**
     * Проверка на nullptr.
     */
    bool isNull() const;
    /**
     * Присваивает умному указателю новый для владения.
     */
    void reset(Matrix *obj = nullptr);

    Matrix *operator->() const;
    Matrix &operator*() const;

private:
    class Storage;
    void free_storage();
    void share_storage(Storage *storage);

private:
    Storage *storage_;
};
