#ifndef MEMORY_H
#define MEMORY_H

#include <vector>
using namespace std;

class Memory {
private:
	vector<int> globalInteger;
	vector<double> globalDecimal;
	vector<string> globalText;
	vector<char> globalChar;
	vector<bool> globalFlag;

	vector<int> localInteger;
	vector<double> localDecimal;
	vector<string> localText;
	vector<char> localChar;
	vector<bool> localFlag;

	vector<int> temporalInteger;
	vector<double> temporalDecimal;
	vector<string> temporalText;
	vector<char> temporalChar;
	vector<bool> temporalFlag;

public:
	int gloInt = 0;
	int gloDec = 0;
	int gloTex = 0;
	int gloCha = 0;
	int gloFla = 0;

	int locInt = 0;
	int locDec = 0;
	int locTex = 0;
	int locCha = 0;
	int locFla = 0;

	int temInt = 0;
	int temDec = 0;
	int temTex = 0;
	int temCha = 0;
	int temFla = 0;

	void changeVariablesGlobal(int integer, int decimal, int text, int character, int flag);
	void changeVariablesLocal(int integer, int decimal, int text, int character, int flag);
	void changeVariablesTemporal(int integer, int decimal, int text, int character, int flag);

	void initializeGlobal();
	void initializeLocal();
	void initializeTemporal();


	int getGlobalInteger(int dir);
	double getGlobalDecimal(int dir);
	string getGlobalText(int dir);
	char getGlobalChar(int dir);
	bool getGlobalFlag(int dir);

	int getLocalInteger(int dir);
	double getLocalDecimal(int dir);
	string getLocalText(int dir);
	char getLocalChar(int dir);
	bool getLocalFlag(int dir);

	int getTemporalInteger(int dir);
	double getTemporalDecimal(int dir);
	string getTemporalText(int dir);
	char getTemporalChar(int dir);
	bool getTemporalFlag(int dir);

	void setGlobalInteger(int dir, int val);
	void setGlobalDecimal(int dir, double val);
	void setGlobalText(int dir, string val);
	void setGlobalChar(int dir, char val);
	void setGlobalFlag(int dir, bool val);

	void setLocalInteger(int dir, int val);
	void setLocalDecimal(int dir, double val);
	void setLocalText(int dir, string val);
	void setLocalChar(int dir, char val);
	void setLocalFlag(int dir, bool val);

	void setTemporalInteger(int dir, int val);
	void setTemporalDecimal(int dir, double val);
	void setTemporalText(int dir, string val);
	void setTemporalChar(int dir, char val);
	void setTemporalFlag(int dir, bool val);

};

#endif
