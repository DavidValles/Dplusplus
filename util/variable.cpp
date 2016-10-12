#ifndef VARIABLE_CPP
#define VARIABLE_CPP

#include "variable.h"

using namespace std;

Variable::Variable() {
}

Variable::Variable(string name, int address) {
    this->name = name;
    this->address = address;
}

#endif
