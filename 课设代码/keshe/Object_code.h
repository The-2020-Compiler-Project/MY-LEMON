#pragma once
#include <iostream>
#include <fstream>
#include <stdio.h>
#include<vector>
#include <stdlib.h>
#include <stack>
#include<string>
#include<sstream>
#include <string.h>
#include<map>
#include"main_structure.h"

using namespace std;

struct ObjectCode       //������
{
	int id;//���
	string operation;//����ָ��
	string dest;//Ŀ�������
	string source;//Դ������
};

struct Active       //������Ծ��Ϣ
{
	string qt_name;
	bool qt_ac = 1;
};


vector<ObjectCode>ob_code;//Ŀ�����洢��
map<string, int> offsetTable;//����ڴ��ַ
map<string, int> FUNCoffsetTable;//��������ڴ��ַ
stack<string> ifelseSTK;    //ifelse���ջ
stack<string> whileSTK;     //while���ջ
stack<string> xingcanSTK;   //�β�ջ�����������β�
stack<string> funcSTK;      //������ջ����ǵ�ǰ����ĺ���


//��������
int reOpNum = 0;
int notNum = 0;
int ifNum = 0;
int whileNum = 0;

void CODE(string operation, string dest, string source) //��Ŀ����뵽������
{
	ObjectCode tempOb;
	tempOb.id = ob_code.size();
	tempOb.operation = operation;
	tempOb.dest = dest;
	tempOb.source = source;
	ob_code.push_back(tempOb);
}

bool isNum(string s)        //�ж��Ƿ������֣������ж��Ƿ�Ϊ����
{
	for (int i = 0; i < s.length(); i++)
		if (s[i] < '0' || s[i]>'9')
			return false;
	return true;
}

void active_info_write(int dstart, int dend)    //��Ծ��Ϣ����д
{
	vector<Active>Ac;
	Active act;
	int i, k, m;
	bool flagq2;
	bool flagq3;
	bool flagq4;
	for (i = dstart; i < dend; i++)
	{
		flagq2 = true;
		flagq3 = true;
		flagq4 = true;
		if ((NewQt[i].first == "el") || (NewQt[i].first == "ie") || (NewQt[i].first == "wh") || (NewQt[i].first == "we"))
		{
			continue;
		}
		if (Ac.size() == 0)//�����ǰ����Ϊ�գ��������һ������
		{
			act.qt_name = NewQt[i].second;
			if (act.qt_name.at(0) == 't')
			{
				act.qt_ac = false;  // ��ʱ������ʼʱ�Ļ�Ծ��ϢΪfalse
			}
			else
			{
				act.qt_ac = true;
			}
			Ac.push_back(act);
		}
		for (k = 0; k < Ac.size(); k++)
		{
			if ((NewQt[i].second == " ") || (NewQt[i].second == Ac[k].qt_name))
			{
				flagq2 = false;
				break;
			}
		}
		if (flagq2)
		{
			act.qt_name = NewQt[i].second;
			if (act.qt_name.at(0) == 't')
			{
				act.qt_ac = false;
			}
			else
			{
				act.qt_ac = true;
			}
			Ac.push_back(act);
		}
		for (k = 0; k < Ac.size(); k++)
		{
			if ((NewQt[i].third == " ") || (NewQt[i].third == Ac[k].qt_name))
			{
				flagq3 = false;
				break;
			}
		}
		if (flagq3)
		{
			act.qt_name = NewQt[i].third;
			if (act.qt_name.at(0) == 't')
			{
				act.qt_ac = false;
			}
			else
			{
				act.qt_ac = true;
			}
			Ac.push_back(act);
		}
		for (k = 0; k < Ac.size(); k++)
		{
			if ((NewQt[i].fourth == " ") || (NewQt[i].fourth == Ac[k].qt_name))
			{
				flagq4 = false;
				break;
			}
		}
		if (flagq4)
		{
			act.qt_name = NewQt[i].fourth;
			if (act.qt_name.at(0) == 't')
			{
				act.qt_ac = false;
			}
			else
			{
				act.qt_ac = true;
			}
			Ac.push_back(act);
		}
	}  //��ʼ������
	for (i = dend - 1; i >= dstart; i--)        //����ɨ����»�Ծ��Ϣ
	{
		for (m = 0; m < Ac.size(); m++)
		{
			if (NewQt[i].fourth == Ac[m].qt_name)
			{
				NewQt[i].fourthac = Ac[m].qt_ac;
				Ac[m].qt_ac = false;
			}
			if ((NewQt[i].second != " ") && (NewQt[i].second == Ac[m].qt_name))
			{
				NewQt[i].secondac = Ac[m].qt_ac;
				Ac[m].qt_ac = true;
			}
			if ((NewQt[i].third != " ") && (NewQt[i].third == Ac[m].qt_name))
			{
				NewQt[i].thirdac = Ac[m].qt_ac;
				Ac[m].qt_ac = true;
			}
		}
	}
}

