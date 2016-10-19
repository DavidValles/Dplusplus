#ifndef TYPEADAPTER_CPP
#define TYPEADAPTER_CPP

#include "headers/typeAdapter.h"

using namespace std;

TypeAdapter::TypeAdapter() {
    integer.set(0, 1000);
    decimal.set(integer.max + 1, 1000);
    text.set(decimal.max + 1, 1000);
    character.set(text.max + 1, 1000);
    flag.set(character.max + 1, 1000);
    array.set(flag.max + 1, 1000);
    matrix.set(array.max + 1, 1000);
    none.set(matrix.max + 1, 1000);
    avail.set(none.max + 1, 1000);
    integerConstant.set(avail.max + 1, 1000);
    decimalConstant.set(integerConstant.max + 1, 1000);
    stringConstant.set(decimalConstant.max + 1, 1000);
    characterConstant.set(stringConstant.max + 1, 1000);
}

int TypeAdapter::getIntegerMin() {
	return integer.min;
}

int TypeAdapter::getDecimalMin() {
	return decimal.min;
}

int TypeAdapter::getTextMin() {
	return text.min;
}

int TypeAdapter::getCharacterMin() {
	return character.min;
}

int TypeAdapter::getFlagMin() {
	return flag.min;
}

int TypeAdapter::getArrayMin() {
	return array.min;
}

int TypeAdapter::getMatrixMin() {
	return matrix.min;
}

int TypeAdapter::getNoneMin() {
	return none.min;
}

int TypeAdapter::getAvailMin() {
	return avail.min;
}

int TypeAdapter::getIntegerConstantMin() {
	return integerConstant.min;
}

int TypeAdapter::getDecimalConstantMin() {
	return decimalConstant.min;
}

int TypeAdapter::getStringConstantMin() {
	return stringConstant.min;
}

int TypeAdapter::getCharacterConstantMin() {
	return characterConstant.min;
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

void TypeAdapter::getNextAddress(int &address){
    address++;
	if(address == integer.max) {
        cout<<"Memory limit exceeded."<<endl;
	}
	else if(address == decimal.max) {
        cout<<"Memory limit exceeded."<<endl;
	}
	else if(address == text.max) {
        cout<<"Memory limit exceeded."<<endl;
	}
	else if(address == character.max) {
        cout<<"Memory limit exceeded."<<endl;
	}
	else if(address == flag.max) {
        cout<<"Memory limit exceeded."<<endl;
	}
	else if(address == array.max) {
        cout<<"Memory limit exceeded."<<endl;
	}
	else if(address == matrix.max) {
        cout<<"Memory limit exceeded."<<endl;
	}
	else if(address == none.max) {
        cout<<"Memory limit exceeded."<<endl;
	}
	else if(address == avail.max) {
        cout<<"Memory limit exceeded."<<endl;
	}
	else if(address == integerConstant.max) {
        cout<<"Memory limit exceeded."<<endl;
	}
	else if(address == decimalConstant.max) {
        cout<<"Memory limit exceeded."<<endl;
	}
}

#endif
