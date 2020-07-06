#include <iostream>
#include <fstream>
#include <stack>
#include<string>
#include<sstream>
#include 'Scanner.h'

int rangenum = 0;
int oldrange = 0;

FILE* infile;
string word;
SYNBLTerm* SYNBLp;
int  AINFaddr = 0;
int  PFINFaddr = 0;
int OFF = 0;
AINFLTerm* AINFLp = AINFLHead;
PARAMLTerm* PARAMp;
PFINFLTerm* PFINFLp = PFINFLHead;
PFINFLTerm* PFINFLp2 = PFINFLHead;
AINFLTerm* AINFLp2 = AINFLHead;

struct Term
{
    char content[30];
    int type;
    int addr;
    Term* next;
}; Term* Head;

string logger(string x)
{
    return x;
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

void SEND(string first, string second, string third, string fourth)
{
    QtTerm temQt;
    temQt.first = first;
    temQt.second = second;
    temQt.third = third;
    temQt.fourth = fourth;
    Qt.push_back(temQt);
}

void Next(FILE* infile);

Term* putongbianliangshengming(Term* arrow)//��ͨ��������
{
    if (arrow->type == 0)//�жϱ�ʶ��
    {
        SymL_p = SymL_Head[rangenum];
        while (strcmp(SymL_p->content, arrow->content) != 0) { SymL_p = SymL_p->next; }
        if (SymL_p->next != NULL) { logger("�ظ�����"); exit(0); }
        SymL_p->type = type;
        Next(infile);
        arrow = arrow->next;
        if (strcmp(arrow->content, "=") == 0)
        {
            Next(infile);
            arrow = arrow->next;
            if ((arrow->type == 1) || (arrow->type == 2))//�ж��Ƿ��ǳ���(�ַ���������)
            {
                Next(infile);
                arrow = arrow->next;
            }
            else
            {
                arrow = suanshubiaodashi(arrow);//�������ʽ
            }
        }
        else {

        }
    }
    else
    {
        logger("err");
    }
    strcpy_s(SymL_p->cat, "v");
    SymL_p->addr = OFF;
    if (type == "int") OFF = OFF + 4;
    if (type == "float")  OFF = OFF + 8;
    if (type == "bool" || type == "char")  OFF = OFF + 1;
    // SYNBLp->addr=OFF;                                               //��ַָ��vall
    return arrow;
}

Term* houjibianliangshengming(Term* arrow)//��̱�������
{
    if (strcmp(arrow->content, ";") == 0)
    {
        Next(infile);
        arrow = arrow->next;
    }
    else if (strcmp(arrow->content, ",") == 0)
    {
        Next(infile);
        arrow = arrow->next;
        arrow = putongbianliangshengming(arrow);//��ͨ��������
        arrow = houjibianliangshengming(arrow);//��̱�������
    }
    else
    {
        logger("err");
    }
    return arrow;
}

Term* canshuliebiao(Term* arrow)//�����б�
{
    if ((strcmp(arrow->content, "int") == 0) || (strcmp(arrow->content, "char") == 0) || (strcmp(arrow->content, "float") == 0) || (strcmp(arrow->content, "bool") == 0))
    {
        FN++;
        arrow = canshushengming(arrow);//��������
        arrow = houjicanshushengming(arrow);//��̲�������
    }
    return arrow;
}

Term* canshushengming(Term* arrow)//��������
{
    if ((strcmp(arrow->content, "int") == 0) || (strcmp(arrow->content, "char") == 0) || (strcmp(arrow->content, "float") == 0) || (strcmp(arrow->content, "bool") == 0))
    {
        type = arrow->content;//��type׼��д������ܱ�

        Next(infile);
        arrow = arrow->next;
        Parameter_List* PARAMp2 = new Parameter_List();
        strcpy_s(PARAMp2->content, arrow->content);
        if (arrow->type == 0)//�ж��Ƿ��Ǳ�ʶ��
        {
            SEND("canshu", "_", "_", arrow->content);
            SymL_p = SymL_Head[rangenum];

            while (strcmp(SymL_p->content, arrow->content) != 0) { SymL_p = SymL_p->next; }
            if (SymL_p->next != NULL) { logger("�ظ�����"); exit(0); }
            SymL_p->type = type;
            SymL_p->addr = OFF;
            PARAMp2->type = type;
            if (type == "int")OFF = OFF + 4;
            if (type == "float")OFF = OFF + 8;
            if (type == "bool" || type == "char")OFF = OFF + 1;
            Next(infile);
            arrow = arrow->next;
            if (strcmp(arrow->content, "=") == 0)
            {
                strcpy_s(SymL_p->cat, "p");

                strcpy_s(PARAMp2->cat, "p");
                SymL_p->addr = OFF;
                //��ַָ��vall
                PARAMp2->next - NULL;
                Para_p->next = PARAMp2;
                Para_p = Para_p->next;
                Next(infile);
                arrow = arrow->next;
                if ((arrow->type == 1) || (arrow->type == 2))//�ж��Ƿ��ǳ���(�ַ���������)
                {
                    Next(infile);
                    arrow = arrow->next;
                }
                else
                {
                    strcpy_s(SymL_p->cat, "p");

                    //��ַָ��vall
                    arrow = suanshubiaodashi(arrow);//�������ʽ
                }
            }
            else { strcpy_s(SymL_p->cat, "p"); }
        }
        else
        {
            logger("err");
        }
    }
    else
    {
        logger("err");
    }
    return arrow;
}

Term* houjicanshushengming(Term* arrow)//��̲�������
{
    if (strcmp(arrow->content, ",") == 0)
    {
        Next(infile);
        arrow = arrow->next;
        FN++;
        arrow = canshushengming(arrow);//��������
        arrow = houjicanshushengming(arrow);//��̲�������
    }
    return arrow;
}


Term* luojiyuju(Term* arrow)//�߼����
{
    arrow = luojixiang(arrow);
    while (1)
    {
        if (strcmp(arrow->content, "||") == 0)
        {
            Next(infile);
            arrow = arrow->next;
            arrow = luojixiang(arrow);
            GEQ("||");
        }
        else
        {
            break;
        }
    }
    return arrow;
}

Term* luojixiang(Term* arrow)//�߼���
{
    arrow = luojiyinzi1(arrow);
    while (1)
    {
        if (strcmp(arrow->content, "&&") == 0)
        {
            Next(infile);
            arrow = arrow->next;
            arrow = luojixiang(arrow);
            GEQ("&&");
        }
        else
        {
            break;
        }
    }
    return arrow;
}

Term* luojiyinzi1(Term* arrow)//�߼�����1
{
    arrow = luojiyinzi2(arrow);
    while (1)
    {
        if (strcmp(arrow->content, "==") == 0)
        {
            Next(infile);
            arrow = arrow->next;
            arrow = luojiyinzi2(arrow);
            GEQ("==");
        }
        else if (strcmp(arrow->content, "!=") == 0)
        {
            Next(infile);
            arrow = arrow->next;
            arrow = luojiyinzi2(arrow);
            GEQ("!=");
        }
        else
        {
            break;
        }
    }
    return arrow;
}

Term* luojiyinzi2(Term* arrow)//�߼�����2
{
    arrow = suanshubiaodashi(arrow);
    while (1)
    {
        if (strcmp(arrow->content, ">=") == 0)
        {
            Next(infile);
            arrow = arrow->next;
            arrow = suanshubiaodashi(arrow);//�������ʽ
            GEQ(">=");
        }
        else if (strcmp(arrow->content, ">") == 0)
        {
            Next(infile);
            arrow = arrow->next;
            arrow = suanshubiaodashi(arrow);
            GEQ(">");
        }
        else if (strcmp(arrow->content, "<=") == 0)
        {
            Next(infile);
            arrow = arrow->next;
            arrow = suanshubiaodashi(arrow);
            GEQ("<=");
        }
        else if (strcmp(arrow->content, "<") == 0)
        {
            Next(infile);
            arrow = arrow->next;
            arrow = suanshubiaodashi(arrow);
            GEQ("<");
        }
        else
        {
            break;
        }
    }
    return arrow;
}
