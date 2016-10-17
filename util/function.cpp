#ifndef FUNCTION_CPP
#define FUNCTION_CPP

#include "headers/function.h"

using namespace std;

Function::Function() {
}

Function::Function(string name, int type) {
	this->name = name;
	this->type = type;
}

#endif
