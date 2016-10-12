#ifndef TYPEADAPTER_H
#define TYPEADAPTER_H

using namespace std;

class typeAdapter {
public:
	typeAdapter();
	int getIntegerMin();
	int getDecimalMin();
	int getTextMin();
	int getCharacterMin();
	int getFlagMin();
	int getType(int address);
	int integerMin;
	int integerMax;
	int decimalMin;
	int decimalMax;
	int textMin;
	int textMax;
	int characterMin;
	int characterMax;
	int flagMin;
	int flagMax;
};

#endif