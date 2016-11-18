#ifndef MEMORY_CPP
#define MEMORY_CPP

#include "headers/memory.h"

void Memory::changeVariables(int integer, int decimal, int character, int flag){
	this->integer = integer;
	this->decimal = decimal;
	this->character = character;
	this->flag = flag;
}

void Memory::changeVariablesTemporal(int integer, int decimal, int character, int flag){
	temInteger = integer;
	temDecimal = decimal;
	temCharacter = character;
	temFlag = flag;
}

void Memory::initialize(){
	vinteger.resize(integer);
	vdecimal.resize(decimal);
	vcharacter.resize(character);
	vflag.resize(flag);
}

void Memory::initializeTemporal(){
	temporalInteger.resize(temInteger);
	temporalDecimal.resize(temDecimal);
	temporalCharacter.resize(temCharacter);
	temporalFlag.resize(temFlag);
}

int Memory::getInteger(int dir){
	return vinteger[dir];
}

double Memory::getDecimal(int dir){
	return vdecimal[dir];
}

char Memory::getCharacter(int dir){
	return vcharacter[dir];
}

bool Memory::getFlag(int dir){
	return vflag[dir];
}

//////////////////
int Memory::getTemporalInteger(int dir){
	return temporalInteger[dir];
}

double Memory::getTemporalDecimal(int dir){
	return temporalDecimal[dir];
}

char Memory::getTemporalCharacter(int dir){
	return temporalCharacter[dir];
}

bool Memory::getTemporalFlag(int dir){
	return temporalFlag[dir];
}

/////////////
void Memory::setInteger(int dir, int val){
	vinteger[dir] = val;
}

void Memory::setDecimal(int dir, double val){
	vdecimal[dir] = val;
}

void Memory::setCharacter(int dir, char val){
	vcharacter[dir] = val;
}

void Memory::setFlag(int dir, bool val){
	vflag[dir] = val;
}

///////////
void Memory::setTemporalInteger(int dir, int val){
	temporalInteger[dir] = val;
}

void Memory::setTemporalDecimal(int dir, double val){
	temporalDecimal[dir] = val;
}

void Memory::setTemporalCharacter(int dir, char val){
	temporalCharacter[dir] = val;
}

void Memory::setTemporalFlag(int dir, bool val){
	temporalFlag[dir] = val;
}

#endif