void active_info()//����������д��Ծ��Ϣ
{
	int m, n;
	for (m = 0, n = 0; m < NewQt.size(); m++)
	{
		if ((NewQt[m].first == "if") || (NewQt[m].first == "el") || (NewQt[m].first == "ie") || (NewQt[m].first == "do") || (NewQt[m].first == "we") || (NewQt[m].first == "we") || (NewQt[m].first == "wh"))
		{
			active_info_write(n, m + 1);
			n = m + 1;
		}
	}
	if (n < m) {
		active_info_write(n, m);
	}
}

void load_DX(int rdl, int& DIoffset, int offset)        //����DX���ݵ��ڴ�
{
	if (funcSTK.top() == "main")
	{

		if (offsetTable.count(NewQt[rdl].fourth) == 0)
		{
			CODE("MOV", "[DI+" + to_string(DIoffset + offset) + "],", "DX");
			DIoffset += offset;
			offsetTable.insert(pair<string, int>(NewQt[rdl].fourth, DIoffset));
		}
		else
		{
			int add = offsetTable.find(NewQt[rdl].fourth)->second;
			CODE("MOV", "[DI+" + to_string(add) + "],", "DX");
		}
	}
	else
	{
		if (FUNCoffsetTable.count(NewQt[rdl].fourth) == 0)
		{
			CODE("MOV", "[SI+" + to_string(DIoffset + offset) + "],", "DX");
			DIoffset += offset;
			FUNCoffsetTable.insert(pair<string, int>(NewQt[rdl].fourth, DIoffset));
		}
		else
		{
			int add = FUNCoffsetTable.find(NewQt[rdl].fourth)->second;
			CODE("MOV", "[SI+" + to_string(add) + "],", "DX");
		}
	}
}

void obcode_DX(string DX, int i, int& DIoffset, int rdl)    //��second��DX
{
	int offset = 2;
	if (DX == " ")
	{
		if (isNum(NewQt[i].second))
		{
			CODE("MOV", "DX,", NewQt[i].second);
		}
		else
		{
			if (funcSTK.top() == "main")
			{
				int add = offsetTable.find(NewQt[i].second)->second;
				CODE("MOV", "DX", "[DI+" + to_string(add) + "],");
			}
			else
			{
				int add = FUNCoffsetTable.find(NewQt[i].second)->second;
				CODE("MOV", "DX", "[SI+" + to_string(add) + "],");
			}
		}
	}
	else if (DX == NewQt[i].second)
	{
		if (NewQt[i].secondac)
		{
			load_DX(rdl, DIoffset, offset);
			/*CODE("MOV", "[DI+" + to_string(DIoffset + offset) + "],", "DX");
			DIoffset += offset;
			offsetTable.insert(pair<string, int>(NewQt[rdl].fourth, DIoffset));*/
		}
	}
	else
	{
		if ((rdl != -1) && (NewQt[rdl].fourthac))
		{
			load_DX(rdl, DIoffset, offset);
			/*CODE("MOV", "[DI+" + to_string(DIoffset + offset) + "],", "DX");
			DIoffset += offset;
			offsetTable.insert(pair<string, int>(NewQt[rdl].fourth, DIoffset));*/
		}
		if (isNum(NewQt[i].second))
		{
			CODE("MOV", "DX,", NewQt[i].second);
		}
		else
		{
			if (funcSTK.top() == "main")
			{
				int add = offsetTable.find(NewQt[i].second)->second;
				CODE("MOV", "DX,", "[DI+" + to_string(add) + "]");
			}
			else
			{
				int add = FUNCoffsetTable.find(NewQt[i].second)->second;
				CODE("MOV", "DX,", "[SI+" + to_string(add) + "]");
			}
		}
	}
}

