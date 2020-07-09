#pragma once
#include <sstream>
#include <cstdlib>
#include<cstdio>
#include"main_structure.h"

using namespace std;

struct DAGnode//DAG图的节点结构体
{
    string operate;			//运算符
    int ChildLeft = -1;		//左后继
    int ChildRight = -1;	//右后继
    vector<string>SecondMark;//附加标记
    string FirstMark;	//主标记
    string s[4];		//为特殊四元式准备的数组
};
vector<DAGnode>DAG;
vector<string>s;//存放函数返回值变量名
vector<string>Funcandnum;//存放函数返回值变量名

double StringToDouble(string s)
{
    stringstream ss;
    double n;
    ss << s;
    ss >> n;
    return n;
}

string DoubleToString(double n)
{
    stringstream ss;
    ss << n;
    return ss.str();
}

bool Find(string string)//判断能否找到string在s里
{
    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] == string)
            return true;
    }
    return false;
}

void Pre_process()//对四元式预处理
{
    for (int i = 0; i < Quaternary.size(); i++)
    {
        if (Quaternary[i].first == "temp")
            s.push_back(Quaternary[i].fourth);
        if (Quaternary[i].first == "diaoyong")
          {
                if (Find(Quaternary[i].fourth))
                {
                    Quaternary[i].fourth = Quaternary[i].fourth + DoubleToString(fun_value);
                    Funcandnum.push_back(Quaternary[i].fourth);
                }
          }
        if (Quaternary[i].first == "=")//是赋值语句
        {
            if (Find(Quaternary[i].second) && Quaternary[i].third=="_")//且在s中能找到
            {
                Quaternary[i].second = Quaternary[i].second + DoubleToString(fun_value);
                Quaternary[i].secondac = true;
                Funcandnum.push_back(Quaternary[i].second);
                fun_value++;
            }
        }
    }
}

bool IsNumber(string s)//判断是否为数字
{
    for (int i = 0; i < s.length(); i++)
    {
        if ((s[i] >= '0' && s[i] <= '9') || s[i] == '.')
            continue;
        else
            return false;
    }
    return true;
}

int LengthOfNum(double num)
{
    string str;
    str = DoubleToString(num);
    return str.length();
}

int IsDefined_FS(string s)//查主标志和附加标志里是否被定义过
{
    for (int n = DAG.size() - 1; n >= 0; n--)
    {
        if (DAG[n].FirstMark == s)//查DAG主标记
            return n;
        else//查DAG附加标记
        {
            for (int i = DAG[n].SecondMark.size() - 1; i >= 0; i--)
            {
                if (DAG[n].SecondMark[i] == s)
                    return n;
            }
        }
    }
    return -1;
}

void IsDefined_S(string s, int n)//除了被插入附加标记的那个节点，其他节点只查附加标志,有就删去，无则继续
{
    for (n = n - 1; n >= 0; n--)
    {
        for (int i = DAG[n].SecondMark.size() - 1; i >= 0; i--)
        {
            if (DAG[n].SecondMark[i] == s)
                DAG[n].SecondMark.erase(DAG[n].SecondMark.begin() + i);
        }
    }
}

double Compute(double a, string s, double b)
{
    if (s == "*")return a * b;
    if (s == "/")return a / b;
    if (s == "+")return a + b;
    if (s == "-")return a - b;
    if (s == "%")return (int)a % (int)b;
    if (s == "||")return a || b;
    if (s == "&&")return a && b;
    if (s == "==")return a == b;
    if (s == "!=")return a != b;
    if (s == ">=")return a >= b;
    if (s == ">")return a > b;
    if (s == "<")return a < b;
    if (s == "<=")return a <= b;
}

bool InSymbleTerm(string s)//判断是否是临时变量
{
    if (s[0] == '-')
        return false;
    return true;
}

