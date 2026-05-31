#include "Position.hpp"
#include <iostream>

const int kInitialCapacity = 10;

class Stack {
private:
    Position* data;
    int capacity;
    int topIndex;

    void resize();

public:
    Stack(int initialCapacity = kInitialCapacity);
    ~Stack();
    void push(Position pos);
    void pop();
    bool empty();
    Position top();
};
