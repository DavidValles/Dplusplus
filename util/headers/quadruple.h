#ifndef QUADRUPLE_H
#define QUADRUPLE_H

/*
 * Quadruple
 *
 * Simple structure to keep track of quadruples
 */

class Quadruple {
public:
    Quadruple(int oper, int operand1, int operand2, int result);
    int oper;
    int operand1;
    int operand2;
    int result;
    void display();
};

#endif
