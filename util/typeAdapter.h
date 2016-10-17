#ifndef TYPEADAPTER_H
#define TYPEADAPTER_H

using namespace std;

class TypeAdapter {
public:
	TypeAdapter();
	int getIntegerMin();
	int getDecimalMin();
	int getTextMin();
	int getCharacterMin();
	int getFlagMin();
	int getArrayMin();
	int getMatrixMin();
	int getNoneMin();
	int getAvailMin();

	int getType(int address);
    void getNextAddress(int& address);

private:
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
	int arrayMin;
	int arrayMax;
	int matrixMin;
	int matrixMax;
	int noneMin;
	int noneMax;
	int availMin;
	int availMax;
};

#endif
