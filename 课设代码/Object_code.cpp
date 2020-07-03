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

struct Active       //变量活跃信息
{
    string qt_name;
    bool qt_ac = 1;
};
string RDL;

struct ObjectCode       //汇编语句
{
    int id;//标号
    string operation;//操作指令
    string dest;//目标操作数
    string source;//源操作数
};

vector<QtTerm>NewQt;
vector<ObjectCode>ob_code;//目标代码存储区
stack<int>Sem;//语义栈
map<string, int> offsetTable;//汇编内存地址
stack<string> ifelseNum;
int reOpNum = 0;
int ifNum = 0;

void CODE(string operation, string dest, string source) //送目标代码到缓存区
{
    ObjectCode tempOb;
    tempOb.id = ob_code.size();
    tempOb.operation = operation;
    tempOb.dest = dest;
    tempOb.source = source;
    ob_code.push_back(tempOb);
}

bool isNum(string s)
{
    for (int i = 0; i < s.length(); i++)
        if (s[i] < '0' || s[i]>'9')
            return false;
    return true;
}

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


void active_info_write(int dstart, int dend)    //活跃信息的填写
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
        if (Ac.size() == 0)//如果当前数组为空，则先添加一个数据
        {
            act.qt_name = NewQt[i].second;
            if (act.qt_name.at(0) == 't')
            {
                act.qt_ac = false;  // 临时变量初始时的活跃信息为false
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
    }  //初始化结束
    for (i = dend - 1; i >= dstart; i--)        //逆序扫描更新活跃信息
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
    //objectcode_virtual(dstart, dend);  //虚拟机的生成
    //objectcode_asm(dstart, dend);       //汇编代码生成
    for (int i = 0; i < NewQt.size(); i++)
    {
        //cout << NewQt[i].first << " " << NewQt[i].firstac << endl;
        cout << NewQt[i].second << " " << NewQt[i].secondac << endl;
        cout << NewQt[i].third << " " << NewQt[i].thirdac << endl;
        cout << NewQt[i].fourth << " " << NewQt[i].fourthac << endl;
    }
}

void obcode_DX(string DX, int i, int& DIoffset, int rdl)
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
            int add = offsetTable.find(NewQt[i].second)->second;
            CODE("MOV", "DX,", "[DI+" + to_string(add) + "]");
        }
    }
    else if (DX == NewQt[i].second)
    {
        if (NewQt[i].secondac)
        {
            CODE("MOV", "[DI+" + to_string(DIoffset + offset) + "],", "DX");
            DIoffset += offset;
            offsetTable.insert(pair<string, int>(NewQt[rdl].fourth, DIoffset));
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
    }
}

void obcode_DXBX(string DX, int i, int& DIoffset, int rdl)
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

}

