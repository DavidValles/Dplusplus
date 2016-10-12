#ifndef TYPEADAPTER_CPP
#define TYPEADAPTER_CPP

#include "typeAdapter.h"

using namespace std;

typeAdapter::typeAdapter() {
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
}

int typeAdapter::getIntegerMin() {
	return integerMin;
}

int typeAdapter::getDecimalMin() {
	return decimalMin;
}

int typeAdapter::getTextMIn() {
	return textMin;
}

int typeAdapter::getCharacterMin() {
	return characterMin;
}

int typeAdapter::getFlagMin() {
	return flagMin;
}

int typeAdapter::getType(int address){
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
	else {
		return -1;
	}
}

#endif