#pragma once
#include "string"
#include <fstream>
#include<vector>
#include<stack>
using namespace std;
int rangenum = 0;

struct Symbol_List //符号表
{
	char name[20];
	string type;
	char cat[3];
	int addr;
	char active;
	Symbol_List* next;
};
Symbol_List* SymL_Head[30];
Symbol_List* SymL_ptr;

struct Array_List//数组表
{
	int low;
	int up;
	string  tp;
	int len;
	Array_List* next;
};
Array_List*ArrL_Head;
Array_List* ArrL_ptr;

struct Parameter_List  //形参表
{
	char name[20];
	string type;
	char cat[3];
	int addr;
	Parameter_List* next;
};
Parameter_List* ParaL_Head;
Parameter_List* ParaL_ptr;

struct Function_List //函数表
{
	string name;
	int level;
	int off;
	int fn;
	Parameter_List* param;
	int entry;
	Function_List* next;
};
Function_List* FuncL_Head;
Function_List* FuncL_ptr;

void Print_Table()
{
	ofstream file("symbol.txt");
	SymL_ptr = SymL_Head[0]; //打印符号表
	file << "全局符号表" << endl;
	file << '\t' << "name" << '\t' << "type" << '\t' << "cat" << endl;
	while (SymL_ptr->next != NULL)
	{
		SymL_ptr = SymL_ptr->next;
		file << '\t' << SymL_ptr->name << '\t' << SymL_ptr->type << '\t' << SymL_ptr->cat << endl;
	}
	file << endl;
	int i = 1;
	while (SymL_Head[i] != NULL && SymL_Head[i]->next != NULL) {
		SymL_ptr = SymL_Head[i];
		file << "现行函数符号表" << i << endl; //打印每个函数的符号表
		file << '\t' << "name" << '\t' << "type" << '\t' << "cat" << '\t' << "addr" << endl;
		while (SymL_ptr->next != NULL)
		{
			SymL_ptr = SymL_ptr->next;
			file << '\t' << SymL_ptr->name << '\t' << SymL_ptr->type << '\t' << SymL_ptr->cat << endl;
		}
		file << endl;
		FuncL_ptr = FuncL_Head;
		i++;
	}
	ArrL_ptr = ArrL_Head;   //打印数组表
	file << "数组表" << endl;
	file << '\t' << "length" << '\t' << "type" << '\t' << "low" << '\t' << "up" << endl;
	while (ArrL_ptr->next != NULL)
	{
		ArrL_ptr = ArrL_ptr->next;
		file << '\t' << ArrL_ptr->len << '\t' << ArrL_ptr->tp << '\t' << ArrL_ptr->low << '\t' << ArrL_ptr->up << endl;
	}
	file << endl;
	file << "函数表" << endl;  //打印函数表
	file << '\t' << "name" << '\t' << "level" << '\t' << "off" << '\t' << "fn" << endl;
	while (FuncL_ptr->next != NULL)
	{
		FuncL_ptr = FuncL_ptr->next;
		file << '\t' << FuncL_ptr->name << '\t' << FuncL_ptr->level << '\t' << FuncL_ptr->off << '\t' << FuncL_ptr->fn << endl;
	}
	file << endl;
	file.close();
}

