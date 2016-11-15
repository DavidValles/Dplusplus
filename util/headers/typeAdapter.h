#ifndef TYPEADAPTER_H
#define TYPEADAPTER_H

#include "section.h"

using namespace std;

class TypeAdapter {
public:
	TypeAdapter();
	int getType(int address);

	Section none;

    // Global scope variables
	Section integerG;
	Section decimalG;
	Section textG;
	Section characterG;
	Section flagG;

    // Local scope
	Section integerL;
	Section decimalL;
	Section textL;
	Section characterL;
	Section flagL;

    // Temporal 
	Section integerT;
	Section decimalT;
	Section textT;
	Section characterT;
	Section flagT;

    // Constants
    Section integerConstant;
    Section decimalConstant;
    Section textConstant;
    Section characterConstant;
};

#endif