void SecondToFirst()//把主标记换成非临时变量
{
    for (int i = 0; i < DAG.size(); i++)
    {
        if (!InSymbleTerm(DAG[i].FirstMark))//主标记不在符号表
        {
            for (int j = 0; j < DAG[i].SecondMark.size(); j++)
            {
                if (InSymbleTerm(DAG[i].SecondMark[j]))//查附标记是否有在符号表里的,有则替换
                {
                    string temp = DAG[i].SecondMark[j];
                    DAG[i].SecondMark[j] = DAG[i].FirstMark;
                    DAG[i].FirstMark = temp;
                    break;
                }
            }
        }
    }
}

void DAGyouhua(int begin, int end)//优化建图
{
    Pre_process();
    for (int i = begin; i <= end; i++)
    {
        if (Quaternary[i].first == "=")
        {
            if (IsDefined_FS(Quaternary[i].second) != -1)//该被定义过
            {
                DAG[IsDefined_FS(Quaternary[i].second)].SecondMark.push_back(Quaternary[i].fourth);//加入附加标志

            }
            else//没被定义，则建节点
            {
                DAGnode dagnode;
                dagnode.FirstMark = Quaternary[i].second;
                dagnode.SecondMark.push_back(Quaternary[i].fourth);
                DAG.push_back(dagnode);
            }
            if (Quaternary[i].secondac != true)//不是函数返回值标记
            {
                IsDefined_S(Quaternary[i].fourth, IsDefined_FS(Quaternary[i].second));//被定义在附加标记里过，则删去那个标记
            }
            else
            {
                int n = 0;
                for (int k = DAG.size() - 2; k >= 0; k--)
                {
                    for (int j = DAG[k].SecondMark.size() - 1; j >= 0; j--)
                    {
                        if (DAG[k].SecondMark[j] == Quaternary[i].fourth && !IsNumber(DAG[k].FirstMark))//找到之前的定义且主标记不为数字
                            DAG[k].SecondMark.erase(DAG[k].SecondMark.begin() + j);
                    }
                }
            }
        }
        else if (Quaternary[i].first == "*" || Quaternary[i].first == "/" || Quaternary[i].first == "%" || Quaternary[i].first == "||" || Quaternary[i].first == "&&"
            || Quaternary[i].first == "==" || Quaternary[i].first == "!=" || Quaternary[i].first == ">=" || Quaternary[i].first == ">" || Quaternary[i].first == "<="
            || Quaternary[i].first == "<" || Quaternary[i].first == "+" || (Quaternary[i].first == "-" && Quaternary[i].third != "_"))//双目运算符
        {
            if (IsNumber(Quaternary[i].second) && IsNumber(Quaternary[i].third))//运算对象同为数字
            {
                double a = StringToDouble(Quaternary[i].second);
                double b = StringToDouble(Quaternary[i].third);
                double result = Compute(a, Quaternary[i].first, b);
                if (IsDefined_FS(DoubleToString(result)) != -1)//该被定义过
                {
                    DAG[IsDefined_FS(DoubleToString(result))].SecondMark.push_back(Quaternary[i].fourth);
                }
                else//没被定义，则建节点
                {
                    DAGnode dagnode;
                    dagnode.FirstMark = DoubleToString(result);
                    dagnode.SecondMark.push_back(Quaternary[i].fourth);
                    DAG.push_back(dagnode);
                }
                IsDefined_S(Quaternary[i].fourth, IsDefined_FS(Quaternary[i].second));//如果forth在之前的附加标记，则删去
            }
            else
            {
                int left = -2, right = -2, flag = 0;
                if (IsDefined_FS(Quaternary[i].second) != -1)
                {
                    left = IsDefined_FS(Quaternary[i].second);
                }
                else //没被定义过，建点
                {
                    DAGnode dagnode;
                    dagnode.FirstMark = Quaternary[i].second;
                    DAG.push_back(dagnode);
                }
                if (IsDefined_FS(Quaternary[i].third) != -1)//右孩子被定义过
                {
                    right = IsDefined_FS(Quaternary[i].third);
                }
                else
                {
                    DAGnode dagnode;
                    dagnode.FirstMark = Quaternary[i].third;
                    DAG.push_back(dagnode);
                }
                for (int j = DAG.size() - 1; j >= 0; j--)
                {
                    if (DAG[j].operate == Quaternary[i].first && DAG[j].ChildLeft == left && DAG[j].ChildRight == right)//有公共表达式
                    {
                        DAG[j].SecondMark.push_back(Quaternary[i].fourth);
                        flag = 1;
                        break;
                    }
                }
                if (flag != 1)//无公共表达式，则执行此
                {
                    DAGnode dagnode;
                    dagnode.FirstMark = Quaternary[i].fourth;
                    dagnode.operate = Quaternary[i].first;
                    dagnode.ChildLeft = IsDefined_FS(Quaternary[i].second);
                    dagnode.ChildRight = IsDefined_FS(Quaternary[i].third);
                    DAG.push_back(dagnode);
                }
                IsDefined_S(Quaternary[i].fourth, IsDefined_FS(Quaternary[i].second));//forth之前附加标志定义过，则删除之
            }
        }
        else if (Quaternary[i].first == "!" || (Quaternary[i].first == "-" && Quaternary[i].third == "_"))//单目运算符
        {
            int left = -2, flag = 0;
            if (IsDefined_FS(Quaternary[i].second) != -1)//如果存在
            {
                left = IsDefined_FS(Quaternary[i].second);
            }
            else//不存在，建结点
            {
                DAGnode dagnode;
                dagnode.FirstMark = Quaternary[i].second;
                DAG.push_back(dagnode);
            }
            for (int j = DAG.size() - 1; j >= 0; j--)
            {
                if (DAG[j].operate == Quaternary[i].first && DAG[j].ChildLeft == left)//有公共表达式
                {
                    DAG[j].SecondMark.push_back(Quaternary[i].fourth);
                    flag = 1;
                    break;
                }
            }
            if (flag != 1)
            {
                DAGnode dagnode;
                dagnode.FirstMark = Quaternary[i].fourth;
                dagnode.ChildLeft = IsDefined_FS(Quaternary[i].second);
                dagnode.operate = Quaternary[i].first;
                DAG.push_back(dagnode);
            }
            IsDefined_S(Quaternary[i].fourth, IsDefined_FS(Quaternary[i].second));
        }
        else if (Quaternary[i].first == "hanshu" || Quaternary[i].first == "re" || Quaternary[i].first == "diaoyong" || Quaternary[i].first == "wh" ||
            Quaternary[i].first == "xingcan" || Quaternary[i].first == "canshu" || Quaternary[i].first == "END" || Quaternary[i].first == "if" ||
            Quaternary[i].first == "el" || Quaternary[i].first == "do" || Quaternary[i].first == "we" || Quaternary[i].first == "ie" || Quaternary[i].first == "temp")
        {
            DAGnode dagnode;
            dagnode.s[0] = Quaternary[i].first;
            dagnode.s[1] = Quaternary[i].second;
            dagnode.s[2] = Quaternary[i].third;
            dagnode.s[3] = Quaternary[i].fourth;
            DAG.push_back(dagnode);
        }
    }
    SecondToFirst();//主标记换成非临时变量
}

