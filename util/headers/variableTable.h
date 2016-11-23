#ifndef VARIABLETABLE_H
#define VARIABLETABLE_H

#include "../variable.cpp"
#include <iostream>

using namespace std;

/*
 * VariableTable
 *
 * Structure to keep track of variables.
 *
 * It can insert a variable, find a variable in current table and aprent table,
 * display it, clear it, define dimensions of a variable, and other getters.
 *
 */
class VariableTable {
public:
    VariableTable();
    VariableTable(VariableTable* parent);
    void insertVariable(string id, int address);
    bool findVariable(string id);
    bool findVariableInCurrentTable(string id);
    void displayTable();
    void clearVarTable();
    void insertFirstDimension(string id, int dim);
    void insertSecondDimension(string id, int dim);
    int getDimension(string id, int dim);
    int getAddress(string id);
    Variable getVariable(string id);

private:
    unordered_map<string, Variable> varTable;
    VariableTable* parent;
};


#endif
