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

using namespace std;

struct QtTerm       //四元式
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

struct Active   //活跃信息
{
    string q;
    bool qac = 1;
};

struct ObjectCode       //目标代码
{
    int id;             //标号
    string operation;   //操作指令
    string dest;        //目标操作数
    string source;      //源操作数
};

vector<QtTerm>NewQt;       //优化后四元式
vector<ObjectCode>ob_code;//目标代码存储区

void objectcode_virtual(int dstart, int dend);
void objectcode_asm(int dstart, int dend);     
void active_info_write(int dstart, int dend);

void active_info()//基本块内填写活跃信息
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

void active_info_write(int dstart, int dend)   //活跃信息的填写
{
    vector<Active>Ac;
    Active act;
    int i, k;
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
        if (Ac.size() == 0)     //初始化
        {
            act.q = NewQt[i].second;
            if (act.q.at(0) == 't')
            {
                act.qac = false;
            }
            else
            {
                act.qac = true;
            }
            Ac.push_back(act);
        }
        for (k = 0; k < Ac.size(); k++)
        {
            if ((NewQt[i].second == " ") || (NewQt[i].second == Ac[k].q))
            {
                flagq2 = false;
                break;
            }
        }

        if (flagq2)
        {
            act.q = NewQt[i].second;
            if (act.q.at(0) == 't')
            {
                act.qac = false;
            }
            else
            {
                act.qac = true;
            }
            Ac.push_back(act);
        }
        for (k = 0; k < Ac.size(); k++)
        {
            if ((NewQt[i].third == " ") || (NewQt[i].third == Ac[k].q))
            {
                flagq3 = false;
                break;
            }
        }
        if (flagq3)
        {
            act.q = NewQt[i].third;
            if (act.q.at(0) == 't')
            {
                act.qac = false;
            }
            else
            {
                act.qac = true;
            }
            Ac.push_back(act);
        }
        for (k = 0; k < Ac.size(); k++)
        {
            if ((NewQt[i].fourth == " ") || (NewQt[i].fourth == Ac[k].q))
            {
                flagq4 = false;
                break;
            }
        }
        if (flagq4)
        {
            act.q = NewQt[i].fourth;
            if (act.q.at(0) == 't')
            {
                act.qac = false;
            }
            else
            {
                act.qac = true;
            }
            Ac.push_back(act);
        }
    }
    int m;
    for (i = dend - 1; i >= dstart; i--)
    {
        for (m = 0; m < Ac.size(); m++)
        {
            if (NewQt[i].fourth == Ac[m].q)
            {
                NewQt[i].fourthac = Ac[m].qac;
                Ac[m].qac = false;
            }
            if ((NewQt[i].second != " ") && (NewQt[i].second == Ac[m].q))
            {
                NewQt[i].secondac = Ac[m].qac;
                Ac[m].qac = true;
            }
            if ((NewQt[i].third != " ") && (NewQt[i].third == Ac[m].q))
            {
                NewQt[i].thirdac = Ac[m].qac;
                Ac[m].qac = true;
            }
        }
    }
    objectcode_virtual(dstart, dend);  //虚拟机的生成
    objectcode_asm(dstart, dend);       //汇编代码生成
 
}

void CODE(string operation, string dest, string source)//送目标代码到存储区
{
    ObjectCode tempOb;
    tempOb.id = ob_code.size();
    tempOb.operation = operation;
    tempOb.dest = dest;
    tempOb.source = source;
    ob_code.push_back(tempOb);
}

void objectcode_virtual(int dstart, int dend)   //生成虚拟机目标代码
{

}

bool isNum(string s)
{
    for (int i = 0; i < s.length(); i++)
        if (s[i] < '0' || s[i]>'9')
            return false;
    return true;
}

