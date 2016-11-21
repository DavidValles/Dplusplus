#ifndef VIRTUALMACHINE_CPP
#define VIRTUALMACHINE_CPP

#include "headers/virtualMachine.h"
#include "typeAdapter.cpp"
#include <string>
#include <limits>

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
    mGlobal.changeVariablesTemporal(
            variableCount["integerT"],
            variableCount["decimalT"],
            variableCount["characterT"],
            variableCount["flagT"]);
    mGlobal.initialize();
    mGlobal.initializeTemporal();

    mTemporal = &mGlobal;

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

void VirtualMachine::getPointerContent(int& pointer) {
    int absdir;
    if (pointer <= typeAdapter.integerG.min * -1) { // Scope is always temporal
        pointer *= -1;
        switch (typeAdapter.getType(pointer)) {
            case 0: { // int
                absdir = pointer - typeAdapter.integerT.min;
                pointer = mTemporal->getTemporalInteger(absdir);
            } break;
            default: cout<<"Type not found (Pointer)"<<endl; break;
        }
    }
}

void VirtualMachine::sum(){
    double oper1, oper2;
    int absdir1, absdir2, absdir3;

    // If the operands are pointers, get their directions
    getPointerContent(current.operand1);
    getPointerContent(current.operand2);

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
                    oper1 = mLocal->getInteger(absdir1);
                } break;
                case 1: { // dec;
                    absdir1 = current.operand1 - typeAdapter.decimalL.min;
                    oper1 = mLocal->getDecimal(absdir1);
                } break;
            } break;
        case 2: // temporal
            switch(typeAdapter.getType(current.operand1)){
                case 0: { // int;
                    absdir1 = current.operand1 - typeAdapter.decimalT.min;
                    oper1 = mTemporal->getTemporalInteger(absdir1);
                } break;
                case 1: { // dec;
                    absdir1 = current.operand1 - typeAdapter.integerT.min;
                    oper1 = mTemporal->getTemporalDecimal(absdir1);
                } break;
            } break;
        case 3:// constante
            switch(typeAdapter.getType(current.operand1)){
                case 0: { // int;
                    absdir1 = current.operand1;
                    oper1 = stoi(constantTable.getValue(absdir1));
                } break;
                case 1: { // dec;
                    absdir1 = current.operand1;
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
                    oper2 = mLocal->getInteger(absdir2);
                } break;
                case 1: { // dec;
                    absdir2 = current.operand2 - typeAdapter.decimalL.min;
                    oper2 = mLocal->getDecimal(absdir2);
                } break;
            } break;
        case 2: // temporal
            switch(typeAdapter.getType(current.operand2)){
                case 0: { // int;
                    absdir2 = current.operand2 - typeAdapter.integerT.min;
                    oper2 = mTemporal->getTemporalInteger(absdir2);
                } break;
                case 1: { // dec;
                    absdir2 = current.operand2 - typeAdapter.decimalT.min;
                    oper2 = mTemporal->getTemporalDecimal(absdir2);
                } break;
            } break;
        case 3:// constante
            switch(typeAdapter.getType(current.operand2)){
                case 0: { // int;
                    absdir2 = current.operand2;
                    oper2 = stoi(constantTable.getValue(absdir2));
                } break;
                case 1: { // dec;
                    absdir2 = current.operand2;
                    oper2 = stod(constantTable.getValue(absdir2));
                } break;
            } break;
    }

    if (current.result <= typeAdapter.integerG.min) {
        current.result *= -1;
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
                    mLocal->setInteger(absdir3, oper1 + oper2);
                } break;
                case 1: { // dec;
                    absdir3 = current.result - typeAdapter.decimalL.min;
                    mLocal->setDecimal(absdir3, oper1 + oper2);
                } break;
            } break;
        case 2: // temporal
            switch(typeAdapter.getType(current.result)){
                case 0: { // int;
                    absdir3 = current.result - typeAdapter.integerT.min;
                    mTemporal->setTemporalInteger(absdir3, oper1 + oper2);
                } break;
                case 1: { // dec;
                    absdir3 = current.result - typeAdapter.decimalT.min;
                    mTemporal->setTemporalDecimal(absdir3, oper1 + oper2);
                } break;
            } break;
    }
}

void VirtualMachine::minus(){
    double oper1, oper2;
    int absdir1, absdir2, absdir3;

    // If the operands are pointers, get their directions
    getPointerContent(current.operand1);
    getPointerContent(current.operand2);

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
                    oper1 = mLocal->getInteger(absdir1);
                } break;
                case 1: { // dec;
                    absdir1 = current.operand1 - typeAdapter.decimalL.min;
                    oper1 = mLocal->getDecimal(absdir1);
                } break;
            } break;
        case 2: // temporal
            switch(typeAdapter.getType(current.operand1)){
                case 0: { // int;
                    absdir1 = current.operand1 - typeAdapter.decimalT.min;
                    oper1 = mTemporal->getTemporalInteger(absdir1);
                } break;
                case 1: { // dec;
                    absdir1 = current.operand1 - typeAdapter.integerT.min;
                    oper1 = mTemporal->getTemporalDecimal(absdir1);
                } break;
            } break;
        case 3:// constante
            switch(typeAdapter.getType(current.operand1)){
                case 0: { // int;
                    absdir1 = current.operand1;
                    oper1 = stoi(constantTable.getValue(absdir1));
                } break;
                case 1: { // dec;
                    absdir1 = current.operand1;
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
                    oper2 = mLocal->getInteger(absdir2);
                } break;
                case 1: { // dec;
                    absdir2 = current.operand2 - typeAdapter.decimalL.min;
                    oper2 = mLocal->getDecimal(absdir2);
                } break;
            } break;
        case 2: // temporal
            switch(typeAdapter.getType(current.operand2)){
                case 0: { // int;
                    absdir2 = current.operand2 - typeAdapter.integerT.min;
                    oper2 = mTemporal->getTemporalInteger(absdir2);
                } break;
                case 1: { // dec;
                    absdir2 = current.operand2 - typeAdapter.decimalT.min;
                    oper2 = mTemporal->getTemporalDecimal(absdir2);
                } break;
            } break;
        case 3:// constante
            switch(typeAdapter.getType(current.operand2)){
                case 0: { // int;
                    absdir2 = current.operand2;
                    oper2 = stoi(constantTable.getValue(absdir2));
                } break;
                case 1: { // dec;
                    absdir2 = current.operand2;
                    oper2 = stod(constantTable.getValue(absdir2));
                } break;
            } break;
    }

    switch(typeAdapter.getScope(current.result)){
        case 0:  // global
            switch(typeAdapter.getType(current.result)){
                case 0: { // int;
                    absdir3 = current.result - typeAdapter.integerG.min;
                    mGlobal.setInteger(absdir3, oper1 - oper2);
                } break;
                case 1: { // dec;
                    absdir3 = current.result - typeAdapter.decimalG.min;
                    mGlobal.setDecimal(absdir3, oper1 - oper2);
                } break;
            } break;
        case 1:  // local
            switch(typeAdapter.getType(current.result)){
                case 0: { // int;
                    absdir3 = current.result - typeAdapter.integerL.min;
                    mLocal->setInteger(absdir3, oper1 - oper2);
                } break;
                case 1: { // dec;
                    absdir3 = current.result - typeAdapter.decimalL.min;
                    mLocal->setDecimal(absdir3, oper1 - oper2);
                } break;
            } break;
        case 2: // temporal
            switch(typeAdapter.getType(current.result)){
                case 0: { // int;
                    absdir3 = current.result - typeAdapter.integerT.min;
                    mTemporal->setTemporalInteger(absdir3, oper1 - oper2);
                } break;
                case 1: { // dec;
                    absdir3 = current.result - typeAdapter.decimalT.min;
                    mTemporal->setTemporalDecimal(absdir3, oper1 - oper2);
                } break;
            } break;
    }
}

