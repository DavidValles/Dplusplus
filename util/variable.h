#ifndef VARIABLE_H
#define VARIABLE_H

using namespace std;

class Variable {
public:
    Variable();
    Variable(string name, int direction);
    string name;
    int direction;
};

#endif
