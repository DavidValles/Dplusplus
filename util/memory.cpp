#ifndef MEMORY_CPP
#define MEMORY_CPP

#include "headers/memory.h"

Memory::changeVariablesGlobal(int integer, int decimal, int text, int character, int flag){
	gloInt = integer;
	gloDec = decimal;
	gloTex =  text;
	gloCha = character;
	gloFla = flag;
}

Memory::changeVariablesLocal(int integer, int decimal, int text, int character, int flag){
	locInt = integer;
	locDec = decimal;
	locTex =  text;
	locCha = character;
	locFla = flag;
}

Memory::changeVariablesGlobal(int integer, int decimal, int text, int character, int flag){
	temInt = integer;
	temDec = decimal;
	temTex =  text;
	temCha = character;
	temFla = flag;
}

Memory::initializeGlobal(){
	globalInteger.resize(gloInt);
	globalDecimal.resize(gloDec);
	globalText.resize(gloTex);
	globalChar.resize(gloCha);
	globalFlag.resize(gloFla);
}

Memory::initializeLocal(){
	localInteger.resize(locInt);
	localDecimal.resize(locDec);
	localText.resize(locTex);
	localChar.resize(locCha);
	localFlag.resize(locFla);
}

Memory::initializeTemporal(){
	temporalInteger.resize(temInt);
	temporalDecimal.resize(temDec);
	temporalText.resize(temTex);
	temporalChar.resize(temCha);
	temporalFlag.resize(temFla);
}

Memory::getGlobalInteger(int dir){
	return globalInteger[dir];
}

Memory::getGlobalDecimal(int dir){
	return globalDecimal[dir];
}

Memory::getGlobalText(int dir){
	return globalText[dir];
}

Memory::getGlobalChar(int dir){
	return globalChar[dir];
}

Memory::getGlobalFlag(int dir){
	return globalFlag[dir];
}

/////////////////////
Memory::getLocalInteger(int dir){
	return localInteger[dir];
}

Memory::getLocalDecimal(int dir){
	return localDecimal[dir];
}

Memory::getLocalText(int dir){
	return localText[dir];
}

Memory::getLocalChar(int dir){
	return localChar[dir];
}

Memory::getLocalFlag(int dir){
	return localFlag[dir];
}

//////////////////
Memory::getTemporalInteger(int dir){
	return temporalInteger[dir];
}

Memory::getTemporalDecimal(int dir){
	return temporalDecimal[dir];
}

Memory::getTemporalText(int dir){
	return temporalText[dir];
}

Memory::getTemporalChar(int dir){
	return temporalChar[dir];
}

Memory::getTemporalFlag(int dir){
	return temporalFlag[dir];
}

/////////////
Memory::setGlobalInteger(int dir, int val){
	globalInteger[dir] = val;
}

Memory::setGlobalDecimal(int dir, double val){
	globalDecimal[dir] = val;
}

Memory::setGlobalText(int dir, string val){
	globalText[dir] = val;
}

Memory::setGlobalChar(int dir, char val){
	globalChar[dir] = val;
}

Memory::setGlobalFlag(int dir, bool val){
	globalFlag[dir] = val;
}

////////////
Memory::setLocalInteger(int dir, int val){
	localInteger[dir] = val;
}

Memory::setLocalDecimal(int dir, double val){
	localDecimal[dir] = val;
}

Memory::setLocalText(int dir, string val){
	localText[dir] = val;
}

Memory::setLocalChar(int dir, char val){
	localChar[dir] = val;
}

Memory::setLocalFlag(int dir, bool val){
	localFlag[dir] = val;
}

///////////
Memory::setTemporalInteger(int dir, int val){
	temporalInteger[dir] = val;
}

Memory::setTemporalDecimal(int dir, double val){
	temporalDecimal[dir] = val;
}

Memory::setTemporalText(int dir, string val){
	temporalText[dir] = val;
}

Memory::setTemporalChar(int dir, char val){
	temporalChar[dir] = val;
}

Memory::setTemporalFlag(int dir, bool val){
	temporalFlag[dir] = val;
}