void VirtualMachine::division(){
    double oper1, oper2;
    int absdir1, absdir2, absdir3;

    // If the operands are pointers, get their directions
    getPointerContent(current.operand1);
    getPointerContent(current.operand2);

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
                    oper1 = mLocal->getInteger(absdir1);
                } break;
                case 1: { // dec;
                    absdir1 = current.operand1 - typeAdapter.decimalL.min;
                    oper1 = mLocal->getDecimal(absdir1);
                } break;
            } break;
        case 2: // temporal
            switch(typeAdapter.getType(current.operand1)){
                case 0: { // int;
                    absdir1 = current.operand1 - typeAdapter.decimalT.min;
                    oper1 = mTemporal->getTemporalInteger(absdir1);
                } break;
                case 1: { // dec;
                    absdir1 = current.operand1 - typeAdapter.integerT.min;
                    oper1 = mTemporal->getTemporalDecimal(absdir1);
                } break;
            } break;
        case 3:// constante
            switch(typeAdapter.getType(current.operand1)){
                case 0: { // int;
                    absdir1 = current.operand1;
                    oper1 = stoi(constantTable.getValue(absdir1));
                } break;
                case 1: { // dec;
                    absdir1 = current.operand1;
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
                    oper2 = mLocal->getInteger(absdir2);
                } break;
                case 1: { // dec;
                    absdir2 = current.operand2 - typeAdapter.decimalL.min;
                    oper2 = mLocal->getDecimal(absdir2);
                } break;
            } break;
        case 2: // temporal
            switch(typeAdapter.getType(current.operand2)){
                case 0: { // int;
                    absdir2 = current.operand2 - typeAdapter.integerT.min;
                    oper2 = mTemporal->getTemporalInteger(absdir2);
                } break;
                case 1: { // dec;
                    absdir2 = current.operand2 - typeAdapter.decimalT.min;
                    oper2 = mTemporal->getTemporalDecimal(absdir2);
                } break;
            } break;
        case 3:// constante
            switch(typeAdapter.getType(current.operand2)){
                case 0: { // int;
                    absdir2 = current.operand2;
                    oper2 = stoi(constantTable.getValue(absdir2));
                } break;
                case 1: { // dec;
                    absdir2 = current.operand2;
                    oper2 = stod(constantTable.getValue(absdir2));
                } break;
            } break;
    }

    switch(typeAdapter.getScope(current.result)){
        case 0:  // global
            switch(typeAdapter.getType(current.result)){
                case 0: { // int;
                    absdir3 = current.result - typeAdapter.integerG.min;
                    mGlobal.setInteger(absdir3, oper1 / oper2);
                } break;
                case 1: { // dec;
                    absdir3 = current.result - typeAdapter.decimalG.min;
                    mGlobal.setDecimal(absdir3, oper1 / oper2);
                } break;
            } break;
        case 1:  // local
            switch(typeAdapter.getType(current.result)){
                case 0: { // int;
                    absdir3 = current.result - typeAdapter.integerL.min;
                    mLocal->setInteger(absdir3, oper1 / oper2);
                } break;
                case 1: { // dec;
                    absdir3 = current.result - typeAdapter.decimalL.min;
                    mLocal->setDecimal(absdir3, oper1 / oper2);
                } break;
            } break;
        case 2: // temporal
            switch(typeAdapter.getType(current.result)){
                case 0: { // int;
                    absdir3 = current.result - typeAdapter.integerT.min;
                    mTemporal->setTemporalInteger(absdir3, oper1 / oper2);
                } break;
                case 1: { // dec;
                    absdir3 = current.result - typeAdapter.decimalT.min;
                    mTemporal->setTemporalDecimal(absdir3, oper1 / oper2);
                } break;
            } break;
    }
}

void VirtualMachine::multiplication(){
    double oper1, oper2;
    int absdir1, absdir2, absdir3;

    // If the operands are pointers, get their directions
    getPointerContent(current.operand1);
    getPointerContent(current.operand2);

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
                    oper1 = mLocal->getInteger(absdir1);
                } break;
                case 1: { // dec;
                    absdir1 = current.operand1 - typeAdapter.decimalL.min;
                    oper1 = mLocal->getDecimal(absdir1);
                } break;
            } break;
        case 2: // temporal
            switch(typeAdapter.getType(current.operand1)){
                case 0: { // int;
                    absdir1 = current.operand1 - typeAdapter.decimalT.min;
                    oper1 = mTemporal->getTemporalInteger(absdir1);
                } break;
                case 1: { // dec;
                    absdir1 = current.operand1 - typeAdapter.integerT.min;
                    oper1 = mTemporal->getTemporalDecimal(absdir1);
                } break;
            } break;
        case 3:// constante
            switch(typeAdapter.getType(current.operand1)){
                case 0: { // int;
                    absdir1 = current.operand1;
                    oper1 = stoi(constantTable.getValue(absdir1));
                } break;
                case 1: { // dec;
                    absdir1 = current.operand1;
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
                    oper2 = mLocal->getInteger(absdir2);
                } break;
                case 1: { // dec;
                    absdir2 = current.operand2 - typeAdapter.decimalL.min;
                    oper2 = mLocal->getDecimal(absdir2);
                } break;
            } break;
        case 2: // temporal
            switch(typeAdapter.getType(current.operand2)){
                case 0: { // int;
                    absdir2 = current.operand2 - typeAdapter.integerT.min;
                    oper2 = mTemporal->getTemporalInteger(absdir2);
                } break;
                case 1: { // dec;
                    absdir2 = current.operand2 - typeAdapter.decimalT.min;
                    oper2 = mTemporal->getTemporalDecimal(absdir2);
                } break;
            } break;
        case 3:// constante
            switch(typeAdapter.getType(current.operand2)){
                case 0: { // int;
                    absdir2 = current.operand2;
                    oper2 = stoi(constantTable.getValue(absdir2));
                } break;
                case 1: { // dec;
                    absdir2 = current.operand2;
                    oper2 = stod(constantTable.getValue(absdir2));
                } break;
            } break;
    }

    switch(typeAdapter.getScope(current.result)){
        case 0:  // global
            switch(typeAdapter.getType(current.result)){
                case 0: { // int;
                    absdir3 = current.result - typeAdapter.integerG.min;
                    mGlobal.setInteger(absdir3, oper1 * oper2);
                } break;
                case 1: { // dec;
                    absdir3 = current.result - typeAdapter.decimalG.min;
                    mGlobal.setDecimal(absdir3, oper1 * oper2);
                } break;
            } break;
        case 1:  // local
            switch(typeAdapter.getType(current.result)){
                case 0: { // int;
                    absdir3 = current.result - typeAdapter.integerL.min;
                    mLocal->setInteger(absdir3, oper1 * oper2);
                } break;
                case 1: { // dec;
                    absdir3 = current.result - typeAdapter.decimalL.min;
                    mLocal->setDecimal(absdir3, oper1 * oper2);
                } break;
            } break;
        case 2: // temporal
            switch(typeAdapter.getType(current.result)){
                case 0: { // int;
                    absdir3 = current.result - typeAdapter.integerT.min;
                    mTemporal->setTemporalInteger(absdir3, oper1 * oper2);
                } break;
                case 1: { // dec;
                    absdir3 = current.result - typeAdapter.decimalT.min;
                    mTemporal->setTemporalDecimal(absdir3, oper1 * oper2);
                } break;
            } break;
    }
}

void VirtualMachine::modulo(){}

void VirtualMachine::greaterThan(){
    double oper1, oper2;
    int absdir1, absdir2, absdir3;

    // If the operands are pointers, get their directions
    getPointerContent(current.operand1);
    getPointerContent(current.operand2);

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
                    oper1 = mLocal->getInteger(absdir1);
                } break;
                case 1: { // dec;
                    absdir1 = current.operand1 - typeAdapter.decimalL.min;
                    oper1 = mLocal->getDecimal(absdir1);
                } break;
            } break;
        case 2: // temporal
            switch(typeAdapter.getType(current.operand1)){
                case 0: { // int;
                    absdir1 = current.operand1 - typeAdapter.integerT.min;
                    oper1 = mTemporal->getTemporalInteger(absdir1);
                } break;
                case 1: { // dec;
                    absdir1 = current.operand1 - typeAdapter.decimalT.min;
                    oper1 = mTemporal->getTemporalDecimal(absdir1);
                } break;
            } break;
        case 3:// constante
            switch(typeAdapter.getType(current.operand1)){
                case 0: { // int;
                    absdir1 = current.operand1;
                    oper1 = stoi(constantTable.getValue(absdir1));
                } break;
                case 1: { // dec;
                    absdir1 = current.operand1;
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
                    oper2 = mLocal->getInteger(absdir2);
                } break;
                case 1: { // dec;
                    absdir2 = current.operand2 - typeAdapter.decimalL.min;
                    oper2 = mLocal->getDecimal(absdir2);
                } break;
            } break;
        case 2: // temporal
            switch(typeAdapter.getType(current.operand2)){
                case 0: { // int;
                    absdir2 = current.operand2 - typeAdapter.integerT.min;
                    oper2 = mTemporal->getTemporalInteger(absdir2);
                } break;
                case 1: { // dec;
                    absdir2 = current.operand2 - typeAdapter.decimalT.min;
                    oper2 = mTemporal->getTemporalDecimal(absdir2);
                } break;
            } break;
        case 3:// constante
            switch(typeAdapter.getType(current.operand2)){
                case 0: { // int;
                    absdir2 = current.operand2;
                    oper2 = stoi(constantTable.getValue(absdir2));
                } break;
                case 1: { // dec;
                    absdir2 = current.operand2;
                    oper2 = stod(constantTable.getValue(absdir2));
                } break;
            } break;
    }

    switch(typeAdapter.getScope(current.result)){
        case 0:  // global
            switch(typeAdapter.getType(current.result)){
                case 4: { // flag;
                    absdir3 = current.result - typeAdapter.flagG.min;
                    mGlobal.setFlag(absdir3, oper1 > oper2);
                } break;
            } break;
        case 1:  // local
            switch(typeAdapter.getType(current.result)){
                case 4: { // flag;
                    absdir3 = current.result - typeAdapter.flagL.min;
                    mLocal->setFlag(absdir3, oper1 > oper2);
                } break;
            } break;
        case 2: // temporal
            switch(typeAdapter.getType(current.result)){
                case 4: { // flag
                    absdir3 = current.result - typeAdapter.flagT.min;
                    mTemporal->setTemporalFlag(absdir3, oper1 > oper2);
                } break;
            } break;
    }
} //

