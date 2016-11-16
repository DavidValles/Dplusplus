#ifndef QUADRUPLE_CPP
#define QUADRUPLE_CPP

#include "headers/quadruple.h"

Quadruple::Quadruple(int oper, int operand1, int operand2, int result) {
    this->oper = oper;
    this->operand1 = operand1;
    this->operand2 = operand2;
    this->result = result;
}

void Quadruple::display() {
    string operS;
    switch (oper) {
        case 0: operS = "Add"; break;
        case 1: operS = "Minus"; break;
        case 2: operS = "Div"; break;
        case 3: operS = "Mult"; break;
        case 4: operS = "Modulo"; break;
        case 5: operS = ">"; break;
        case 6: operS = "<"; break;
        case 7: operS = "="; break;
        case 8: operS = "and"; break;
        case 9: operS = "or"; break;
        case 10: operS = "not"; break;
        case 11: operS = "!="; break;
        case 12: operS = "=="; break;
        case 13: operS = "<="; break;
        case 14: operS = ">="; break;
        case 15: operS = "print"; break;
        case 16: operS = "read"; break;
        case 17: operS = "Floor"; break;
        case 18: operS = "Goto"; break;
        case 19: operS = "GotoTrue"; break;
        case 20: operS = "GotoFalse"; break;
        case 21: operS = "Era"; break;
        case 22: operS = "Param"; break;
        case 23: operS = "Gosub"; break;
        case 24: operS = "Return"; break;
        case 25: operS = "Endproc"; break;
        case 26: operS = "End"; break;
        default: operS = "Not Found"; break;
    }
    cout<<operS<<" "<<operand1<<" "<<operand2<<" "<<result<<endl;
}

#endif
