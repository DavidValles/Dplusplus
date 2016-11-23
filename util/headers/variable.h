#ifndef VARIABLE_H
#define VARIABLE_H

using namespace std;

/*
 * Variable
 *
 * Contains all information pertaining a variable
 */
class Variable {
public:
    Variable();
    Variable(string name, int address);
    string name;
    int address;
    int dimension1;
    int dimension2;
};

#endif