void VirtualMachine::lessThan(){
    double oper1, oper2;
    int absdir1, absdir2, absdir3;

    // If the operands are pointers, get their directions
    getPointerContent(current.operand1);
    getPointerContent(current.operand2);

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
                    oper1 = mLocal->getInteger(absdir1);
                } break;
                case 1: { // dec;
                    absdir1 = current.operand1 - typeAdapter.decimalL.min;
                    oper1 = mLocal->getDecimal(absdir1);
                } break;
            } break;
        case 2: // temporal
            switch(typeAdapter.getType(current.operand1)){
                case 0: { // int;
                    absdir1 = current.operand1 - typeAdapter.integerT.min;
                    oper1 = mTemporal->getTemporalInteger(absdir1);
                } break;
                case 1: { // dec;
                    absdir1 = current.operand1 - typeAdapter.decimalT.min;
                    oper1 = mTemporal->getTemporalDecimal(absdir1);
                } break;
            } break;
        case 3:// constante
            switch(typeAdapter.getType(current.operand1)){
                case 0: { // int;
                    absdir1 = current.operand1;
                    oper1 = stoi(constantTable.getValue(absdir1));
                } break;
                case 1: { // dec;
                    absdir1 = current.operand1;
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
                    oper2 = mLocal->getInteger(absdir2);
                } break;
                case 1: { // dec;
                    absdir2 = current.operand2 - typeAdapter.decimalL.min;
                    oper2 = mLocal->getDecimal(absdir2);
                } break;
            } break;
        case 2: // temporal
            switch(typeAdapter.getType(current.operand2)){
                case 0: { // int;
                    absdir2 = current.operand2 - typeAdapter.integerT.min;
                    oper2 = mTemporal->getTemporalInteger(absdir2);
                } break;
                case 1: { // dec;
                    absdir2 = current.operand2 - typeAdapter.decimalT.min;
                    oper2 = mTemporal->getTemporalDecimal(absdir2);
                } break;
            } break;
        case 3:// constante
            switch(typeAdapter.getType(current.operand2)){
                case 0: { // int;
                    absdir2 = current.operand2;
                    oper2 = stoi(constantTable.getValue(absdir2));
                } break;
                case 1: { // dec;
                    absdir2 = current.operand2;
                    oper2 = stod(constantTable.getValue(absdir2));
                } break;
            } break;
    }

    switch(typeAdapter.getScope(current.result)){
        case 0:  // global
            switch(typeAdapter.getType(current.result)){
                case 4: { // flag;
                    absdir3 = current.result - typeAdapter.flagG.min;
                    mGlobal.setFlag(absdir3, oper1 < oper2);
                } break;
            } break;
        case 1:  // local
            switch(typeAdapter.getType(current.result)){
                case 4: { // flag;
                    absdir3 = current.result - typeAdapter.flagL.min;
                    mLocal->setFlag(absdir3, oper1 < oper2);
                } break;
            } break;
        case 2: // temporal
            switch(typeAdapter.getType(current.result)){
                case 4: { // flag
                    absdir3 = current.result - typeAdapter.flagT.min;
                    mTemporal->setTemporalFlag(absdir3, oper1 < oper2);
                } break;
            } break;
    }
} //

void VirtualMachine::equal(){
    double dOper1;
    char cOper1;
    bool bOper1;
    int absdir1, absdir3;

    getPointerContent(current.operand1);
    getPointerContent(current.result);

    switch(typeAdapter.getScope(current.operand1)){
        case 0:  // global
            switch(typeAdapter.getType(current.operand1)){
                case 0: { // int;
                    absdir1 = current.operand1 - typeAdapter.integerG.min;
                    dOper1 = mGlobal.getInteger(absdir1);
                } break;
                case 1: { // dec;
                    absdir1 = current.operand1 - typeAdapter.decimalG.min;
                    dOper1 = mGlobal.getDecimal(absdir1);
                } break;
                case 2: // text;
                break;
                case 3: { // char;
                    absdir1 = current.operand1 - typeAdapter.characterG.min;
                    cOper1 = mGlobal.getCharacter(absdir1);
                } break;
                case 4: { // flag;
                    absdir1 = current.operand1 - typeAdapter.flagG.min;
                    bOper1 = mGlobal.getFlag(absdir1);
                } break;
            } break;
        case 1: // local
            switch(typeAdapter.getType(current.operand1)){
                case 0: { // int;
                    absdir1 = current.operand1 - typeAdapter.integerL.min;
                    dOper1 = mLocal->getInteger(absdir1);
                } break;
                case 1: { // dec;
                    absdir1 = current.operand1 - typeAdapter.decimalL.min;
                    dOper1 = mLocal->getDecimal(absdir1);
                } break;
                case 2: // text;
                break;
                case 3: { // char;
                    absdir1 = current.operand1 - typeAdapter.characterL.min;
                    cOper1 = mLocal->getCharacter(absdir1);
                } break;
                case 4: { // flag;
                    absdir1 = current.operand1 - typeAdapter.flagL.min;
                    bOper1 = mLocal->getFlag(absdir1);
                } break;
            } break;
        case 2: // temporal
            switch(typeAdapter.getType(current.operand1)){
                case 0: { // int;
                    absdir1 = current.operand1 - typeAdapter.integerT.min;
                    dOper1 = mTemporal->getTemporalInteger(absdir1);
                } break;
                case 1: { // dec;
                    absdir1 = current.operand1 - typeAdapter.decimalT.min;
                    dOper1 = mTemporal->getTemporalDecimal(absdir1);
                } break;
                case 2: // text;
                break;
                case 3: { // char;
                    absdir1 = current.operand1 - typeAdapter.characterT.min;
                    cOper1 = mTemporal->getTemporalCharacter(absdir1);
                } break;
                case 4: { // flag;
                    absdir1 = current.operand1 - typeAdapter.flagT.min;
                    bOper1 = mTemporal->getTemporalFlag(absdir1);
                } break;
            } break;
        case 3: // constante
            switch(typeAdapter.getType(current.operand1)){
                case 0: { // int;
                    dOper1 = stoi(constantTable.getValue(current.operand1));
                } break;
                case 1: { // dec;
                    dOper1 = stod(constantTable.getValue(current.operand1));
                } break;
                case 2: { // text;
                    // TODO: result = constantTable.getValue(current.result);
                } break;
                case 3: { // char;
                    cOper1 = constantTable.getValue(current.operand1)[0];
                } break;
                case 4: { // flag;
                    if (constantTable.getValue(current.operand1) == "true") {
                        bOper1 = true;
                    }
                    else {
                        bOper1 = false;
                    }
                } break;
            } break;
    }

    switch(typeAdapter.getScope(current.result)){
        case 0:  // global
            switch(typeAdapter.getType(current.result)){
                case 0: { // int;
                    absdir3 = current.result - typeAdapter.integerG.min;
                    mGlobal.setInteger(absdir3, dOper1);
                } break;
                case 1: { // dec;
                    absdir3 = current.result - typeAdapter.decimalG.min;
                    mGlobal.setDecimal(absdir3, dOper1);
                } break;
                case 2: // text;
                break;
                case 3: { // char;
                    absdir3 = current.result - typeAdapter.characterG.min;
                    mGlobal.setCharacter(absdir3, cOper1);
                } break;
                case 4: { // flag;
                    absdir3 = current.result - typeAdapter.flagG.min;
                    mGlobal.setFlag(absdir3, bOper1);
                } break;
            } break;
        case 1: // local
            switch(typeAdapter.getType(current.result)){
                case 0: { // int;
                    absdir3 = current.result - typeAdapter.integerL.min;
                    mLocal->setInteger(absdir3, dOper1);
                } break;
                case 1: { // dec;
                    absdir3 = current.result - typeAdapter.decimalL.min;
                    mLocal->setDecimal(absdir3, dOper1);
                } break;
                case 2: // text;
                break;
                case 3: { // char;
                    absdir3 = current.result - typeAdapter.characterL.min;
                    mLocal->setCharacter(absdir3, cOper1);
                } break;
                case 4: { // flag;
                    absdir3 = current.result - typeAdapter.flagL.min;
                    mLocal->setFlag(absdir3, bOper1);
                } break;
            } break;
        case 2: // temporal
            switch(typeAdapter.getType(current.result)){
                case 0: { // int;
                    absdir3 = current.result - typeAdapter.integerT.min;
                    mTemporal->setTemporalInteger(absdir3, dOper1);
                } break;
                case 1: { // dec;
                    absdir3 = current.result - typeAdapter.decimalT.min;
                    mTemporal->setTemporalDecimal(absdir3, dOper1);
                } break;
                case 2: // text;
                break;
                case 3: { // char;
                    absdir3 = current.result - typeAdapter.characterT.min;
                    mTemporal->setTemporalCharacter(absdir3, cOper1);
                } break;
                case 4: { // flag;
                    absdir3 = current.result - typeAdapter.flagT.min;
                    mTemporal->setTemporalFlag(absdir3, bOper1);
                } break;
            } break;
    }
}

