%code requires
{
    #include <string>
    using namespace std;
}

%{
#include <stdio.h>
#include <stdlib.h>

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iostream>
#include <stack>

#include "util/variableTable.cpp"
#include "util/functionTable.cpp"
#include "util/constantTable.cpp"
#include "util/cube.cpp"
#include "util/quadruple.cpp"
#include "util/typeAdapter.cpp"
#include "util/operations.h"
#include "util/headers/section.h"

using namespace std;

void yyerror (const char *s);
void addGotoAndFillGotoFalse();
void insertConstantToTable(Section &constantAddress);
void checkRedefinition();
void checkVariable();
void checkFunction();
bool checkConstant();
int getTemporalAddress(int type);
void checkOperator(int oper1, int oper2 = -1, bool isRelational = false);
void returnProcess();

extern "C"
{
    int yylex();
}

extern int yylineno;
bool declaring = false;
string id, dId;
bool hasDimension = false;

/*
    Utilities to find identifiers
    currTable indicates the curren table in scope
*/
VariableTable globalTable;
VariableTable localTable(&globalTable);
VariableTable* currTable = &globalTable;
Section* currentType;

/*
    Utilities for functions and syntax checking
*/
FunctionTable functionTable;
string currentFunction;
int currentParameter;

/*
    Utilities for constants
*/
ConstantTable constantTable;

// Quadruples
Cube cube;
vector<Quadruple> quadruples;
TypeAdapter typeAdapter;
stack<int> typeStack;
stack<int> operandStack;
stack<int> operatorStack;
stack<int> jumpStack;
stack<string> dimensionStack;
unordered_set<int> relationalOperators = {
    Ops::GreaterThan,
    Ops::LessThan,
    Ops::NotEqualTo,
    Ops::EqualTo,
    Ops::LessThanOrEqualTo,
    Ops::GreaterThanOrEqualTo
};

%}

%start start
%token INCLUDE
%token MAIN
%token RETURN
%token NONE

%token VAR
%token FUNC
%token INT
%token DECIMAL
%token TEXT
%token CHARACTER
%token FLAG
%token ARRAY
%token MATRIX

%token IF
%token ELSE
%token ELSEIF

%token WHILE
%token DO

%token CLASS
%token PRIVATE
%token PUBLIC

%token PRINT
%token READ

%token NOTEQUALTO
%token EQUALTO
%token LESSTHANOREQUALTO
%token GREATERTHANOREQUALTO
%token AND
%token OR
%token NOT

%token ID
%token ICONSTANT
%token DCONSTANT
%token SCONSTANT
%token CCONSTANT

%union {
  string* stringValue;
  float floatValue;
  int intValue;
}

%%

/*
    A file in D++ can either be a program file with a main function or a
    class file with a class definition.
*/
start       :   program
                {
                    cout<<"Main class compiled."<<endl;
                    Quadruple qEnd(Ops::End, -1 , -1, -1);
                    quadruples.push_back(qEnd);
                }
            | class
                {
                    cout<<"User defined class compiled."<<endl;
                }
            ;

/*
    Basic program structure
    A program can have includes, global variables and functions (0...*)
    It should always have a main method with a block
*/
program     : includes variable_section
                {
                    Quadruple mainQ(Ops::Goto, -1, -1, -1);
                    jumpStack.push(quadruples.size());
                    quadruples.push_back(mainQ);
                }
                functions MAIN
                {
                    // Add goto jump for main quadruple
                    int mainGoTo = jumpStack.top();
                    jumpStack.pop();
                    quadruples[mainGoTo].result = quadruples.size();
                }
                block
            ;

/*
    Include section for global scope
    E.g. include "myClass.h"
*/
includes    : INCLUDE SCONSTANT includes
            |
            ;

/*
    Variable section for global scope
    E.g. var integer id1;
*/
variable_section    : variables variable_section
                    |
                    ;

/*
    Declaring one or more variables in one line
    E.g. var integer id1, id2 = 3, id3 = 2 + 23 - id1;
*/
variables   : VAR
                {
                    // This is used for assignment rule.
                    declaring = true;
                }
                type variable ';'
                {
                    declaring = false;
                }
            ;

