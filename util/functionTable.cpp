#ifndef FUNCTIONTABLE_CPP
#define FUNCTIONTABLE_CPP

#include "headers/functionTable.h"

using namespace std;

FunctionTable::FunctionTable() {
}

void FunctionTable::insertFunction(string id, int type) {
	Function newFunc(id, type);
	this->funcTable[id] = newFunc;
}

void FunctionTable::addParameterToFunction(string id, int type) {
    if (this->findFunction(id)) {
        this->funcTable[id].addParameter(type);
    }
}

bool FunctionTable::findFunction(string id) {
	if(this->funcTable.find(id) != this->funcTable.end()) {
		return true;
	}
	return false;
}

void FunctionTable::displayTable() {
	for(auto it = this->funcTable.begin(); it != this->funcTable.end(); it++) {
		cout<<it->first<<" "<<it->second.type<<" Parameters: ";
        auto parameters = it->second.getParameters();
        for (int i=0; i < parameters.size(); i++) {
            cout<<parameters[i]<<" ";
        }
        cout<<endl;
	}
}

void FunctionTable::clearFunTable() {
	this->funcTable.clear();
}

#endif
