#ifndef VARIABLE_CPP
#define VARIABLE_CPP

#include "variable.h"

using namespace std;

Variable::Variable() {
}

Variable::Variable(string name, int type) {
    this->name = name;
    this->type = type;
}

#endif
