#pragma once
#include "string"
#include <fstream>
#include<vector>
using namespace std;
int rangenum = 0;

struct Symbol_List //���ű�
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

struct Array_List//�����
{
	int low;
	int up;
	string  tp;
	int len;
	Array_List* next;
};
Array_List*ArrL_Head;
Array_List* ArrL_ptr;

struct Parameter_List  //�βα�
{
	char name[20];
	string type;
	char cat[3];
	int addr;
	Parameter_List* next;
};
Parameter_List* ParaL_Head;
Parameter_List* ParaL_ptr;

struct Function_List //������
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
	SymL_ptr = SymL_Head[0]; //��ӡ���ű�
	file << "ȫ�ַ��ű�" << endl;
	file << " " << "name" << " "  << "type" << " " << "cat" << endl;
	while (SymL_ptr->next != NULL)
	{
		SymL_ptr = SymL_ptr->next;
		file << " " << SymL_ptr->name << " " << SymL_ptr->type << " " << SymL_ptr->cat << endl;
	}
	file << endl;
	int i = 1;
	while (SymL_Head[i] != NULL && SymL_Head[i]->next != NULL) {
		SymL_ptr = SymL_Head[i];
		file << "���к������ű�" << i << endl; //��ӡÿ�������ķ��ű�
		file << " " << "name" << " " << "type" << " " << "cat" << " " << "addr" << endl;
		while (SymL_ptr->next != NULL)
		{
			SymL_ptr = SymL_ptr->next;
			file << " " << SymL_ptr->name << " " << SymL_ptr->type << " " << SymL_ptr->cat << endl;
		}
		file << endl;
		FuncL_ptr = FuncL_Head;
		i++;
	}
	ArrL_ptr = ArrL_Head;   //��ӡ�����
	file << "�����" << endl;
	file << " " << "length" << " " << "type" << " " << "low" << " " << "up" << endl;
	while (ArrL_ptr->next != NULL)
	{
		ArrL_ptr = ArrL_ptr->next;
		file << " " << ArrL_ptr->len << " " << ArrL_ptr->tp << " " << ArrL_ptr->low << " " << ArrL_ptr->up << endl;
	}
	file << endl;
	file << "������" << endl;  //��ӡ������
	file << " " << "name" << " " << "level" << " " << "off" << " " << "fn" << endl;
	while (FuncL_ptr->next != NULL)
	{
		FuncL_ptr = FuncL_ptr->next;
		file << " " << FuncL_ptr->name << " " << FuncL_ptr->level << " " << FuncL_ptr->off << " " << FuncL_ptr->fn << endl;
	}
	file << endl;
	file.close();
}

