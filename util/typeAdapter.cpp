#ifndef TYPEADAPTER_CPP
#define TYPEADAPTER_CPP

#include "typeAdapter.h"

using namespace std;

TypeAdapter::TypeAdapter() {
	integerMin = 0;
	integerMax = 999;
	decimalMin = 1000;
	decimalMax = 1999;
	textMin = 2000;
	textMax = 2999;
	characterMin = 3000;
	characterMax = 3999;
	flagMin = 4000;
	flagMax = 4999;
	arrayMin = 5000;
	arrayMax = 5999;
	matrixMin = 6000;
	matrixMax = 6999;
	noneMin = 7000;
	noneMax = 7999;
	availMin = 8000;
	availMax = 8999;
}

int TypeAdapter::getIntegerMin() {
	return integerMin;
}

int TypeAdapter::getDecimalMin() {
	return decimalMin;
}

int TypeAdapter::getTextMin() {
	return textMin;
}

int TypeAdapter::getCharacterMin() {
	return characterMin;
}

int TypeAdapter::getFlagMin() {
	return flagMin;
}

int TypeAdapter::getArrayMin() {
	return arrayMin;
}

int TypeAdapter::getMatrixMin() {
	return matrixMin;
}

int TypeAdapter::getNoneMin() {
	return noneMin;
}

int TypeAdapter::getAvailMin() {
	return availMin;
}

int TypeAdapter::getType(int address){
	if(integerMin <= address && address <= integerMax) {
		return 0;
	}
	else if(decimalMin <= address && address <= decimalMax) {
		return 1;
	}
	else if(textMin <= address && address <= textMax) {
		return 2;
	}
	else if(characterMin <= address && address <= characterMax) {
		return 3;
	}
	else if(flagMin <= address && address <= flagMax) {
		return 4;
	}
	else if(arrayMin <= address && address <= arrayMax) {
		return 5;
	}
	else if(matrixMin <= address && address <= matrixMax) {
		return 6;
	}
	else if(noneMin <= address && address <= noneMax) {
		return 7;
	}
	else if(availMin <= address && address <= availMax) {
		return 8;
	}
	else {
		return -1;
	}
}

void TypeAdapter::getNextAddress(int &address){
    address++;
	if(address == integerMax) {
        cout<<"Memory limit exceeded."<<endl;
	}
	else if(address == decimalMax) {
        cout<<"Memory limit exceeded."<<endl;
	}
	else if(address == textMax) {
        cout<<"Memory limit exceeded."<<endl;
	}
	else if(address == characterMax) {
        cout<<"Memory limit exceeded."<<endl;
	}
	else if(address == flagMax) {
        cout<<"Memory limit exceeded."<<endl;
	}
	else if(address == arrayMax) {
        cout<<"Memory limit exceeded."<<endl;
	}
	else if(address == matrixMax) {
        cout<<"Memory limit exceeded."<<endl;
	}
	else if(address == noneMax) {
        cout<<"Memory limit exceeded."<<endl;
	}
	else if(address == availMax) {
        cout<<"Memory limit exceeded."<<endl;
	}
}

#endif
