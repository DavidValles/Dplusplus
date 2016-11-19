#ifndef CUBE_CPP
#define CUBE_CPP

#include "headers/cube.h"

using namespace std;

/*
error = -1
INT = 0
DECIMAL = 1
TEXT = 2
Character = 3
FLAG = 4

+ = 0
- = 1
/ 2
* 3
% 4
> 5
< 6
= 7
AND 8
OR 9
NOT 10
NOTEQUALTO 11
EQUALTO 12
LESSTHANOREQUALTO 13
GREATERTHANNOREQUALTO 14
*/

Cube::Cube() {
	// tip1 tip2 operador = tip3
	//INT +
	cube[0][0][0] = 0; // int + int = int;
	cube[0][1][0] = 1; // int + decimal = decimal
	cube[0][2][0] = -1;// int + text = error
	cube[0][3][0] = -1;// int + character = error
	cube[0][4][0] = -1; // int + flag = error

	//INT -
	cube[0][0][1] = 0; // int - int = int;
	cube[0][1][1] = 1;// int - decimal = decimal
	cube[0][2][1] = -1;// int - text = error
	cube[0][3][1] = -1;// int - character = error
	cube[0][4][1] = -1; // int - flag = error

	//INT /
	cube[0][0][2] = 0; // int / int = int;
	cube[0][1][2] = 1;// int / decimal = decimal
	cube[0][2][2] = -1;// int / text = error
	cube[0][3][2] = -1;// int / character = error
	cube[0][4][2] = -1; // int / flag = error

	//INT *
	cube[0][0][3] = 0; // int * int = int;
	cube[0][1][3] = 1;// int * decimal = decimal
	cube[0][2][3] = -1;// int * text = error
	cube[0][3][3] = -1;// int * character = error
	cube[0][4][3] = -1;// int * flag = error

	//INT %
	cube[0][0][4] = 0; // int % int = int;
	cube[0][1][4] = -1;// int % decimal = error
	cube[0][2][4] = -1;// int % text = error
	cube[0][3][4] = -1;// int % character = error
	cube[0][4][4] = -1; // int % flag = error

	//INT >
	cube[0][0][5] = 4; // int > int = flag
	cube[0][1][5] = 4;// int > decimal = flag
	cube[0][2][5] = -1;// int > text = error
	cube[0][3][5] = -1;// int > character = error
	cube[0][4][5] = -1; // int > flag = error

	//INT <
	cube[0][0][6] = 4; // int < int = flag
	cube[0][1][6] = 4; // int < decimal = flag
	cube[0][2][6] = -1;// int < text = error
	cube[0][3][6] = -1;// int < character = error
	cube[0][4][6] = -1;// int < flag = error

	//INT =
	cube[0][0][7] = 0; // int = int = int;
	cube[0][1][7] = -1;// int = decimal = error
	cube[0][2][7] = -1;// int = text = error
	cube[0][3][7] = -1;// int = character = error
	cube[0][4][7] = -1;// int = flag = error

	//INT and
	cube[0][0][8] = -1;// int and int = error
	cube[0][1][8] = -1;// int and decimal = error
	cube[0][2][8] = -1;// int and text = error
	cube[0][3][8] = -1;// int and character = error
	cube[0][4][8] = -1;// int and flag = error

	//INT or 
	cube[0][0][9] = -1;// int or int = error
	cube[0][1][9] = -1;// int or decimal = error
	cube[0][2][9] = -1;// int or text = error
	cube[0][3][9] = -1;// int or character = error
	cube[0][4][9] = -1;// int or flag = error

	//INT not
	cube[0][-1][10] = -1;// int not int = error
	cube[0][-1][10] = -1;// int not decimal = error
	cube[0][-1][10] = -1;// int not text = error
	cube[0][-1][10] = -1;// int not character = error
	cube[0][-1][10] = -1;// int not flag = error

	//INT notequalto
	cube[0][0][11] = 4;// int ntq int = error
	cube[0][1][11] = -1;// int ntq decimal = error
	cube[0][2][11] = -1;// int ntq text = error
	cube[0][3][11] = -1;// int ntq character = error
	cube[0][4][11] = -1;// int ntq flag = error

	//INT equalto 
	cube[0][0][12] = 4;// int eqt int = error
	cube[0][1][12] = -1;// int eqt decimal = error
	cube[0][2][12] = -1;// int eqt text = error
	cube[0][3][12] = -1;// int eqt character = error
	cube[0][4][12] = -1;// int eqt flag = error

	//INT lessthanorequalto
	cube[0][0][13] = 4;// int leqt int = error
	cube[0][1][13] = 4;// int leqt decimal = error
	cube[0][2][13] = -1;// int leqt text = error
	cube[0][3][13] = -1;// int leqt character = error
	cube[0][4][13] = -1;// int leqt flag = error

	//INT greaterthanorequalto
	cube[0][0][14] = 4;// int geqt int = error
	cube[0][1][14] = 4;// int geqt decimal = error
	cube[0][2][14] = -1;// int geqt text = error
	cube[0][3][14] = -1; // int geqt character = error
	cube[0][4][14] = -1; // int geqt flag = error

	///////////// DECIMAL

	//decimal +
	cube[1][0][0] = 1; // decimal + int = decimal;
	cube[1][1][0] = 1; // decimal + decimal = decimal
	cube[1][2][0] = -1; // decimal + text = error
	cube[1][3][0] = -1; // decimal + character = error
	cube[1][4][0] = -1; // decimal + flag = error

    //decimal -
	cube[1][0][1] = 1; // decimal - int = int;
	cube[1][1][1] = 1; // decimal - decimal = decimal
	cube[1][2][1] = -1; // decimal - text = error
	cube[1][3][1] = -1; // decimal - character = error
	cube[1][4][1] = -1; // decimal - flag = error

	cube[1][0][2] = 1; // int / int = int;
	cube[1][1][2] = 1; // int / decimal = decimal
	cube[1][2][2] = -1; // int / text = error
	cube[1][3][2] = -1; // int / character = error
	cube[1][4][2] = -1;// int / flag = error

	cube[1][0][3] = 1; // int * int = int;
	cube[1][1][3] = 1; // int * decimal = decimal
	cube[1][2][3] = -1; // int * text = error
	cube[1][3][3] = -1;// int * character = error
	cube[1][4][3] = -1;// int * flag = error

	cube[1][0][4] = -1; // int % int = int;
	cube[1][1][4] = -1; // int % decimal = error
	cube[1][2][4] = -1;// int % text = error
	cube[1][3][4] = -1;// int % character = error
	cube[1][4][4] = -1;// int % flag = error

	cube[1][0][5] = 4; // int > int = flag
	cube[1][1][5] = 4; // int > decimal = flag
	cube[1][2][5] = -1; // int > text = error
	cube[1][3][5] = -1; // int > character = error
	cube[1][4][5] = -1; // int > flag = error

	cube[1][0][6] = 4; // int < int = flag
	cube[1][1][6] = 4; // int < decimal = flag
	cube[1][2][6] = -1; // int < text = error
	cube[1][3][6] = -1; // int < character = error
	cube[1][4][6] = -1; // int < flag = error

	cube[1][0][7] = 1; // int = int = int;
	cube[1][1][7] = 1; // int = decimal = error
	cube[1][2][7] = -1; // int = text = error
	cube[1][3][7] = -1; // int = character = error
	cube[1][4][7] = -1; // int = flag = error

	cube[1][0][8] = -1; // int and int = error
	cube[1][1][8] = -1; // int and decimal = error
	cube[1][2][8] = -1;// int and text = error
	cube[1][3][8] = -1;// int and character = error
	cube[1][4][8] = -1;// int and flag = error

	cube[1][0][9] = -1;// int or int = error
	cube[1][1][9] = -1;// int or decimal = error
	cube[1][2][9] = -1;// int or text = error
	cube[1][3][9] = -1;// int or character = error
	cube[1][4][9] = -1;// int or flag = error

	cube[1][-1][10] = -1; // int not int = error
	cube[1][-1][10] = -1;// int not decimal = error
	cube[1][-1][10] = -1;// int not text = error
	cube[1][-1][10] = -1;// int not character = error
	cube[1][-1][10] = -1;// int not flag = error

	cube[1][0][11] = 4;// int ntq int = error
	cube[1][1][11] = 4;// int ntq decimal = error
	cube[1][2][11] = -1;// int ntq text = error
	cube[1][3][11] = -1;// int ntq character = error
	cube[1][4][11] = -1;// int ntq flag = error

	cube[1][0][12] = 4; // int eqt int = error
	cube[1][1][12] = 4; // int eqt decimal = error
	cube[1][2][12] = -1; // int eqt text = error
	cube[1][3][12] = -1;// int eqt character = error
	cube[1][4][12] = -1;// int eqt flag = error

	cube[1][0][13] = 4; // int leqt int = error
	cube[1][1][13] = 4;// int leqt decimal = error
	cube[1][2][13] = -1; // int leqt text = error
	cube[1][3][13] = -1; // int leqt character = error
	cube[1][4][13] = -1; // int leqt flag = error

	cube[1][0][14] = 4;// int geqt int = error
	cube[1][1][14] = 4;// int geqt decimal = error
	cube[1][2][14] = -1;// int geqt text = error
	cube[1][3][14] = -1;// int geqt character = error
	cube[1][4][14] = -1;// int geqt flag = error

	////////////// TEXT

	cube[2][0][0] = -1; // decimal + int = decimal;
	cube[2][1][0] = -1;// decimal + decimal = decimal
	cube[2][2][0] = 2; // decimal + text = error
	cube[2][3][0] = 2; // decimal + character = error
	cube[2][4][0] = -1;// decimal + flag = error

	cube[2][0][1] = -1; // decimal - int = int;
	cube[2][1][1] = -1;// decimal - decimal = decimal
	cube[2][2][1] = -1; // decimal - text = error
	cube[2][3][1] = -1;// decimal - character = error
	cube[2][4][1] = -1;// decimal - flag = error

	cube[2][0][2] = -1; // int / int = int;
	cube[2][1][2] = -1; // int / decimal = decimal
	cube[2][2][2] = -1; // int / text = error
	cube[2][3][2] = -1;// int / character = error
	cube[2][4][2] = -1;// int / flag = error

	cube[2][0][3] = -1; // int * int = int;
	cube[2][1][3] = -1;// int * decimal = decimal
	cube[2][2][3] = -1; // int * text = error
	cube[2][3][3] = -1;// int * character = error
	cube[2][4][3] = -1;// int * flag = error

	cube[2][0][4] = -1; // int % int = int;
	cube[2][1][4] = -1; // int % decimal = error
	cube[2][2][4] = -1;// int % text = error
	cube[2][3][4] = -1;// int % character = error
	cube[2][4][4] = -1;// int % flag = error

	cube[2][0][5] = -1; // int > int = flag
	cube[2][1][5] = -1;// int > decimal = flag
	cube[2][2][5] = -1; // int > text = error
	cube[2][3][5] = -1; // int > character = error
	cube[2][4][5] = -1; // int > flag = error

	cube[2][0][6] = -1; // int < int = flag
	cube[2][1][6] = -1; // int < decimal = flag
	cube[2][2][6] = -1; // int < text = error
	cube[2][3][6] = -1; // int < character = error
	cube[2][4][6] = -1; // int < flag = error

	cube[2][0][7] = -1; // int = int = int;
	cube[2][1][7] = -1; // int = decimal = error
	cube[2][2][7] = 2; // int = text = error
	cube[2][3][7] = -1; // int = character = error
	cube[2][4][7] = -1; // int = flag = error

	cube[2][0][8] = -1; // int and int = error
	cube[2][1][8] = -1; // int and decimal = error
	cube[2][2][8] = -1;// int and text = error
	cube[2][3][8] = -1;// int and character = error
	cube[2][4][8] = -1;// int and flag = error

	cube[2][0][9] = -1;// int or int = error
	cube[2][1][9] = -1;// int or decimal = error
	cube[2][2][9] = -1;// int or text = error
	cube[2][3][9] = -1;// int or character = error
	cube[2][4][9] = -1;// int or flag = error

	cube[2][-1][10] = -1; // int not int = error
	cube[2][-1][10] = -1;// int not decimal = error
	cube[2][-1][10] = -1;// int not text = error
	cube[2][-1][10] = -1;// int not character = error
	cube[2][-1][10] = -1;// int not flag = error

	cube[2][0][11] = -1;// int ntq int = error
	cube[2][1][11] = -1;// int ntq decimal = error
	cube[2][2][11] =  4;// int ntq text = error
	cube[2][3][11] = -1;// int ntq character = error
	cube[2][4][11] = -1;// int ntq flag = error

	cube[2][0][12] = -1; // int eqt int = error
	cube[2][1][12] = -1;// int eqt decimal = error
	cube[2][2][12] = 4; // int eqt text = error
	cube[2][3][12] = -1;// int eqt character = error
	cube[2][4][12] = -1;// int eqt flag = error

	cube[2][0][13] = -1; // int leqt int = error
	cube[2][1][13] = -1; // int leqt decimal = error
	cube[2][2][13] = -1; // int leqt text = error
	cube[2][3][13] = -1; // int leqt character = error
	cube[2][4][13] = -1; // int leqt flag = error

	cube[2][0][14] = -1;// int geqt int = error
	cube[2][1][14] = -1;// int geqt decimal = error
	cube[2][2][14] = -1;// int geqt text = error
	cube[2][3][14] = -1;// int geqt character = error
	cube[2][4][14] = -1;// int geqt flag = error

	///////////////// Character

	cube[3][0][0] = -1; // decimal + int = decimal;
	cube[3][1][0] = -1;// decimal + decimal = decimal
	cube[3][2][0] = 2;// decimal + text = error
	cube[3][3][0] = 2;// decimal + character = error
	cube[3][4][0] = -1;// decimal + flag = error

	cube[3][0][1] = -1; // decimal - int = int;
	cube[3][1][1] = -1;// decimal - decimal = decimal
	cube[3][2][1] = -1; // decimal - text = error
	cube[3][3][1] = -1;// decimal - character = error
	cube[3][4][1] = -1;// decimal - flag = error

	cube[3][0][2] = -1; // int / int = int;
	cube[3][1][2] = -1; // int / decimal = decimal
	cube[3][2][2] = -1; // int / text = error
	cube[3][3][2] = -1;// int / character = error
	cube[3][4][2] = -1;// int / flag = error

	cube[3][0][3] = -1; // int * int = int;
	cube[3][1][3] = -1;// int * decimal = decimal
	cube[3][2][3] = -1; // int * text = error
	cube[3][3][3] = -1;// int * character = error
	cube[3][4][3] = -1;// int * flag = error

	cube[3][0][4] = -1; // int % int = int;
	cube[3][1][4] = -1; // int % decimal = error
	cube[3][2][4] = -1;// int % text = error
	cube[3][3][4] = -1;// int % character = error
	cube[3][4][4] = -1;// int % flag = error

	cube[3][0][5] = -1; // int > int = flag
	cube[3][1][5] = -1;// int > decimal = flag
	cube[3][2][5] = -1; // int > text = error
	cube[3][3][5] = -1; // int > character = error
	cube[3][4][5] = -1; // int > flag = error

	cube[3][0][6] = -1; // int < int = flag
	cube[3][1][6] = -1; // int < decimal = flag
	cube[3][2][6] = -1; // int < text = error
	cube[3][3][6] = -1; // int < character = error
	cube[3][4][6] = -1; // int < flag = error

	cube[3][0][7] = -1; // int = int = int;
	cube[3][1][7] = -1; // int = decimal = error
	cube[3][2][7] = -1; // int = text = error
	cube[3][3][7] = 3;// int = character = error
	cube[3][4][7] = -1; // int = flag = error

	cube[3][0][8] = -1; // int and int = error
	cube[3][1][8] = -1; // int and decimal = error
	cube[3][2][8] = -1;// int and text = error
	cube[3][3][8] = -1;// int and character = error
	cube[3][4][8] = -1;// int and flag = error

	cube[3][0][9] = -1;// int or int = error
	cube[3][1][9] = -1;// int or decimal = error
	cube[3][2][9] = -1;// int or text = error
	cube[3][3][9] = -1;// int or character = error
	cube[3][4][9] = -1;// int or flag = error

	cube[3][-1][10] = -1; // int not int = error
	cube[3][-1][10] = -1;// int not decimal = error
	cube[3][-1][10] = -1;// int not text = error
	cube[3][-1][10] = -1;// int not character = error
	cube[3][-1][10] = -1;// int not flag = error

	cube[3][0][11] = -1;// int ntq int = error
	cube[3][1][11] = -1;// int ntq decimal = error
	cube[3][2][11] = -1;// int ntq text = error
	cube[3][3][11] =  4;// int ntq character = error
	cube[3][4][11] = -1;// int ntq flag = error

	cube[3][0][12] = -1; // int eqt int = error
	cube[3][1][12] = -1;// int eqt decimal = error
	cube[3][2][12] = -1; // int eqt text = error
	cube[3][3][12] = 4;// int eqt character = error
	cube[3][4][12] = -1;// int eqt flag = error

	cube[3][0][13] = -1; // int leqt int = error
	cube[3][1][13] = -1; // int leqt decimal = error
	cube[3][2][13] = -1; // int leqt text = error
	cube[3][3][13] = -1; // int leqt character = error
	cube[3][4][13] = -1; // int leqt flag = error

	cube[3][0][14] = -1;// int geqt int = error
	cube[3][1][14] = -1;// int geqt decimal = error
	cube[3][2][14] = -1;// int geqt text = error
	cube[3][3][14] = -1;// int geqt character = error
	cube[3][4][14] = -1;// int geqt flag = error

	///////////////// flag

	cube[4][0][0] = -1; // decimal + int = decimal;
	cube[4][1][0] = -1;// decimal + decimal = decimal
	cube[4][2][0] = -1; // decimal + text = error
	cube[4][3][0] = -1;// decimal + character = error
	cube[4][4][0] = -1;// decimal + flag = error

	cube[4][0][1] = -1; // decimal - int = int;
	cube[4][1][1] = -1;// decimal - decimal = decimal
	cube[4][2][1] = -1; // decimal - text = error
	cube[4][3][1] = -1;// decimal - character = error
	cube[4][4][1] = -1;// decimal - flag = error

	cube[4][0][2] = -1; // int / int = int;
	cube[4][1][2] = -1; // int / decimal = decimal
	cube[4][2][2] = -1; // int / text = error
	cube[4][3][2] = -1;// int / character = error
	cube[4][4][2] = -1;// int / flag = error

	cube[4][0][3] = -1; // int * int = int;
	cube[4][1][3] = -1;// int * decimal = decimal
	cube[4][2][3] = -1; // int * text = error
	cube[4][3][3] = -1;// int * character = error
	cube[4][4][3] = -1;// int * flag = error

	cube[4][0][4] = -1; // int % int = int;
	cube[4][1][4] = -1; // int % decimal = error
	cube[4][2][4] = -1;// int % text = error
	cube[4][3][4] = -1;// int % character = error
	cube[4][4][4] = -1;// int % flag = error

	cube[4][0][5] = -1; // int > int = flag
	cube[4][1][5] = -1;// int > decimal = flag
	cube[4][2][5] = -1; // int > text = error
	cube[4][3][5] = -1; // int > character = error
	cube[4][4][5] = 4;// int > flag = error

	cube[4][0][6] = -1; // int < int = flag
	cube[4][1][6] = -1; // int < decimal = flag
	cube[4][2][6] = -1; // int < text = error
	cube[4][3][6] = -1; // int < character = error
	cube[4][4][6] = 4;// int < flag = error

	cube[4][0][7] = -1; // int = int = int;
	cube[4][1][7] = -1; // int = decimal = error
	cube[4][2][7] = -1; // int = text = error
	cube[4][3][7] = -1; // int = character = error
	cube[4][4][7] = 4;// int = flag = error

	cube[4][0][8] = -1; // int and int = error
	cube[4][1][8] = -1; // int and decimal = error
	cube[4][2][8] = -1;// int and text = error
	cube[4][3][8] = -1;// int and character = error
	cube[4][4][8] = 4;// int and flag = error

	cube[4][0][9] = -1;// int or int = error
	cube[4][1][9] = -1;// int or decimal = error
	cube[4][2][9] = -1;// int or text = error
	cube[4][3][9] = -1;// int or character = error
	cube[4][4][9] = 4;// int or flag = error

	cube[4][-1][10] = -1; // int not int = error
	cube[4][-1][10] = -1;// int not decimal = error
	cube[4][-1][10] = -1;// int not text = error
	cube[4][-1][10] = -1;// int not character = error
	cube[4][-1][10] = 4;// int not flag = error

	cube[4][0][11] = -1;// int ntq int = error
	cube[4][1][11] = -1;// int ntq decimal = error
	cube[4][2][11] = -1;// int ntq text = error
	cube[4][3][11] = -1;// int ntq character = error
	cube[4][4][11] = 4;// int ntq flag = error

	cube[4][0][12] = -1; // int eqt int = error
	cube[4][1][12] = -1;// int eqt decimal = error
	cube[4][2][12] = -1; // int eqt text = error
	cube[4][3][12] = -1;// int eqt character = error
	cube[4][4][12] = 4;// int eqt flag = error

	cube[4][0][13] = -1; // int leqt int = error
	cube[4][1][13] = -1; // int leqt decimal = error
	cube[4][2][13] = -1; // int leqt text = error
	cube[4][3][13] = -1; // int leqt character = error
	cube[4][4][13] = -1; // int leqt flag = error

	cube[4][0][14] = -1;// int geqt int = error
	cube[4][1][14] = -1;// int geqt decimal = error
	cube[4][2][14] = -1;// int geqt text = error
	cube[4][3][14] = -1;// int geqt character = error
	cube[4][4][14] = -1;// int geqt flag = error
}

#endif
