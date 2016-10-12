#ifndef QUADRUPLE_H
#define QUADRUPLE_H

class Quadruple {
public:
    Quadruple(int oper, int operand1, int operand2, int result);
    int oper;
    int operand1;
    int operand2;
    int result;
    void display();
}

#endif