void DAGoutput()//重组四元式
{
    for (int j = 0; j < DAG.size(); j++)
    {
        if (DAG[j].SecondMark.size() != 0 && DAG[j].ChildLeft == -1 && DAG[j].ChildRight == -1)//带有附加标记的叶节点
        {
            if (InSymbleTerm(DAG[j].FirstMark))//主标记为非临时变量
            {
                for (int i = 0; i < DAG[j].SecondMark.size(); i++)//为非临时变量，则生成赋值四元式
                {
                    if (InSymbleTerm(DAG[j].SecondMark[i]))
                    {
                        QtTerm newqt;
                        newqt.first = "=";
                        newqt.second = DAG[j].FirstMark;
                        newqt.third = "_";
                        newqt.fourth = DAG[j].SecondMark[i];
                        NewQt.push_back(newqt);
                    }
                }
            }
        }
        else if (DAG[j].ChildLeft != -1 || DAG[j].ChildRight != -1)//带附加标记，并且不为叶节点
        {
            if (DAG[j].ChildLeft != -1 && DAG[j].ChildRight != -1)//两个后继
            {
                QtTerm newqt;
                newqt.first = DAG[j].operate;
                newqt.second = DAG[DAG[j].ChildLeft].FirstMark;
                newqt.third = DAG[DAG[j].ChildRight].FirstMark;
                newqt.fourth = DAG[j].FirstMark;
                NewQt.push_back(newqt);
            }
            else if (DAG[j].ChildLeft != -1 && DAG[j].ChildRight == -1)//只有左孩子
            {
                QtTerm newqt;
                newqt.first = DAG[j].operate;
                newqt.second = DAG[DAG[j].ChildLeft].FirstMark;
                newqt.third = "_";
                newqt.fourth = DAG[j].FirstMark;
                NewQt.push_back(newqt);
            }
            if (!DAG[j].SecondMark.empty())//如果带有附加标记
            {
                if (InSymbleTerm(DAG[j].FirstMark))//主标记为非临时变量
                {
                    for (int i = 0; i < DAG[j].SecondMark.size(); i++)//附加标志为非临时变量，则生成赋值四元式
                    {
                        if (InSymbleTerm(DAG[j].SecondMark[i]))//在附加标记里如果是非临时变量，则产生赋值四元式
                        {
                            QtTerm newqt;
                            newqt.first = "=";
                            newqt.second = DAG[j].FirstMark;
                            newqt.third = "_";
                            newqt.fourth = DAG[j].SecondMark[i];
                            NewQt.push_back(newqt);
                        }
                    }
                }
            }
        }
        else if (!DAG[j].s[0].empty())
        {
            QtTerm newqt;
            newqt.first = DAG[j].s[0];
            newqt.second = DAG[j].s[1];
            newqt.third = DAG[j].s[2];
            newqt.fourth = DAG[j].s[3];
            NewQt.push_back(newqt);
        }
    }
    DAG.clear();
}

