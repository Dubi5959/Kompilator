#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <fstream>

using namespace std;

typedef struct procedureHeader{
    string procedureName;
    vector<string> arguments;
} procedureHeader;

struct instruction{
    string instructionName;
};


procedureHeader* proc_head_function(string* s, vector<pair<string*,bool>>* argumentsList, long long a);
void varDeclaration(string* s,long long a);
void arrayDeclaration(string* s,long long sizeArray, long long a);
string generateNumber(long long address);
vector<instruction>*  read(vector<instruction>* a);
vector<instruction>*  write (vector<instruction>* a);

void saveProgram(string output, vector<instruction>* i1, vector<instruction>* i2);
vector<instruction>* makeEmptyVec();
vector<instruction>* procedures (vector<instruction>* a,procedureHeader* b, vector<instruction>* c);
void setMain(); 
vector<instruction>* merge(vector<instruction>*  a, vector<instruction>*  b);
vector<instruction>* assign(vector<instruction>*  a, vector<instruction>*  b);
vector<instruction>* ifElseFunction(vector<instruction>*  a, vector<instruction>*  b, vector<instruction>*  c);
vector<instruction>* ifFunction(vector<instruction>*  a, vector<instruction>*  b);
vector<instruction>* whileDoFunction(vector<instruction>*  a, vector<instruction>*  b);
vector<instruction>* repeatUntilFunction(vector<instruction>*  b, vector<instruction>*  a);
vector<instruction>* proc_call_function(string* s,vector<string>* a, long long b);
vector<pair<string*,bool>>* args_decl_function(vector<pair<string*,bool>>* a,string* s, bool b);
vector<pair<string*,bool>>* args_decl_function(string* s, bool b);
vector<string>*  args_calling(vector<string>* a,string* s);
vector<string>*  args_calling(string* s);
vector<instruction>* add(vector<instruction>*  a, vector<instruction>*  b);
vector<instruction>* sub(vector<instruction>*  a, vector<instruction>*  b);
vector<instruction>* multi(vector<instruction>*  a, vector<instruction>*  b);
vector<instruction>* division(vector<instruction>*  a, vector<instruction>*  b);
vector<instruction>* mod(vector<instruction>*  a, vector<instruction>*  b);
vector<instruction>* isEqual(vector<instruction>*  a, vector<instruction>*  b);
vector<instruction>* isNotEqual(vector<instruction>*  a, vector<instruction>*  b);
vector<instruction>* isMore(vector<instruction>*  a, vector<instruction>*  b);
vector<instruction>* isLess(vector<instruction>*  a, vector<instruction>*  b);
vector<instruction>* isMoreEqual(vector<instruction>*  a, vector<instruction>*  b);
vector<instruction>* isLessEqual(vector<instruction>*  a, vector<instruction>*  b);
vector<instruction>* loadNumber(long long num);
void checkVar(string* s, bool b, long long a);
vector<instruction>* loadVar(string *s);
vector<instruction>* loadArray(string *s, long long a);
vector<instruction>* loadArray(string *s, string *r);

string toString(vector<instruction>* a);
void error(string s, long long a);

vector<instruction>* addHalt(vector<instruction>*  a);




//uzupełnić