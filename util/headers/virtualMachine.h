#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHINE_H

#include "util/quadruple.cpp"
#include "../memory.cpp"
#include <vector>
#include <stack>
#include <map>

using namespace std;

class VirtualMachine {
	//map<int,int> dirProcedure
	Memory mGlobal; 			// global memory
	Memory mConstant			// constant memory
	Memory mGeneral;			// temporal and local memory
	Quadruples current;			// quadruple in execution
	vector<Quadruples> program;	//list of quadruples to execute
	stack<int> jumpStack; 		//jump stack for quadruples
	stack<Memory> sMemory;		//stack

	void run();
	void assign();
	void add();
	void minus();
	void multiply();
	void divide();
	void module();
	void lessThan();
	void greaterThan();
	void equal();
	void and();
	void or();
	void not();
	void notEqual();
	void greaterEqual();
	void lessEqual();
	void print();
	void read();
	void goto();
	void gotoTrue();
	void gotoFalse();
	void era();
	void gosub();


};