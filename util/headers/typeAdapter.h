#ifndef TYPEADAPTER_H
#define TYPEADAPTER_H

#include "section.h"

using namespace std;

class TypeAdapter {
public:
	TypeAdapter();
	int getType(int address);
	int getScope(int address);

	Section none;

    // Global scope variables
	Section integerG;
	Section decimalG;
	Section characterG;
	Section flagG;

    // Local scope
	Section integerL;
	Section decimalL;
	Section characterL;
	Section flagL;

    // Temporal
	Section integerT;
	Section decimalT;
	Section characterT;
	Section flagT;

    // Constants
    Section integerConstant;
    Section decimalConstant;
    Section textConstant;
    Section characterConstant;
    Section flagConstant;
};

#endif
