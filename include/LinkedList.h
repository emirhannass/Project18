class MyString {
private:
    char* str;
    int len;

public:
    MyString(const char* s = "") {
        len = 0;
        while (s[len] != '\0') len++;
        str = new char[len + 1];
        for (int i = 0; i < len; i++) str[i] = s[i];
        str[len] = '\0';
    }

    MyString(const MyString& other) {
        len = other.len;
        str = new char[len + 1];
        for (int i = 0; i <= len; i++) str[i] = other.str[i];
    }

    ~MyString() { delete[] str; }

    bool operator==(const MyString& other) const {
        if (len != other.len) return false;
        for (int i = 0; i < len; i++) {
            if (str[i] != other.str[i]) return false;
        }
        return true;
    }

    const char* c_str() const { return str; }
};

#ifndef MYVECTOR_H
#define MYVECTOR_H

template <typename T>
class MyVector {
private:
    T* data;           
    int _size;        
    int _capacity;     

    void resize() {
        _capacity = (_capacity == 0) ? 1 : _capacity * 2;
        T* newData = new T[_capacity];
        
        for (int i = 0; i < _size; i++) {
            newData[i] = data[i]; // Elemanları yeni diziye kopyalar
        }
        
        delete[] data; 
        data = newData;
    }

public:
    
    MyVector() : data(nullptr), _size(0), _capacity(0) {}

    ~MyVector() {
        delete[] data;
    }

    void push_back(const T& value) {
        if (_size == _capacity) {
            resize();
        }
        data[_size++] = value;
    }

    T& operator[](int index) {
        return data[index];
    }

    const T& operator[](int index) const {
        return data[index];
    }

    int size() const {
        return _size;
    }

    MyVector(const MyVector& other) {
        _size = other._size;
        _capacity = other._capacity;
        data = new T[_capacity];
        for (int i = 0; i < _size; i++) {
            data[i] = other.data[i];
        }
    }

    MyVector& operator=(const MyVector& other) {
        if (this != &other) {
            delete[] data;
            _size = other._size;
            _capacity = other._capacity;
            data = new T[_capacity];
            for (int i = 0; i < _size; i++) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }
};

#endif