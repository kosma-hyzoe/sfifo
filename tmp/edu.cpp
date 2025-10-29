#include <iostream>

using namespace std;

template <typename T> struct Vector {
    int capacity = 1;
    T *data = new T[capacity];
    int size = 0;

    void push(T x) {
        if (size == capacity){
            grow();
        }
        data[size++] = x;
    }

    T &operator[](int i) { return data[i]; }

    void grow() {
        capacity *= 2;
        T *next = new T[capacity];
        for (int i = 0; i < size; i++) {
            next[i] = data[i];
        }
        delete[] data;
        data = next;
    }
};

int main()
{
    struct Vector<int> foo;
    foo.push(1);
    foo.push(2);
    foo.push(3);
    cout << foo.data[2] << endl;

}
