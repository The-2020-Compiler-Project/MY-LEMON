#include<string>
#include<sstream>
#include<vector>

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
vector<QtTerm>Qt;
int t_value;

void SEND(string first, string second, string third, string fourth)
{
	QtTerm qt;
	qt.first = first;
	qt.second = second;
	qt.third = third;
	qt.fourth = fourth;
	Qt.push_back(qt);
}
void IF()
{
	string SEM_top;
	SEM_top = SEM.top();
	SEM.pop();
	SEND("if", SEM_top, "_", "_");
}
void ELlSE()
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
void GEQ(string oper)//双目运算符语义动作
{
	stringstream ss;
	string t_num, t;
	ss << t_value;
	ss >> t_num;//临时变量标号
	t = "-t" + t_num;//临时变量
	string SEM_top, SEM_top1;
	SEM_top = SEM.top();
	SEM.pop();
	SEM_top1 = SEM.top();
	SEM.pop();
	SEM.push(t);
	SEND(oper, SEM_top1, SEM_top, t);
	t_value++;
}
void GEQSingle(string oper)//单目运算符
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
void ASSI(string s)//赋值语义动作
{
	string SEM_top;
	SEM_top = SEM.top();
	SEM.pop();
	SEND("=", SEM_top, "_", s);
}
