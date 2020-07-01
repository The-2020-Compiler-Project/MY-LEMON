#pragma once
#include<iostream>
#include <fstream>
#include<string>

using namespace std;

FILE *infile;
string word;
SYNBLTerm* SYNBLp;
int  AINFaddr = 0;
int  PFINFaddr = 0;
int OFF = 0;
AINFLTerm* AINFLp = AINFLHead;
PARAMLTerm* PARAMp;
PFINFLTerm* PFINFLp = PFINFLHead;
PFINFLTerm* PFINFLp2 = PFINFLHead;
AINFLTerm*  AINFLp2 = AINFLHead;

struct Token {
	char content[30];
	int type;
	int addr;
	Token * next;
}; Token *Head;
void error() {
	cout << "ERROR!" << endl;
}

void initSYNBL(int i)
{
	SYNBLHead[i] = new SYNBLTerm();
	strcpy_s(SYNBLHead[i]->content, "");
	SYNBLHead[i]->type = " ";
	strcpy_s(SYNBLHead[1]->cat, "");
	SYNBLHead[i]->addr = 0;
	SYNBLHead[i]->active = ' ';
	SYNBLHead[i]->next = NULL;

}

Token *Start(Token *  arrow)
{
	Next(infile);
	arrow = arrow->next;
	arrow = Chengxu(arrow);
	arrow = Houjichengxu(arrow);
	return  arrow;
}

Token *Houjichengxu(Token *  arrow)
{
	if (arrow->type == 45);//判断#,45为“#”在Token表中的顺序号码
	else {
		arrow = Chengxu(arrow); 
		arrow = Houjichengxu(arrow); 

	}
	return arrow;
}