void objectcode_asm(int dstart, int dend)
{
    int i;//当前处理的四元式
    int rdl = -1;
    string AX, BX, CX, DX = " ";   //寄存器状态
    map<string, int> offsetTable;
    int offset = 2;
    int DIoffset = 0;
    CODE("DSEG", "SEGMENT", " ");
    CODE("TEMP", "DW", "2 DUP(?)");
    CODE("DATA", "DW", "100 DUP(?)");
    CODE("DSEG", "ENDS", " ");
    CODE("SSEG", "SEGMENT", " ");
    CODE("STACK", "DW", "50 DUP(0)");
    CODE("SSEG", "ENDS", " ");

    for (i = dstart; i < dend; i++)
    {
        if (NewQt[i].first == "hanshu" && NewQt[i].fourth == "main")
        {
            CODE("CSEG", "SEGMENT", "");
            CODE("", "ASSUME", "CS:CSEG,DS:DSEG,SS:SSEG,ES:DSEG");
            CODE("START:", "", "");
            CODE("MOV", "AX,", "DSEG");
            CODE("MOV", "DS,", "AX");
            CODE("MOV", "ES,", "AX");
            CODE("MOV", "AX,", "SSEG");
            CODE("MOV", "SS,", "AX");
            CODE("MOV", "SP,", "SIZE STACK");
            CODE("MOV", "DI,", "OFFSET DATA");
            CODE("MOV", "SI,", "OFFSET TEMP");
        }
        else if (NewQt[i].first != "hanshu" && NewQt[i].fourth == "main")
        {

        }
        else if (NewQt[i].first == "END")
        {

        }
        else if ((NewQt[i].first == "+") && (NewQt[i].third != "_"))        //双目加法
        {
            if (DX == " ")
            {
                CODE("MOV", "DX,", NewQt[i].second);
                CODE("MOV", "BX,", NewQt[i].third);
            }
            else if (DX == NewQt[i].second)
            {
                if (NewQt[i].secondac)
                {
                    CODE("MOV", "[DI+" + to_string(DIoffset + offset) + "],", "DX");
                    DIoffset += offset;
                    offsetTable.insert(pair<string, int>(NewQt[rdl].fourth, DIoffset));
                }
                if (isNum(NewQt[i].third))
                {
                    CODE("MOV", "BX,", NewQt[i].third);
                }
                else
                {
                    int add = offsetTable.find(NewQt[i].third)->second;
                    CODE("MOV", "BX,", "[DI+" + to_string(add) + "]");
                }
            }
            else
            {
                if ((rdl != -1) && (NewQt[rdl].fourthac))
                {
                    CODE("MOV", "[DI+" + to_string(DIoffset + offset) + "],", "DX");
                    DIoffset += offset;
                    offsetTable.insert(pair<string, int>(NewQt[rdl].fourth, DIoffset));
                }
                if (isNum(NewQt[i].second))
                {
                    CODE("MOV", "DX,", NewQt[i].second);
                }
                else
                {
                    int add = offsetTable.find(NewQt[i].second)->second;
                    CODE("MOV", "DX,", "[DI+" + to_string(add) + "]");
                }
                if (isNum(NewQt[i].third))
                {
                    CODE("MOV", "BX,", NewQt[i].third);
                }
                else
                {
                    int add = offsetTable.find(NewQt[i].third)->second;
                    CODE("MOV", "BX,", "[DI+" + to_string(add) + "]");
                }
            }
            CODE("ADD", "DX,", "BX");
            BX = NewQt[i].third;
            DX = NewQt[i].fourth;
            rdl = i;
        }
        else if ((NewQt[i].first == "-") && (NewQt[i].third != "_"))             //双目减法
        {
            if (DX == " ")
            {
                CODE("MOV", "DX,", NewQt[i].second);
                CODE("MOV", "BX,", NewQt[i].third);
            }
            else if (DX == NewQt[i].second)
            {
                if (NewQt[i].secondac)
                {
                    CODE("MOV", "[DI+" + to_string(DIoffset + offset) + "],", "DX");
                    DIoffset += offset;
                    offsetTable.insert(pair<string, int>(NewQt[rdl].fourth, DIoffset));
                }
                if (isNum(NewQt[i].third))
                {
                    CODE("MOV", "BX,", NewQt[i].third);
                }
                else
                {
                    int add = offsetTable.find(NewQt[i].third)->second;
                    CODE("MOV", "BX,", "[DI+" + to_string(add) + "]");
                }
            }
            else
            {
                if ((rdl != -1) && (NewQt[rdl].fourthac))
                {
                    CODE("MOV", "[DI+" + to_string(DIoffset + offset) + "],", "DX");
                    DIoffset += offset;
                    offsetTable.insert(pair<string, int>(NewQt[rdl].fourth, DIoffset));
                }
                if (isNum(NewQt[i].second))
                {
                    CODE("MOV", "DX,", NewQt[i].second);
                }
                else
                {
                    int add = offsetTable.find(NewQt[i].second)->second;
                    CODE("MOV", "DX,", "[DI+" + to_string(add) + "]");
                }
                if (isNum(NewQt[i].third))
                {
                    CODE("MOV", "BX,", NewQt[i].third);
                }
                else
                {
                    int add = offsetTable.find(NewQt[i].third)->second;
                    CODE("MOV", "BX,", "[DI+" + to_string(add) + "]");
                }
            }
            CODE("SUB", "DX,", "BX");
            BX = NewQt[i].third;
            DX = NewQt[i].fourth;
            rdl = i;
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

int main()
{
    active_info();
    obcode_to_file();
}
