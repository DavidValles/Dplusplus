#ifndef FUNCTIONTABLE_CPP
#define FUNCTIONTABLE_CPP

#include "headers/functionTable.h"

using namespace std;

FunctionTable::FunctionTable() {
}

void FunctionTable::insertFunction(string id, int type, int quadruple, int rAddress) {
	Function newFunc(id, type, quadruple, rAddress);
	this->funcTable[id] = newFunc;
}

int FunctionTable::getParametersSize(string id) {
    if (this->findFunction(id)) {
        return this->funcTable[id].getParameters().size();
    }
    return -1;
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

Function FunctionTable::getFunction(string id) {
	if(this->findFunction(id)) {
		return this->funcTable[id];
	}
    Function emptyFunction;
	return emptyFunction;
}

bool FunctionTable::checkTypeOfParameter(string func, int type, int param) {
    auto parameters = funcTable[func].getParameters();
    if (param >= parameters.size()) return false;
    return parameters[param] == type;
}

void FunctionTable::setVariableCount(string id, map<string, int> counts) {
    if (findFunction(id)) {
        funcTable[id].variableCounts = counts;
    }
}

void FunctionTable::displayTable() {
	for(auto it = this->funcTable.begin(); it != this->funcTable.end(); it++) {
		cout<<it->first<<" "<<it->second.type<<" Parameters: ";
        auto parameters = it->second.getParameters();
        for (int i=0; i < parameters.size(); i++) {
            cout<<parameters[i]<<" ";
        }
        cout<<endl;
        auto m = it->second.variableCounts;
        for (auto it2 = m.begin(); it2 != m.end(); it2++) {
            cout<<it2->first<<" "<<it2->second<<endl;
        }
	}
}

void FunctionTable::clearFunTable() {
	this->funcTable.clear();
}

#endif