variable    : ID
                {
                    id = *yylval.stringValue;
                    checkRedefinition();
                    currTable->insertVariable(id, currentType->current);
                    currentType->setNextAddress();
                }
                dimension
                {
                    // Add dimension if such exist
                    int dim1 = currTable->getDimension(id, 1);
                    int dim2 = currTable->getDimension(id, 2);
                    if (dim2 != 0) {
                        currentType->setNextAddress(
                            currTable->getDimension(id, 1) *
                            currTable->getDimension(id, 2) - 1);
                    } else if (dim1 != 0) {
                        currentType->setNextAddress(
                            currTable->getDimension(id, 1) - 1);
                    }
                }
                variable_
            | assignment variable_
            ;

variable_   : ',' variable
            |
            ;

dimension   : '(' ICONSTANT
                {
                    string constant = *yylval.stringValue;
                    int constantInt = stoi(constant);
                    currTable->insertFirstDimension(id, constantInt);
                }
                dimension_ ')'
            |
            ;

dimension_  : ',' ICONSTANT
                {
                    string constant = *yylval.stringValue;
                    int constantInt = stoi(constant);
                    currTable->insertSecondDimension(id, constantInt);
                }
            |
            ;

assignment  : ID
                {
                    id = *yylval.stringValue;

                    // If the assignment rule comes from a block rule it should
                    // be checked and if it has a dimension, pushed to stack.
                    if (!declaring) {
                        checkVariable();
                        if (currTable->getDimension(id, 1)) {
                            dimensionStack.push(id);
                        }
                    }
                }
                dim
                {
                    id = *yylval.stringValue;

                    // If the assignment rule is used by the variable rule
                    // this means it is being declaring and has to be checked
                    // for redefinition and insert it into the table.
                    if (declaring) {
                        checkRedefinition();
                        currTable->insertVariable(id, currentType->current);
                        currentType->setNextAddress();
                    }

                    // If it had dimension it was already processed.
                    if (!hasDimension) {
                        int address = currTable->getAddress(id);
                        int type = typeAdapter.getType(address);
                        operandStack.push(address);
                        typeStack.push(type);
                    }
                    operatorStack.push(Ops::Equal);
                }
                assignment_
                {
                    int oper = operatorStack.top();
                    if (oper == Ops::Equal) {
                        operatorStack.pop();
                        int assign = operandStack.top();
                        operandStack.pop();
                        int to = operandStack.top();
                        operandStack.pop();
                        int assignType = typeStack.top();
                        typeStack.pop();
                        int toType = typeStack.top();
                        typeStack.pop();

                        if (cube.cube[toType][assignType][Ops::Equal] == -1 ) {
                            cout<<"Error in assignment with type. Line "<<
                                    yylineno<<endl;
                        }
                        Quadruple quadruple(Ops::Equal, assign, -1, to);
                        quadruples.push_back(quadruple);
                    }
                }
            ;

/*
    Function section for global scope
    E.g.
*/
functions   : singlefunction functions
            |
            ;

singlefunction  : FUNC singlefunction_ ID
                    {
                        // Insert function to table
                        currentFunction = *yylval.stringValue;
                        functionTable.insertFunction(currentFunction,
                                        currentType->type, quadruples.size(),
                                        currentType->current);
                        currentType->setNextAddress();

                        // Switch scope to local table
                        currTable = &localTable;
                    }
                    '(' params ')' block
                    {
                        Function cFunction =
                                functionTable.getFunction(currentFunction);

                        // Check last quadruple to determine if there was a
                        //      return value
                        if (cFunction.type != typeAdapter.none.type &&
                                quadruples[quadruples.size() - 1].oper !=
                                Ops::Return) {
                            cout<<"Error: function should have return value"<<
                                    endl;
                        }

                        Quadruple qEndproc(Ops::Endproc, -1, -1, -1);
                        quadruples.push_back(qEndproc);

                        // For debugging purposes
                        cout<<"Displaying function table"<<endl;
                        functionTable.displayTable();
	                    cout<<"Displaying local variable table"<<endl;
                        currTable->displayTable();

                        // Clear the local variable table and change to global
                        //      scope
                        currTable->clearVarTable();
                        currTable = &globalTable;
                    }
                ;

singlefunction_ : type
                | NONE { currentType = &typeAdapter.none; }
                ;

/*
    Declaring zero to n paramaters
*/
params      : type
                {
                    functionTable.addParameterToFunction(currentFunction,
                                        currentType->type);
                }
                ID
                {
                    id = *yylval.stringValue;
                    currTable->insertVariable(id, currentType->current);
                    currentType->setNextAddress();
                }
                params_
            |
            ;

