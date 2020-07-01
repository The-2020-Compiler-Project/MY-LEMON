#include<vector>
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include<cstdio>


using namespace std;

struct QtTerm//四元式结构体
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

vector<QtTerm>Qt;//四元式向量
vector<QtTerm>NewQt;//新四元式向量


struct DAGnode//DAG图的节点结构体
{
	string operate;			//运算符
	int ChildLeft = -1;		//左后继
	int ChildRight = -1;	//右后继
	vector<string>SecondMark;//附加标记
	string FirstMark;		//主标记
};
vector<DAGnode>DAG;

double StringToDouble(string s)
{
	double n = atof(s.c_str());
	return n;
}

string DoubleToString(double n)
{
	char buffer[20];
	sprintf_s(buffer, "%f", n);
	string s = buffer;
	return s;
}

bool IsNumber(string s)//判断是否为数字
{
	for (int i = 0; i < s.length(); i++)
	{
		if ((s[i] >= '0' && s[i] <= '9')||s[i]=='.')
			continue;
		else
			return false;
	}
	return true;
}

int IsDefined_FS(string s)//查主标志和附加标志里是否被定义过
{
	for (int n = DAG.size()-1; n >= 0; n--)
	{
		if (DAG[n].FirstMark == s)//查DAG主标记
			return n;
		else//查DAG附加标记
		{
			for (int i = DAG[n].SecondMark.size() - 1; i >= 0; i++)
			{
				if (DAG[n].SecondMark[i] == s)
					return n;
			}
		}
	}
	return -1;
}

void IsDefined_S(string s)//只查附加标志,有就删去，无则继续
{
	for (int n = DAG.size() - 1; n >= 0; n--)
	{
		for (int i = DAG[n].SecondMark.size() - 1; i >= 0; i++)
		{
			if (DAG[n].SecondMark[i] == s)
				DAG[n].SecondMark.erase(DAG[n].SecondMark.begin()+i);
		}
	}
}

double Compute(double a,string s,double b)
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

int InSymbleTerm(string s)//判断是否在符号表中
{

}

void SecondToFirst()
{
	for (int i = 0; i < DAG.size(); i++)
	{

	}
}

void DAGyouhua(int begin,int end)
{
	for (int i=begin; i <= end; i++)
	{
		if (Qt[i].first == "=")
		{
			if (IsDefined_FS(Qt[i].second) != -1)//该被定义过
			{
				DAG[IsDefined_FS(Qt[i].second)].SecondMark.push_back(Qt[i].fourth);
			}
			else//没被定义，则建节点
			{
				DAGnode dagnode;
				dagnode.FirstMark = Qt[i].second;
				dagnode.SecondMark.push_back(Qt[i].fourth);
				DAG.push_back(dagnode);
			}
			IsDefined_S(Qt[i].fourth);
		}
		else if (Qt[i].first == "*" || Qt[i].first == "/" || Qt[i].first == "%" || Qt[i].first == "||" || Qt[i].first == "&&"
			|| Qt[i].first == "==" || Qt[i].first == "!=" || Qt[i].first == ">=" || Qt[i].first == ">" || Qt[i].first == "<="
			|| Qt[i].first == "<" || Qt[i].first == "+" || (Qt[i].first == "-"&&Qt[i].third!="_"))//双目运算符
		{
			if (IsNumber(Qt[i].second) && IsNumber(Qt[i].third))//运算对象同为数字
			{
				double a = StringToDouble(Qt[i].second);
				double b = StringToDouble(Qt[i].third);
				double result = Compute(a,Qt[i].first,b);
				if (IsDefined_FS(DoubleToString(result)) != -1)//该被定义过
				{
					DAG[IsDefined_FS(DoubleToString(result))].SecondMark.push_back(Qt[i].fourth);
				}
				else//没被定义，则建节点
				{
					DAGnode dagnode;
					dagnode.FirstMark = DoubleToString(result);
					dagnode.SecondMark.push_back(Qt[i].fourth);
					DAG.push_back(dagnode);
				}
			    IsDefined_S(Qt[i].fourth);//如果forth在之前的附加标记，则删去
			}
			else 
			{
				int left = -2, right = -2, flag;
				if (IsDefined_FS(Qt[i].second) != -1)
				{
					left = IsDefined_FS(Qt[i].second);
				}
				else //没被定义过，建点
				{
					DAGnode dagnode;
					dagnode.FirstMark = Qt[i].second;
					DAG.push_back(dagnode);
				}
				if (IsDefined_FS(Qt[i].third) != -1)
				{
					right = IsDefined_FS(Qt[i].third);
				}
				else
				{
					DAGnode dagnode;
					dagnode.FirstMark = Qt[i].third;
					DAG.push_back(dagnode);
				}
				for (int j = DAG.size() - 1; j >= 0; j--)
				{
					if (DAG[j].operate == Qt[i].first && DAG[j].ChildLeft == left && DAG[j].ChildRight == right)//有公共表达式
					{
						DAG[j].SecondMark.push_back(Qt[i].fourth);
						flag = 1;
						break;
					}
				}
				if (flag != 1)//无公共表达式，则执行此
				{
					DAGnode dagnode;
					dagnode.FirstMark = Qt[i].fourth;
					dagnode.operate = Qt[i].first;
					dagnode.ChildLeft = IsDefined_FS(Qt[i].second);
					dagnode.ChildRight = IsDefined_FS(Qt[i].third);
					DAG.push_back(dagnode);
				}
				IsDefined_S(Qt[i].fourth);//forth之前附加标志定义过，则删除之
			}
		}
		else if (Qt[i].first == "!" || (Qt[i].first == "-" && Qt[i].third == "_"))//单目运算符
		{
			int left = -2, flag = 0;
			if (IsDefined_FS(Qt[i].second) != -1)//如果存在
			{
				left = IsDefined_FS(Qt[i].second);
			}
			else//不存在，建结点
			{
				DAGnode dagnode;
				dagnode.FirstMark = Qt[i].second;
				DAG.push_back(dagnode);
			}
			for (int j = DAG.size() - 1; j >= 0; j--)
			{
				if (DAG[j].operate == Qt[i].first && DAG[j].ChildLeft == left )//有公共表达式
				{
					DAG[j].SecondMark.push_back(Qt[i].fourth);
					flag = 1;
					break;
				}
			}
			if (flag != 1)
			{
				DAGnode dagnode;
				dagnode.FirstMark = Qt[i].fourth;
				dagnode.ChildLeft = IsDefined_FS(Qt[i].second);
				dagnode.operate = Qt[i].first;
				DAG.push_back(dagnode);
			}
			IsDefined_S(Qt[i].fourth);
		}
		else if (Qt[i].first == "hanshu" || Qt[i].first == "re" || Qt[i].first == "diaoyong" || Qt[i].first == "xingcan"
			|| Qt[i].first == "canshu" || Qt[i].first == "END" || Qt[i].first == "temp")
		{
			
		}
	}

}

void DAGoutput()
{

}

void DivideBaseblock()//划分基本块
{
	int begin = 0, end = 0;//基本块的开始与结束
	for (end; end < Qt.size(); end++)
	{	
		if (Qt[end].first == "if" || Qt[end].first == "el" || Qt[end].first == "ie" || Qt[end].first == "wh" || Qt[end].first == "do" || Qt[end].first == "we")
		{
			DAGyouhua(begin, end);
			begin = end+1;
		}
	}
	if (begin < end)
		DAGyouhua(begin, end - 1);
}