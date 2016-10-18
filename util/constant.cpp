#ifndef CONSTANT_CPP
#define CONSTANT_CPP

#include "headers/constant.h"

using namespace std;

Constant::Constant() {
}

Constant::Constant(string name, int address) {
    this->name = name;
    this->address = address;
}

#endif