params_     : ',' params
            |
            ;

/*
    Typical block of code with statements
*/
block       : '{' block_ return '}'
            ;

block_      : statement block_
            |
            ;

return      : RETURN expression
                {
                    if (currTable == &globalTable) {
                        cout<<"Error: cannot have return in main."<<endl;
                    }
                    returnProcess();
                }
                ';'
            |
            ;

/*
    Various statements available for blocks
*/
statement   : assignment ';'
            | cycle
            | if
            | print
            | read
            | variables
            | functioncall ';'
            ;

/*
    Assignments can have expressions, a string " ",
        and a character ' '
*/
assignment_ : '=' assignment__
            ;

assignment__: expression
            | SCONSTANT
                {
                    insertConstantToTable(typeAdapter.textConstant);
                }
            | CCONSTANT
                {
                    insertConstantToTable(typeAdapter.characterConstant);
                }
            | functioncall
            ;

/*
    Function call structure. Can have 0 to n parameters.
*/
functioncall    : ID
                    {
                        checkFunction();

                        // Set current values for parameter checking
                        currentFunction = *yylval.stringValue;
                        currentParameter = 0;
                        // TODO: El era deberia ser con un tag de la func
                        Quadruple eraQ(Ops::Era, functionTable.getFunction
                                (currentFunction).quadruple, -1, -1);
                        quadruples.push_back(eraQ);
                    }
                    '(' functioncall_ ')'
                    {
                        Function cFunction =
                                functionTable.getFunction(currentFunction);

                        // Check if parameters in function call match
                        //    the necessary in the declaration
                        if (functionTable.getParametersSize(currentFunction)
                                != currentParameter) {
                            cout<<"Incorrect number of parameters in function "
                                <<currentFunction<<" in line "<<yylineno
                                <<endl;
                        }

                        Quadruple goSubQ(Ops::GoSub, cFunction.quadruple,
                                -1, -1);
                        quadruples.push_back(goSubQ);

                        // Push the return value to operandStack
                        // Plus function address is moved to avail to deal with
                        //      recursive calls.
                        if (cFunction.type != typeAdapter.none.type) {
                            int tempReturn = getTemporalAddress(cFunction.type);

                            Quadruple moveReturn(Ops::Equal,
                                    cFunction.returnAddress, -1, tempReturn);
                            quadruples.push_back(moveReturn);

                            operandStack.push(tempReturn);
                            typeStack.push(cFunction.type);
                        }
                    }
                ;

functioncall_   : expression
                    {
                        // Get type of the parameter from the expression
                        int address = operandStack.top();
                        operandStack.pop();
                        int type = typeStack.top();
                        typeStack.pop();

                        // Check if parameter matches type from function def
                        if (!functionTable.checkTypeOfParameter(
                                    currentFunction, type, currentParameter)) {
                            cout<<"Wrong parameter type in function "<<
                                currentFunction<<", parameter #"<<
                                currentParameter + 1<<" in line "<<yylineno<<endl;
                        }
                        Quadruple paramQ(Ops::Param, address, -1, -1);
                        quadruples.push_back(paramQ);
                        currentParameter++;
                    }
                    functioncall__
                |
                ;

functioncall__  : ',' functioncall_
                |
                ;

