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

const char* KT[] = { "void","char","short","int","long","float","double","bool","if","else","while","do","for","main","return" }; //关键字表
const char* PT[] = { ">=","<=","==","!=",">","<","=","&&","||","!","+","-","*","/","%","<<",">>",",",";","(",")","[","]","{","}",".","#" };//界符表


struct Term
{
    char name[20];
    int type;
    int addr;
    Term* next;
}; 
Term* Head;  //token表


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
};            //四元式

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

int seekKT(char* word)   //验证是否为关键字
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


void createNewTerm(char* name, int type, int addr) //添加新token表项
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
            flag = 1;      //检验之前是否有重复表项

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
        addr_temp = a->addr;   //若符号表中已有，则查token表并统一地址

    }


    if (flag == 0)  
    {
        Symbol_List* temp = new Symbol_List();
        addr_temp = ++static_iden;   //新建时地址赋值为新建符号表项数量
        strcpy(temp->name, name);
        temp->type = " ";
        strcpy(temp->cat, " ");
        temp->addr = 0;
        temp->next = NULL;
        p->next = temp;    //若没有，则新建并加入符号表
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
    } while (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t');  //当读取到空格 换行等转义符时读下一个

    if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || ch == '_')//字母或下划线开头则为标识符或关键字
    {
        while ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9') || ch == '_')
        {
            array[i++] = ch;
            ch = fgetc(infile);
        }   //将检测到的字母或下划线开始的数字字母串放进array中(用户定义的标识符）
        word = new char[i + 1];
        memcpy(word, array, i);
        word[i] = '\0';    //将标识符中插入终结符
        int seekTemp = seekKT(word); //检查是否为关键字
        if (seekTemp != -99)
        {
            createNewTerm(word, seekTemp, -1);   //是则建立token表项
        }
        else
        {
            int addr_tmp = createSyml_Term(word, seekTemp, -1);//将标识符加入符号表并返回地址
            createNewTerm(word, 0, addr_tmp);    //不是关键字（那就说明是标识符，类型为0）创建token表项
        }
        fseek(infile, -1L, SEEK_CUR); //指针后退一个字节，退回最近读的字符之前（最近读的字符还未判断）
    }

    else if (ch >= '0' && ch <= '9') //数字开头则为各种数字
    {
        int decimal = 0;//小数标记
        int sci_notation = 0;//科学计数法标记
        while (ch >= '0' && ch <= '9')
        {
            array[i++] = ch;
            ch = fgetc(infile);
        } //假设为整数(或小数的整数部分或科学计数法有效数字部分）
        if (ch == '.')
        {
            decimal = 1;
            array[i++] = ch;//小数点加入
            ch = fgetc(infile);
            if (ch >= '0' && ch <= '9')
            {
                while (ch >= '0' && ch <= '9')
                {
                    array[i++] = ch;
                    ch = fgetc(infile);
                }//加入后面的小数部分
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
            }//检查并加入正负号
            if (ch >= '0' && ch <= '9')
            {
                while (ch >= '0' && ch <= '9')
                {
                    array[i++] = ch;
                    ch = fgetc(infile);
                }

            }//加入指数部分
        }

        word = new char[i + 1];
        memcpy(word, array, i);
        word[i] = '\0';
        createNewTerm(word, 3, -1);
        fseek(infile, -1L, SEEK_CUR);//向后回退一位
    }

    else if (ch == '\'')//检测到单引号

    {
        array[i++] = ch;
        ch = fgetc(infile);
        if (ch == '\\')//检测到斜杠表示后面的是转义字符
        {
            array[i++] = ch;
            ch = fgetc(infile);
            if (ch == 'a' || ch == 'b' || ch == 'f' || ch == 'a' || ch == 'n' || ch == 'r' || ch == 't' || ch == 'v' || ch == '\\' || ch == '\'' || ch == '\"' || ch == '0')
                array[i++] = ch;
            else
            {
                cout << "转义符不存在" << endl;
                exit(-1);
            }
            ch = fgetc(infile);
        }
        else {
            array[i++] = ch;
            ch = fgetc(infile); //不是转义字符则为字符串
        }
        if (ch == '\'')
        {
            array[i++] = ch;
            ch = fgetc(infile);
        }   //字符串终结
        else {
            cout << "token出错" << endl;
            exit(0);
        }//异常处理
        word = new char[i + 1];
        memcpy(word, array, i);
        word[i] = '\0';

        createNewTerm(word, 1, -1);
        fseek(infile, -1L, SEEK_CUR);//向后回退一位
    }

    else if (ch == '"') //检测到双引号 为字符串
    {
        array[i++] = ch;
        ch = fgetc(infile);
        while (ch != '"')
        {
            array[i++] = ch;
            ch = fgetc(infile);
        }//循环输入字符串内容
        array[i++] = ch;//输入最后的双引号
        ch = fgetc(infile); 
        word = new char[i + 1];
        memcpy(word, array, i);
        word[i] = '\0';

        createNewTerm(word, 2, -1);
        fseek(infile, -1L, SEEK_CUR);//向后回退一位
    }

    //界符
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
        int seekTemp = seekPT(word);  //得出类型编号
        createNewTerm(word, seekTemp, -1);
        fseek(infile, -1L, SEEK_CUR); //回退一格
    }
    else {
        cout << "token出错" << endl;
        exit(0);
    }


}


void printtoken()
{

    Term* p = Head;
    p = p->next;
    ofstream file("token.txt");
    file << "token表" << endl ;
    file << " " << "内容" <<" " << "种别码" <<" " << "标识符地址"  << endl;
    while (p != NULL)
    {

       file << " " << p->name << " " << p->type << " " << p->addr  << endl;
       p = p->next;
    }
    file << endl;
    file.close();
}