void obcode_DXBX(string DX, int i, int& DIoffset, int rdl) //��second��DX����third��BX
{
	int offset = 2;
	if (DX == " ")
	{
		if (isNum(NewQt[i].second))
		{
			CODE("MOV", "DX,", NewQt[i].second);
		}
		else
		{
			if (funcSTK.top() == "main")
			{
				int add = offsetTable.find(NewQt[i].second)->second;
				CODE("MOV", "DX,", "[DI+" + to_string(add) + "]");
			}
			else
			{
				int add = FUNCoffsetTable.find(NewQt[i].second)->second;
				CODE("MOV", "DX,", "[SI+" + to_string(add) + "]");
			}
		}
		if (isNum(NewQt[i].third))
		{
			CODE("MOV", "BX,", NewQt[i].third);
		}
		else
		{
			if (funcSTK.top() == "main")
			{
				int add = offsetTable.find(NewQt[i].third)->second;
				CODE("MOV", "BX,", "[DI+" + to_string(add) + "]");
			}
			else
			{
				int add = FUNCoffsetTable.find(NewQt[i].third)->second;
				CODE("MOV", "BX,", "[SI+" + to_string(add) + "]");
			}
		}
	}
	else if (DX == NewQt[i].second)
	{
		if (NewQt[i].secondac)
		{
			load_DX(rdl, DIoffset, offset);
		}
		if (isNum(NewQt[i].third))
		{
			CODE("MOV", "BX,", NewQt[i].third);
		}
		else
		{
			if (funcSTK.top() == "main")
			{
				int add = offsetTable.find(NewQt[i].third)->second;
				CODE("MOV", "BX,", "[DI+" + to_string(add) + "]");
			}
			else
			{
				int add = FUNCoffsetTable.find(NewQt[i].third)->second;
				CODE("MOV", "BX,", "[SI+" + to_string(add) + "]");
			}
		}
	}
	else
	{
		if ((rdl != -1) && (NewQt[rdl].fourthac))
		{
			load_DX(rdl, DIoffset, offset);
		}
		if (isNum(NewQt[i].second))
		{
			CODE("MOV", "DX,", NewQt[i].second);
		}
		else
		{
			if (funcSTK.top() == "main")
			{
				int add = offsetTable.find(NewQt[i].second)->second;
				CODE("MOV", "DX,", "[DI+" + to_string(add) + "]");
			}
			else
			{
				int add = FUNCoffsetTable.find(NewQt[i].second)->second;
				CODE("MOV", "DX,", "[SI+" + to_string(add) + "]");
			}
		}
		if (isNum(NewQt[i].third))
		{
			CODE("MOV", "BX,", NewQt[i].third);
		}
		else
		{
			if (funcSTK.top() == "main")
			{
				int add = offsetTable.find(NewQt[i].third)->second;
				CODE("MOV", "BX,", "[DI+" + to_string(add) + "]");
			}
			else
			{
				int add = FUNCoffsetTable.find(NewQt[i].third)->second;
				CODE("MOV", "BX,", "[SI+" + to_string(add) + "]");
			}
		}
	}

}

void Arithmetic_operation(int& i, string& DX, string& BX, int& DIoffset, int& rdl)     //��������
{
	if ((NewQt[i].first == "+") && (NewQt[i].third != "_") && (NewQt[i].third != " "))        //˫Ŀ�ӷ�
	{
		obcode_DXBX(DX, i, DIoffset, rdl);
		CODE("ADD", "DX,", "BX");
		BX = NewQt[i].third;
		DX = NewQt[i].fourth;
		rdl = i;
	}
	else if ((NewQt[i].first == "-") && (NewQt[i].third != "_") && (NewQt[i].third != " "))             //˫Ŀ����
	{
		obcode_DXBX(DX, i, DIoffset, rdl);
		CODE("SUB", "DX,", "BX");
		BX = NewQt[i].third;
		DX = NewQt[i].fourth;
		rdl = i;
	}
	else if ((NewQt[i].first == "*") && (NewQt[i].third != "_"))             //˫Ŀ�˷�
	{
		obcode_DXBX(DX, i, DIoffset, rdl);
		CODE("MOV", "AX,", "DX");
		CODE("MUL", "BX", "");
		CODE("MOV", "DX,", "AX");
		BX = NewQt[i].third;
		DX = NewQt[i].fourth;
		rdl = i;
	}
	else if ((NewQt[i].first == "/") && (NewQt[i].third != "_"))             //˫Ŀ����
	{
		obcode_DXBX(DX, i, DIoffset, rdl);
		CODE("MOV", "AX,", "DX");
		CODE("XOR", "DX,", "DX");
		CODE("DIV", "BX", "");
		CODE("MOV", "DX,", "AX");
		BX = NewQt[i].third;
		DX = NewQt[i].fourth;
		rdl = i;
	}
	else if ((NewQt[i].first == "%") && (NewQt[i].third != "_"))             //˫Ŀȡ��
	{
		obcode_DXBX(DX, i, DIoffset, rdl);
		CODE("MOV", "AX,", "DX");
		CODE("XOR", "DX,", "DX");
		CODE("DIV", "BX", "");
		BX = NewQt[i].third;
		DX = NewQt[i].fourth;
		rdl = i;
	}
}

