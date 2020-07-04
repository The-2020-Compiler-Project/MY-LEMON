#pragma once
#include <iostream>
#include <fstream>
#include <stdio.h>
#include<vector>
#include <stdlib.h>
#include <string.h>
#include <iomanip>
#include "fuhaobiao.h"
#define IDENTIFER -99
using namespace std;
int static_iden = 0;

const char* KT[] = { "void","char","short","int","long","float","double","bool","if","else","while","do","for","main","return" }; //�ؼ��ֱ�
const char* PT[] = { ">=","<=","==","!=",">","<","=","&&","||","!","+","-","*","/","%","<<",">>",",",";","(",")","[","]","{","}",".","#" };//�����


struct Term
{
    char name[20];
    int type;
    int addr;
    Term* next;
}; 
Term* Head;  //token��


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
};            //��Ԫʽ

vector<QtTerm>Qt;
int t_value;

void initTerm()
{
    Head = new Term();
    strcpy(Head->name, " ");
    Head->type = -1;
    Head->addr = -1;
    Head->next = NULL;


    SymL_Head[0] = new Symbol_List();
    strcpy(SymL_Head[0]->name, " ");
    SymL_Head[0]->type = " ";
    strcpy(SymL_Head[0]->cat, " ");
    SymL_Head[0]->addr = 0;
    SymL_Head[0]->active = ' ';
    SymL_Head[0]->next = NULL;



    ArrL_Head = new Array_List();
    ArrL_Head->low = 0;
    ArrL_Head->up = 0;
    ArrL_Head->tp = " ";
    ArrL_Head->len = 0;
    ArrL_Head->next = NULL;
 

    FuncL_Head = new Function_List();
    FuncL_Head->name = " ";
    FuncL_Head->level = 0;
    FuncL_Head->off = 0;
    FuncL_Head->fn = 0;
    FuncL_Head->param = NULL;
    FuncL_Head->entry = 0;
    FuncL_Head->next = NULL;

}
void initSYNBL(int i)
{
    SymL_Head[i] = new Symbol_List();
    strcpy(SymL_Head[i]->name, " ");
    SymL_Head[i]->type = " ";
    strcpy(SymL_Head[i]->cat, " ");
    SymL_Head[i]->addr = 0;
    SymL_Head[i]->active = ' ';
    SymL_Head[i]->next = NULL;


}

void initPARAML(Parameter_List* ParaL_Head)
{
    ParaL_Head = new Parameter_List();
    strcpy(SymL_Head[rangenum]->name, " ");
    SymL_Head[rangenum]->type = " ";
    strcpy(SymL_Head[rangenum]->cat, " ");
    SymL_Head[rangenum]->addr = 0;
    SymL_Head[rangenum]->active = ' ';
    SymL_Head[rangenum]->next = NULL;

}

int seekKT(char* word)   //��֤�Ƿ�Ϊ�ؼ���
{
    for (int i = 0; i < 15; i++)
    {
        if (strcmp(word, KT[i]) == 0)
        {
            return i + 4;
        }
    }
    return -99;
}


int seekPT(char* word)
{
    for (int i = 0; i < 27; i++)
    {
        if (strcmp(word, PT[i]) == 0)
        {
            return i + 19;
        }
    }
    return -99;
}


