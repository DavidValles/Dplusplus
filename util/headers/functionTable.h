#ifndef FUNCTIONTABLE_H
#define FUNCTIONTABLE_H

#include "../function.cpp"
#include <iostream>

using namespace std;

class FunctionTable {
public:
	FunctionTable();
	void insertFunction(string id, int type);
    void addParameterToFunction(string id, int type);
	bool findFunction(string id);
    bool checkTypeOfParameter(string func, int type, int param);
	void displayTable();
	void clearFunTable();

private:
	unordered_map<string, Function> funcTable;
};

#endif
