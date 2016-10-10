#ifndef FUNCTIONTABLE_H
#define FUNCTIONTABLE_H

#include "function.cpp"
#include <iostream>

using namespace std;

class FunctionTable {
public:
	FunctionTable();
	FunctionTable(FunctionTable* parent);
	void insertFunction(string id, int type);
	bool findFunction(string id);
	void displayTable();
	void clearFunTable();

private:
	unordered_map<string, Function> funcTable;
	FunctionTable* parent;
};

#endif