void After_process()//后处理
{
    fun_value--;
    for (int i = NewQt.size()-1; i >= 0; i--)
    {
        if (NewQt[i].first == "=")
        {
            for (int n = 0; n < Funcandnum.size(); n++)
            {
                if (NewQt[i].second == Funcandnum[n])
                {
                    NewQt[i].second.erase(NewQt[i].second.end()-LengthOfNum(fun_value));
                }
            }
        }
        if (NewQt[i].first == "diaoyong")
        {
            NewQt[i].fourth.erase(NewQt[i].fourth.end() - LengthOfNum(fun_value));
            fun_value--;
        }
    }
}

void DivideBaseblock()//划分基本块
{
    Pre_process();
    int begin = 0, end = 0;//基本块的开始与结束
    for (end; end < Quaternary.size(); end++)
    {
        if (Quaternary[end].first == "if" || Quaternary[end].first == "el" || Quaternary[end].first == "ie" || Quaternary[end].first == "wh" || Quaternary[end].first == "do" || Quaternary[end].first == "we")
        {
            DAGyouhua(begin, end - 1);
            DAGoutput();
            begin = end;
        }
    }
    if (begin < end)
    {
        DAGyouhua(begin, end - 1);
        DAGoutput();
    }
    After_process();
}

void NewQtToFile()
{
    ofstream file("xinsiyusnshi.txt");
    for (int i = 0; i < NewQt.size(); i++)
    {
        file <<NewQt[i].first << '\t' << NewQt[i].second << '\t' << NewQt[i].third << '\t' << NewQt[i].fourth<< endl;
    }
    file.close();
}

void QtToFile()
{
    ofstream file("siyusnshi.txt");
    for (int i = 0; i < Quaternary.size(); i++)
    {
        file << Quaternary[i].first << '\t' << Quaternary[i].second << '\t' << Quaternary[i].third << '\t' << Quaternary[i].fourth << endl;
    }
    file.close();
}
