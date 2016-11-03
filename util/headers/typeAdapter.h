#ifndef TYPEADAPTER_H
#define TYPEADAPTER_H

#include "section.h"

using namespace std;

class TypeAdapter {
public:
	TypeAdapter();
	int getType(int address);

	Section integer;
	Section decimal;
	Section text;
	Section character;
	Section flag;
	Section array;
	Section matrix;
	Section none;
	Section avail;
    Section integerConstant;
    Section decimalConstant;
    Section stringConstant;
    Section characterConstant;
};

#endif
