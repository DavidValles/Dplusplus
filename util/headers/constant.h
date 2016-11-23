#ifndef CONSTANT_H
#define CONSTANT_H

using namespace std;

/*
 * Constant
 *
 * Contains all information pertaining a constant
 */
class Constant {
public:
    Constant();
    Constant(string name, int address);
    string name;
    int address;
};

#endif