/*
    While and do while strucutre
*/
cycle       : DO
                {
                    // Keep track of where to return after while
                    jumpStack.push(quadruples.size());
                }
                block while
                {
                    // Verify that the expression yielded a boolean value
                    if (typeStack.top() != typeAdapter.flagG.type) {
                        cout<<"Error! Line "<<yylineno<<". Condition in while"
                                <<" must be a flag."<<endl;
                    }
                    else {
                        // Quadruple index of where to return
                        int gotoTrue = jumpStack.top();
                        jumpStack.pop();

                        // Get while condition's address
                        int address = operandStack.top();
                        operandStack.pop();
                        typeStack.pop();

                        Quadruple quadruple(Ops::GotoTrue, address, -1,
                                gotoTrue);
                        quadruples.push_back(quadruple);
                    }
                }
            | while
                {
                    // Verify that the expression yielded a boolean value
                    if (typeStack.top() != typeAdapter.flagG.type) {
                        cout<<"Error! Line "<<yylineno<<". Condition in while"
                                <<" must be a flag."<<endl;
                    }
                    else{
                        // Keep track of GoToFalse
                        jumpStack.push(quadruples.size());

                        int address = operandStack.top();
                        operandStack.pop();
                        typeStack.pop();
                        Quadruple quadruple(Ops::GotoFalse, address, -1, -1);
                        quadruples.push_back(quadruple);

                        // Keep track of where to return at the end of while
                        //      block.
                        jumpStack.push(quadruples.size() - 1);
                    }
                }
                block
                {
                    // Where to return after while block.
                    int returnTop = jumpStack.top();
                    jumpStack.pop();
                    Quadruple quadruple(Ops::Goto, -1, -1, returnTop);
                    quadruples.push_back(quadruple);

                    // Fill out goToFalse
                    int gotoFalse = jumpStack.top();
                    jumpStack.pop();
                    quadruples[gotoFalse].result = quadruples.size();
                }
            ;

while       : WHILE '(' expression ')'
            ;

/*
    If structure
*/
if          : IF
                {
                    // Floor for else if GoTo generation
                    jumpStack.push(-1);
                }
                if_ else
                {
                    int gotoJump = jumpStack.top();

                    // Fill out all GoTo from if and else ifs
                    while (gotoJump != -1) {
                        jumpStack.pop();
                        quadruples[gotoJump].result = quadruples.size();
                        gotoJump = jumpStack.top();
                    }
                    if (gotoJump == -1) {
                        jumpStack.pop();
                    }
                }
            ;

if_         : '(' expression ')'
                {
                    // Verify that the expression yielded a boolean value
                    if(typeStack.top() != typeAdapter.flagG.type){
                        cout<<"Error! Line "<<yylineno<<". Condition in if"
                                <<" must be a flag."<<endl;
                    }
                    else {
                        // Gotofalse after if or else if expression
                        int address = operandStack.top();
                        operandStack.pop();
                        typeStack.pop();
                        Quadruple quadruple(Ops::GotoFalse, address, -1, -1);
                        quadruples.push_back(quadruple);

                        // Keep track of where GoToFalse is
                        jumpStack.push(quadruples.size() - 1);
                    }
                }
                block else_if
            ;

else_if     : ELSEIF
                {
                    addGotoAndFillGotoFalse();
                }
              if_
            |
            ;

else        : ELSE
                {
                    addGotoAndFillGotoFalse();
                }
              block
            |
            ;

/*
    Print structure
*/
print       : PRINT '(' print_ ')' ';'
            ;

print_      : expression
                {
                    int address = operandStack.top();
                    operandStack.pop();
                    // TODO Check cube for print
                    Quadruple quadruple(Ops::Print, -1, -1, address);
                    quadruples.push_back(quadruple);
                }
                print__
            | SCONSTANT print__
            ;

print__     : ',' print_
            |
            ;

/*
    Print structure
*/
read       : READ '(' read_ ')' ';'
            ;

read_      : ID
                {
                    checkVariable();
                    id = *yylval.stringValue;
                    if (currTable->getDimension(id, 1)) {
                        dimensionStack.push(id);
                    }
                }
                dim
                {
                    if (!hasDimension) {
                        id = *yylval.stringValue;
                        int address = currTable->getAddress(id);
                        Quadruple quadruple(Ops::Read, -1, -1, address);
                        quadruples.push_back(quadruple);
                    }
                }
                read__
            ;

read__     : ',' read_
            |
            ;

/*
    Class structure
*/

class       : CLASS ID '{' classblock '}'
            ;

classblock  : classblock_ classblock
            |
            ;

classblock_ : PRIVATE ':'
            | PUBLIC ':'
            | variables
            | singlefunction
            ;

