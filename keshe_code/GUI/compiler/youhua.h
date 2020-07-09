#pragma once
#include <sstream>
#include <cstdlib>
#include<cstdio>
#include"main_structure.h"

using namespace std;

struct DAGnode//DAGͼ�Ľڵ�ṹ��
{
    string operate;			//�����
    int ChildLeft = -1;		//����
    int ChildRight = -1;	//�Һ��
    vector<string>SecondMark;//���ӱ��
    string FirstMark;	//�����
    string s[4];		//Ϊ������Ԫʽ׼��������
};
vector<DAGnode>DAG;
vector<string>s;//��ź�������ֵ������
vector<string>Funcandnum;//��ź�������ֵ������

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

bool Find(string string)//�ж��ܷ��ҵ�string��s��
{
    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] == string)
            return true;
    }
    return false;
}

void Pre_process()//����ԪʽԤ����
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
        if (Quaternary[i].first == "=")//�Ǹ�ֵ���
        {
            if (Find(Quaternary[i].second) && Quaternary[i].third=="_")//����s�����ҵ�
            {
                Quaternary[i].second = Quaternary[i].second + DoubleToString(fun_value);
                Quaternary[i].secondac = true;
                Funcandnum.push_back(Quaternary[i].second);
                fun_value++;
            }
        }
    }
}

bool IsNumber(string s)//�ж��Ƿ�Ϊ����
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

