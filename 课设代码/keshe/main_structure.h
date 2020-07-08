#pragma once
#include<iostream>
#include<vector>
#include "string.h"
#include "Symbol_table.h"
#pragma warning(disable:4996)
struct Term
{
	char name[20];
	int type;
	int addr;
	Term* next;
};
Term* Head;  //token表

FILE* infile = fopen("data.txt","r");
stack<string> SEM;
int pass;
string type;
Array_List* AINFLp = ArrL_Head;
Parameter_List* Para_p;
Function_List* FuncL_p = FuncL_Head;
Function_List* FuncL_p2 = FuncL_Head;
Array_List* ArrL_p2 = ArrL_Head;
int  Arr_addr = 0;
int  Func_addr = 0;
Symbol_List* SymL_p;
int off = 0;

double fun_value = 0;
int t_value = 0;
struct QtTerm
{
	string first;
	bool firstac;
	string second;
	bool secondac;
	string third;
	bool thirdac;
	string fourth;
	bool fourthac;
};
vector<QtTerm>Qt;
vector<QtTerm>NewQt;//新四元式向量