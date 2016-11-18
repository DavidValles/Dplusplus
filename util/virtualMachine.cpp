#ifndef VIRTUALMACHINE_CPP
#define VIRTUALMACHINE_CPP

#include "headers/virtualMachine.h"
#include "headers/typeAdapter.cpp"
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

void VirtualMachine::sum(){
	double oper1, oper2;
	int absdir1, absdir2;

	switch(typeAdapter.getScope(current.operand1)){
		case 0:  // global
			switch(typeAdapter.getType(current.operand1)){
				case 0: // int;
					absdir1 = current.operand1 - typeAdapter.integerG.min;
					oper1 = mGlobal.getGlobalInteger(absdir);
				break;
				case 1: // dec;
				break;
				case 2: // char;
				break;
				case 3: // text;
				break;
				case 4: // flag;
				break;
			} break;
		case 1: // local 
		case 2: // temporal
		case 3: // constante
	}
}

void VirtualMachine::minus(){}
void VirtualMachine::division(){}
void VirtualMachine::multiplication(){}
void VirtualMachine::modulo(){}
void VirtualMachine::greaterThan(){}
void VirtualMachine::lessThan(){}
void VirtualMachine::equal(){}
void VirtualMachine::and_(){}
void VirtualMachine::or_(){}
void VirtualMachine::not_(){}
void VirtualMachine::notEqualTo(){}
void VirtualMachine::equalTo(){}
void VirtualMachine::lessThanOrEqualTo(){}
void VirtualMachine::greaterThanOrEqualTo(){}

void VirtualMachine::print(){
	string result;
	int absdir;

	switch(typeAdapter.getScope(current.result)){
		case 0:  // global
			switch(typeAdapter.getType(current.result)){
				case 0: // int;
					absdir1 = current.result - typeAdapter.integerG.min;
					result = to_string(mGlobal.getInteger(absdir));

				break;
				case 1: // dec;
					absdir1 = current.result - typeAdapter.decimalG.min;
					result = to_string(mGlobal.getDecimal(absdir));
				break;
				case 2: // char;
					absdir1 = current.result - typeAdapter.characterG.min;
					result = mGlobal.getChar(absdir);
				break;
				case 3: // text;
				break;
				case 4: // flag;
					absdir1 = current.result - typeAdapter.flagG.min;
					if(mGlobal.getFlag(absdir) == 1){
						result = "True";
					}
					else{
						result = "False";
					}  
				break;
			} break;
		case 1: // local
			switch(typeAdapter.getType(current.result)){
				case 0: // int;
					absdir1 = current.result - typeAdapter.integerL.min;
					result = to_string(mLocal.getInteger(absdir));
				break;
				case 1: // dec;
					absdir1 = current.result - typeAdapter.decimalL.min;
					result = to_string(mLocal.getDecimal(absdir));
				break;
				case 2: // char;
					absdir1 = current.result - typeAdapter.characterL.min;
					result = mLocal.getChar(absdir);
				break;
				case 3: // text;
				break;
				case 4: // flag;
					absdir1 = current.result - typeAdapter.flagL.min;
					if(mLocal.getFlag(absdir) == 1){
						result = "True";
					}
					else{
						result = "False";
					}
				break;
			} break; 
		case 2: // temporal
			switch(typeAdapter.getType(current.result)){
				case 0: // int;
					absdir1 = current.result - typeAdapter.integerT.min;
					result = to_string(mLocal.getTemporalInteger(absdir));
				break;
				case 1: // dec;
					absdir1 = current.result - typeAdapter.decimalT.min;
					result = to_string(mLocal.getTemporalDecimal(absdir));
				break;
				case 2: // char;
					absdir1 = current.result - typeAdapter.characterT.min;
					result = mLocal.getTemporalChar(absdir);
				break;
				case 3: // text;
				break;
				case 4: // flag;
					absdir1 = current.result - typeAdapter.flagT.min;
					if(mLocal.getTemporalFlag(absdir) == 1){
						result = "True";
					}
					else{
						result = "False";
					}
				break;
			} break;
		case 3: // constante
			switch(typeAdapter.getType(current.result)){
				case 0: // int;
					absdir1 = current.result - typeAdapter.integerConstant.min;
					result = to_string(constantTable.getValue(absdir1));
				break;
				case 1: // dec;
					absdir1 = current.result - typeAdapter.decimalConstant.min;
					result = to_string(constantTable.getValue(absdir1));
				break;
				case 2: // char;
					absdir1 = current.result - typeAdapter.characterConstant.min;
					result = constantTable.getValue(absdir1);
				break;
				case 3: // text;
					absdir1 = current.result - typeAdapter.textConstant.min;
					result = constantTable.getValue(absdir1);
				break;
				case 4: // flag;
					absdir1 = current.result - typeAdapter.flagConstant.min;
					 if(constantTable.getValue(absdir1) == 1){
					 	result = "True";
					 }
					 else{
					 	result = "False";
					 }
				break;
			} break;
	}

	cout<<result<<endl;
}

void VirtualMachine::read(){}
void VirtualMachine::floor(){}
void VirtualMachine::goto_(){}
void VirtualMachine::gotoTrue(){}
void VirtualMachine::gotoFalse(){}
void VirtualMachine::era(){}
void VirtualMachine::param(){}
void VirtualMachine::gosub(){}
void VirtualMachine::ret(){}
void VirtualMachine::endproc(){}
void VirtualMachine::end(){}
void VirtualMachine::check(){}


#endif