#ifndef CONSTANTTABLE_CPP
#define CONSTANTTABLE_CPP

#include "headers/constantTable.h"

using namespace std;

ConstantTable::ConstantTable() {
}

void ConstantTable::insertConstant(string id, int address) {
    Constant constant(id, address);
    this->constantTable[id] = constant;
}

bool ConstantTable::findConstant(string id) {
	if(this->constantTable.find(id) != this->constantTable.end()) {
		return true;
	}
	return false;
}

string ConstantTable::getValue(int address) {
    string value = "";
    for (auto it = constantTable.begin(); it != constantTable.end(); it++) {
        if (it->second.address == address) {
            value = it->first;
        }
    }
    return value;
}


int ConstantTable::getAddress(string id) {
    if (constantTable.find(id) != constantTable.end()) {
        return constantTable[id].address;
    }
    return -1;
}

#endif
