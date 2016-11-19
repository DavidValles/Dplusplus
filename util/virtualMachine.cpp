#ifndef VIRTUALMACHINE_CPP
#define VIRTUALMACHINE_CPP

#include "headers/virtualMachine.h"
#include "typeAdapter.cpp"
#include <string>

using namespace std;

VirtualMachine::VirtualMachine(FunctionTable fTable, vector<Quadruple> prog,
        ConstantTable cTable, TypeAdapter& tA) : current(0, 0, 0, 0) {
    program = prog;
    functionTable = fTable;
    constantTable = cTable;
    typeAdapter = tA;
    cQuad = 0;

    auto variableCount = functionTable.getVariableCounts("0");
    mGlobal.changeVariables(
            variableCount["integerG"],
            variableCount["decimalG"],
            variableCount["characterG"],
            variableCount["flagG"]);

    run();
}

void VirtualMachine::run(){
	for(; cQuad < program.size(); cQuad++){
		current = program[cQuad];
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
	int absdir1, absdir2, absdir3;

	switch(typeAdapter.getScope(current.operand1)){
		case 0:  // global
			switch(typeAdapter.getType(current.operand1)){
				case 0: { // int;
					absdir1 = current.operand1 - typeAdapter.integerG.min;
					oper1 = mGlobal.getInteger(absdir1);
				} break;
				case 1: { // dec;
					absdir1 = current.operand1 - typeAdapter.decimalG.min;
					oper1 = mGlobal.getDecimal(absdir1);
				} break;
			} break;
		case 1:  // local
			switch(typeAdapter.getType(current.operand1)){
				case 0: { // int;
					absdir1 = current.operand1 - typeAdapter.integerL.min;
					oper1 = mLocal.getInteger(absdir1);
				} break;
				case 1: { // dec;
					absdir1 = current.operand1 - typeAdapter.decimalL.min;
					oper1 = mLocal.getDecimal(absdir1);
				} break;
			} break;
		case 2: // temporal
			switch(typeAdapter.getType(current.operand1)){
				case 0: { // int;
					absdir1 = current.operand1 - typeAdapter.decimalT.min;
					oper1 = mLocal.getTemporalInteger(absdir1);
				} break;
				case 1: { // dec;
					absdir1 = current.operand1 - typeAdapter.integerT.min;
					oper1 = mLocal.getTemporalDecimal(absdir1);
				} break;
			} break;
		case 3:// constante
			switch(typeAdapter.getType(current.operand1)){
				case 0: { // int;
					absdir1 = current.operand1 - typeAdapter.integerConstant.min;
					oper1 = stoi(constantTable.getValue(absdir1));
				} break;
				case 1: { // dec;
					absdir1 = current.operand1 - typeAdapter.decimalConstant.min;
					oper1 = stod(constantTable.getValue(absdir1));
				} break;
			} break;
	}

	switch(typeAdapter.getScope(current.operand2)){
		case 0:  // global
			switch(typeAdapter.getType(current.operand2)){
				case 0: { // int;
					absdir2 = current.operand2 - typeAdapter.integerG.min;
					oper2 = mGlobal.getInteger(absdir2);
				} break;
				case 1: { // dec;
					absdir2 = current.operand2 - typeAdapter.decimalG.min;
					oper2 = mGlobal.getDecimal(absdir2);
				} break;
			} break;
		case 1:  // local
			switch(typeAdapter.getType(current.operand2)){
				case 0: { // int;
					absdir2 = current.operand2 - typeAdapter.integerL.min;
					oper2 = mLocal.getInteger(absdir2);
				} break;
				case 1: { // dec;
					absdir2 = current.operand2 - typeAdapter.decimalL.min;
					oper2 = mLocal.getDecimal(absdir2);
				} break;
			} break;
		case 2: // temporal
			switch(typeAdapter.getType(current.operand2)){
				case 0: { // int;
					absdir2 = current.operand2 - typeAdapter.integerT.min;
					oper2 = mLocal.getTemporalInteger(absdir2);
				} break;
				case 1: { // dec;
					absdir2 = current.operand2 - typeAdapter.decimalT.min;
					oper2 = mLocal.getTemporalDecimal(absdir2);
				} break;
			} break;
		case 3:// constante
			switch(typeAdapter.getType(current.operand2)){
				case 0: { // int;
					absdir2 = current.operand2 - typeAdapter.integerConstant.min;
					oper2 = stoi(constantTable.getValue(absdir2));
				} break;
				case 1: { // dec;
					absdir2 = current.operand2 - typeAdapter.decimalConstant.min;
					oper2 = stod(constantTable.getValue(absdir2));
				} break;
			} break;
	}

	switch(typeAdapter.getScope(current.result)){
		case 0:  // global
			switch(typeAdapter.getType(current.result)){
				case 0: { // int;
					absdir3 = current.result - typeAdapter.integerG.min;
					mGlobal.setInteger(absdir3, oper1 + oper2);
				} break;
				case 1: { // dec;
					absdir3 = current.result - typeAdapter.decimalG.min;
					mGlobal.setDecimal(absdir3, oper1 + oper2);
				} break;
			} break;
		case 1:  // local
			switch(typeAdapter.getType(current.result)){
				case 0: { // int;
					absdir3 = current.result - typeAdapter.integerL.min;
					mLocal.setInteger(absdir3, oper1 + oper2);
				} break;
				case 1: { // dec;
					absdir3 = current.result - typeAdapter.decimalL.min;
					mLocal.setDecimal(absdir3, oper1 + oper2);
				} break;
			} break;
		case 2: // temporal
			switch(typeAdapter.getType(current.result)){
				case 0: { // int;
					absdir3 = current.result - typeAdapter.decimalT.min;
					mLocal.setTemporalInteger(absdir3, oper1 + oper2);
				} break;
				case 1: { // dec;
					absdir3 = current.result - typeAdapter.decimalT.min;
					mLocal.setTemporalDecimal(absdir3, oper1 + oper2);
				} break;
			} break;
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
				case 0: { // int;
					absdir = current.result - typeAdapter.integerG.min;
					result = to_string(mGlobal.getInteger(absdir));
                } break;
				case 1: { // dec;
					absdir = current.result - typeAdapter.decimalG.min;
					result = to_string(mGlobal.getDecimal(absdir));
                } break;
				case 2: // text;
				break;
				case 3: { // char;
					absdir = current.result - typeAdapter.characterG.min;
					result = mGlobal.getCharacter(absdir);
                } break;
				case 4: { // flag;
					absdir = current.result - typeAdapter.flagG.min;
					if(mGlobal.getFlag(absdir) == 1){
						result = "True";
					}
					else{
						result = "False";
					}
                } break;
			} break;
		case 1: // local
			switch(typeAdapter.getType(current.result)){
				case 0: { // int;
					absdir = current.result - typeAdapter.integerL.min;
					result = to_string(mLocal.getInteger(absdir));
                } break;
				case 1: { // dec;
					absdir = current.result - typeAdapter.decimalL.min;
					result = to_string(mLocal.getDecimal(absdir));
                } break;
				case 2: // text;
				break;
				case 3: { // char;
					absdir = current.result - typeAdapter.characterL.min;
					result = mLocal.getCharacter(absdir);
                } break;
				case 4: { // flag;
					absdir = current.result - typeAdapter.flagL.min;
					if(mLocal.getFlag(absdir) == 1){
						result = "True";
					}
					else{
						result = "False";
					}
                } break;
			} break;
		case 2: // temporal
			switch(typeAdapter.getType(current.result)){
				case 0: { // int;
					absdir = current.result - typeAdapter.integerT.min;
					result = to_string(mLocal.getTemporalInteger(absdir));
                } break;
				case 1: { // dec;
					absdir = current.result - typeAdapter.decimalT.min;
					result = to_string(mLocal.getTemporalDecimal(absdir));
                } break;
				case 2: // text;
				break;
				case 3: { // char;
					absdir = current.result - typeAdapter.characterT.min;
					result = mLocal.getTemporalCharacter(absdir);
                } break;
				case 4: // flag;
					absdir = current.result - typeAdapter.flagT.min;
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
				case 0: { // int;
					result = constantTable.getValue(current.result);
                } break;
				case 1: { // dec;
					result = constantTable.getValue(current.result);
                } break;
				case 2: { // text;
					result = constantTable.getValue(current.result);
                } break;
				case 3: { // char;
					result = constantTable.getValue(current.result);
                } break;
				case 4: { // flag;
					 if(constantTable.getValue(current.result) == "true"){
					 	result = "True";
					 }
					 else{
					 	result = "False";
					 }
                } break;
			} break;
        defualt: cout<<"Scope not found"<<endl;
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
