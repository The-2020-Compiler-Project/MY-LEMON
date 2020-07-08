#pragma once
#include<string>
#include<sstream>
#include<vector>
#include"main_structure.h"
#include"yufa.h"
#include"youhua.h"
using namespace std;


void SEND(string first, string second, string third, string fourth)
{
	QtTerm qt;
	qt.first = first;
	qt.second = second;
	qt.third = third;
	qt.fourth = fourth;
    Quaternary.push_back(qt);
}

void IF()
{
	string SEM_top;
	SEM_top = SEM.top();
	SEM.pop();
	SEND("if", SEM_top, "_", "_");
}
void ELSE()
{
	SEND("el", "_", "_", "_");
}
void IFEND()
{
	SEND("ie", "_", "_", "_");
}
void WHILE()
{
	SEND("wh", "_", "_", "_");
}
void DO()
{
	string SEM_top;
	SEM_top = SEM.top();
	SEM.pop();
	SEND("do", SEM_top, "_", "_");
}
void WHILEEND()
{
	SEND("we", "_", "_", "_");
}
void GEQ(string oper)//˫Ŀ��������嶯��
{
	stringstream ss;
	string t_num, t;
	ss << t_value;
	ss >> t_num;//��ʱ�������
	t = "-t" + t_num;//��ʱ����
	string SEM_top, SEM_top1;
	SEM_top = SEM.top();
	SEM.pop();
	SEM_top1 = SEM.top();
	SEM.pop();
	SEM.push(t);
	SEND(oper, SEM_top1, SEM_top, t);
	t_value++;
}
void GEQSingle(string oper)//��Ŀ�����
{
	stringstream ss;
	string t_num, t;
	ss << t_value;
	ss >> t_num;
	t = "-t" + t_num;
	string SEM_top;
	SEM_top = SEM.top();
	SEM.pop();
	SEM.push(t);
	SEND(oper, SEM_top, "_", t);
	t_value++;
}
void ASSI(string s)//��ֵ���嶯��
{
	string SEM_top;
	SEM_top = SEM.top();
	SEM.pop();
	SEND("=", SEM_top, "_", s);
}