void VirtualMachine::and_(){
    bool oper1, oper2;
    int absdir1, absdir2, absdir3;

    // If the operands are pointers, get their directions
    getPointerContent(current.operand1);
    getPointerContent(current.operand2);

    switch(typeAdapter.getScope(current.operand1)){
        case 0:  // global
            switch(typeAdapter.getType(current.operand1)){
                case 4: { // flag;
                    absdir1 = current.operand1 - typeAdapter.flagG.min;
                    oper1 = mGlobal.getFlag(absdir1);
                } break;
            } break;
        case 1: // local
            switch(typeAdapter.getType(current.operand1)){
                case 4: { // flag;
                    absdir1 = current.operand1 - typeAdapter.flagL.min;
                    oper1 = mLocal->getFlag(absdir1);
                } break;
            } break;
        case 2: // temporal
            switch(typeAdapter.getType(current.operand1)){
                case 4: { // flag;
                    absdir1 = current.operand1 - typeAdapter.flagT.min;
                    oper1 = mTemporal->getTemporalFlag(absdir1);
                } break;
            } break;
        case 3: // constante
            switch(typeAdapter.getType(current.operand1)){
                case 4: { // flag;
                     if(constantTable.getValue(current.operand1) == "true"){
                        oper1 = true;
                     }
                     else{
                        oper1 = false;
                     }
                } break;
            } break;
    }

    switch(typeAdapter.getScope(current.operand2)){
        case 0:  // global
            switch(typeAdapter.getType(current.operand2)){
                case 4: { // flag;
                    absdir2 = current.operand2 - typeAdapter.flagG.min;
                    oper2 = mGlobal.getFlag(absdir2);
                } break;
            } break;
        case 1: // local
            switch(typeAdapter.getType(current.operand2)){
                case 4: { // flag;
                    absdir2 = current.operand2 - typeAdapter.flagL.min;
                    oper2 = mLocal->getFlag(absdir2);
                } break;
            } break;
        case 2: // temporal
            switch(typeAdapter.getType(current.operand2)){
                case 4: { // flag;
                    absdir2 = current.operand2 - typeAdapter.flagT.min;
                    oper2 = mTemporal->getTemporalFlag(absdir2);
                } break;
            } break;
        case 3: // constante
            switch(typeAdapter.getType(current.operand2)){
                case 4: { // flag;
                     if(constantTable.getValue(current.operand2) == "true"){
                        oper2 = true;
                     }
                     else{
                        oper2 = false;
                     }
                } break;
            } break;
    }

    switch(typeAdapter.getScope(current.result)){
        case 0:  // global
            switch(typeAdapter.getType(current.result)){
                case 4: { // flag;
                    absdir3 = current.result - typeAdapter.flagG.min;
                    mGlobal.setFlag(absdir3, oper1 && oper2);
                } break;
            } break;
        case 1: // local
            switch(typeAdapter.getType(current.result)){
                case 4: { // flag;
                    absdir3 = current.result - typeAdapter.flagL.min;
                    mLocal->setFlag(absdir3, oper1 && oper2);
                } break;
            } break;
        case 2: // temporal
            switch(typeAdapter.getType(current.result)){
                case 4:{ // flag;
                    absdir3 = current.result - typeAdapter.flagT.min;
                    mTemporal->setTemporalFlag(absdir3, oper1 && oper2);
                } break;
            } break;
    }
} //

void VirtualMachine::or_(){
    bool oper1, oper2;
    int absdir1, absdir2, absdir3;

    // If the operands are pointers, get their directions
    getPointerContent(current.operand1);
    getPointerContent(current.operand2);

    switch(typeAdapter.getScope(current.operand1)){
        case 0:  // global
            switch(typeAdapter.getType(current.operand1)){
                case 4: { // flag;
                    absdir1 = current.operand1 - typeAdapter.flagG.min;
                    oper1 = mGlobal.getFlag(absdir1);
                } break;
            } break;
        case 1: // local
            switch(typeAdapter.getType(current.operand1)){
                case 4: { // flag;
                    absdir1 = current.operand1 - typeAdapter.flagL.min;
                    oper1 = mLocal->getFlag(absdir1);
                } break;
            } break;
        case 2: // temporal
            switch(typeAdapter.getType(current.operand1)){
                case 4: { // flag;
                    absdir1 = current.operand1 - typeAdapter.flagT.min;
                    oper1 = mTemporal->getTemporalFlag(absdir1);
                } break;
            } break;
        case 3: // constante
            switch(typeAdapter.getType(current.operand1)){
                case 4: { // flag;
                     if(constantTable.getValue(current.operand1) == "true"){
                        oper1 = true;
                     }
                     else{
                        oper1 = false;
                     }
                } break;
            } break;
    }

    switch(typeAdapter.getScope(current.operand2)){
        case 0:  // global
            switch(typeAdapter.getType(current.operand2)){
                case 4: { // flag;
                    absdir2 = current.operand2 - typeAdapter.flagG.min;
                    oper2 = mGlobal.getFlag(absdir2);
                } break;
            } break;
        case 1: // local
            switch(typeAdapter.getType(current.operand2)){
                case 4: { // flag;
                    absdir2 = current.operand2 - typeAdapter.flagL.min;
                    oper2 = mLocal->getFlag(absdir2);
                } break;
            } break;
        case 2: // temporal
            switch(typeAdapter.getType(current.operand2)){
                case 4: { // flag;
                    absdir2 = current.operand2 - typeAdapter.flagT.min;
                    oper2 = mTemporal->getTemporalFlag(absdir2);
                } break;
            } break;
        case 3: // constante
            switch(typeAdapter.getType(current.operand2)){
                case 4: { // flag;
                     if(constantTable.getValue(current.operand2) == "true"){
                        oper2 = true;
                     }
                     else{
                        oper2 = false;
                     }
                } break;
            } break;
    }

    switch(typeAdapter.getScope(current.result)){
        case 0:  // global
            switch(typeAdapter.getType(current.result)){
                case 4: { // flag;
                    absdir3 = current.result - typeAdapter.flagG.min;
                    mGlobal.setFlag(absdir3, oper1 || oper2);
                } break;
            } break;
        case 1: // local
            switch(typeAdapter.getType(current.result)){
                case 4: { // flag;
                    absdir3 = current.result - typeAdapter.flagL.min;
                    mLocal->setFlag(absdir3, oper1 || oper2);
                } break;
            } break;
        case 2: // temporal
            switch(typeAdapter.getType(current.result)){
                case 4:{// flag;
                    absdir3 = current.result - typeAdapter.flagT.min;
                    mTemporal->setTemporalFlag(absdir3, oper1 || oper2);
                } break;
            } break;
    }
} //

