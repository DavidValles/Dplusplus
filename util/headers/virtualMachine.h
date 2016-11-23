#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHINE_H

#include "../quadruple.cpp"
#include "../memory.cpp"
#include "../FunctionTable.cpp"
#include "../ConstantTable.cpp"
#include "../operations.h"
#include <vector>
#include <stack>
#include <map>

using namespace std;

/*
 * Virtual machine
 *
 * Parameters:
 *  FunctionTable - for memory allocation
 *  ConstantTable - for constant accessing
 *  vector<Quadruple> - all quadruples
 *  typeAdapter - to properly allocate and access memory
 *
 * Out:
 *  Yields a program excecution
 */

class VirtualMachine {

public:
	FunctionTable functionTable;
	ConstantTable constantTable;
	Memory mGlobal; 			// global memory
	Memory* mLocal;			    // temporal and local memory
	Memory* mTemporal;			// temporal and local memory
	Quadruple current;			// quadruple in execution
	vector<Quadruple> program;	//list of quadruples to execute
	stack<int> jumpStack; 		//jump stack for quadruples
	stack<Memory> sMemory;		//stack
    TypeAdapter typeAdapter;
    int cQuad;
    vector<int> integerParams;
    vector<float> decimalParams;
    vector<string> textParams;
    vector<char> characterParams;
    vector<bool> flagParams;

	VirtualMachine(FunctionTable proc, vector<Quadruple> prog,
            ConstantTable cTable, TypeAdapter& tA);
    void getPointerContent(int& pointer);
	void run();
	void sum();
	void minus();
	void division();
	void multiplication();
	void modulo();
	void greaterThan();
	void lessThan();
	void equal();
	void and_();
	void or_();
	void not_();
	void notEqualTo();
	void equalTo();
	void lessThanOrEqualTo();
	void greaterThanOrEqualTo();
	void print();
	void read();
	void floor();
	void goto_();
	void gotoTrue();
	void gotoFalse();
	void era();
	void param();
	void gosub();
	void ret();
	void endproc();
	void end();
	void check();
};

#endif
