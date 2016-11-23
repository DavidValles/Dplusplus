#ifndef FUNCTIONTABLE_H
#define FUNCTIONTABLE_H

#include "../function.cpp"
#include <iostream>

using namespace std;

/*
 * Function Table
 *
 * Maps Functions to ids. Has utilities for accessing and modifying Functions,
 * parametrization and insertion.
 */

class FunctionTable {
public:
	FunctionTable();
	void insertFunction(string id, int type, int quadruple, int rAddress);
    int getParametersSize(string id);
    void addParameterToFunction(string id, int type);
	bool findFunction(string id);
    Function getFunction(string id);
    Function getFunction(int address);
    map<string, int> getVariableCounts(string id);
    bool checkTypeOfParameter(string func, int type, int param);
    void setVariableCount(string id, map<string, int> counts);
	void displayTable();
	void clearFunTable();

private:
	unordered_map<string, Function> funcTable;
};

#endif
