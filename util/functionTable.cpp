#ifndef FUNCTIONTABLE_CPP
#define FUNCTIONTABLE_CPP

#include "functionTable.h"

using namespace std;

FunctionTable::FunctionTable() {
	parent = NULL;
}

FunctionTable::FunctionTable(FunctionTable* parent) {
	this->parent = parent;
}

void FunctionTable::insertFunction(string id, int type) {
	Function newFunc(id, type);
	this->funcTable[id] = newFunc;
}

bool FunctionTable::findFunction(string id) {
	if(this->funcTable.find(id) != this->funcTable.end()) {
		return true;
	}
	if(!parent) return false;
	auto parentTable = (*this->parent).funcTable;
	if(parentTable.find(id) != parentTable.end()) {
		return true;
	}
	return false;
}

void FunctionTable::displayTable() {
	for(auto it = this->funcTable.begin(); it != this->funcTable.end(); it++) {
		cout<<it->first<<" "<<it->second.type<<endl;
	}
}

void FunctionTable::clearFunTable() {
	this->funcTable.clear();
}

#endif