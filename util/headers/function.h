#ifndef FUNCTION_H
#define FUNCTION_H

#include <vector>

using namespace std;

class Function {
public:
	Function();
	Function(string name, int type, int quadruple);
    void addParameter(int type);
    vector<int> getParameters();

	string name;
	int type;
    int quadruple;

private:
    vector<int> parameters;
};

#endif
