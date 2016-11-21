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
    if (dir >= temporalInteger.size()) cout<<"Memory out of bounds "<<dir<<endl;
	return temporalInteger[dir];
}

double Memory::getTemporalDecimal(int dir){
    if (dir >= temporalDecimal.size()) cout<<"Memory out of bounds "<<dir<<endl;
	return temporalDecimal[dir];
}

char Memory::getTemporalCharacter(int dir){
    if (dir >= temporalCharacter.size()) cout<<"Memory out of bounds "<<dir<<endl;
	return temporalCharacter[dir];
}

bool Memory::getTemporalFlag(int dir){
    if (dir >= temporalFlag.size()) cout<<"Memory out of bounds "<<dir<<endl;
	return temporalFlag[dir];
}

/////////////
void Memory::setInteger(int dir, int val){
    if (dir >= vinteger.size()) cout<<"Memory out of bounds "<<dir<<endl;
	vinteger[dir] = val;
}

void Memory::setDecimal(int dir, double val){
    if (dir >= vdecimal.size()) cout<<"Memory out of bounds "<<dir<<endl;
	vdecimal[dir] = val;
}

void Memory::setCharacter(int dir, char val){
    if (dir >= vcharacter.size()) cout<<"Memory out of bounds "<<dir<<endl;
	vcharacter[dir] = val;
}

void Memory::setFlag(int dir, bool val){
    if (dir >= vflag.size()) cout<<"Memory out of bounds "<<dir<<endl;
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
