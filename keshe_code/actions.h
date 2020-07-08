#include <iostream>
#include <fstream>
#include <stdio.h>
#include<vector>
#include <stdlib.h>
#include <string.h>
#include 'DataStructure.h'
#include <stack>

stack<string> SEM;

struct Term
{
    char content[30];
    int type;
    int addr;
    Term* next;
}; Term* Head;

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



void error()
{
    printf("error");
    exit(0);
}
void SEND(string first, string second, string third, string fourth)
{
    QtTerm temQt;
    temQt.first = first;
    temQt.second = second;
    temQt.third = third;
    temQt.fourth = fourth;
    Qt.push_back(temQt);
}
void IF()
{
    string tM;
    tM = SEM.top();
    SEM.pop();
    SEND("if", tM, " ", " ");
}
void ELSE()
{
    SEND("el", " ", " ", " ");
}
void IFEND()
{
    SEND("ie", " ", " ", " ");
}
void WHILE()
{
    SEND("wh", " ", " ", " ");
}
void DO()
{
    string tM;
    tM = SEM.top();
    SEM.pop();
    SEND("do", tM, " ", " ");
}
void WHILEEND()
{
    SEND("we", " ", " ", " ");
}
void GEQ(string oper)
{
    stringstream ss;
    string numt, Numt = "t";
    ss << t_value;
    ss >> numt;
    Numt = Numt + numt;
    string tM, tM1;
    tM = SEM.top();
    SEM.pop();
    tM1 = SEM.top();
    SEM.pop();
    SEM.push(Numt);
    SEND(oper, tM1, tM, Numt);
    t_value++;
}
void GEQSingle(string oper)
{
    stringstream ss;
    string numt, Numt = "t";
    ss << t_value;
    ss >> numt;
    Numt = Numt + numt;
    string tM;
    tM = SEM.top();
    SEM.pop();
    SEM.push(Numt);
    SEND(oper, tM, " ", Numt);
    t_value++;
}