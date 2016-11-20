#ifndef TYPEADAPTER_CPP
#define TYPEADAPTER_CPP

#include "headers/typeAdapter.h"

using namespace std;

TypeAdapter::TypeAdapter() {
    none.set(0, 1000, 7);

    // Global scope
    integerG.set(none.max + 1, 1000, 0);
    decimalG.set(integerG.max + 1, 1000, 1);
    characterG.set(decimalG.max + 1, 1000, 3);
    flagG.set(characterG.max + 1, 1000, 4);

    // Local scope
    integerL.set(flagG.max + 1, 1000, 0);
    decimalL.set(integerL.max + 1, 1000, 1);
    characterL.set(decimalL.max + 1, 1000, 3);
    flagL.set(characterL.max + 1, 1000, 4);

    // Temporal
    integerT.set(flagL.max + 1, 1000, 0);
    decimalT.set(integerT.max + 1, 1000, 1);
    characterT.set(decimalT.max + 1, 1000, 3);
    flagT.set(characterT.max + 1, 1000, 4);

    // Constants
    integerConstant.set(flagT.max + 1, 1000, 0);
    decimalConstant.set(integerConstant.max + 1, 1000, 1);
    textConstant.set(decimalConstant.max + 1, 1000, 2);
    characterConstant.set(textConstant.max + 1, 1000, 3);
    flagConstant.set(characterConstant.max + 1, 2, 4);
}

int TypeAdapter::getType(int address){
    if((integerG.min <= address && address <= integerG.max) ||
       (integerL.min <= address && address <= integerL.max) ||
       (integerT.min <= address && address <= integerT.max) ||
       (integerConstant.min <= address &&  address <= integerConstant.max)) {
        return 0;
    }
    else if((decimalG.min <= address && address <= decimalG.max) ||
       (decimalL.min <= address && address <= decimalL.max) ||
       (decimalT.min <= address && address <= decimalT.max) ||
       (decimalConstant.min <= address && address <= decimalConstant.max)) {
        return 1;
    }
    else if(textConstant.min <= address && address <= textConstant.max) {
        return 2;
    }
    else if((characterG.min <= address && address <= characterG.max) ||
       (characterL.min <= address && address <= characterL.max) ||
       (characterT.min <= address && address <= characterT.max) ||
       (characterConstant.min <= address && address <= characterConstant.max)){
        return 3;
    }
    else if((flagG.min <= address && address <= flagG.max) ||
       (flagL.min <= address && address <= flagL.max) ||
       (flagT.min <= address && address <= flagT.max) ||
       (flagConstant.min <= address && address <= flagConstant.max)) {
        return 4;
    }
    cout<<"Type not found"<<endl;
    return -1;
}

int TypeAdapter::getScope(int address) {
    if((integerG.min <= address && address <= integerG.max) ||
       (decimalG.min <= address && address <= decimalG.max) ||
       (characterG.min <= address && address <= characterG.max) ||
       (flagG.min <= address && address <= flagG.max)) {
        return 0;
    }
    else if((integerL.min <= address && address <= integerL.max) ||
       (decimalL.min <= address && address <= decimalL.max) ||
       (characterL.min <= address && address <= characterL.max) ||
       (flagL.min <= address && address <= flagL.max)) {
        return 1;
    }
    else if((integerT.min <= address && address <= integerT.max) ||
       (decimalT.min <= address && address <= decimalT.max) ||
       (characterT.min <= address && address <= characterT.max) ||
       (flagT.min <= address && address <= flagT.max)) {
        return 2;
    }
    else if((integerConstant.min<= address && address <= integerConstant.max) ||
       (decimalConstant.min <= address && address <= decimalConstant.max) ||
       (characterConstant.min <= address && address <= characterConstant.max) ||
       (textConstant.min <= address && address <= textConstant.max) ||
       (flagConstant.min <= address && address <= flagConstant.max)) {
        return 3;
    }
    cout<<"Scope not found"<<endl;
    return -1;
}

#endif