void Logical_operation(int& i, string& DX, string& BX, int& DIoffset, int& rdl)  //�߼�����
{
	if ((NewQt[i].first == "&&") && (NewQt[i].third != "_"))             //˫Ŀ ��
	{
		obcode_DXBX(DX, i, DIoffset, rdl);
		CODE("AND", "DX,", "BX");
		BX = NewQt[i].third;
		DX = NewQt[i].fourth;
		rdl = i;
	}
	else if ((NewQt[i].first == "||") && (NewQt[i].third != "_"))             //˫Ŀ ��
	{
		obcode_DXBX(DX, i, DIoffset, rdl);
		CODE("OR", "DX,", "BX");
		BX = NewQt[i].third;
		DX = NewQt[i].fourth;
		rdl = i;
	}
	else if ((NewQt[i].first == "!"))             //��Ŀ ��
	{
		obcode_DX(DX, i, DIoffset, rdl);
		CODE("TEST", "DX,", "DX");
		CODE("JZ", "NOT" + to_string(notNum), "");
		CODE("XOR", "DX,", "DX");
		CODE("JMP", "NOTCONTI" + to_string(notNum), "");
		CODE("NOT" + to_string(notNum) + ":", "", "");
		CODE("INC", "DX,", "");
		CODE("NOTCONTI" + to_string(notNum) + ":", "", "");
		DX = NewQt[i].fourth;
		rdl = i;
		notNum++;
	}
}

void Relational_operation(int& i, string& DX, string& BX, int& DIoffset, int& rdl)  //��ϵ�����
{
	string jmp_code = " ";
	if (NewQt[i].first == "<") { jmp_code = "JB"; }
	else if (NewQt[i].first == "<=") { jmp_code = "JBE"; }
	else if (NewQt[i].first == ">") { jmp_code = "JA"; }
	else if (NewQt[i].first == ">=") { jmp_code = "JAE"; }
	else if (NewQt[i].first == "==") { jmp_code = "JE"; }
	else if (NewQt[i].first == "!=") { jmp_code = "JNE"; }
	obcode_DXBX(DX, i, DIoffset, rdl);
	CODE("CMP", "DX,", "BX");
	CODE(jmp_code, "ROP" + to_string(reOpNum), "");
	CODE("MOV", "DX,", "0");
	CODE("JMP", "ROPCONTI" + to_string(reOpNum), "");
	CODE("ROP" + to_string(reOpNum) + ":", "", "");
	CODE("MOV", "DX,", "1");
	CODE("ROPCONTI" + to_string(reOpNum) + ":", "", "");
	BX = NewQt[i].third;
	DX = NewQt[i].fourth;
	rdl = i;
	reOpNum++;
}