int IsDefined_FS(string s)//������־�͸��ӱ�־���Ƿ񱻶����
{
    for (int n = DAG.size() - 1; n >= 0; n--)
    {
        if (DAG[n].FirstMark == s)//��DAG�����
            return n;
        else//��DAG���ӱ��
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

void IsDefined_S(string s, int n)//���˱����븽�ӱ�ǵ��Ǹ��ڵ㣬�����ڵ�ֻ�鸽�ӱ�־,�о�ɾȥ���������
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

bool InSymbleTerm(string s)//�ж��Ƿ�����ʱ����
{
    if (s[0] == '-')
        return false;
    return true;
}

void SecondToFirst()//������ǻ��ɷ���ʱ����
{
    for (int i = 0; i < DAG.size(); i++)
    {
        if (!InSymbleTerm(DAG[i].FirstMark))//����ǲ��ڷ��ű�
        {
            for (int j = 0; j < DAG[i].SecondMark.size(); j++)
            {
                if (InSymbleTerm(DAG[i].SecondMark[j]))//�鸽����Ƿ����ڷ��ű����,�����滻
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

void DAGyouhua(int begin, int end)//�Ż���ͼ
{
    Pre_process();
    for (int i = begin; i <= end; i++)
    {
        if (Quaternary[i].first == "=")
        {
            if (IsDefined_FS(Quaternary[i].second) != -1)//�ñ������
            {
                DAG[IsDefined_FS(Quaternary[i].second)].SecondMark.push_back(Quaternary[i].fourth);//���븽�ӱ�־

            }
            else//û�����壬�򽨽ڵ�
            {
                DAGnode dagnode;
                dagnode.FirstMark = Quaternary[i].second;
                dagnode.SecondMark.push_back(Quaternary[i].fourth);
                DAG.push_back(dagnode);
            }
            if (Quaternary[i].secondac != true)//���Ǻ�������ֵ���
            {
                IsDefined_S(Quaternary[i].fourth, IsDefined_FS(Quaternary[i].second));//�������ڸ��ӱ���������ɾȥ�Ǹ����
            }
            else
            {
                int n = 0;
                for (int k = DAG.size() - 2; k >= 0; k--)
                {
                    for (int j = DAG[k].SecondMark.size() - 1; j >= 0; j--)
                    {
                        if (DAG[k].SecondMark[j] == Quaternary[i].fourth && !IsNumber(DAG[k].FirstMark))//�ҵ�֮ǰ�Ķ���������ǲ�Ϊ����
                            DAG[k].SecondMark.erase(DAG[k].SecondMark.begin() + j);
                    }
                }
            }
        }
        else if (Quaternary[i].first == "*" || Quaternary[i].first == "/" || Quaternary[i].first == "%" || Quaternary[i].first == "||" || Quaternary[i].first == "&&"
            || Quaternary[i].first == "==" || Quaternary[i].first == "!=" || Quaternary[i].first == ">=" || Quaternary[i].first == ">" || Quaternary[i].first == "<="
            || Quaternary[i].first == "<" || Quaternary[i].first == "+" || (Quaternary[i].first == "-" && Quaternary[i].third != "_"))//˫Ŀ�����
        {
            if (IsNumber(Quaternary[i].second) && IsNumber(Quaternary[i].third))//�������ͬΪ����
            {
                double a = StringToDouble(Quaternary[i].second);
                double b = StringToDouble(Quaternary[i].third);
                double result = Compute(a, Quaternary[i].first, b);
                if (IsDefined_FS(DoubleToString(result)) != -1)//�ñ������
                {
                    DAG[IsDefined_FS(DoubleToString(result))].SecondMark.push_back(Quaternary[i].fourth);
                }
                else//û�����壬�򽨽ڵ�
                {
                    DAGnode dagnode;
                    dagnode.FirstMark = DoubleToString(result);
                    dagnode.SecondMark.push_back(Quaternary[i].fourth);
                    DAG.push_back(dagnode);
                }
                IsDefined_S(Quaternary[i].fourth, IsDefined_FS(Quaternary[i].second));//���forth��֮ǰ�ĸ��ӱ�ǣ���ɾȥ
            }
            else
            {
                int left = -2, right = -2, flag = 0;
                if (IsDefined_FS(Quaternary[i].second) != -1)
                {
                    left = IsDefined_FS(Quaternary[i].second);
                }
                else //û�������������
                {
                    DAGnode dagnode;
                    dagnode.FirstMark = Quaternary[i].second;
                    DAG.push_back(dagnode);
                }
                if (IsDefined_FS(Quaternary[i].third) != -1)//�Һ��ӱ������
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
                    if (DAG[j].operate == Quaternary[i].first && DAG[j].ChildLeft == left && DAG[j].ChildRight == right)//�й������ʽ
                    {
                        DAG[j].SecondMark.push_back(Quaternary[i].fourth);
                        flag = 1;
                        break;
                    }
                }
                if (flag != 1)//�޹������ʽ����ִ�д�
                {
                    DAGnode dagnode;
                    dagnode.FirstMark = Quaternary[i].fourth;
                    dagnode.operate = Quaternary[i].first;
                    dagnode.ChildLeft = IsDefined_FS(Quaternary[i].second);
                    dagnode.ChildRight = IsDefined_FS(Quaternary[i].third);
                    DAG.push_back(dagnode);
                }
                IsDefined_S(Quaternary[i].fourth, IsDefined_FS(Quaternary[i].second));//forth֮ǰ���ӱ�־���������ɾ��֮
            }
        }
        else if (Quaternary[i].first == "!" || (Quaternary[i].first == "-" && Quaternary[i].third == "_"))//��Ŀ�����
        {
            int left = -2, flag = 0;
            if (IsDefined_FS(Quaternary[i].second) != -1)//�������
            {
                left = IsDefined_FS(Quaternary[i].second);
            }
            else//�����ڣ������
            {
                DAGnode dagnode;
                dagnode.FirstMark = Quaternary[i].second;
                DAG.push_back(dagnode);
            }
            for (int j = DAG.size() - 1; j >= 0; j--)
            {
                if (DAG[j].operate == Quaternary[i].first && DAG[j].ChildLeft == left)//�й������ʽ
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
    SecondToFirst();//����ǻ��ɷ���ʱ����
}

void DAGoutput()//������Ԫʽ
{
    for (int j = 0; j < DAG.size(); j++)
    {
        if (DAG[j].SecondMark.size() != 0 && DAG[j].ChildLeft == -1 && DAG[j].ChildRight == -1)//���и��ӱ�ǵ�Ҷ�ڵ�
        {
            if (InSymbleTerm(DAG[j].FirstMark))//�����Ϊ����ʱ����
            {
                for (int i = 0; i < DAG[j].SecondMark.size(); i++)//Ϊ����ʱ�����������ɸ�ֵ��Ԫʽ
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
        else if (DAG[j].ChildLeft != -1 || DAG[j].ChildRight != -1)//�����ӱ�ǣ����Ҳ�ΪҶ�ڵ�
        {
            if (DAG[j].ChildLeft != -1 && DAG[j].ChildRight != -1)//�������
            {
                QtTerm newqt;
                newqt.first = DAG[j].operate;
                newqt.second = DAG[DAG[j].ChildLeft].FirstMark;
                newqt.third = DAG[DAG[j].ChildRight].FirstMark;
                newqt.fourth = DAG[j].FirstMark;
                NewQt.push_back(newqt);
            }
            else if (DAG[j].ChildLeft != -1 && DAG[j].ChildRight == -1)//ֻ������
            {
                QtTerm newqt;
                newqt.first = DAG[j].operate;
                newqt.second = DAG[DAG[j].ChildLeft].FirstMark;
                newqt.third = "_";
                newqt.fourth = DAG[j].FirstMark;
                NewQt.push_back(newqt);
            }
            if (!DAG[j].SecondMark.empty())//������и��ӱ��
            {
                if (InSymbleTerm(DAG[j].FirstMark))//�����Ϊ����ʱ����
                {
                    for (int i = 0; i < DAG[j].SecondMark.size(); i++)//���ӱ�־Ϊ����ʱ�����������ɸ�ֵ��Ԫʽ
                    {
                        if (InSymbleTerm(DAG[j].SecondMark[i]))//�ڸ��ӱ��������Ƿ���ʱ�������������ֵ��Ԫʽ
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

void After_process()//����
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

void DivideBaseblock()//���ֻ�����
{
    Pre_process();
    int begin = 0, end = 0;//������Ŀ�ʼ�����
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
