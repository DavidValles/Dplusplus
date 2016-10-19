#ifndef TYPEADAPTER_H
#define TYPEADAPTER_H

using namespace std;

struct Section {
    void set(int a, int b) {
        min = a;
        size = b;
        max = min + size - 1;
    }
    int min;
    int size;
    int max;
};

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
    int getIntegerConstantMin();
    int getDecimalConstantMin();

	int getType(int address);
    void getNextAddress(int& address);

private:
	Section integer;
	Section decimal;
	Section text;
	Section character;
	Section flag;
	Section array;
	Section matrix;
	Section none;
	Section avail;
    Section integerConstant;
    Section decimalConstant;
};

#endif
