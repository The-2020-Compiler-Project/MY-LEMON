#include "string"
#include<fstream>
#include <stdio.h>
#include<vector>
#include <stdlib.h>
#include <string.h>
#include <iomanip>
#include "yufa.h"
#include"youhua.h"
#include"Object_code.h"
#include"main_structure.h"
#include"Symbol_table.h"

int main()
{
	initTerm();
	FuncL_p2 = FuncL_Head;
	ArrL_p2 = ArrL_Head;
	SymL_p = SymL_Head[0];
	Symbol_List* p = SymL_Head[1];
	if (!infile)
	{
		cout << "���ļ�ʧ��" << endl;
		return 0;
	}
	Term* arrow = Head;
	arrow = Start(arrow);
	cout << "�﷨����ͨ��" << endl;
	Print_Table();
	DivideBaseblock();//�Ż�
	active_info();
	objectcode_asm(0, NewQt.size());       //����������
	printtoken();//token���
	NewQtToFile();//�Ż���Ԫʽ������ļ�
	obcode_to_file();//���������ļ�
}