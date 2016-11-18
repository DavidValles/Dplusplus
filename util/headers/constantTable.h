#ifndef CONSTANTTABLE_H
#define CONSTANTTABLE_H

#include "../constant.cpp"

using namespace std;

class ConstantTable {
public:
    ConstantTable();
    void insertConstant(string id, int address);
    bool findConstant(string id);
    int getAddress(string id);
    string getValue(int address);

private:
    unordered_map<string, Constant> constantTable;
};

#endif
