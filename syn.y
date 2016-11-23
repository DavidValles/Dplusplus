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
#include "util/variable.cpp"
#include "util/functionTable.cpp"
#include "util/constantTable.cpp"
#include "util/cube.cpp"
#include "util/quadruple.cpp"
#include "util/typeAdapter.cpp"
#include "util/virtualMachine.cpp"
#include "util/operations.h"
#include "util/headers/section.h"

using namespace std;

void yyerror (const char *s);

/* Function specification in implementation */
void setAuxForTemporalVars();
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

/* Keeps track of line number in file */
extern int yylineno;
extern FILE *yyin;

bool debug = false;

/* Helpers for dimension checking */
bool declaring = false;
string id, dId;
bool hasDimension = false;

/* To keep track of temporal variable count in functions */
int integerTaux, decimalTaux, characterTaux, flagTaux;
map<string, int> globalCounts;

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
string currentFunctionInCall;
int currentParameter;

/*
    Utilities for constants
*/
ConstantTable constantTable;

/*
    Utilities for quadruples
*/
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

%token IF
%token ELSE
%token ELSEIF

%token WHILE
%token DO

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
%token BCONSTANT

%union {
  string* stringValue;
  float floatValue;
  int intValue;
}

%%

/*
    A file in D++ can either be a program file with a main function
*/
start       :   program
                {
                    if (debug) {
                        cout<<"Main class compiled."<<endl;
                    }
                    Quadruple qEnd(Ops::End, -1 , -1, -1);
                    quadruples.push_back(qEnd);
                }
            ;