/*
    Various accepted types
*/
type        : INT
                {
                    if (currTable == &globalTable) {
                        currentType = &typeAdapter.integerG;
                    } else {
                        currentType = &typeAdapter.integerL;
                    }
                }
            | DECIMAL
                {
                    if (currTable == &globalTable) {
                        currentType = &typeAdapter.decimalG;
                    } else {
                        currentType = &typeAdapter.decimalL;
                    }
                }
            | TEXT
                {
                    if (currTable == &globalTable) {
                        currentType = &typeAdapter.textG;
                    } else {
                        currentType = &typeAdapter.textL;
                    }
                }
            | CHARACTER
                {
                    if (currTable == &globalTable) {
                        currentType = &typeAdapter.characterG;
                    } else {
                        currentType = &typeAdapter.characterL;
                    }
                }
            | FLAG
                {
                    if (currTable == &globalTable) {
                        currentType = &typeAdapter.flagG;
                    } else {
                        currentType = &typeAdapter.flagL;
                    }
                }
                /* TODO: Array and Matrix;
            ;


/*
    Basic expression structure
*/
expression  : and
                {
                    checkOperator(Ops::Or);
                }
                expression_
            ;

expression_ : OR
                {
                    operatorStack.push(Ops::Or);
                }
                expression
            |
            ;

and         : numexp
                {
                    checkOperator(Ops::And);
                }
                and_
            ;

and_        : AND
                {
                    operatorStack.push(Ops::And);
                }
                and
            |
            ;

numexp      : NOT
                {
                    operatorStack.push(Ops::Not);
                    operandStack.push(-1);
                    typeStack.push(typeAdapter.flagG.type);
                }
                exp numexp_ { checkOperator(Ops::Not); }
            | exp numexp_
            ;

numexp_     : '>' { operatorStack.push(Ops::GreaterThan); }
                exp
                { checkOperator(Ops::GreaterThan, -1, true); }
            | '<' { operatorStack.push(Ops::LessThan); }
                exp
                { checkOperator(Ops::LessThan, -1, true); }
            | LESSTHANOREQUALTO
                { operatorStack.push(Ops::LessThanOrEqualTo); }
                exp
                { checkOperator(Ops::LessThanOrEqualTo, -1, true); }
            | GREATERTHANOREQUALTO
                { operatorStack.push(Ops::GreaterThanOrEqualTo); }
                exp
                { checkOperator(Ops::GreaterThanOrEqualTo, -1, true); }
            | EQUALTO { operatorStack.push(Ops::EqualTo); }
                exp
                { checkOperator(Ops::EqualTo, -1, true); }
            | NOTEQUALTO { operatorStack.push(Ops::NotEqualTo); }
                exp
                { checkOperator(Ops::NotEqualTo, -1, true); }
            |
            ;

exp         : term
                {
                    checkOperator(Ops::Sum, Ops::Minus);
                }
                exp_
            ;

exp_        : '+'
                {
                    operatorStack.push(Ops::Sum);
                }
                exp
            | '-'
                {
                    operatorStack.push(Ops::Minus);
                }
                exp
            |
            ;

term        : factor
                {
                    checkOperator(Ops::Multiplication, Ops::Division);
                }
                term_
            ;

term_       : '*'
                {
                    operatorStack.push(Ops::Multiplication);
                }
                term
            | '/'
                {
                    operatorStack.push(Ops::Division);
                }
                term
            |
            ;

factor      :   {
                    operatorStack.push(Ops::Floor);
                }
                '(' expression ')'
                {
                    operatorStack.pop();
                }
            | constvar
            | '+' constvar /* TODO: solve this */
            | '-' constvar
            ;

constvar    : ID
                {
                    id = *yylval.stringValue;
                    if (currTable->getDimension(id, 1)) {
                        dimensionStack.push(id);
                        operatorStack.push(Ops::Floor);
                    }
                    checkVariable();
                }
                dim
                {
                    if (hasDimension) {
                        dimensionStack.pop();
                        operatorStack.pop();
                    }
                    else {
                        // Push operand and type to respective stacks
                        id = *yylval.stringValue;
                        int address = currTable->getAddress(id);
                        int type = typeAdapter.getType(address);
                        typeStack.push(type);
                        operandStack.push(address);
                    }
                }
            | ICONSTANT
                {
                    insertConstantToTable(typeAdapter.integerConstant);
                }
            | DCONSTANT
                {
                    insertConstantToTable(typeAdapter.decimalConstant);
                }
            ;

