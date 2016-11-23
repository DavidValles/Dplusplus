#ifndef MEMORY_H
#define MEMORY_H

#include <vector>
using namespace std;

/*
 * Memory
 *
 * A scopeless memory, simply has static variables and temporal variables.
 *
 * VM uses this class, vm is in charge of giving scope to this class object
 * through instantiation.
 *
 * Simple functions to access, extend and modify memeory.
 */

class Memory {
private:
	vector<int> vinteger;
	vector<double> vdecimal;
	vector<char> vcharacter;
	vector<bool> vflag;

	vector<int> temporalInteger;
	vector<double> temporalDecimal;
	vector<char> temporalCharacter;
	vector<bool> temporalFlag;

public:
	int integer = 0;
	int decimal = 0;
	int character = 0;
	int flag = 0;

	int temInteger = 0;
	int temDecimal = 0;
	int temCharacter = 0;
	int temFlag = 0;

	void changeVariables(int integer, int decimal, int character, int flag);
	void changeVariablesTemporal(int integer, int decimal, int character, int flag);

	void initialize();
	void initializeTemporal();

	int getInteger(int dir);
	double getDecimal(int dir);
	char getCharacter(int dir);
	bool getFlag(int dir);

	int getTemporalInteger(int dir);
	double getTemporalDecimal(int dir);
	char getTemporalCharacter(int dir);
	bool getTemporalFlag(int dir);

	void setInteger(int dir, int val);
	void setDecimal(int dir, double val);
	void setCharacter(int dir, char val);
	void setFlag(int dir, bool val);

	void setTemporalInteger(int dir, int val);
	void setTemporalDecimal(int dir, double val);
	void setTemporalCharacter(int dir, char val);
	void setTemporalFlag(int dir, bool val);

};

#endif