/*
    Basic program structure
    A program can have includes, global variables and functions (0...*)
    It should always have a main method with a block
*/
program     :
            {
                setAuxForTemporalVars();
            }
            variable_section
            {
                // Set the amount of variable used in the global vars
                globalCounts["integerT"] += typeAdapter.integerT.current -
                        integerTaux;
                globalCounts["decimalT"] += typeAdapter.decimalT.current -
                        decimalTaux;
                globalCounts["characterT"] += typeAdapter.characterT.current -
                        characterTaux;
                globalCounts["flagT"] += typeAdapter.flagT.current - flagTaux;

                // Jump to main function
                Quadruple mainQ(Ops::Goto, -1, -1, -1);
                jumpStack.push(quadruples.size());
                quadruples.push_back(mainQ);
            }
            functions
            {
                setAuxForTemporalVars();
            }
            MAIN
            {
                // Add goto jump for main quadruple
                int mainGoTo = jumpStack.top();
                jumpStack.pop();
                quadruples[mainGoTo].result = quadruples.size();
            }
            block
            {
                functionTable.insertFunction("0", -1, -1, -1);

                // Set the amount of variable used in the function
                globalCounts["integerG"] += typeAdapter.integerG.current -
                        typeAdapter.integerG.min;
                globalCounts["decimalG"] += typeAdapter.decimalG.current -
                        typeAdapter.decimalG.min;
                globalCounts["characterG"] += typeAdapter.characterG.current -
                        typeAdapter.characterG.min;
                globalCounts["flagG"] += typeAdapter.flagG.current -
                        typeAdapter.flagG.min;
                globalCounts["integerT"] += typeAdapter.integerT.current -
                        integerTaux;
                globalCounts["decimalT"] += typeAdapter.decimalT.current -
                        decimalTaux;
                globalCounts["characterT"] += typeAdapter.characterT.current -
                        characterTaux;
                globalCounts["flagT"] += typeAdapter.flagT.current - flagTaux;

                functionTable.setVariableCount("0", globalCounts);

                if (debug) {
                    cout<<"Displaying all funcitons"<<endl;
                    functionTable.displayTable();
                    cout<<"Displaying constan table"<<endl;
                    constantTable.displayTable();
                }
            }
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
                            if (!debug)  exit (1);
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

                        // Keep track of where temporal vars where
                        setAuxForTemporalVars();

                        // Reset temporal vars for function use
                        typeAdapter.integerT.current =
                                typeAdapter.integerT.min;
                        typeAdapter.decimalT.current =
                                typeAdapter.decimalT.min;
                        typeAdapter.characterT.current =
                                typeAdapter.characterT.min;
                        typeAdapter.flagT.current =
                                typeAdapter.flagT.min;

                    }
                    '(' params ')' block
                    {
                        Function cFunction =
                                functionTable.getFunction(currentFunction);

                        // Check last quadruple to determine if there was a
                        //      return value
                        if (cFunction.type != typeAdapter.none.type &&
                                quadruples[quadruples.size() - 1].oper !=
                                Ops::Endproc) {
                            cout<<"Error: function should have return value"<<
                                    endl;
                            if (!debug)  exit (1);
                        }

                        Quadruple qEndproc(Ops::Endproc, -1, -1, -1);
                        quadruples.push_back(qEndproc);

                        // Set the amount of variable used in the function
                        map<string, int> counts;
                        counts["integerL"] = typeAdapter.integerL.current -
                                typeAdapter.integerL.min;
                        counts["decimalL"] = typeAdapter.decimalL.current -
                                typeAdapter.decimalL.min;
                        counts["characterL"] = typeAdapter.characterL.current -
                                typeAdapter.characterL.min;
                        counts["flagL"] = typeAdapter.flagL.current -
                                typeAdapter.flagL.min;
                        counts["integerT"] = typeAdapter.integerT.current -
                                typeAdapter.integerT.min;
                        counts["decimalT"] = typeAdapter.decimalT.current -
                                typeAdapter.decimalT.min;
                        counts["characterT"] = typeAdapter.characterT.current -
                                typeAdapter.characterT.min;
                        counts["flagT"] = typeAdapter.flagT.current -
                                typeAdapter.flagT.min;

                        functionTable.setVariableCount(currentFunction,
                                counts);

                        // Reset variables by function to global value
                        typeAdapter.integerL.current =
                                typeAdapter.integerL.min;
                        typeAdapter.decimalL.current =
                                typeAdapter.decimalL.min;
                        typeAdapter.characterL.current =
                                typeAdapter.characterL.min;
                        typeAdapter.flagL.current =
                                typeAdapter.flagL.min;

                        // Reset temp variables by function to global value
                        typeAdapter.integerT.current =
                                integerTaux;
                        typeAdapter.decimalT.current =
                                decimalTaux;
                        typeAdapter.characterT.current =
                                characterTaux;
                        typeAdapter.flagT.current =
                                flagTaux;

                        // For debugging purposes
                        if (debug) {
                            cout<<"Variable table of "<<currentFunction<<endl;
                            currTable->displayTable();
                        }

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
                        if (!debug)  exit (1);
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
            ;

/*
    Function call structure. Can have 0 to n parameters.
*/
functioncall    : ID
                    {
                        operatorStack.push(Ops::Floor);
                        checkFunction();

                        // Set current values for parameter checking
                        currentFunctionInCall = *yylval.stringValue;
                        currentParameter = 0;
                        // TODO: El era deberia ser con un tag de la func
                        Quadruple eraQ(Ops::Era, functionTable.getFunction
                                (currentFunctionInCall).returnAddress, -1, -1);
                        quadruples.push_back(eraQ);
                    }
                    '(' functioncall_ ')'
                    {
                        Function cFunction =
                                functionTable.getFunction(currentFunctionInCall);

                        // Check if parameters in function call match
                        //    the necessary in the declaration
                        if (functionTable.getParametersSize(currentFunctionInCall)
                                != currentParameter) {
                            cout<<"Incorrect number of parameters in function "
                                <<currentFunctionInCall<<" in line "<<yylineno
                                <<endl;
                            if (!debug)  exit (1);
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
                        operatorStack.pop();
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
                                    currentFunctionInCall, type, currentParameter)) {
                            cout<<"Wrong parameter type in function "<<
                                currentFunctionInCall<<", parameter #"<<
                                currentParameter + 1<<" in line "<<yylineno<<endl;
                            if (!debug)  exit (1);
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
                        if (!debug)  exit (1);
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
                        if (!debug)  exit (1);
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
                    Quadruple quadruple(Ops::Goto, -1, -1, returnTop - 1);
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
                        if (!debug)  exit (1);
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
print       : PRINT '(' print_ ')'
                {
                    // This floor is to indicate end of function, vm then
                    //      prints an endline.
                    Quadruple quadruple(Ops::Floor, -1, -1, -1);
                    quadruples.push_back(quadruple);
                }
                ';'
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
            | SCONSTANT
                {
                    insertConstantToTable(typeAdapter.textConstant);
                    int address = operandStack.top();
                    operandStack.pop();
                    // TODO Check cube for print
                    Quadruple quadruple(Ops::Print, -1, -1, address);
                    quadruples.push_back(quadruple);
                }
                print__
            ;

print__     : ',' print_
            |
            ;

/*
    Read structure
*/
read       : READ '(' read_ ')' ';'
            ;

read_      : ID
                {
                    checkVariable();
                    id = *yylval.stringValue;
                }
                dim
                {
                    if (!hasDimension) {
                        id = *yylval.stringValue;
                        int address = currTable->getAddress(id);
                        Quadruple quadruple(Ops::Read, -1, -1, address);
                        quadruples.push_back(quadruple);
                    }
                    else {
                        int address = operandStack.top();
                        operandStack.pop();

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
                /*
            | TEXT
                {
                    if (currTable == &globalTable) {
                        // TODO: do strings
                        currentType = &typeAdapter.none;
                    } else {
                        currentType = &typeAdapter.none;
                    }
                }
                */
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
            | '-'
                {
                    operatorStack.push(Ops::Multiplication);
                    *yylval.stringValue = "-1";
                    insertConstantToTable(typeAdapter.integerConstant);
                }
                constvar
                {
                    checkOperator(Ops::Multiplication, Ops::Division);
                }
            ;

constvar    : ID
                {
                    id = *yylval.stringValue;
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
            | BCONSTANT
                {
                    insertConstantToTable(typeAdapter.flagConstant);
                }
            | functioncall
            ;

/*
    Rules for determining if variable is dimensionated
*/
dim         : '['
                {
                    if (currTable->getDimension(id, 1)) {
                        dimensionStack.push(id);
                        operatorStack.push(Ops::Floor);
                    }
                }
                expression
                {
                    dId = dimensionStack.top();
                    int dim1 = currTable->getDimension(dId, 1);
                    if (!dim1) {
                        cout<<"Error! Line "<<yylineno<<". Variable has no "
                            <<"such dimension: "<<dId<<endl;
                        if (!debug)  exit (1);
                    }
                    if (typeStack.top() != typeAdapter.integerG.type) {
                        cout<<"Error! Line "<<yylineno<<". Index must be an"
                            <<" integer."<<endl;
                        if (!debug)  exit (1);
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

                        *yylval.stringValue = to_string(baseAddress);
                        insertConstantToTable(typeAdapter.integerConstant);

                        int baseAddressAsConstant = operandStack.top();
                        operandStack.pop();
                        typeStack.pop();

                        Quadruple qS1(Ops::Sum, baseAddressAsConstant, index, result);
                        quadruples.push_back(qS1);

                        typeAdapter.integerT.setNextAddress();

                        operandStack.push(result);
                        typeStack.push(typeAdapter.getType(baseAddress));
                    }
                    else {
                        *yylval.stringValue = to_string(dim2);
                        insertConstantToTable(typeAdapter.integerConstant);

                        int dim2AsConstant = operandStack.top();
                        operandStack.pop();
                        typeStack.pop();

                        Quadruple qS1D2(Ops::Multiplication, index,
                                dim2AsConstant, typeAdapter.integerT.current);
                        quadruples.push_back(qS1D2);

                        operandStack.push(typeAdapter.integerT.current);
                        typeStack.push(typeAdapter.integerT.type);
                        typeAdapter.integerT.setNextAddress();
                    }
                }
                dim_ ']'
            |
                {
                    hasDimension = false;
                    if (currTable->findVariable(id)) {
                        Variable var = currTable->getVariable(id);
                        if (var.dimension1 != 0) {
                            cout<<"Error! Line "<<yylineno<<". Specify index for"
                                <<" first dimension of variable "<<id<<endl;
                            if (!debug)  exit (1);
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
                        if (!debug)  exit (1);
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

                    *yylval.stringValue = to_string(baseAddress);
                    insertConstantToTable(typeAdapter.integerConstant);

                    int baseAddressAsConstant = operandStack.top();
                    operandStack.pop();
                    typeStack.pop();

                    typeAdapter.integerT.setNextAddress();
                    Quadruple qS1(Ops::Sum, baseAddressAsConstant, s1d2s2, result);
                    quadruples.push_back(qS1);

                    operandStack.push(result);
                    typeStack.push(typeAdapter.getType(baseAddress));
                }
            |
                {
                    int dim2 = currTable->getDimension(dId, 2);
                    if (dim2) {
                        cout<<"Error! Line "<<yylineno<<". Specify index for"
                            <<" second dimension of variable "<<id<<endl;
                        if (!debug)  exit (1);
                    }
                }
            ;

%%

/*
    Keeps track of temporal variables
*/
void setAuxForTemporalVars() {
    integerTaux = typeAdapter.integerT.current;
    decimalTaux = typeAdapter.decimalT.current;
    characterTaux = typeAdapter.characterT.current;
    flagTaux = typeAdapter.flagT.current;
}

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
                if (!debug)  exit (1);
            }
        }
    }
}

/*
    Checks if the variable id in the current yylval has already been declared
*/
void checkRedefinition() {
    string id = *yylval.stringValue;
    if (currTable->findVariableInCurrentTable(id)) {
        cout<<"Error! Line: "<<yylineno
            <<". Variable has already been defined: "<<id<<"."<<endl;
        if (!debug)  exit (1);
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
        if (!debug)  exit (1);
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
        if (!debug)  exit (1);
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

/*
    Steps required for when a retun is found
*/
void returnProcess() {
    Function function =
            functionTable.getFunction(currentFunction);

    if (function.type == typeAdapter.none.type) {
        cout<<"None functions cannot have a return statement"
                <<endl;
        if (!debug)  exit (1);
    }

    int returnAddress = operandStack.top();
    operandStack.pop();
    int returnType = typeStack.top();
    typeStack.pop();

    if (function.type != returnType) {
        cout<<"Error in line "<<yylineno<<
                ": Incorrect return type."<<endl;
        if (!debug)  exit (1);
    }

    Quadruple assignReturn(Ops::Equal, returnAddress, -1,
            function.returnAddress);
    quadruples.push_back(assignReturn);

    operandStack.push(function.returnAddress);
    typeStack.push(function.type);

    //Generate return quadruple
    Quadruple returnQ(Ops::Endproc, -1, -1, -1);
    quadruples.push_back(returnQ);
}

int main(int argc, char **argv)
{
    FILE *myfile;
    if ((argc > 1))
    {
        myfile = fopen(argv[1], "r");
        if (!myfile) {
            printf("Could not open file.\n");
            exit( 1 );
        }
    }
    yyin = myfile;

    if (argc > 2) {
        debug = argv[2];
    }

    yyparse();

    if (debug) {
	    cout<<"Displaying global variable table"<<endl;
        currTable->displayTable();

	    cout<<"Displaying quadruples"<<endl;
        for (int i=0; i<quadruples.size(); i++) {
            cout<<i<<". ";
            quadruples[i].display();
        }
        cout<<"STARTING PROGRAM (VM)"<<endl;
    }

    VirtualMachine vm(functionTable, quadruples, constantTable, typeAdapter);

    return 0;
}

void yyerror (const char *s) {
    fprintf (stderr, "%s at line %d.\n", s, yylineno);
}