void objectcode_asm(int dstart, int dend)
{
	int i;//��ǰ�������Ԫʽ
	int rdl = -1;
	string AX, BX, CX, DX = " ";   //�Ĵ���״̬

	int offset = 2;
	int DIoffset = 0;
	int SIoffset = 0;
	//funcSTK.push("main");
	CODE("DSEG", "SEGMENT", " ");
	CODE("DATA", "DW", "100 DUP(?)");
	CODE("TEMP", "DW", "20 DUP(?)");
	CODE("SPSP", "DW", "?");
	CODE("RESULT", "DW", "?");
	CODE("DSEG", "ENDS", " ");
	CODE("SSEG", "SEGMENT", "STACK");
	CODE("STK", "DW", "50 DUP(0)");
	CODE("SSEG", "ENDS", " ");
	CODE("CSEG", "SEGMENT", "");
	CODE("", "ASSUME", "CS:CSEG,DS:DSEG,SS:SSEG,ES:DSEG");

	for (i = dstart; i < dend; i++)
	{
		if (NewQt[i].first == "hanshu" && NewQt[i].fourth == "main")
		{
			CODE("main", "PROC", "");
			CODE("START:", "", "");
			CODE("MOV", "AX,", "DSEG");
			CODE("MOV", "DS,", "AX");
			CODE("MOV", "ES,", "AX");
			CODE("MOV", "AX,", "SSEG");
			CODE("MOV", "SS,", "AX");
			CODE("MOV", "SP,", "SIZE STK");
			CODE("MOV", "DI,", "OFFSET DATA");
			CODE("MOV", "SI,", "OFFSET TEMP");
			funcSTK.push(NewQt[i].fourth);
		}
		else if (NewQt[i].first == "hanshu" && NewQt[i].fourth != "main")
		{
			CODE(NewQt[i].fourth, "PROC", "");
			CODE("POP", "AX", "");
			CODE("MOV", "SPSP,", "AX");
			funcSTK.push(NewQt[i].fourth);
		}
		else if (NewQt[i].first == "canshu")
		{
			CODE("POP", "DX", "");
			if (FUNCoffsetTable.count(NewQt[i].fourth) == 0)
			{
				CODE("MOV", "[SI+" + to_string(SIoffset + offset) + "],", "DX");
				SIoffset += offset;
				FUNCoffsetTable.insert(pair<string, int>(NewQt[i].fourth, SIoffset));
			}
			else
			{
				int add = FUNCoffsetTable.find(NewQt[rdl].fourth)->second;
				CODE("MOV", "[SI+" + to_string(add) + "],", "DX");
			}
			DX = NewQt[i].fourth;
			rdl = i;
		}
		else if (NewQt[i].first == "re")
		{
			if ((DX != " ") && (rdl != -1) && (NewQt[rdl].fourthac))
			{
				//load_DX(rdl, DIoffset, offset);
			}   //����bug
			DX = " ";
			if (isNum(NewQt[i].third))
			{
				CODE("MOV", "DX,", NewQt[i].third);
			}
			else
			{
				int add = FUNCoffsetTable.find(NewQt[i].third)->second;
				CODE("MOV", "DX,", "[SI+" + to_string(add) + "]");
			}
			CODE("MOV", "RESULT,", "DX");
			CODE("MOV", "AX,", "SPSP");
			CODE("PUSH", "AX", "");
			CODE("RET", "", "");
		}
		else if (NewQt[i].first == "END")
		{
			CODE(funcSTK.top(), "ENDP", "");
			funcSTK.pop();
		}
		else if (NewQt[i].first == "xingcan")
		{
			/*if (isNum(NewQt[i].fourth))
			{
			}
			else
			{
				int add = offsetTable.find(NewQt[i].fourth)->second;
				CODE("MOV", "DX,", "[DI+" + to_string(add) + "]");
			}*/

			xingcanSTK.push(NewQt[i].fourth);
			DX = NewQt[i].fourth;
			rdl = i;
		}
		else if (NewQt[i].first == "diaoyong")
		{
			for (int k = 0; k < xingcanSTK.size() + 2; k++)
			{
				string xingcan = xingcanSTK.top();
				xingcanSTK.pop();
				if (isNum(xingcan))
				{
					CODE("MOV", "DX,", xingcan);
				}
				else
				{
					if (funcSTK.top() == "main")
					{
						int add = offsetTable.find(xingcan)->second;
						CODE("MOV", "DX,", "[DI+" + to_string(add) + "]");
					}
					else
					{
						int add = FUNCoffsetTable.find(xingcan)->second;
						CODE("MOV", "DX,", "[SI+" + to_string(add) + "]");
					}
				}
				CODE("PUSH", "DX", "");
			}
			CODE("CALL", NewQt[i].fourth, "");
			CODE("MOV", "DX,", "RESULT");
			load_DX(i, DIoffset, offset);
			DX = NewQt[i].fourth;
			rdl = i;
		}
		else if (NewQt[i].first == "temp")
		{
			DX = NewQt[i].fourth;
			rdl = i;
		}
		else if (((NewQt[i].first == "+") || (NewQt[i].first == "-") || (NewQt[i].first == "*") || (NewQt[i].first == "/") || (NewQt[i].first == "%")) \
			&& (NewQt[i].third != "_") && (NewQt[i].third != " "))        //���������
		{
			Arithmetic_operation(i, DX, BX, DIoffset, rdl);
		}
		else if (((NewQt[i].first == "&&") || (NewQt[i].first == "||") || (NewQt[i].first == "!")) && (NewQt[i].third != "_"))     //�߼������
		{
			Logical_operation(i, DX, BX, DIoffset, rdl);
		}
		else if ((NewQt[i].first == "<") || (NewQt[i].first == "<=")    \
			|| (NewQt[i].first == ">") || (NewQt[i].first == ">=")  \
			|| (NewQt[i].first == "==") || (NewQt[i].first == "!=") && (NewQt[i].third != "_"))       //��ϵ�����
		{
			Relational_operation(i, DX, BX, DIoffset, rdl);
		}
		else if (NewQt[i].first == "=")                //��Ŀ ��ֵ
		{
			//obcode_DX(DX, i, DIoffset, rdl);
			if (isNum(NewQt[i].second))
			{
				CODE("MOV", "DX,", NewQt[i].second);
			}
			else
			{
				if (funcSTK.top() == "main")
				{
					int add = offsetTable.find(NewQt[i].second)->second;
					CODE("MOV", "DX,", "[DI+" + to_string(add) + "]");
				}
				else
				{
					int add = FUNCoffsetTable.find(NewQt[i].second)->second;
					CODE("MOV", "DX,", "[SI+" + to_string(add) + "]");
				}
			}
			load_DX(i, DIoffset, offset);
			//CODE("MOV", "[DI+" + to_string(DIoffset + offset) + "],", "DX");
			//DIoffset += offset;
			//offsetTable.insert(pair<string, int>(NewQt[i].fourth, DIoffset));

			DX = NewQt[i].fourth;
			rdl = i;
		}
		else if (NewQt[i].first == "+")                //��Ŀ ��
		{
			obcode_DX(DX, i, DIoffset, rdl);
			DX = NewQt[i].fourth;
			rdl = i;
		}
		else if (NewQt[i].first == "-")                //��Ŀ ��
		{
			obcode_DX(DX, i, DIoffset, rdl);
			CODE("NEG", "DX", "");
			DX = NewQt[i].fourth;
			rdl = i;
		}
		else if (NewQt[i].first == "if")             //if ���
		{
			ifelseSTK.push(to_string(ifNum));
			ifNum++;
			obcode_DX(DX, i, DIoffset, rdl);
			CODE("CMP", "DX,", "0");
			CODE("JNE", "PD1" + ifelseSTK.top(), "");
			CODE("JE", "PD0" + ifelseSTK.top(), "");
			CODE("PD1" + ifelseSTK.top() + ":", "", "");
			DX = NewQt[i].fourth;
			DX = " ";
		}
		else if (NewQt[i].first == "el")            //else���
		{
			if ((DX != " ") && (rdl != -1) && (NewQt[rdl].fourthac))
			{
				load_DX(rdl, DIoffset, offset);
			}
			CODE("JMP", "CONTI" + ifelseSTK.top(), " ");
			CODE("PD0" + ifelseSTK.top() + ":", "", "");
			DX = " ";
		}
		else if (NewQt[i].first == "ie")        //ifend���
		{
			if ((DX != " ") && (rdl != -1) && (NewQt[rdl].fourthac))
			{
				load_DX(rdl, DIoffset, offset);
			}
			CODE("CONTI" + ifelseSTK.top() + ":", "", "");
			ifelseSTK.pop();
			DX = " ";
		}
		else if (NewQt[i].first == "wh")             //while ���
		{
			whileSTK.push(to_string(whileNum));
			whileNum++;
			CODE("LOOP" + whileSTK.top() + ":", "", "");
			DX = " ";
		}
		else if (NewQt[i].first == "do")            //do���
		{
			obcode_DX(DX, i, DIoffset, rdl);
			CODE("CMP", "DX,", "0");
			CODE("JE", "LPCONTI" + whileSTK.top(), "");
			DX = " ";
		}
		else if (NewQt[i].first == "we")        //whileend���
		{
			if ((DX != " ") && (rdl != -1) && (NewQt[rdl].fourthac))
			{
				load_DX(rdl, DIoffset, offset);
			}
			CODE("JMP", "LOOP" + whileSTK.top(), "");
			CODE("LPCONTI" + whileSTK.top() + ":", "", "");
			whileSTK.pop();
			DX = " ";
		}
	}
	CODE("MOV", "AH,", "4CH");
	CODE("INT", "21H", "");
	CODE("CSEG", "ENDS", " ");
	CODE("", "END", "START");
}


void obcode_to_file() {
	ofstream file("mubiao.txt");
	for (int p = 0; p < ob_code.size(); p++) {
		//file << _code[p].id << '\t' << _code[p].operation << '\t' << _code[p].dest << '\t' << _code[p].source << '\t' << endl;
		file << ob_code[p].operation << '\t' << ob_code[p].dest << '\t' << ob_code[p].source << '\t' << endl;
	}
	file.close();
}