void objectcode_asm(int dstart, int dend)
{
    int i;//当前处理的四元式
    int rdl = -1;
    string AX, BX, CX, DX = " ";   //寄存器状态

    int offset = 2;
    int DIoffset = 0;
    CODE("DSEG", "SEGMENT", " ");
    CODE("TEMP", "DW", "2 DUP(?)");
    CODE("DATA", "DW", "100 DUP(?)");
    CODE("DSEG", "ENDS", " ");
    CODE("SSEG", "SEGMENT", "STACK");
    CODE("STK", "DW", "50 DUP(0)");
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
            CODE("MOV", "SP,", "SIZE STK");
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
            obcode_DXBX(DX, i, DIoffset, rdl);
            CODE("ADD", "DX,", "BX");
            BX = NewQt[i].third;
            DX = NewQt[i].fourth;
            rdl = i;
        }
        else if ((NewQt[i].first == "-") && (NewQt[i].third != "_"))             //双目减法
        {
            obcode_DXBX(DX, i, DIoffset, rdl);
            CODE("SUB", "DX,", "BX");
            BX = NewQt[i].third;
            DX = NewQt[i].fourth;
            rdl = i;
        }
        else if ((NewQt[i].first == "*") && (NewQt[i].third != "_"))             //双目乘法
        {
            obcode_DXBX(DX, i, DIoffset, rdl);
            CODE("MOV", "AX,", "DX");
            CODE("MUL", "BX", "");
            CODE("MOV", "DX,", "AX");
            AX = NewQt[i].fourth;
            BX = NewQt[i].third;
            DX = NewQt[i].fourth;
            rdl = i;
        }
        else if ((NewQt[i].first == "/") && (NewQt[i].third != "_"))             //双目除法
        {
            obcode_DXBX(DX, i, DIoffset, rdl);
            CODE("MOV", "AX,", "DX");
            CODE("XOR", "DX,", "DX");
            CODE("DIV", "BX", "");
            CODE("MOV", "DX,", "AX");
            AX = NewQt[i].fourth;
            BX = NewQt[i].third;
            DX = NewQt[i].fourth;
            rdl = i;
        }
        else if ((NewQt[i].first == "%") && (NewQt[i].third != "_"))             //双目取余
        {
            obcode_DXBX(DX, i, DIoffset, rdl);
            CODE("MOV", "AX,", "DX");
            CODE("XOR", "DX,", "DX");
            CODE("DIV", "BX", "");
            BX = NewQt[i].third;
            DX = NewQt[i].fourth;
            rdl = i;
        }
        else if ((NewQt[i].first == "&&") && (NewQt[i].third != "_"))             //双目 与
        {
            obcode_DXBX(DX, i, DIoffset, rdl);
            CODE("AND", "DX,", "BX");
            BX = NewQt[i].third;
            DX = NewQt[i].fourth;
            rdl = i;
        }
        else if ((NewQt[i].first == "||") && (NewQt[i].third != "_"))             //双目 与
        {
            obcode_DXBX(DX, i, DIoffset, rdl);
            CODE("OR", "DX,", "BX");
            BX = NewQt[i].third;
            DX = NewQt[i].fourth;
            rdl = i;
        }
        else if ((NewQt[i].first == "<") || (NewQt[i].first == "<=")    \
            || (NewQt[i].first == ">") || (NewQt[i].first == ">=")  \
            || (NewQt[i].first == "==") || (NewQt[i].first == "!=") && (NewQt[i].third != "_"))       //双目 关系运算符
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
        else if (NewQt[i].first == "=")                //单目 赋值
        {
            obcode_DX(DX, i, DIoffset, rdl);
            if (NewQt[i].fourthac)
            {
                CODE("MOV", "[DI+" + to_string(DIoffset + offset) + "],", "DX");
                DIoffset += offset;
                offsetTable.insert(pair<string, int>(NewQt[i].fourth, DIoffset));
            }
            DX = NewQt[i].fourth;
            rdl = i;
        }
        else if (NewQt[i].first == "if")             //if 语句
        {
            Sem.push(ob_code.size());
            ifelseNum.push(to_string(ifNum));
            ifNum++;
            obcode_DX(DX, i, DIoffset, rdl);
            CODE("CMP", "DX,", "0");
            CODE("JNE", "PD1" + ifelseNum.top(), "");
            CODE("JE", "PD0" + ifelseNum.top(), "");
            CODE("PD1" + ifelseNum.top() + ":", "", "");

            DX = NewQt[i].fourth;
            DX = " ";
        }
        else if (NewQt[i].first == "el")
        {
            if ((DX != " ") && (rdl != -1) && (NewQt[rdl].fourthac))
            {
                CODE("MOV", "[DI+" + to_string(DIoffset + offset) + "],", "DX");
                DIoffset += offset;
                offsetTable.insert(pair<string, int>(NewQt[rdl].fourth, DIoffset));
            }
            CODE("JMP", "CONTI" + ifelseNum.top(), " ");
            CODE("PD0" + ifelseNum.top() + ":", "", "");
            DX = " ";
        }
        else if (NewQt[i].first == "ie")
        {
            if ((DX != " ") && (rdl != -1) && (NewQt[rdl].fourthac))
            {
                CODE("MOV", "[DI+" + to_string(DIoffset + offset) + "],", "DX");
                DIoffset += offset;
                offsetTable.insert(pair<string, int>(NewQt[rdl].fourth, DIoffset));
            }
            CODE("CONTI" + ifelseNum.top() + ":", "", "");
            ifelseNum.pop();
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

int main()
{
    active_info();
    objectcode_asm(0, NewQt.size());       //汇编代码生成
    obcode_to_file();
}