void VirtualMachine::not_(){
    bool oper1;
    int absdir1, absdir3;

    // If the operands are pointers, get their directions
    getPointerContent(current.operand2);

    switch(typeAdapter.getScope(current.operand2)){
        case 0:  // global
            switch(typeAdapter.getType(current.operand2)){
                case 4: { // flag;
                    absdir1 = current.operand2 - typeAdapter.flagG.min;
                    oper1 = mGlobal.getFlag(absdir1);
                } break;
            } break;
        case 1: // local
            switch(typeAdapter.getType(current.operand2)){
                case 4: { // flag;
                    absdir1 = current.operand2 - typeAdapter.flagL.min;
                    oper1 = mLocal->getFlag(absdir1);
                } break;
            } break;
        case 2: // temporal
            switch(typeAdapter.getType(current.operand2)){
                case 4: { // flag;
                    absdir1 = current.operand2 - typeAdapter.flagT.min;
                    oper1 = mTemporal->getTemporalFlag(absdir1);
                } break;
            } break;
        case 3: // constante
            switch(typeAdapter.getType(current.operand2)){
                case 4: { // flag;
                     if(constantTable.getValue(current.operand2) == "true"){
                        oper1 = true;
                     }
                     else{
                        oper1 = false;
                     }
                } break;
            } break;
    }

    switch(typeAdapter.getScope(current.result)){
        case 0:  // global
            switch(typeAdapter.getType(current.result)){
                case 4: { // flag;
                    absdir3 = current.result - typeAdapter.flagG.min;
                    mGlobal.setFlag(absdir3, (!oper1));
                } break;
            } break;
        case 1: // local
            switch(typeAdapter.getType(current.result)){
                case 4: { // flag;
                    absdir3 = current.result - typeAdapter.flagL.min;
                    mLocal->setFlag(absdir3, (!oper1));
                } break;
            } break;
        case 2: // temporal
            switch(typeAdapter.getType(current.result)){
                case 4:{// flag;
                    absdir3 = current.result - typeAdapter.flagT.min;
                    mTemporal->setTemporalFlag(absdir3, (!oper1));
                } break;
            } break;
    }
}

void VirtualMachine::notEqualTo(){
    double oper1, oper2;
    int absdir1, absdir2, absdir3;

    // If the operands are pointers, get their directions
    getPointerContent(current.operand1);
    getPointerContent(current.operand2);

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
                case 4: { // flag;
                    absdir1 = current.operand1 - typeAdapter.flagG.min;
                    oper1 = mGlobal.getFlag(absdir1);
                } break;
            } break;
        case 1:  // local
            switch(typeAdapter.getType(current.operand1)){
                case 0: { // int;
                    absdir1 = current.operand1 - typeAdapter.integerL.min;
                    oper1 = mLocal->getInteger(absdir1);
                } break;
                case 1: { // dec;
                    absdir1 = current.operand1 - typeAdapter.decimalL.min;
                    oper1 = mLocal->getDecimal(absdir1);
                } break;
                case 4: { // flag;
                    absdir1 = current.operand1 - typeAdapter.flagL.min;
                    oper1 = mLocal->getFlag(absdir1);
                } break;
            } break;
        case 2: // temporal
            switch(typeAdapter.getType(current.operand1)){
                case 0: { // int;
                    absdir1 = current.operand1 - typeAdapter.integerT.min;
                    oper1 = mTemporal->getTemporalInteger(absdir1);
                } break;
                case 1: { // dec;
                    absdir1 = current.operand1 - typeAdapter.decimalT.min;
                    oper1 = mTemporal->getTemporalDecimal(absdir1);
                } break;
                case 4: { // flag;
                    absdir1 = current.operand1 - typeAdapter.flagT.min;
                    oper1 = mTemporal->getTemporalFlag(absdir1);
                } break;
            } break;
        case 3:// constante
            switch(typeAdapter.getType(current.operand1)){
                case 0: { // int;
                    absdir1 = current.operand1;
                    oper1 = stoi(constantTable.getValue(absdir1));
                } break;
                case 1: { // dec;
                    absdir1 = current.operand1;
                    oper1 = stod(constantTable.getValue(absdir1));
                } break;
                case 4: { // flag;
                    absdir1 = current.operand1;
                    if (constantTable.getValue(absdir1) == "true") {
                        oper1 = 1;
                    } else if (constantTable.getValue(absdir1) == "false") {
                        oper1 = 0;
                    }
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
                case 4: { // flag;
                    absdir2 = current.operand2 - typeAdapter.flagG.min;
                    oper2 = mGlobal.getFlag(absdir2);
                } break;
            } break;
        case 1:  // local
            switch(typeAdapter.getType(current.operand2)){
                case 0: { // int;
                    absdir2 = current.operand2 - typeAdapter.integerL.min;
                    oper2 = mLocal->getInteger(absdir2);
                } break;
                case 1: { // dec;
                    absdir2 = current.operand2 - typeAdapter.decimalL.min;
                    oper2 = mLocal->getDecimal(absdir2);
                } break;
                case 4: { // flag;
                    absdir2 = current.operand2 - typeAdapter.flagL.min;
                    oper2 = mLocal->getFlag(absdir2);
                } break;
            } break;
        case 2: // temporal
            switch(typeAdapter.getType(current.operand2)){
                case 0: { // int;
                    absdir2 = current.operand2 - typeAdapter.integerT.min;
                    oper2 = mTemporal->getTemporalInteger(absdir2);
                } break;
                case 1: { // dec;
                    absdir2 = current.operand2 - typeAdapter.decimalT.min;
                    oper2 = mTemporal->getTemporalDecimal(absdir2);
                } break;
                case 4: { // flag;
                    absdir2 = current.operand2 - typeAdapter.flagT.min;
                    oper2 = mTemporal->getTemporalFlag(absdir2);
                } break;
            } break;
        case 3:// constante
            switch(typeAdapter.getType(current.operand2)){
                case 0: { // int;
                    absdir2 = current.operand2;
                    oper2 = stoi(constantTable.getValue(absdir2));
                } break;
                case 1: { // dec;
                    absdir2 = current.operand2;
                    oper2 = stod(constantTable.getValue(absdir2));
                } break;
                case 4: { // flag;
                    absdir2 = current.operand2;
                    if (constantTable.getValue(absdir2) == "true") {
                        oper2 = 1;
                    } else if (constantTable.getValue(absdir2) == "false") {
                        oper2 = 0;
                    }
                } break;
            } break;
    }

    switch(typeAdapter.getScope(current.result)){
        case 0:  // global
            switch(typeAdapter.getType(current.result)){
                case 4: { // flag;
                    absdir3 = current.result - typeAdapter.flagG.min;
                    mGlobal.setFlag(absdir3, oper1 != oper2);
                } break;
            } break;
        case 1:  // local
            switch(typeAdapter.getType(current.result)){
                case 4: { // flag;
                    absdir3 = current.result - typeAdapter.flagL.min;
                    mLocal->setFlag(absdir3, oper1 != oper2);
                } break;
            } break;
        case 2: // temporal
            switch(typeAdapter.getType(current.result)){
                case 4: { // flag
                    absdir3 = current.result - typeAdapter.flagT.min;
                    mTemporal->setTemporalFlag(absdir3, oper1 != oper2);
                } break;
            } break;
    }
}