dim         : '[' expression
                {
                    dId = dimensionStack.top();
                    int dim1 = currTable->getDimension(dId, 1);
                    if (!dim1) {
                        cout<<"Error! Line "<<yylineno<<". Variable has no "
                            <<"such dimension: "<<dId<<endl;
                    }
                    if (typeStack.top() != typeAdapter.integerG.type) {
                        cout<<"Error! Line "<<yylineno<<". Index must be an"
                            <<" integer."<<endl;
                    }
                    hasDimension = true;
                    int dim2 = currTable->getDimension(dId, 2);
                    int index = operandStack.top();
                    typeStack.pop();
                    operandStack.pop();

                    Quadruple qCheck(Ops::Check, 0, dim1, index);
                    quadruples.push_back(qCheck);

                    if (!dim2) {
                        int baseAddress = currTable->getAddress(dId);
                        int result = typeAdapter.integerT.current * -1;
                        Quadruple qS1(Ops::Sum, baseAddress, index, result);
                        quadruples.push_back(qS1);

                        typeAdapter.integerT.setNextAddress();

                        operandStack.push(result);
                        typeStack.push(typeAdapter.integerT.type);
                    }
                    else {
                        Quadruple qS1D2(Ops::Multiplication, index, dim2,
                                typeAdapter.integerT.current);
                        quadruples.push_back(qS1D2);

                        operandStack.push(typeAdapter.integerT.current);
                        typeStack.push(typeAdapter.integerT.type);
                        typeAdapter.integerT.setNextAddress();
                    }
                }
                dim_ ']'
            |
                {
                    if (currTable->findVariable(id)) {
                        hasDimension = false;
                        int dim1 = currTable->getDimension(id, 1);
                        if (dim1) {
                            cout<<"Error! Line "<<yylineno<<". Specify index for"
                                <<" first dimension of variable "<<id<<endl;
                        }
                    }
                }
            ;

dim_        : ',' expression
                {
                    int dim2 = currTable->getDimension(dId, 2);
                    if (!dim2) {
                        cout<<"Error! Line "<<yylineno<<". Variable has no "
                            <<"such dimension: "<<dId<<endl;
                    }
                    hasDimension = true;
                    // Get the result from the expression
                    int index = operandStack.top();
                    typeStack.pop();
                    operandStack.pop();

                    Quadruple qCheck(Ops::Check, 0, dim2, index);
                    quadruples.push_back(qCheck);

                    int s2 = operandStack.top();
                    typeStack.pop();
                    operandStack.pop();

                    int s1d2s2 = typeAdapter.integerT.current;
                    typeAdapter.integerT.setNextAddress();
                    Quadruple qs1d2s2(Ops::Sum, index, s2, s1d2s2);
                    quadruples.push_back(qs1d2s2);

                    int baseAddress = currTable->getAddress(dId);
                    int result = typeAdapter.integerT.current * -1;

                    typeAdapter.integerT.setNextAddress();
                    Quadruple qS1(Ops::Sum, baseAddress, s1d2s2, result);
                    quadruples.push_back(qS1);

                    operandStack.push(result);
                    typeStack.push(typeAdapter.integerT.type);
                }
            |
                {
                    int dim2 = currTable->getDimension(dId, 2);
                    if (dim2) {
                        cout<<"Error! Line "<<yylineno<<". Specify index for"
                            <<" second dimension of variable "<<id<<endl;
                    }
                }
            ;

%%

/*
    Generates a goto after a condition block and fills its previous GoToFalse.
*/
void addGotoAndFillGotoFalse() {
    int gotoFalse = jumpStack.top();
    jumpStack.pop();

    Quadruple quadruple(Ops::Goto, -1, -1, -1);
    quadruples.push_back(quadruple);

    quadruples[gotoFalse].result = quadruples.size();
    jumpStack.push(quadruples.size() - 1);
}

/*
    Inserts a constant to the table of constants depending on its type
    It also pushes the constant and type to their stacks
*/
void insertConstantToTable(Section &constant) {
    string id = *yylval.stringValue;
    if (!checkConstant()) {
        constantTable.insertConstant(id, constant.current);
        constant.setNextAddress();
    }
    int address = constantTable.getAddress(id);
    int type = constant.type;
    typeStack.push(type);
    operandStack.push(address);
}

