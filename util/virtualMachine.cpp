#ifndef VIRTUALMACHINE_CPP
#define VIRTUALMACHINE_CPP

#include "headers/virtualMachine.h"
using namespace std;

VirtualMachine::VirtualMachine(FunctionTable fTable, vector<Quadruple> prog,
        ConstantTable cTable, TypeAdapter& tA) : current(0, 0, 0, 0) {
    program = prog;
    functionTable = fTable;
    constantTable = cTable;
    typeAdapter = tA;
    cQuad = 0;
    this->current = prog[cQuad];
}

void VirtualMachine::run(){
	for(; cQuad < program.size(); cQuad++){
		current = program.at(cQuad);
		    switch (current.oper) {
		        case Ops::Sum : sum(); break;
		        case Ops::Minus : minus(); break;
		        case Ops::Division : division(); break;
		        case Ops::Multiplication : multiplication(); break;
		        case Ops::Modulo : modulo(); break;
		        case Ops::GreaterThan : greaterThan(); break;
		        case Ops::LessThan : lessThan(); break;
		        case Ops::Equal :  equal(); break;
		        case Ops::And : and_(); break;
		        case Ops::Or : or_(); break;
		        case Ops::Not : not_(); break;
		        case Ops::NotEqualTo : notEqualTo(); break;
		        case Ops::EqualTo : equalTo(); break;
		        case Ops::LessThanOrEqualTo : lessThanOrEqualTo(); break;
		        case Ops::GreaterThanOrEqualTo : greaterThanOrEqualTo(); break;
		        case Ops::Print : print(); break;
		        case Ops::Read : read(); break;
		        case Ops::Floor : floor(); break;
		        case Ops::Goto : goto_(); break;
		        case Ops::GotoTrue : gotoTrue(); break;
		        case Ops::GotoFalse : gotoFalse(); break;
		        case Ops::Era : era(); break;
		        case Ops::Param : param(); break;
		        case Ops::GoSub : gosub(); break;
		        case Ops::Return : ret(); break;
		        case Ops::Endproc : endproc(); break;
		        case Ops::End : end(); break;
		        case Ops::Check : check(); break;
		        default:
		        	break;
		    }
	}
}

#endif
