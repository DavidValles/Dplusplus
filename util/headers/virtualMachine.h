#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHINE_H

#include "util/quadruple.cpp"
#include "../memory.cpp"
#include <vector>
#include <stack>
#include <map>

using namespace std;

class VirtualMachine {

	map<string,int> dirProcedure
	Memory mGlobal; 			// global memory
	Memory mConstant			// constant memory
	Memory mGeneral;			// temporal and local memory
	Quadruples current;			// quadruple in execution
	vector<Quadruples> program;	//list of quadruples to execute
	stack<int> jumpStack; 		//jump stack for quadruples
	stack<Memory> sMemory;		//stack
	//tabla de constantes


	VirtualMachine(map<string,int> proc, vector<Quadruples> prog, constantTable cTable);
	void run();
	void sum();
	void minus();
	void division();
	void multiplication();
	void modulo();
	void greaterThan();
	void lessThan();
	void equal();
	void and();
	void or();
	void not();
	void notEqualTo();
	void equalTo();
	void lessThanOrEqualTo();
	void greaterThanOrEqualTo();
	void print();
	void read();
	void floor();
	void goto();
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