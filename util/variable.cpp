#ifndef VARIABLE_CPP
#define VARIABLE_CPP

#include "variable.h"

using namespace std;

Variable::Variable() {
}

Variable::Variable(string name, int direction) {
    this->name = name;
    this->direction = direction;
}

#endif
