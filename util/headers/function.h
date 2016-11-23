#ifndef FUNCTION_H
#define FUNCTION_H

#include <vector>
#include <map>
#include "section.h"

using namespace std;

/*
 * Function
 *
 * Contains all information for functions. Also has some utility for
 * parametrization.
 */
class Function {
public:
	Function();
	Function(string name, int type, int quadruple, int returnAddress);
    void addParameter(int type);
    vector<int> getParameters();

	string name;
    map<string, int> variableCounts;
	int type;
    int quadruple;
    int returnAddress;

private:
    vector<int> parameters;
};

#endif