/*
    Checks if the top operator of the stack matches the wanted operators
        (the parameters).
    i.e. checkOperator(Ops::Multiplication, Ops::Division)

    Also can be called without parameters to check if the operator belongs to
        the relationalOperators like <, >
*/
void checkOperator(int oper1, int oper2, bool isRelational) {
    if (!operatorStack.empty()) {
        int oper = operatorStack.top();
        if (oper == oper1 || oper == oper2 ||
                (relationalOperators.find(oper) != relationalOperators.end()
                && isRelational)) {
            operatorStack.pop();
            int type2 = typeStack.top();
            typeStack.pop();
            int type1 = typeStack.top();
            typeStack.pop();
            int resultType = cube.cube[type1][type2][oper];

            // Check if result is valid
            if (resultType != -1) {
                int operand2 = operandStack.top();
                operandStack.pop();
                int operand1 = operandStack.top();
                operandStack.pop();
                int tempAddress = getTemporalAddress(resultType);
                Quadruple quadruple(oper, operand1, operand2, tempAddress);
                quadruples.push_back(quadruple);
                operandStack.push(tempAddress);
                typeStack.push(resultType);
            }
            else {
                cout<<"Type mismatch in line: "<<yylineno<<endl;
            }
        }
    }
}

/*
    Checks if the variable id in the current yylval has already been declared
*/
void checkRedefinition() {
    string id = *yylval.stringValue;
    if (currTable->findVariable(id)) {
        cout<<"Error! Line: "<<yylineno
            <<". Variable has already been defined: "<<id<<"."<<endl;
    }
}

/*
    Checks if the variable id in the current yylval has been declared
*/
void checkVariable() {
    string id = *yylval.stringValue;
    if (!currTable->findVariable(id)) {
        cout<<"Error! Line: "<<yylineno<<". Variable not defined: "<<id<<
            "."<<endl;
    }
}

/*
    Checks if the function id in the current yylval has been declared
*/
void checkFunction() {
    string id = *yylval.stringValue;
    if(!functionTable.findFunction(id)) {
        cout<<"Error! Line: "<<yylineno<<". Function not defined: "<<
            id<<"."<<endl;
    }
}

/*
    Checks if the constant id in the current yylval has been added to the
        constant table
*/
bool checkConstant() {
    string id = *yylval.stringValue;
    bool check = constantTable.findConstant(id);
    return check;
}

/*
    Retruns a temporal addres for a certain type
*/
int getTemporalAddress(int type) {
    int tempAddress;
    if (type == typeAdapter.integerT.type) {
        tempAddress = typeAdapter.integerT.current;
        typeAdapter.integerT.setNextAddress();
    }
    else if (type == typeAdapter.decimalT.type) {
        tempAddress = typeAdapter.decimalT.current;
        typeAdapter.decimalT.setNextAddress();
    }
    else if (type == typeAdapter.textT.type) {
        tempAddress = typeAdapter.textT.current;
        typeAdapter.textT.setNextAddress();
    }
    else if (type == typeAdapter.characterT.type) {
        tempAddress = typeAdapter.characterT.current;
        typeAdapter.characterT.setNextAddress();
    }
    else if (type == typeAdapter.flagT.type) {
        tempAddress = typeAdapter.flagT.current;
        typeAdapter.flagT.setNextAddress();
    }
    return tempAddress;
}

void returnProcess() {
    Function function =
            functionTable.getFunction(currentFunction);

    if (function.type == typeAdapter.none.type) {
        cout<<"None functions cannot have a return statement"
                <<endl;
    }

    int returnAddress = operandStack.top();
    operandStack.pop();
    int returnType = typeStack.top();
    typeStack.pop();

    if (function.type != returnType) {
        cout<<"Error in line "<<yylineno<<
                ": Incorrect return type."<<endl;
    }

    Quadruple assignReturn(Ops::Equal, returnAddress, -1,
            function.returnAddress);
    quadruples.push_back(assignReturn);

    operandStack.push(function.returnAddress);
    typeStack.push(function.type);

    //Generate return quadruple
    Quadruple returnQ(Ops::Return, -1, -1, -1);
    quadruples.push_back(returnQ);
}

int main(int argc, char **argv)
{
    if ((argc > 1) && (freopen(argv[1], "r", stdin) == NULL))
    {
        printf("Could not open file.\n");
        exit( 1 );
    }

    yyparse();

	cout<<"Displaying global variable table"<<endl;
    currTable->displayTable();

	cout<<"Displaying quadruples"<<endl;
    for (int i=0; i<quadruples.size(); i++) {
        cout<<i<<". ";
        quadruples[i].display();
    }

    return 0;
}

void yyerror (const char *s) {
    fprintf (stderr, "%s at line %d.\n", s, yylineno);
}