void createNewTerm(char* name, int type, int addr) //�����token����
{
    Term* p = Head;
    Term* temp = new Term();

    while (p->next != NULL)
    {
        p = p->next;
    }

    strcpy(temp->name, name);
    temp->type = type;
    temp->addr = addr;
    temp->next = NULL;
    p->next = temp;
}
int createSyml_Term(char* name, int type, int addr)
{
    //  Symbol_List* p = Symbol_Head[0];
    Symbol_List* p = SymL_Head[rangenum];
    int flag = 0;
    int i = 0;
    int addr_temp = -2;
    while (p->next != NULL)
    {
        i++;
        if (strcmp(name, p->next->name) == 0)
        {
            flag = 1;      //����֮ǰ�Ƿ����ظ�����

        }
        p = p->next;
    }
    if (flag == 1)
    {
        Term* a = Head;
        while (strcmp(a->name, name) != 0)
        {
            a = a->next;
        }
        addr_temp = a->addr;   //�����ű������У����token��ͳһ��ַ

    }


    if (flag == 0)  
    {
        Symbol_List* temp = new Symbol_List();
        addr_temp = ++static_iden;   //�½�ʱ��ַ��ֵΪ�½����ű�������
        strcpy(temp->name, name);
        temp->type = " ";
        strcpy(temp->cat, " ");
        temp->addr = 0;
        temp->next = NULL;
        p->next = temp;    //��û�У����½���������ű�
    }

    return addr_temp;
}
void Next(FILE* infile)
{
    char array[20];
    char ch;
    char* word;
    int i = 0;
    do
    {
        ch = fgetc(infile);
    } while (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t');  //����ȡ���ո� ���е�ת���ʱ����һ��

    if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || ch == '_')//��ĸ���»��߿�ͷ��Ϊ��ʶ����ؼ���
    {
        while ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9') || ch == '_')
        {
            array[i++] = ch;
            ch = fgetc(infile);
        }   //����⵽����ĸ���»��߿�ʼ��������ĸ���Ž�array��(�û�����ı�ʶ����
        word = new char[i + 1];
        memcpy(word, array, i);
        word[i] = '\0';    //����ʶ���в����ս��
        int seekTemp = seekKT(word); //����Ƿ�Ϊ�ؼ���
        if (seekTemp != -99)
        {
            createNewTerm(word, seekTemp, -1);   //������token����
        }
        else
        {
            int addr_tmp = createSyml_Term(word, seekTemp, -1);//����ʶ��������ű����ص�ַ
            createNewTerm(word, 0, addr_tmp);    //���ǹؼ��֣��Ǿ�˵���Ǳ�ʶ��������Ϊ0������token����
        }
        fseek(infile, -1L, SEEK_CUR); //ָ�����һ���ֽڣ��˻���������ַ�֮ǰ����������ַ���δ�жϣ�
    }

    else if (ch >= '0' && ch <= '9') //���ֿ�ͷ��Ϊ��������
    {
        int decimal = 0;//С�����
        int sci_notation = 0;//��ѧ���������
        while (ch >= '0' && ch <= '9')
        {
            array[i++] = ch;
            ch = fgetc(infile);
        } //����Ϊ����(��С�����������ֻ��ѧ��������Ч���ֲ��֣�
        if (ch == '.')
        {
            decimal = 1;
            array[i++] = ch;//С�������
            ch = fgetc(infile);
            if (ch >= '0' && ch <= '9')
            {
                while (ch >= '0' && ch <= '9')
                {
                    array[i++] = ch;
                    ch = fgetc(infile);
                }//��������С������
            }
        }
        if (ch == 'e')
        {
            sci_notation = 1;
            array[i++] = ch;
            ch = fgetc(infile);
            if (ch == '+' || ch == '-')
            {
                array[i++] = ch;
                ch = fgetc(infile);
            }//��鲢����������
            if (ch >= '0' && ch <= '9')
            {
                while (ch >= '0' && ch <= '9')
                {
                    array[i++] = ch;
                    ch = fgetc(infile);
                }

            }//����ָ������
        }

        word = new char[i + 1];
        memcpy(word, array, i);
        word[i] = '\0';
        createNewTerm(word, 3, -1);
        fseek(infile, -1L, SEEK_CUR);//������һλ
    }

    else if (ch == '\'')//��⵽������

    {
        array[i++] = ch;
        ch = fgetc(infile);
        if (ch == '\\')//��⵽б�ܱ�ʾ�������ת���ַ�
        {
            array[i++] = ch;
            ch = fgetc(infile);
            if (ch == 'a' || ch == 'b' || ch == 'f' || ch == 'a' || ch == 'n' || ch == 'r' || ch == 't' || ch == 'v' || ch == '\\' || ch == '\'' || ch == '\"' || ch == '0')
                array[i++] = ch;
            else
            {
                cout << "ת���������" << endl;
                exit(-1);
            }
            ch = fgetc(infile);
        }
        else {
            array[i++] = ch;
            ch = fgetc(infile); //����ת���ַ���Ϊ�ַ���
        }
        if (ch == '\'')
        {
            array[i++] = ch;
            ch = fgetc(infile);
        }   //�ַ����ս�
        else {
            cout << "token����" << endl;
            exit(0);
        }//�쳣����
        word = new char[i + 1];
        memcpy(word, array, i);
        word[i] = '\0';

        createNewTerm(word, 1, -1);
        fseek(infile, -1L, SEEK_CUR);//������һλ
    }

    else if (ch == '"') //��⵽˫���� Ϊ�ַ���
    {
        array[i++] = ch;
        ch = fgetc(infile);
        while (ch != '"')
        {
            array[i++] = ch;
            ch = fgetc(infile);
        }//ѭ�������ַ�������
        array[i++] = ch;//��������˫����
        ch = fgetc(infile); 
        word = new char[i + 1];
        memcpy(word, array, i);
        word[i] = '\0';

        createNewTerm(word, 2, -1);
        fseek(infile, -1L, SEEK_CUR);//������һλ
    }

    //���
    else  if ((ch >= ';' && ch <= '>') || (ch >= '(' && ch <= '/') || (ch >= '{' && ch <= '}') || ch == '%' || ch == '[' || ch == ']' || ch == '&' || ch == '#' || ch == '!')
    {
        array[i++] = ch;
        
         if (ch == '<')
        {
            ch = fgetc(infile);
            if ((ch == '=')||(ch=='<'))
            {
                array[i++] = ch;
                ch = fgetc(infile);
            }
        }
        else if (ch == '>')
        {
            ch = fgetc(infile);
            if (ch == '>'||ch == '=')
            {
                array[i++] = ch;
                ch = fgetc(infile);
            }
        }
        else if (ch == '!' || ch == '=')
         {
             ch = fgetc(infile);
             if (ch == '=')
             {
                 array[i++] = ch;
                 ch = fgetc(infile);
             }
         }
        else if (ch == '&')
        {
            ch = fgetc(infile);
            if (ch == '&')
            {
                array[i++] = ch;
                ch = fgetc(infile);
            }
        }
        else if (ch == '|')
        {
            ch = fgetc(infile);
            if (ch == '|')
            {
                array[i++] = ch;
                ch = fgetc(infile);
            }
        }
        else    ch = fgetc(infile);
        word = new char[i + 1];
        memcpy(word, array, i);
        word[i] = '\0';
        int seekTemp = seekPT(word);  //�ó����ͱ��
        createNewTerm(word, seekTemp, -1);
        fseek(infile, -1L, SEEK_CUR); //����һ��
    }
    else {
        cout << "token����" << endl;
        exit(0);
    }


}


void printtoken()
{

    Term* p = Head;
    p = p->next;
    ofstream file("token.txt");
    file << "token��" << endl ;
    file << " " << "����" <<" " << "�ֱ���" <<" " << "��ʶ����ַ"  << endl;
    while (p != NULL)
    {

       file << " " << p->name << " " << p->type << " " << p->addr  << endl;
       p = p->next;
    }
    file << endl;
    file.close();
}