void VirtualMachine::equalTo(){
    double oper1, oper2;
    int absdir1, absdir2, absdir3;

    // If the operands are pointers, get their directions
    getPointerContent(current.operand1);
    getPointerContent(current.operand2);

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
                case 4: { // flag;
                    absdir1 = current.operand1 - typeAdapter.flagG.min;
                    oper1 = mGlobal.getFlag(absdir1);
                } break;
            } break;
        case 1:  // local
            switch(typeAdapter.getType(current.operand1)){
                case 0: { // int;
                    absdir1 = current.operand1 - typeAdapter.integerL.min;
                    oper1 = mLocal->getInteger(absdir1);
                } break;
                case 1: { // dec;
                    absdir1 = current.operand1 - typeAdapter.decimalL.min;
                    oper1 = mLocal->getDecimal(absdir1);
                } break;
                case 4: { // flag;
                    absdir1 = current.operand1 - typeAdapter.flagL.min;
                    oper1 = mLocal->getFlag(absdir1);
                } break;
            } break;
        case 2: // temporal
            switch(typeAdapter.getType(current.operand1)){
                case 0: { // int;
                    absdir1 = current.operand1 - typeAdapter.integerT.min;
                    oper1 = mTemporal->getTemporalInteger(absdir1);
                } break;
                case 1: { // dec;
                    absdir1 = current.operand1 - typeAdapter.decimalT.min;
                    oper1 = mTemporal->getTemporalDecimal(absdir1);
                } break;
                case 4: { // flag;
                    absdir1 = current.operand1 - typeAdapter.flagT.min;
                    oper1 = mTemporal->getTemporalFlag(absdir1);
                } break;
            } break;
        case 3:// constante
            switch(typeAdapter.getType(current.operand1)){
                case 0: { // int;
                    absdir1 = current.operand1;
                    oper1 = stoi(constantTable.getValue(absdir1));
                } break;
                case 1: { // dec;
                    absdir1 = current.operand1;
                    oper1 = stod(constantTable.getValue(absdir1));
                } break;
                case 4: { // flag;
                    absdir1 = current.operand1;
                    if (constantTable.getValue(absdir1) == "true") {
                        oper1 = 1;
                    } else if (constantTable.getValue(absdir1) == "false") {
                        oper1 = 0;
                    }
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
                case 4: { // flag;
                    absdir2 = current.operand2 - typeAdapter.flagG.min;
                    oper2 = mGlobal.getFlag(absdir2);
                } break;
            } break;
        case 1:  // local
            switch(typeAdapter.getType(current.operand2)){
                case 0: { // int;
                    absdir2 = current.operand2 - typeAdapter.integerL.min;
                    oper2 = mLocal->getInteger(absdir2);
                } break;
                case 1: { // dec;
                    absdir2 = current.operand2 - typeAdapter.decimalL.min;
                    oper2 = mLocal->getDecimal(absdir2);
                } break;
                case 4: { // flag;
                    absdir2 = current.operand2 - typeAdapter.flagL.min;
                    oper2 = mLocal->getFlag(absdir2);
                } break;
            } break;
        case 2: // temporal
            switch(typeAdapter.getType(current.operand2)){
                case 0: { // int;
                    absdir2 = current.operand2 - typeAdapter.integerT.min;
                    oper2 = mTemporal->getTemporalInteger(absdir2);
                } break;
                case 1: { // dec;
                    absdir2 = current.operand2 - typeAdapter.decimalT.min;
                    oper2 = mTemporal->getTemporalDecimal(absdir2);
                } break;
                case 4: { // flag;
                    absdir2 = current.operand2 - typeAdapter.flagT.min;
                    oper2 = mTemporal->getTemporalFlag(absdir2);
                } break;
            } break;
        case 3:// constante
            switch(typeAdapter.getType(current.operand2)){
                case 0: { // int;
                    absdir2 = current.operand2;
                    oper2 = stoi(constantTable.getValue(absdir2));
                } break;
                case 1: { // dec;
                    absdir2 = current.operand2;
                    oper2 = stod(constantTable.getValue(absdir2));
                } break;
                case 4: { // flag;
                    absdir2 = current.operand2;
                    if (constantTable.getValue(absdir2) == "true") {
                        oper2 = 1;
                    } else if (constantTable.getValue(absdir2) == "false") {
                        oper2 = 0;
                    }
                } break;
            } break;
    }

    switch(typeAdapter.getScope(current.result)){
        case 0:  // global
            switch(typeAdapter.getType(current.result)){
                case 4: { // flag;
                    absdir3 = current.result - typeAdapter.flagG.min;
                    mGlobal.setFlag(absdir3, oper1 == oper2);
                } break;
            } break;
        case 1:  // local
            switch(typeAdapter.getType(current.result)){
                case 4: { // flag;
                    absdir3 = current.result - typeAdapter.flagL.min;
                    mLocal->setFlag(absdir3, oper1 == oper2);
                } break;
            } break;
        case 2: // temporal
            switch(typeAdapter.getType(current.result)){
                case 4: { // flag
                    absdir3 = current.result - typeAdapter.flagT.min;
                    mTemporal->setTemporalFlag(absdir3, oper1 == oper2);
                } break;
            } break;
    }
}

void VirtualMachine::lessThanOrEqualTo(){
    double oper1, oper2;
    int absdir1, absdir2, absdir3;

    // If the operands are pointers, get their directions
    getPointerContent(current.operand1);
    getPointerContent(current.operand2);

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
                    oper1 = mLocal->getInteger(absdir1);
                } break;
                case 1: { // dec;
                    absdir1 = current.operand1 - typeAdapter.decimalL.min;
                    oper1 = mLocal->getDecimal(absdir1);
                } break;
            } break;
        case 2: // temporal
            switch(typeAdapter.getType(current.operand1)){
                case 0: { // int;
                    absdir1 = current.operand1 - typeAdapter.integerT.min;
                    oper1 = mTemporal->getTemporalInteger(absdir1);
                } break;
                case 1: { // dec;
                    absdir1 = current.operand1 - typeAdapter.decimalT.min;
                    oper1 = mTemporal->getTemporalDecimal(absdir1);
                } break;
            } break;
        case 3:// constante
            switch(typeAdapter.getType(current.operand1)){
                case 0: { // int;
                    absdir1 = current.operand1;
                    oper1 = stoi(constantTable.getValue(absdir1));
                } break;
                case 1: { // dec;
                    absdir1 = current.operand1;
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
                    oper2 = mLocal->getInteger(absdir2);
                } break;
                case 1: { // dec;
                    absdir2 = current.operand2 - typeAdapter.decimalL.min;
                    oper2 = mLocal->getDecimal(absdir2);
                } break;
            } break;
        case 2: // temporal
            switch(typeAdapter.getType(current.operand2)){
                case 0: { // int;
                    absdir2 = current.operand2 - typeAdapter.integerT.min;
                    oper2 = mTemporal->getTemporalInteger(absdir2);
                } break;
                case 1: { // dec;
                    absdir2 = current.operand2 - typeAdapter.decimalT.min;
                    oper2 = mTemporal->getTemporalDecimal(absdir2);
                } break;
            } break;
        case 3:// constante
            switch(typeAdapter.getType(current.operand2)){
                case 0: { // int;
                    absdir2 = current.operand2;
                    oper2 = stoi(constantTable.getValue(absdir2));
                } break;
                case 1: { // dec;
                    absdir2 = current.operand2;
                    oper2 = stod(constantTable.getValue(absdir2));
                } break;
            } break;
    }

    switch(typeAdapter.getScope(current.result)){
        case 0:  // global
            switch(typeAdapter.getType(current.result)){
                case 4: { // flag;
                    absdir3 = current.result - typeAdapter.flagG.min;
                    mGlobal.setFlag(absdir3, oper1 <= oper2);
                } break;
            } break;
        case 1:  // local
            switch(typeAdapter.getType(current.result)){
                case 4: { // flag;
                    absdir3 = current.result - typeAdapter.flagL.min;
                    mLocal->setFlag(absdir3, oper1 <= oper2);
                } break;
            } break;
        case 2: // temporal
            switch(typeAdapter.getType(current.result)){
                case 4: { // flag
                    absdir3 = current.result - typeAdapter.flagT.min;
                    mTemporal->setTemporalFlag(absdir3, oper1 <= oper2);
                } break;
            } break;
    }
} //

void VirtualMachine::greaterThanOrEqualTo(){
    double oper1, oper2;
    int absdir1, absdir2, absdir3;

    // If the operands are pointers, get their directions
    getPointerContent(current.operand1);
    getPointerContent(current.operand2);

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
                    oper1 = mLocal->getInteger(absdir1);
                } break;
                case 1: { // dec;
                    absdir1 = current.operand1 - typeAdapter.decimalL.min;
                    oper1 = mLocal->getDecimal(absdir1);
                } break;
            } break;
        case 2: // temporal
            switch(typeAdapter.getType(current.operand1)){
                case 0: { // int;
                    absdir1 = current.operand1 - typeAdapter.integerT.min;
                    oper1 = mTemporal->getTemporalInteger(absdir1);
                } break;
                case 1: { // dec;
                    absdir1 = current.operand1 - typeAdapter.decimalT.min;
                    oper1 = mTemporal->getTemporalDecimal(absdir1);
                } break;
            } break;
        case 3:// constante
            switch(typeAdapter.getType(current.operand1)){
                case 0: { // int;
                    absdir1 = current.operand1;
                    oper1 = stoi(constantTable.getValue(absdir1));
                } break;
                case 1: { // dec;
                    absdir1 = current.operand1;
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
                    oper2 = mLocal->getInteger(absdir2);
                } break;
                case 1: { // dec;
                    absdir2 = current.operand2 - typeAdapter.decimalL.min;
                    oper2 = mLocal->getDecimal(absdir2);
                } break;
            } break;
        case 2: // temporal
            switch(typeAdapter.getType(current.operand2)){
                case 0: { // int;
                    absdir2 = current.operand2 - typeAdapter.integerT.min;
                    oper2 = mTemporal->getTemporalInteger(absdir2);
                } break;
                case 1: { // dec;
                    absdir2 = current.operand2 - typeAdapter.decimalT.min;
                    oper2 = mTemporal->getTemporalDecimal(absdir2);
                } break;
            } break;
        case 3:// constante
            switch(typeAdapter.getType(current.operand2)){
                case 0: { // int;
                    absdir2 = current.operand2;
                    oper2 = stoi(constantTable.getValue(absdir2));
                } break;
                case 1: { // dec;
                    absdir2 = current.operand2;
                    oper2 = stod(constantTable.getValue(absdir2));
                } break;
            } break;
    }

    switch(typeAdapter.getScope(current.result)){
        case 0:  // global
            switch(typeAdapter.getType(current.result)){
                case 4: { // flag;
                    absdir3 = current.result - typeAdapter.flagG.min;
                    mGlobal.setFlag(absdir3, oper1 >= oper2);
                } break;
            } break;
        case 1:  // local
            switch(typeAdapter.getType(current.result)){
                case 4: { // flag;
                    absdir3 = current.result - typeAdapter.flagL.min;
                    mLocal->setFlag(absdir3, oper1 >= oper2);
                } break;
            } break;
        case 2: // temporal
            switch(typeAdapter.getType(current.result)){
                case 4: { // flag
                    absdir3 = current.result - typeAdapter.flagT.min;
                    mTemporal->setTemporalFlag(absdir3, oper1 >= oper2);
                } break;
            } break;
    }
} //

