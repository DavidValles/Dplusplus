#ifndef VIRTUALMACHINE_CPP
#define VIRTUALMACHINE_CPP

#include "headers/virtualMachine.h"
using namespace std;

VirtualMachine::VirtualMachine(){

}
VirtualMachine::VirtualMachine(FunctionTable procs, vector<Quadruples> prog,
        constantTable cTable) {
    program = prog;
    functionTable = procs;
}

void VirtualMachine::run(){
	for(cIt = program.at(0).op1; cIt < program.size(); cIt++){
		current = program.at(cIt);
		switch (current.operador) {
		case Ops::Sum : sum(); break;
		case Ops::Minus : minus(); break;
		case Ops::Divsion : division(); break;
		case Ops::Multiplication : multiplication(); break;
		case Ops::Modulo : modulo(); break;
		case Ops::GreaterThan : greaterThan(); break;
		case Ops::LessThan : lessThan(); break;
		case Ops::Equal :  equal(); break;
		case Ops::And : and(); break;
		case Ops::Or : or(); break;
		case Ops::Not : not(); break;
		case Ops::NotEqualTo : notEqualTo(); break;
		case Ops::EqualTo : equalTo(); break;
		case Ops::LessThanOrEqualTo : lessThanOrEqualTo(); break;
		case Ops::GreaterThanOrEqualTo : greaterThanOrEqualTo(); break;
		case Ops::Print : print(); break;
		case Ops::Read : read(); break;
		case Ops::Floor : floor(); break;
		case Ops::Goto : goto(); break;
		case Ops::GotoTrue : gotoTrue(); break;
		case Ops::GotoFalse : gotoFalse(); break;
		case Ops::Era : era(); break;
		case Ops::Param : param(); break;
		case Ops::Gosub : gosub(); break;
		case Ops::Return : ret(); break;
		case Ops::Endproc : endproc(); break;
		case Ops::End : end(); break;
		case Ops::Check : check(); break;
		default:
			break;
		}
	}
}
