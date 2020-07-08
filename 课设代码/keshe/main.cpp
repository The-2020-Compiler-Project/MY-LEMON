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
		cout << "打开文件失败" << endl;
		return 0;
	}
	Term* arrow = Head;
	arrow = Start(arrow);
	cout << "语法分析通过" << endl;
	Print_Table();
	DivideBaseblock();//优化
	active_info();
	objectcode_asm(0, NewQt.size());       //汇编代码生成
	printtoken();//token输出
	NewQtToFile();//优化四元式输出到文件
	obcode_to_file();//汇编输出到文件
}