#ifndef VARIABLETABLE_CPP
#define VARIABLETABLE_CPP

#include "headers/variableTable.h"

using namespace std;

VariableTable::VariableTable() {
    parent = NULL;
}

VariableTable::VariableTable(VariableTable* parent) {
    this->parent = parent;
}

void VariableTable::insertVariable(string id, int address) {
    Variable newVar(id, address);
    this->varTable[id] = newVar;
}

bool VariableTable::findVariable(string id) {
    if(this->varTable.find(id) != this->varTable.end()) {
        return true;
    }
    if(!parent) return false;
    auto parentTable = (*this->parent).varTable;
    if(parentTable.find(id) != parentTable.end()) {
        return true;
    }
    return false;
}

bool VariableTable::findVariableInCurrentTable(string id) {
    if(this->varTable.find(id) != this->varTable.end()) {
        return true;
    }
    return false;
}

void VariableTable::displayTable() {
	for(unordered_map<string, Variable>::iterator it = this->varTable.begin();
            it != this->varTable.end(); it++) {
		cout<<it->first<<" "<<it->second.address<<endl;
	}
}

void VariableTable::clearVarTable() {
    this->varTable.clear();
}

void VariableTable::insertFirstDimension(string id, int dim) {
    varTable[id].dimension1 = dim;
}

void VariableTable::insertSecondDimension(string id, int dim) {
    varTable[id].dimension2 = dim;
}

int VariableTable::getDimension(string id, int dim) {
    int dimSize = 0;
    if (dim == 1) dimSize = varTable[id].dimension1;
    else if (dim == 2) dimSize = varTable[id].dimension2;
    return dimSize;
}

int VariableTable::getAddress(string id) {
    if (this->varTable.find(id) != this->varTable.end()) {
        return this->varTable[id].address;
    }
    if (this->parent && this->parent->getAddress(id) != -1) {
        return this->parent->getAddress(id);
    }
    return -1;
}

Variable VariableTable::getVariable(string id) {
    Variable var;
    if (this->varTable.find(id) != this->varTable.end()) {
        var = this->varTable[id];
    }
    if (this->parent) {
        var = this->parent->getVariable(id);
    }
    return var;
}

#endif
