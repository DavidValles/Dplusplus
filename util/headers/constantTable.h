#ifndef CONSTANTTABLE_H
#define CONSTANTTABLE_H

#include "../constant.cpp"

using namespace std;

/*
 * ConstantTable
 *
 * Maps constants with ids, also has some utility functions for accessing and
 * inserting
 */

class ConstantTable {
public:
    ConstantTable();
    void insertConstant(string id, int address);
    bool findConstant(string id);
    int getAddress(string id);
    void displayTable();
    string getValue(int address);

private:
    unordered_map<string, Constant> constantTable;
};

#endif
