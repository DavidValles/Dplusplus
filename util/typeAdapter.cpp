#ifndef TYPEADAPTER_CPP
#define TYPEADAPTER_CPP

#include "headers/typeAdapter.h"

using namespace std;

TypeAdapter::TypeAdapter() {
    integer.set(0, 1000, 0);
    decimal.set(integer.max + 1, 1000, 1);
    text.set(decimal.max + 1, 1000, 2);
    character.set(text.max + 1, 1000, 3);
    flag.set(character.max + 1, 1000, 4);
    array.set(flag.max + 1, 1000, 5);
    matrix.set(array.max + 1, 1000, 6);
    none.set(matrix.max + 1, 1000, 7);
    avail.set(none.max + 1, 1000, 8);
    integerConstant.set(avail.max + 1, 1000, 0);
    decimalConstant.set(integerConstant.max + 1, 1000, 1);
    stringConstant.set(decimalConstant.max + 1, 1000, 2);
    characterConstant.set(stringConstant.max + 1, 1000, 3);
}

int TypeAdapter::getType(int address){
    if((integer.min <= address && address <= integer.max) ||
        (integerConstant.min <= address &&  address <= integerConstant.max)) {
        return 0;
    }
    else if((decimal.min <= address && address <= decimal.max) ||
        (decimalConstant.min <= address && address <= decimalConstant.max)) {
        return 1;
    }
    else if((text.min <= address && address <= text.max) ||
        (stringConstant.min <= address && address <= stringConstant.max)) {
        return 2;
    }
    else if((character.min <= address && address <= character.max) ||
       (characterConstant.min <= address && address <= characterConstant.max)){
        return 3;
    }
    else if(flag.min <= address && address <= flag.max) {
        return 4;
    }
    else if(array.min <= address && address <= array.max) {
        return 5;
    }
    else if(matrix.min <= address && address <= matrix.max) {
        return 6;
    }
    else if(none.min <= address && address <= none.max) {
        return 7;
    }
    else if(avail.min <= address && address <= avail.max) {
        return 8;
    }
    else {
        return -1;
    }
}


#endif
