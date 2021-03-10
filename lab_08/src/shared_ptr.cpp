#include "shared_ptr.hpp"

class shared_ptr::Storage {
public:
    explicit Storage(Matrix *mtx) : data_(mtx), ref_count_(1) {}

    ~Storage() {
        delete data_;
    }

    void incr() {
        ++ref_count_;
    }

    void decr() {
        --ref_count_;
    }

    int getCounter() const {
        return ref_count_;
    }

    Matrix *getObject() const {
        return data_;
    }

private:
    Matrix *data_;
    int ref_count_;
};

shared_ptr::shared_ptr(Matrix *obj) {
    storage_ = new Storage(obj);
}

shared_ptr::shared_ptr(const shared_ptr &other) {
    share_storage(other.storage_);
}

void shared_ptr::reset(Matrix *obj) {
    *this = shared_ptr(obj);
}

shared_ptr &shared_ptr::operator=(const shared_ptr &other) {
    if (this == &other)
        return *this;

    free_storage();
    share_storage(other.storage_);
    return *this;
}

bool shared_ptr::isNull() const {
    return storage_->getObject() == nullptr;
}

Matrix *shared_ptr::ptr() const {
    return storage_->getObject();
}

Matrix &shared_ptr::operator*() const {
    return *ptr();
}

Matrix *shared_ptr::operator->() const {
    return ptr();
}

shared_ptr::~shared_ptr() {
    free_storage();
}

void shared_ptr::free_storage() {
    storage_->decr();
    if (storage_->getCounter() == 0)
        delete storage_;
}

void shared_ptr::share_storage(Storage *storage) {
    storage_ = storage;
    storage_->incr();
}