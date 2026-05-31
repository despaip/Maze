#include "Stack.hpp"

void Stack::resize() {
    int newCapacity = capacity * 2;
    Position* newData = new Position[newCapacity];

    for (int i = 0; i < capacity; ++i) {
        newData[i] = data[i];
    }

    delete[] data;
    data = newData;
    capacity = newCapacity;
}

Stack::Stack(int intialCapacity) {
    data = new Position[intialCapacity];
    topIndex = -1;
    capacity = intialCapacity;
}

Stack::~Stack() {
    delete[] data;
}

void Stack::push(Position pos) {
    if (topIndex == capacity - 1) {
        resize();
    }

    ++topIndex;
    data[topIndex] = pos;
}

bool Stack::empty() {
    return topIndex == -1;
}

void Stack::pop() {
    if (empty()) {
        std::cout << "Out of stack range.\n";
        return;
    }

    --topIndex;
}

Position Stack::top() {
    return data[topIndex];
}
