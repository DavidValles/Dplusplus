#ifndef VARIABLE_CPP
#define VARIABLE_CPP

#include "headers/variable.h"

using namespace std;

Variable::Variable() {
    this->dimension1 = 0;
    this->dimension2 = 0;
}

Variable::Variable(string name, int address) {
    this->name = name;
    this->address = address;
    this->dimension1 = 0;
    this->dimension2 = 0;
}

#endif
