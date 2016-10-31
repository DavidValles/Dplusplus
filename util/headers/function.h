#ifndef FUNCTION_H
#define FUNCTION_H

#include <vector>

using namespace std;

class Function {
public:
	Function();
	Function(string name, int type, int quadruple, int returnAddress);
    void addParameter(int type);
    vector<int> getParameters();

	string name;
	int type;
    int quadruple;
    int returnAddress;

private:
    vector<int> parameters;
};

#endif