void VirtualMachine::print(){
    string result;
    int absdir;

    getPointerContent(current.result);

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
                    result = to_string(mLocal->getInteger(absdir));
                } break;
                case 1: { // dec;
                    absdir = current.result - typeAdapter.decimalL.min;
                    result = to_string(mLocal->getDecimal(absdir));
                } break;
                case 2: // text;
                break;
                case 3: { // char;
                    absdir = current.result - typeAdapter.characterL.min;
                    result = mLocal->getCharacter(absdir);
                } break;
                case 4: { // flag;
                    absdir = current.result - typeAdapter.flagL.min;
                    if(mLocal->getFlag(absdir) == 1){
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
                    result = to_string(mTemporal->getTemporalInteger(absdir));
                } break;
                case 1: { // dec;
                    absdir = current.result - typeAdapter.decimalT.min;
                    result = to_string(mTemporal->getTemporalDecimal(absdir));
                } break;
                case 2: // text;
                break;
                case 3: { // char;
                    absdir = current.result - typeAdapter.characterT.min;
                    result = mTemporal->getTemporalCharacter(absdir);
                } break;
                case 4: { // flag;
                    absdir = current.result - typeAdapter.flagT.min;
                    if (mTemporal->getTemporalFlag(absdir) == 1){
                        result = "True";
                    }
                    else{
                        result = "False";
                    }
                } break;
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
    cout<<result;
    if (program[cQuad + 1].oper == Ops::Floor) {
        cout<<endl;
    }
}

void VirtualMachine::read(){
    int absdir;

    // If the operands are pointers, get their directions
    getPointerContent(current.result);

    switch(typeAdapter.getScope(current.result)){
        case 0:  // global
            switch(typeAdapter.getType(current.result)){
                case 0: { // int;
                    int input;
                    if (scanf("%d", &input)) {
                        absdir = current.result - typeAdapter.integerG.min;
                        mGlobal.setInteger(absdir, input);
                    } else {
                        cout<<"Incorrect input"<<endl;
                        cQuad = program.size() - 2;
                    }
                } break;
                case 1: { // dec;
                    float input;
                    if (scanf("%f", &input)) {
                        absdir = current.result - typeAdapter.decimalG.min;
                        mGlobal.setDecimal(absdir, input);
                    } else {
                        cout<<"Incorrect input"<<endl;
                        cQuad = program.size() - 2;
                    }
                } break;
                case 2: // text;
                break;
                case 3: { // char;
                    char input;
                    if (scanf("%c", &input)) {
                        absdir = current.result - typeAdapter.characterG.min;
                        mGlobal.setCharacter(absdir, input);
                    } else {
                        cout<<"Incorrect input"<<endl;
                        cQuad = program.size() - 2;
                    }
                } break;
                case 4: { // flag;
                    string input;
                    int result;
                    if (scanf("%s", &input[0])) {
                        absdir = current.result - typeAdapter.flagG.min;
                        if(input == "true" || input == "True"){
                            result = 1;
                        }
                        else if (input == "false" || input == "False") {
                            result = 0;
                        }
                        mGlobal.setFlag(absdir, result);
                    } else {
                        cout<<"Incorrect input"<<endl;
                        cQuad = program.size() - 2;
                    }
                } break;
            } break;
        case 1: // local
            switch(typeAdapter.getType(current.result)){
                case 0: { // int;
                    int input;
                    if (scanf("%d", &input)) {
                        absdir = current.result - typeAdapter.integerL.min;
                        mLocal->setInteger(absdir, input);
                    } else {
                        cout<<"Incorrect input"<<endl;
                        cQuad = program.size() - 2;
                    }
                } break;
                case 1: { // dec;
                    float input;
                    if (scanf("%f", &input)) {
                        absdir = current.result - typeAdapter.decimalL.min;
                        mLocal->setDecimal(absdir, input);
                    } else {
                        cout<<"Incorrect input"<<endl;
                        cQuad = program.size() - 2;
                    }
                } break;
                case 2: // text;
                break;
                case 3: { // char;
                    char input;
                    if (scanf("%c", &input)) {
                        absdir = current.result - typeAdapter.characterL.min;
                        mLocal->setCharacter(absdir, input);
                    } else {
                        cout<<"Incorrect input"<<endl;
                        cQuad = program.size() - 2;
                    }
                } break;
                case 4: { // flag;
                    string input;
                    int result;
                    if (scanf("%s", &input[0])) {
                        absdir = current.result - typeAdapter.flagL.min;
                        if(input == "true" || input == "True"){
                            result = 1;
                        }
                        else if (input == "false" || input == "False") {
                            result = 0;
                        }
                        mLocal->setFlag(absdir, result);
                    } else {
                        cout<<"Incorrect input"<<endl;
                        cQuad = program.size() - 2;
                    }
                } break;
            } break;
        case 2: // temporal
            switch(typeAdapter.getType(current.result)){
                case 0: { // int;
                    int input;
                    if (scanf("%d", &input)) {
                        absdir = current.result - typeAdapter.integerG.min;
                        mTemporal->setTemporalInteger(absdir, input);
                    } else {
                        cout<<"Incorrect input"<<endl;
                        cQuad = program.size() - 2;
                    }
                } break;
                case 1: { // dec;
                    float input;
                    if (scanf("%f", &input)) {
                        absdir = current.result - typeAdapter.decimalG.min;
                        mTemporal->setTemporalDecimal(absdir, input);
                    } else {
                        cout<<"Incorrect input"<<endl;
                        cQuad = program.size() - 2;
                    }
                } break;
                case 2: // text;
                break;
                case 3: { // char;
                    char input;
                    if (scanf("%c", &input)) {
                        absdir = current.result - typeAdapter.characterG.min;
                        mTemporal->setTemporalCharacter(absdir, input);
                    } else {
                        cout<<"Incorrect input"<<endl;
                        cQuad = program.size() - 2;
                    }
                } break;
                case 4: { // flag;
                    string input;
                    int result;
                    if (scanf("%s", &input[0])) {
                        absdir = current.result - typeAdapter.flagG.min;
                        if(input == "true" || input == "True"){
                            result = 1;
                        }
                        else if (input == "false" || input == "False") {
                            result = 0;
                        }
                        mTemporal->setTemporalFlag(absdir, result);
                    } else {
                        cout<<"Incorrect input"<<endl;
                        cQuad = program.size() - 2;
                    }
                } break;
            } break;
        defualt: cout<<"Scope not found"<<endl;
    }
}

void VirtualMachine::floor(){}

void VirtualMachine::goto_(){
    cQuad = current.result - 1;
}

void VirtualMachine::gotoTrue(){
    int oper1, absdir1;

    // If the operands are pointers, get their directions
    getPointerContent(current.operand1);

    switch(typeAdapter.getScope(current.operand1)){
        case 0:  // global
            switch(typeAdapter.getType(current.operand1)){
                case 4: { // flag;
                    absdir1 = current.operand1 - typeAdapter.flagG.min;
                    oper1 = mGlobal.getFlag(absdir1);
                } break;
            } break;
        case 1:  // local
            switch(typeAdapter.getType(current.operand1)){
                case 4: { // flag;
                    absdir1 = current.operand1 - typeAdapter.flagL.min;
                    oper1 = mLocal->getFlag(absdir1);
                } break;
            } break;
        case 2: // temporal
            switch(typeAdapter.getType(current.operand1)){
                case 4: { // flag;
                    absdir1 = current.operand1 - typeAdapter.flagT.min;
                    oper1 = mTemporal->getTemporalFlag(absdir1);
                } break;
            } break;
        case 3:// constante
            switch(typeAdapter.getType(current.operand1)){
                case 4: { // flag;
                    absdir1 = current.operand1 - typeAdapter.integerConstant.min;
                    if (constantTable.getValue(absdir1) == "true") {
                        oper1 = 1;
                    }
                    else {
                        oper1 = 0;
                    }
                } break;
            } break;
    }

    if (oper1 == 1) {
        cQuad = current.result - 1;
    }
}

