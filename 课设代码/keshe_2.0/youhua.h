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
	for (int i = 0; i < Qt.size(); i++)
	{
		if (Qt[i].first == "temp")
			s.push_back(Qt[i].fourth);
		if (Qt[i].first == "diaoyong")
		{
			if (Find(Qt[i].fourth))
			{
				Qt[i].fourth = Qt[i].fourth + DoubleToString(fun_value);
			}
		}
		if (Qt[i].first == "=")//�Ǹ�ֵ���
		{
			if (Find(Qt[i].second) && Qt[i].third=="_")//����s�����ҵ�
			{
				Qt[i].second = Qt[i].second + DoubleToString(fun_value);
				Qt[i].secondac = true;
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

bool InSymbleTerm(string s)//�ж��Ƿ��ڷ��ű���
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
		if (Qt[i].first == "=")
		{
			if (IsDefined_FS(Qt[i].second) != -1)//�ñ������
			{
				DAG[IsDefined_FS(Qt[i].second)].SecondMark.push_back(Qt[i].fourth);//���븽�ӱ�־

			}
			else//û�����壬�򽨽ڵ�
			{
				DAGnode dagnode;
				dagnode.FirstMark = Qt[i].second;
				dagnode.SecondMark.push_back(Qt[i].fourth);
				DAG.push_back(dagnode);
			}
			if (Qt[i].secondac != true)//���Ǻ�������ֵ���
			{
				IsDefined_S(Qt[i].fourth, IsDefined_FS(Qt[i].second));//�������ڸ��ӱ���������ɾȥ�Ǹ����
			}
			else
			{
				int n = 0;
				for (int k = DAG.size() - 2; k >= 0; k--)
				{
					for (int j = DAG[k].SecondMark.size() - 1; j >= 0; j--)
					{
						if (DAG[k].SecondMark[j] == Qt[i].fourth && !IsNumber(DAG[k].FirstMark))//�ҵ�֮ǰ�Ķ���������ǲ�Ϊ����
							DAG[k].SecondMark.erase(DAG[k].SecondMark.begin() + j);
					}
				}
			}
		}
		else if (Qt[i].first == "*" || Qt[i].first == "/" || Qt[i].first == "%" || Qt[i].first == "||" || Qt[i].first == "&&"
			|| Qt[i].first == "==" || Qt[i].first == "!=" || Qt[i].first == ">=" || Qt[i].first == ">" || Qt[i].first == "<="
			|| Qt[i].first == "<" || Qt[i].first == "+" || (Qt[i].first == "-" && Qt[i].third != "_"))//˫Ŀ�����
		{
			if (IsNumber(Qt[i].second) && IsNumber(Qt[i].third))//�������ͬΪ����
			{
				double a = StringToDouble(Qt[i].second);
				double b = StringToDouble(Qt[i].third);
				double result = Compute(a, Qt[i].first, b);
				if (IsDefined_FS(DoubleToString(result)) != -1)//�ñ������
				{
					DAG[IsDefined_FS(DoubleToString(result))].SecondMark.push_back(Qt[i].fourth);
				}
				else//û�����壬�򽨽ڵ�
				{
					DAGnode dagnode;
					dagnode.FirstMark = DoubleToString(result);
					dagnode.SecondMark.push_back(Qt[i].fourth);
					DAG.push_back(dagnode);
				}
				IsDefined_S(Qt[i].fourth, IsDefined_FS(Qt[i].second));//���forth��֮ǰ�ĸ��ӱ�ǣ���ɾȥ
			}
			else
			{
				int left = -2, right = -2, flag = 0;
				if (IsDefined_FS(Qt[i].second) != -1)
				{
					left = IsDefined_FS(Qt[i].second);
				}
				else //û�������������
				{
					DAGnode dagnode;
					dagnode.FirstMark = Qt[i].second;
					DAG.push_back(dagnode);
				}
				if (IsDefined_FS(Qt[i].third) != -1)//�Һ��ӱ������
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
					if (DAG[j].operate == Qt[i].first && DAG[j].ChildLeft == left && DAG[j].ChildRight == right)//�й������ʽ
					{
						DAG[j].SecondMark.push_back(Qt[i].fourth);
						flag = 1;
						break;
					}
				}
				if (flag != 1)//�޹������ʽ����ִ�д�
				{
					DAGnode dagnode;
					dagnode.FirstMark = Qt[i].fourth;
					dagnode.operate = Qt[i].first;
					dagnode.ChildLeft = IsDefined_FS(Qt[i].second);
					dagnode.ChildRight = IsDefined_FS(Qt[i].third);
					DAG.push_back(dagnode);
				}
				IsDefined_S(Qt[i].fourth, IsDefined_FS(Qt[i].second));//forth֮ǰ���ӱ�־���������ɾ��֮
			}
		}
		else if (Qt[i].first == "!" || (Qt[i].first == "-" && Qt[i].third == "_"))//��Ŀ�����
		{
			int left = -2, flag = 0;
			if (IsDefined_FS(Qt[i].second) != -1)//�������
			{
				left = IsDefined_FS(Qt[i].second);
			}
			else//�����ڣ������
			{
				DAGnode dagnode;
				dagnode.FirstMark = Qt[i].second;
				DAG.push_back(dagnode);
			}
			for (int j = DAG.size() - 1; j >= 0; j--)
			{
				if (DAG[j].operate == Qt[i].first && DAG[j].ChildLeft == left)//�й������ʽ
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
			IsDefined_S(Qt[i].fourth, IsDefined_FS(Qt[i].second));
		}
		else if (Qt[i].first == "hanshu" || Qt[i].first == "re" || Qt[i].first == "diaoyong" || Qt[i].first == "wh" ||
			Qt[i].first == "xingcan" || Qt[i].first == "canshu" || Qt[i].first == "END" || Qt[i].first == "if" ||
			Qt[i].first == "el" || Qt[i].first == "do" || Qt[i].first == "we" || Qt[i].first == "ie" || Qt[i].first == "temp"
			|| Qt[i].first == "cin" || Qt[i].first == "cout")
		{
			DAGnode dagnode;
			dagnode.s[0] = Qt[i].first;
			dagnode.s[1] = Qt[i].second;
			dagnode.s[2] = Qt[i].third;
			dagnode.s[3] = Qt[i].fourth;
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

void DivideBaseblock()//���ֻ�����
{
	Pre_process();
	int begin = 0, end = 0;//������Ŀ�ʼ�����
	for (end; end < Qt.size(); end++)
	{
		if (Qt[end].first == "if" || Qt[end].first == "el" || Qt[end].first == "ie" || Qt[end].first == "wh" || Qt[end].first == "do" || Qt[end].first == "we")
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
	for (int i = 0; i < Qt.size(); i++)
	{
		file << Qt[i].first << '\t' << Qt[i].second << '\t' << Qt[i].third << '\t' << Qt[i].fourth << endl;
	}
	file.close();
}