Token *Chengxu(Token *  arrow)
{
	if (arrow->type == 4 || arrow->type == 7)//判断int 和void
	{
		word = arrow->content;//读word准备写入符号总表
		Next(infile);
		arrow = arrow->next;
		if (arrow->type == 17)//判断main
		{
			rangenum = oldrange;
			rangenum++;
			initSYNBL(rangenum);
			SYNBLp = SYNBLHead[rangenum];
			PFINFLp = new PFINFLTerm();
			PFINFLp->content = "main";
			PFINFLp->FN = 0;
			PFINFLp->PARAM = NULL;

			PFINFLp->ENTRY = OFF;

			PFINFLp->next = NULL;
			PFINFLp2->next = PFINFLp;
			PFINFLp2 = PFINFLp2->next;
			OFF = 0;
			
			SEND("hanshu", "_", "_", "main");

			//填写函数表
			Next(infile);
			arrow = arrow->next;
			if (arrow->type == 38)//判断（
			{
				Next(infile);
				arrow = arrow->next;
				if (arrow->type == 39)//判断）
				{

					Next(infile);
					arrow = arrow->next;
					if (arrow->type == 42)//判断{
					{
						Next(infile);
						arrow = arrow->next;
						arrow = fuheyuju(arrow);
						oldrange = rangenum;
						rangenum = 0;
						PFINFLp->OFF = OFF;
						SEND("END", "_", "_", "_");
						return  arrow;
					}
					else error();
				}
				else error();
			}
			else error();

		}
		else {
			if (arrow->type == 0)//判断标识符
			{
				SYNBLp = SYNBLHead[rangenum];
				while (strcmp(SYNBLp->content, arrow->content) != 0)
				{
					SYNBLp = SYNBLp->next;

				}
				if (SYNBLp->next != NULL) { printf("重复定义"); exit(0); }
				SYNBLp->type = word;
				SYNBLp->addr = OFF;
				Next(infile);
				arrow = arrow->next;
				if (arrow->type == 38)//判断（
				{
					funcname = SYNBLp->content;
					SEND("hanshu", "_", "_", SYNBLp->content);
					strcpy_s(SYNBLp->cat, "f");
					rangenum = oldrange;
					rangenum++;
					initSYNBL(rangenum);
					SYNBLp = SYNBLHead[rangenum];
					initPARAML(PARAMp);
					PFINFLp = new PFINFLTerm();
					PFINFLp->content = funcname;
					FN = 0;
					PFINFLp->PARAM = PARAMp;



					PFINFLp->ENTRY = -1;
					PFINFLp->next = NULL;
					PFINFLp2->next = PFINFLp;
					PFINFLp2 = PFINFLp2->next;
					OFF = 0;

					//填写函数表
					Next(infile);
					arrow = arrow->next;
					arrow = canshuliebiao(arrow);
					if (arrow->type == 39)//判断）
					{
						Next(infile);
						arrow = arrow->next;
						if (arrow->type == 42)//判断{
						{
							Next(infile);
							arrow = arrow->next;
							arrow = fuheyuju(arrow);//进入 复合语句 子程序
							oldrange = rangenum;
							rangenum = 0;
							PFINFLp->OFF = OFF;
							PFINFLp->FN = FN;

							SEND("END", "_", "_", "_");
							return  arrow;
						}
						else error();
					}
					else error();
				}
				else {
					if (arrow->type == 40)//判断[
					{
						strcpy_s(SYNBLp->cat, "a");
						SYNBLp->addr = OFF;
						AINFLp = new AINFLTerm();
						AINFLp->CTP = word;
						AINFLp->LOW = 0;
						AINFLp->next = NULL;
						AINFLp2->next = AINFLp;
						AINFLp2 = AINFLp2->next;

						//填写数组表
						Next(infile);
						arrow = arrow->next;
						if (arrow->type == 3)//判断数字
						{
							AINFLp->UP = atoi(arrow->content);//数字写入up
							if (AINFLp->CTP == "int") AINFLp->CLEN = 4 * AINFLp->UP;
							if (AINFLp->CTP == "float") AINFLp->CLEN = 8 * AINFLp->UP;
							if (AINFLp->CTP == "bool" || AINFLp->CTP == "char") AINFLp->CLEN = AINFLp->UP;
							OFF = OFF + AINFLp->CLEN;
							SYNBLp->addr = OFF;

							Next(infile);
							arrow = arrow->next;
							if (arrow->type == 41)//判断]
							{
								Next(infile);
								arrow = arrow->next;
								arrow = shuzufuzhi(arrow);//进入 数组赋值语句 子程序
								arrow = houjishuzufuzhi(arrow);//进入 后继数组赋值语句 子程序

								return arrow;
							}
							else error();

						}
						else error();
					}
					else {
						if (arrow->type == 25)//判断=
						{
							strcpy_s(SYNBLp->cat, "v");
							Next(infile);
							arrow = arrow->next;
							if (arrow->type == 1 || arrow->type == 2)
							{
								Next(infile);
								arrow = arrow->next;
								arrow = houjibianliangshengming(arrow);//进入 后继变量声明 子程序

								return arrow;
							}
							else {
								arrow = suanshubiaodashi(arrow);//进入 算术表达式 子程序
								arrow = houjibianliangshengming(arrow);//进入 后继变量声明 子程序

								return arrow;
							}
						}
						else {
							strcpy_s(SYNBLp->cat, "v");
							arrow = houjibianliangshengming(arrow);//进入 后继变量声明 子程序

							return arrow;
						}

					}//判断=
				}//判断[

			}
			else error();

		}//判断标识符
	}
	else {
		if (arrow->type == 5 || arrow->type == 9 || arrow->type == 11)//判断bool float char
		{
			word = arrow->content;//读word准备写入符号总表
			Next(infile);
			arrow = arrow->next;
			if (arrow->type == 0)//判断标识符
			{
				SYNBLp = SYNBLHead[rangenum];
				while (strcmp(SYNBLp->content, arrow->content) != 0)
				{
					SYNBLp = SYNBLp->next;

				}
				if (SYNBLp->next != NULL)
				{
					printf("重复定义");
					exit(0);
				}
				SYNBLp->type = word;
				Next(infile);
				arrow = arrow->next;
				if (arrow->type == 38)//判断（
				{
					SYNBLp->addr = OFF;
					if (word == "int") OFF = OFF + 4;
					if (word == "float")  OFF = OFF + 8;
					if (word == "bool" || AINFLp->CTP == "char")  OFF = OFF + 1;
					funcname = SYNBLp->content;
					SEND("hanshu", "_", "_", SYNBLp->content);
					strcpy_s(SYNBLp->cat, "f");
					SYNBLp->addr = OFF;
					SYNBLp->next = NULL;
					rangenum = oldrange;
					rangenum++;
					initSYNBL(rangenum);
					SYNBLp = SYNBLHead[rangenum];
					initPARAML(PARAMp);
					PFINFLp = new PFINFLTerm();
					PFINFLp->content = SYNBLp->content;
					FN = 0;
					PFINFLp->PARAM = PARAMp;
					PFINFLp->ENTRY = 0;
					PFINFLp->next = NULL;
					PFINFLp2->next = PFINFLp;
					PFINFLp2 = PFINFLp2->next;
					OFF = 0;

					//填写函数表
					Next(infile);
					arrow = arrow->next;
					arrow = canshuliebiao(arrow);//进入 参数列表 子程序
					if (arrow->type == 39)//判断）
					{
						Next(infile);
						arrow = arrow->next;
						arrow = fuheyuju(arrow);
						oldrange = rangenum;
						rangenum = 0;
						PFINFLp->OFF = OFF;
						SEND("END", "_", "_", "_");
						return  arrow;
					}
					else error();
				}
				else {
					if (arrow->type == 40)//判断[
					{
						AINFLTerm* AINFLp = new AINFLTerm();
						AINFLp->CTP = word;
						AINFLp->LOW = 0;
						AINFLp->next = NULL;
						AINFLp2->next = AINFLp;
						AINFLp2 = AINFLp2->next;
						//填写数组表
						Next(infile);
						arrow = arrow->next;
						if (arrow->type == 3)//判断数字
						{
							AINFLp->UP = atoi(arrow->content);//数字写入up
							if (AINFLp->CTP == "int") AINFLp->CLEN = 4 * AINFLp->UP;
							if (AINFLp->CTP == "float") AINFLp->CLEN = 8 * AINFLp->UP;
							if (AINFLp->CTP == "bool" || AINFLp->CTP == "char") AINFLp->CLEN = AINFLp->UP;
							OFF = OFF + AINFLp->CLEN;
							Next(infile);
							arrow = arrow->next;
							if (arrow->type == 41)//判断]
							{
								Next(infile);
								arrow = arrow->next;
								arrow = shuzufuzhi(arrow);//进入 数组赋值语句 子程序
								arrow = houjishuzufuzhi(arrow);//进入 后继数组赋值语句 子程序

								return arrow;
							}
							else error();

						}
						else error();
					}//判断[
					else {
						if (arrow->type == 25)//判断=
						{
							strcpy_s(SYNBLp->cat, "v");
							SYNBLp->addr = OFF;
							if (word == "int") OFF = OFF + 4;
							if (word == "float")  OFF = OFF + 8;
							if (word == "bool" || AINFLp->CTP == "char")  OFF = OFF + 1;
							SYNBLp->addr = OFF;                       //地址指向vall
							Next(infile);
							arrow = arrow->next;
							if (arrow->type == 1 || arrow->type == 2)//判断字符，字符串（即常量）
							{
								Next(infile);
								arrow = arrow->next;
								arrow = houjibianliangshengming(arrow);//进入 后继变量声明 子程序

								return arrow;
							}
							else {
								arrow = suanshubiaodashi(arrow);//进入 算术表达式 子程序
								arrow = houjibianliangshengming(arrow);//进入 后继变量声明 子程序

								return arrow;
							}
						}
						else {
							strcpy_s(SYNBLp->cat, "v");
							SYNBLp->addr = OFF;
							if (word == "int") OFF = OFF + 4;
							if (word == "float")  OFF = OFF + 8;
							if (word == "bool" || word == "char")  OFF = OFF + 1;
							SYNBLp->addr = OFF;            //地址指向vall

							arrow = houjibianliangshengming(arrow);//进入 后继变量声明 子程序

							return arrow;
						}

					}


				}
			}
			else error();
		}
		else error();
	}
}

Token *fuheyuju(Token *  arrow) //复合语句
{
	if (arrow->type == 42)//判断{
	{
		Next(infile);
		arrow = arrow->next;
	}
	else error();
	arrow = yujuliebiao(arrow); //进入 语句列表 子程序
	if (arrow->type == 43)//判断}
	{
		Next(infile);
		arrow = arrow->next;
		return arrow;
	}
	else error();
}