void VirtualMachine::gotoFalse(){
    int oper1, absdir1;

    // If the operands are pointers, get their directions
    getPointerContent(current.operand1);

    switch(typeAdapter.getScope(current.operand1)){
        case 0:  // global
            switch(typeAdapter.getType(current.operand1)){
                case 4: { // flag;
                    absdir1 = current.operand1 - typeAdapter.flagG.min;
                    oper1 = mGlobal.getFlag(absdir1);
                } break;
            } break;
        case 1:  // local
            switch(typeAdapter.getType(current.operand1)){
                case 4: { // flag;
                    absdir1 = current.operand1 - typeAdapter.flagL.min;
                    oper1 = mLocal->getFlag(absdir1);
                } break;
            } break;
        case 2: // temporal
            switch(typeAdapter.getType(current.operand1)){
                case 4: { // flag;
                    absdir1 = current.operand1 - typeAdapter.flagT.min;
                    oper1 = mTemporal->getTemporalFlag(absdir1);
                } break;
            } break;
        case 3:// constante
            switch(typeAdapter.getType(current.operand1)){
                case 4: { // flag;
                    absdir1 = current.operand1 - typeAdapter.integerConstant.min;
                    if (constantTable.getValue(absdir1) == "true") {
                        oper1 = 1;
                    }
                    else {
                        oper1 = 0;
                    }
                } break;
            } break;
    }

    if (oper1 == 0) {
        cQuad = current.result - 1;
    }
}

void VirtualMachine::era(){
    auto variableCount = functionTable.getFunction(current.operand1).
                                            variableCounts;

    // Expand memory
    Memory function;

    function.changeVariables(
            variableCount["integerL"],
            variableCount["decimalL"],
            variableCount["characterL"],
            variableCount["flagL"]);
    function.changeVariablesTemporal(
            variableCount["integerT"],
            variableCount["decimalT"],
            variableCount["characterT"],
            variableCount["flagT"]);
    function.initialize();
    function.initializeTemporal();

    // Push function into stack
    sMemory.push(function);

    // Clean params
    integerParams.clear();
    decimalParams.clear();
    textParams.clear();
    characterParams.clear();
    flagParams.clear();
}
void VirtualMachine::param(){
    int ioper1;
    float foper1;
    string soper1;
    char coper1;
    bool boper1;
    int absdir1, absdir2, absdir3;

    // If the operands are pointers, get their directions
    getPointerContent(current.operand1);

    switch(typeAdapter.getScope(current.operand1)){
        case 0:  // global
            switch(typeAdapter.getType(current.operand1)){
                case 0: { // int;
                    absdir1 = current.operand1 - typeAdapter.integerG.min;
                    ioper1 = mGlobal.getInteger(absdir1);
                    integerParams.push_back(ioper1);
                } break;
                case 1: { // dec;
                    absdir1 = current.operand1 - typeAdapter.decimalG.min;
                    foper1 = mGlobal.getDecimal(absdir1);
                    decimalParams.push_back(foper1);
                } break;
                case 2: { // int;
                } break;
                case 3: { // char;
                    absdir1 = current.operand1 - typeAdapter.characterG.min;
                    coper1 = mGlobal.getCharacter(absdir1);
                    characterParams.push_back(coper1);
                } break;
                case 4: { // int;
                    absdir1 = current.operand1 - typeAdapter.flagG.min;
                    boper1 = mGlobal.getFlag(absdir1);
                    flagParams.push_back(foper1);
                } break;
            } break;
        case 1:  // local
            switch(typeAdapter.getType(current.operand1)){
                case 0: { // int;
                    absdir1 = current.operand1 - typeAdapter.integerL.min;
                    ioper1 = mLocal->getInteger(absdir1);
                    integerParams.push_back(ioper1);
                } break;
                case 1: { // dec;
                    absdir1 = current.operand1 - typeAdapter.decimalL.min;
                    foper1 = mLocal->getDecimal(absdir1);
                    decimalParams.push_back(foper1);
                } break;
                case 2: { // int;
                } break;
                case 3: { // char;
                    absdir1 = current.operand1 - typeAdapter.characterL.min;
                    coper1 = mLocal->getCharacter(absdir1);
                    characterParams.push_back(coper1);
                } break;
                case 4: { // int;
                    absdir1 = current.operand1 - typeAdapter.flagL.min;
                    boper1 = mLocal->getFlag(absdir1);
                    flagParams.push_back(foper1);
                } break;
            } break;
        case 2: // temporal
            switch(typeAdapter.getType(current.operand1)){
                case 0: { // int;
                    absdir1 = current.operand1 - typeAdapter.integerT.min;
                    ioper1 = mTemporal->getTemporalInteger(absdir1);
                    integerParams.push_back(ioper1);
                } break;
                case 1: { // dec;
                    absdir1 = current.operand1 - typeAdapter.decimalT.min;
                    foper1 = mTemporal->getTemporalDecimal(absdir1);
                    decimalParams.push_back(foper1);
                } break;
                case 2: { // int;
                } break;
                case 3: { // char;
                    absdir1 = current.operand1 - typeAdapter.characterT.min;
                    coper1 = mTemporal->getTemporalCharacter(absdir1);
                    characterParams.push_back(coper1);
                } break;
                case 4: { // int;
                    absdir1 = current.operand1 - typeAdapter.flagT.min;
                    boper1 = mTemporal->getTemporalFlag(absdir1);
                    flagParams.push_back(foper1);
                } break;
            } break;
        case 3:// constante
            switch(typeAdapter.getType(current.operand1)){
                case 0: { // int;
                    absdir1 = current.operand1;
                    ioper1 = stoi(constantTable.getValue(absdir1));
                    integerParams.push_back(ioper1);
                } break;
                case 1: { // dec;
                    absdir1 = current.operand1;
                    foper1 = stod(constantTable.getValue(absdir1));
                    decimalParams.push_back(foper1);
                } break;
                case 2: { // int;
                } break;
                case 3: { // char;
                    absdir1 = current.operand1;
                    coper1 = constantTable.getValue(absdir1)[0];
                    characterParams.push_back(coper1);
                } break;
                case 4: { // int;
                    absdir1 = current.operand1;
                    if(constantTable.getValue(absdir1) == "true") {
                        foper1 = 1;
                    } else {
                        foper1 = 0;
                    }
                    flagParams.push_back(foper1);
                } break;
            } break;
    }
}
void VirtualMachine::gosub(){
    // Remember where to come back
    jumpStack.push(cQuad);

    mLocal = &sMemory.top();

    // Make temporal memory look for local temporal memory
    mTemporal = mLocal;

    for (int i=0; i<integerParams.size(); i++) {
        mLocal->setInteger(i, integerParams[i]);
    }

    for (int i=0; i<decimalParams.size(); i++) {
        mLocal->setDecimal(i, decimalParams[i]);
    }

    for (int i=0; i<characterParams.size(); i++) {
        mLocal->setCharacter(i, characterParams[i]);
    }

    for (int i=0; i<flagParams.size(); i++) {
        mLocal->setFlag(i, flagParams[i]);
    }

    cQuad = current.operand1 - 1;
}
void VirtualMachine::ret(){}
void VirtualMachine::endproc(){
    int returnTo = jumpStack.top();
    jumpStack.pop();

    sMemory.pop();
    if (sMemory.size() == 0) {
        // Return temporal look up to global
        mTemporal = &mGlobal;
    }
    else {
        mLocal = &sMemory.top();
        mTemporal = mLocal;
    }

    cQuad = returnTo;
}
void VirtualMachine::end(){}
void VirtualMachine::check() {
    int absdir, index;

    switch (typeAdapter.getScope(current.result)) {
        case 0: {
            absdir = current.result - typeAdapter.integerG.min;
            index = mGlobal.getInteger(absdir);
        } break;
        case 1: {
            absdir = current.result - typeAdapter.integerL.min;
            index = mLocal->getInteger(absdir);
        } break;
        case 2: {
            absdir = current.result - typeAdapter.integerT.min;
            index = mTemporal->getTemporalInteger(absdir);
        } break;
        case 3: {
            absdir = current.result;
            index = stoi(constantTable.getValue(absdir));
        } break;
    }

    if (index < current.operand1 || index >= current.operand2) {
        cout<<"Index out of bounds"<<endl;
        cQuad = program.size() - 2;
    }
}

#endif
