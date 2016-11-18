#ifndef MEMORY_CPP
#define MEMORY_CPP

#include "headers/memory.h"

void Memory::changeVariablesGlobal(int integer, int decimal, int text, int character, int flag){
	gloInt = integer;
	gloDec = decimal;
	gloTex =  text;
	gloCha = character;
	gloFla = flag;
}

void Memory::changeVariablesLocal(int integer, int decimal, int text, int character, int flag){
	locInt = integer;
	locDec = decimal;
	locTex =  text;
	locCha = character;
	locFla = flag;
}

void Memory::changeVariablesGlobal(int integer, int decimal, int text, int character, int flag){
	temInt = integer;
	temDec = decimal;
	temTex =  text;
	temCha = character;
	temFla = flag;
}

void Memory::initializeGlobal(){
	globalInteger.resize(gloInt);
	globalDecimal.resize(gloDec);
	globalText.resize(gloTex);
	globalChar.resize(gloCha);
	globalFlag.resize(gloFla);
}

void Memory::initializeLocal(){
	localInteger.resize(locInt);
	localDecimal.resize(locDec);
	localText.resize(locTex);
	localChar.resize(locCha);
	localFlag.resize(locFla);
}

void Memory::initializeTemporal(){
	temporalInteger.resize(temInt);
	temporalDecimal.resize(temDec);
	temporalText.resize(temTex);
	temporalChar.resize(temCha);
	temporalFlag.resize(temFla);
}

int Memory::getGlobalInteger(int dir){
	return globalInteger[dir];
}

double Memory::getGlobalDecimal(int dir){
	return globalDecimal[dir];
}

string Memory::getGlobalText(int dir){
	return globalText[dir];
}

char Memory::getGlobalChar(int dir){
	return globalChar[dir];
}

bool Memory::getGlobalFlag(int dir){
	return globalFlag[dir];
}

/////////////////////
int Memory::getLocalInteger(int dir){
	return localInteger[dir];
}

double Memory::getLocalDecimal(int dir){
	return localDecimal[dir];
}

string Memory::getLocalText(int dir){
	return localText[dir];
}

char Memory::getLocalChar(int dir){
	return localChar[dir];
}

bool Memory::getLocalFlag(int dir){
	return localFlag[dir];
}

//////////////////
int Memory::getTemporalInteger(int dir){
	return temporalInteger[dir];
}

double Memory::getTemporalDecimal(int dir){
	return temporalDecimal[dir];
}

string Memory::getTemporalText(int dir){
	return temporalText[dir];
}

char Memory::getTemporalChar(int dir){
	return temporalChar[dir];
}

bool Memory::getTemporalFlag(int dir){
	return temporalFlag[dir];
}

/////////////
void Memory::setGlobalInteger(int dir, int val){
	globalInteger[dir] = val;
}

void Memory::setGlobalDecimal(int dir, double val){
	globalDecimal[dir] = val;
}

void Memory::setGlobalText(int dir, string val){
	globalText[dir] = val;
}

void Memory::setGlobalChar(int dir, char val){
	globalChar[dir] = val;
}

void Memory::setGlobalFlag(int dir, bool val){
	globalFlag[dir] = val;
}

////////////
void Memory::setLocalInteger(int dir, int val){
	localInteger[dir] = val;
}

void Memory::setLocalDecimal(int dir, double val){
	localDecimal[dir] = val;
}

void Memory::setLocalText(int dir, string val){
	localText[dir] = val;
}

void Memory::setLocalChar(int dir, char val){
	localChar[dir] = val;
}

void Memory::setLocalFlag(int dir, bool val){
	localFlag[dir] = val;
}

///////////
void Memory::setTemporalInteger(int dir, int val){
	temporalInteger[dir] = val;
}

void Memory::setTemporalDecimal(int dir, double val){
	temporalDecimal[dir] = val;
}

void Memory::setTemporalText(int dir, string val){
	temporalText[dir] = val;
}

void Memory::setTemporalChar(int dir, char val){
	temporalChar[dir] = val;
}

void Memory::setTemporalFlag(int dir, bool val){
	temporalFlag[dir] = val;
}
