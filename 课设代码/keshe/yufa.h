#pragma once
#pragma once
#include<iostream>
#include <fstream>
#include<string>
#include<stack>
#include<sstream>
#include "Symbol_table.h"
#include"main_structure.h"
#include"yuyi.h"
#include"cifa.h"


using namespace std;

string funcstring;
string funcname;

int oldrange = 0;
int ARR[5] = { 0 };
int g = 0;
int up;
int fn = 0;


string logger(string x)
{
	pass = 1;
	cout << x << endl;
	exit(-1);
}


Term* Start(Term* arrow);
Term* Houjichengxu(Term* arrow);
Term* Chengxu(Term* arrow);
Term* fuheyuju(Term* arrow);
Term* shuzufuzhi(Term* arrow);
Term* zhiliebiao(Term* arrow);
Term* suanshubiaodashi(Term* arrow);
Term* suanshuxiang(Term* arrow);
Term* suanshuyinzi(Term* arrow);
Term* suanshudanyuan(Term* arrow);
Term* yujuliebiao(Term* arrow);
Term* putongbianliangshengming(Term* arrow);
Term* houjibianliangshengming(Term* arrow);
Term* canshuliebiao(Term* arrow);
Term* canshushengming(Term* arrow);
Term* houjicanshushengming(Term* arrow);
Term* luojiyuju(Term* arrow);
Term* luojixiang(Term* arrow);
Term* luojiyinzi1(Term* arrow);
Term* luojiyinzi2(Term* arrow);
Term* houjishuzufuzhi(Term* arrow);
Term* shuru(Term* arrow);
Term* shuchu(Term* arrow);


Term* Start(Term* arrow)
{
	Next(infile);
	arrow = arrow->next;
	arrow = Chengxu(arrow);//进入 程序 子程序
	arrow = Houjichengxu(arrow);//进入 后继程序 子程序
	return  arrow;
}

Term* Houjichengxu(Term* arrow)
{
	if (arrow->type == 47);//判断#,45为“#”在Token表中的顺序号码
	else {
		arrow = Chengxu(arrow); //进入 程序 子程序
		arrow = Houjichengxu(arrow); //进入 后继程序 子程序

	}
	return arrow;
}

Term* Chengxu(Term* arrow)
{
	if (arrow->type == 4 || arrow->type == 7)//判断int 和void
	{
		type = arrow->name;//读word准备写入符号总表
		Next(infile);
		arrow = arrow->next;
		if (arrow->type == 17)//判断main
		{
			rangenum = oldrange;
			rangenum++;
			InitSymbl(rangenum);
			SymL_p = SymL_Head[rangenum];
			FuncL_p = new Function_List();
			FuncL_p->name = "main";
			FuncL_p->fn = 0;
			FuncL_p->param = NULL;

			FuncL_p->entry = off;

			FuncL_p->next = NULL;
			FuncL_p2->next = FuncL_p;
			FuncL_p2 = FuncL_p2->next;
			off = 0;
			SEND("hanshu", "_", "_", "main");
			//填写函数表

			Next(infile);
			arrow = arrow->next;
			if (arrow->type == 40)//判断（
			{
				Next(infile);
				arrow = arrow->next;
				if (arrow->type == 41)//判断）
				{

					Next(infile);
					arrow = arrow->next;
					//if (arrow->type == 42)//判断{
					//{
					//	Next(infile);
					//	arrow = arrow->next;
					arrow = fuheyuju(arrow);//进入 复合语句 子程序
					oldrange = rangenum;
					rangenum = 0;
					FuncL_p->off = off;
					SEND("END", "_", "_", "_");
					return  arrow;
					//}
					//else logger("err");
				}
				else logger("err");
			}
			else logger("err");

		}
		else if (arrow->type == 0)//判断标识符
		{
			SymL_p = SymL_Head[rangenum];
			while (strcmp(SymL_p->name, arrow->name) != 0)
			{
				SymL_p = SymL_p->next;
			}
			if (SymL_p->next != NULL) { printf("重复定义"); exit(0); }
			SymL_p->type = type;
			SymL_p->addr = off;

			Next(infile);
			arrow = arrow->next;
			if (arrow->type == 40)//判断（
			{
				funcname = SymL_p->name;
				SEND("hanshu", "_", "_", SymL_p->name);
				strcpy_s(SymL_p->cat, "f");
				rangenum = oldrange;
				rangenum++;
				InitSymbl(rangenum);
				SymL_p = SymL_Head[rangenum];
				initPARAML(Para_p);
				FuncL_p = new Function_List();
				FuncL_p->name = funcname;
				fn = 0;
				FuncL_p->param = Para_p;



				FuncL_p->entry = -1;
				FuncL_p->next = NULL;
				FuncL_p2->next = FuncL_p;
				FuncL_p2 = FuncL_p2->next;
				off = 0;

				//填写函数表
				Next(infile);
				arrow = arrow->next;
				arrow = canshuliebiao(arrow);//进入 参数列表 子程序
				if (arrow->type == 41)//判断）
				{
					Next(infile);
					arrow = arrow->next;
					//if (arrow->type == 42)//判断{
					//{
					//	Next(infile);
					//	arrow = arrow->next;
					arrow = fuheyuju(arrow);//进入 复合语句 子程序
					oldrange = rangenum;
					rangenum = 0;
					FuncL_p->off = off;
					FuncL_p->fn = fn;

					SEND("END", "_", "_", "_");
					return  arrow;
					/*}
					else logger("err");*/
				}
				else logger("err");
			}
			else {
				if (arrow->type == 42)//判断[
				{
					strcpy_s(SymL_p->cat, "a");
					//SYNBLp->addr=++AINFaddr;
					SymL_p->addr = off;
					AINFLp = new Array_List();
					AINFLp->tp = type;
					AINFLp->low = 0;
					AINFLp->next = NULL;
					ArrL_p2->next = AINFLp;
					ArrL_p2 = ArrL_p2->next;

					//填写数组表
					Next(infile);
					arrow = arrow->next;
					if (arrow->type == 3)//判断数字
					{
						AINFLp->up = atoi(arrow->name);//数字写入up
						if (AINFLp->tp == "int") AINFLp->len = 4 * AINFLp->up;
						if (AINFLp->tp == "float") AINFLp->len = 8 * AINFLp->up;
						if (AINFLp->tp == "bool" || AINFLp->tp == "char") AINFLp->len = AINFLp->up;
						off = off + AINFLp->len;
						SymL_p->addr = off;

						Next(infile);
						arrow = arrow->next;
						if (arrow->type == 43)//判断]
						{
							Next(infile);
							arrow = arrow->next;
							arrow = shuzufuzhi(arrow);//进入 数组赋值语句 子程序
							arrow = houjishuzufuzhi(arrow);//进入 后继数组赋值语句 子程序

							return arrow;
						}
						else logger("err");

					}
					else logger("err");
				}
				else {
					if (arrow->type == 27)//判断=
					{
						strcpy_s(SymL_p->cat, "v");
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
						strcpy_s(SymL_p->cat, "v");
						arrow = houjibianliangshengming(arrow);//进入 后继变量声明 子程序

						return arrow;
					}

				}//判断=
			}//判断[

		}//判断标识符
		else logger("err");
	}
	else {
		if (arrow->type == 5 || arrow->type == 9 || arrow->type == 11)//判断bool float char
		{
			type = arrow->name;//读word准备写入符号总表
			Next(infile);
			arrow = arrow->next;
			if (arrow->type == 0)//判断标识符
			{
				SymL_p = SymL_Head[rangenum];
				while (strcmp(SymL_p->name, arrow->name) != 0)
				{
					SymL_p = SymL_p->next;

				}
				if (SymL_p->next != NULL)
				{
					printf("重复定义");
					exit(0);
				}
				SymL_p->type = type;
				Next(infile);
				arrow = arrow->next;
				if (arrow->type == 40)//判断（
				{
					SymL_p->addr = off;
					if (type == "int") off = off + 4;
					if (type == "float")  off = off + 8;
					if (type == "bool" || AINFLp->tp == "char")  off = off + 1;
					funcname = SymL_p->name;
					SEND("hanshu", "_", "_", SymL_p->name);
					strcpy_s(SymL_p->cat, "f");
					SymL_p->addr = off;
					//SYNBLp->addr=++PFINFaddr;
					SymL_p->next = NULL;
					rangenum = oldrange;
					rangenum++;
					InitSymbl(rangenum);
					SymL_p = SymL_Head[rangenum];
					initPARAML(Para_p);
					FuncL_p = new Function_List();
					FuncL_p->name = SymL_p->name;
					fn = 0;
					FuncL_p->param = Para_p;
					FuncL_p->entry = 0;
					FuncL_p->next = NULL;
					FuncL_p2->next = FuncL_p;
					FuncL_p2 = FuncL_p2->next;
					off = 0;

					//填写函数表
					Next(infile);
					arrow = arrow->next;
					arrow = canshuliebiao(arrow);//进入 参数列表 子程序
					if (arrow->type == 41)//判断）
					{
						Next(infile);
						arrow = arrow->next;
						arrow = fuheyuju(arrow);
						oldrange = rangenum;
						rangenum = 0;
						FuncL_p->off = off;
						SEND("END", "_", "_", "_");
						return  arrow;
					}
					else logger("err");
				}
				else {
					if (arrow->type == 42)//判断[
					{
						Array_List* AINFLp = new Array_List();
						AINFLp->tp = type;
						AINFLp->low = 0;
						AINFLp->next = NULL;
						ArrL_p2->next = AINFLp;
						ArrL_p2 = ArrL_p2->next;
						//填写数组表
						Next(infile);
						arrow = arrow->next;
						if (arrow->type == 3)//判断数字
						{
							AINFLp->up = atoi(arrow->name);//数字写入up
							if (AINFLp->tp == "int") AINFLp->len = 4 * AINFLp->up;
							if (AINFLp->tp == "float") AINFLp->len = 8 * AINFLp->up;
							if (AINFLp->tp == "bool" || AINFLp->tp == "char") AINFLp->len = AINFLp->up;
							off = off + AINFLp->len;
							Next(infile);
							arrow = arrow->next;
							if (arrow->type == 43)//判断]
							{
								Next(infile);
								arrow = arrow->next;
								arrow = shuzufuzhi(arrow);//进入 数组赋值语句 子程序
								arrow = houjishuzufuzhi(arrow);//进入 后继数组赋值语句 子程序

								return arrow;
							}
							else logger("err");

						}
						else logger("err");
					}//判断[
					else {
						if (arrow->type == 27)//判断=
						{
							strcpy_s(SymL_p->cat, "v");
							SymL_p->addr = off;
							if (type == "int") off = off + 4;
							if (type == "float")  off = off + 8;
							if (type == "bool" || AINFLp->tp == "char")  off = off + 1;
							SymL_p->addr = off;                       //地址指向vall
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
							strcpy_s(SymL_p->cat, "v");
							SymL_p->addr = off;
							if (type == "int") off = off + 4;
							if (type == "float")  off = off + 8;
							if (type == "bool" || type == "char")  off = off + 1;
							SymL_p->addr = off;            //地址指向vall

							arrow = houjibianliangshengming(arrow);//进入 后继变量声明 子程序

							return arrow;
						}

					}


				}
			}
			else logger("err");
		}
		else logger("err");
	}
}

Term* fuheyuju(Term* arrow) //复合语句
{
	if (arrow->type == 44)//判断{
	{
		Next(infile);
		arrow = arrow->next;
	}
	else logger("err");
	arrow = yujuliebiao(arrow); //进入 语句列表 子程序
	if (arrow->type == 45)//判断}
	{
		Next(infile);
		arrow = arrow->next;
		return arrow;
	}
	else logger("err");
}

Term* shuzufuzhi(Term* arrow)//数组赋值
{
	if (strcmp(arrow->name, "=") == 0)
	{
		Next(infile);
		arrow = arrow->next;
		if (strcmp(arrow->name, "{") == 0)
		{
			Next(infile);
			arrow = arrow->next;
			arrow = zhiliebiao(arrow);//值列表
			if (strcmp(arrow->name, "}") == 0)
			{
				Next(infile);
				arrow = arrow->next;
			}
			else {
				logger("err");
			}
		}
		else {
			logger("err");
		}
	}
	return arrow;
}

Term* zhiliebiao(Term* arrow)//值列表
{
	while (1)
	{
		if (arrow->type == 3)//判断是否是数字
		{
			Next(infile);
			arrow = arrow->next;
			if (strcmp(arrow->name, ",") == 0)
			{
				Next(infile);
				arrow = arrow->next;
			}
			else {
				break;
			}
		}
		else {
			logger("err");
		}
	}
	return arrow;
}

Term* suanshubiaodashi(Term* arrow) {
	arrow = suanshuxiang(arrow);
	while (1)
	{
		if (strcmp(arrow->name, "+") == 0)
		{
			Next(infile);
			arrow = arrow->next;
			arrow = suanshuxiang(arrow);
			GEQ("+");
		}
		else if (strcmp(arrow->name, "-") == 0)
		{
			Next(infile);
			arrow = arrow->next;
			arrow = suanshuxiang(arrow);
			GEQ("-");
		}
		else
		{
			break;
		}
	}
	return arrow;

}

Term* suanshuxiang(Term* arrow)//算数项
{
	arrow = suanshuyinzi(arrow);//算数因子
	while (1)
	{
		if (strcmp(arrow->name, "*") == 0)
		{
			Next(infile);
			arrow = arrow->next;
			arrow = suanshuyinzi(arrow);
			GEQ("*");
		}
		else if (strcmp(arrow->name, "/") == 0)
		{
			Next(infile);
			arrow = arrow->next;
			arrow = suanshuyinzi(arrow);
			GEQ("/");
		}
		else if (strcmp(arrow->name, "%") == 0)
		{
			Next(infile);
			arrow = arrow->next;
			arrow = suanshuyinzi(arrow);
			GEQ("%");
		}
		else
		{
			break;
		}
	}
	return arrow;
}

Term* suanshuyinzi(Term* arrow)//算数因子
{
	if (strcmp(arrow->name, "+") == 0)
	{
		Next(infile);
		arrow = arrow->next;
		arrow = suanshudanyuan(arrow);
		GEQSingle("+");
	}
	else if (strcmp(arrow->name, "-") == 0)
	{
		Next(infile);
		arrow = arrow->next;
		arrow = suanshudanyuan(arrow);
		GEQSingle("-");
	}
	else if (strcmp(arrow->name, "!") == 0)
	{
		Next(infile);
		arrow = arrow->next;
		arrow = suanshudanyuan(arrow);
		GEQSingle("!");
	}
	else
	{
		arrow = suanshudanyuan(arrow);
	}
	return arrow;
}

Term* suanshudanyuan(Term* arrow)//算数单元
{
	string newString = "";
	funcstring = "";
	if (arrow->type == 0)//判断是否是标识符
	{
		SymL_p = SymL_Head[rangenum];
		while (strcmp(SymL_p->name, arrow->name) != 0 && SymL_p->next != NULL)
		{
			SymL_p = SymL_p->next;
		}
		if (SymL_p->type == " ")
		{
			SymL_p = SymL_Head[0];
			while (strcmp(SymL_p->name, arrow->name) != 0 && SymL_p->next != NULL)
			{
				SymL_p = SymL_p->next;
			}
			if (SymL_p->type == " " || strcmp(SymL_p->name, arrow->name) != 0)
			{
				cout << "调用未定义变量" << endl; exit(0);
			}
		}

		newString = arrow->name;
		funcstring = arrow->name;
		Next(infile);
		arrow = arrow->next;
		if (strcmp(arrow->name, "[") == 0)//判断是否是[
		{
			newString = newString + '[';
			Next(infile);
			arrow = arrow->next;
			if (arrow->type == 3)//判断是否是数字
			{
				newString = newString + (arrow->name);
				Next(infile);
				arrow = arrow->next;
				if (strcmp(arrow->name, "]") == 0)//判断是否是]
				{
					newString = newString + ']';
					Next(infile);
					arrow = arrow->next;
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
		}

		else

		{
			if (strcmp(arrow->name, "(") == 0)//判断是否是(
			{
				string funcflag = SymL_p->name;

				Next(infile);
				arrow = arrow->next;
				if (strcmp(arrow->name, ")") != 0)
				{
					SEND("temp", "_", "_", funcflag);
					arrow = suanshubiaodashi(arrow);
					SEND("xingcan", "_", "_", funcstring);
					if (strcmp(arrow->name, ",") == 0)
					{
						do
						{
							Next(infile);
							arrow = arrow->next;
							arrow = suanshubiaodashi(arrow);
							SEND("xingcan", "_", "_", funcstring);
						} while (strcmp(arrow->name, ",") == 0);
					}
				}
				//判断是否是,
				if (strcmp(arrow->name, ")") == 0)//判断是否是)
				{
					SEND("diaoyong", "_", "_", funcflag);
					Next(infile);
					arrow = arrow->next;
				}
				else  logger("err");


			}
		}


	}
	else if ((arrow->type == 1) || (arrow->type == 2) || (arrow->type == 3))//常量
	{
		newString = arrow->name;
		funcstring = arrow->name;
		Next(infile);
		arrow = arrow->next;
	}

	else if (strcmp(arrow->name, "(") == 0)
	{
		Next(infile);
		arrow = arrow->next;
		arrow = luojiyuju(arrow);
		if (strcmp(arrow->name, ")") == 0)
		{
			Next(infile);
			arrow = arrow->next;
			return arrow;
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
	SEM.push(newString);
	return arrow;
}

Term* yujuliebiao(Term* arrow) //语句列表
{
	if (arrow->type == 12)//判断if
	{
		Next(infile);
		arrow = arrow->next;
		if (arrow->type == 40)//判断（
		{
			Next(infile);
			arrow = arrow->next;
			arrow = luojiyuju(arrow); //进入 逻辑语句 子程序
			if (arrow->type == 41)//判断）
			{
				IF();//IF四元式生成
				Next(infile);
				arrow = arrow->next;
				arrow = fuheyuju(arrow); //进入 复合语句 子程序
				if (arrow->type == 13)//判断else
				{
					ELSE();//ELSE四元式生成
					Next(infile);
					arrow = arrow->next;
					arrow = fuheyuju(arrow); //进入 复合语句 子程序
					IFEND();//IFEND四元式生成
					arrow = yujuliebiao(arrow); //进入 语句列表 子程序
					return arrow;
				}
				else {
					IFEND();  //IFEND四元式生成
					arrow = yujuliebiao(arrow); //进入 语句列表 子程序
					return arrow;
				}
			}
			else logger("err");
		}
		else logger("err");
	}
	else {
		if (arrow->type == 14)//判断while
		{
			WHILE();//生成WHILE四元式
			Next(infile);
			arrow = arrow->next;
			if (arrow->type == 40)//判断（
			{
				Next(infile);
				arrow = arrow->next;
				arrow = luojiyuju(arrow); //进入 逻辑语句 子程序
				if (arrow->type == 41)//判断）
				{
					DO();//生成DO四元式
					Next(infile);
					arrow = arrow->next;
					{
						arrow = fuheyuju(arrow); //进入 复合语句 子程序
						WHILEEND();//生成WHILEEND四元式
						arrow = yujuliebiao(arrow); //进入 语句列表 子程序
						return arrow;
					}
				}
				else logger("err");
			}
			else logger("err");
		}
		else {
			if (arrow->type == 18)//判断return
			{
				Next(infile);
				arrow = arrow->next;
				if (arrow->type == 0)//判断标识符
				{
					SymL_p = SymL_Head[rangenum];
					while (strcmp(SymL_p->name, arrow->name) != 0 && SymL_p->next != NULL)
					{
						SymL_p = SymL_p->next;
					}
					if (SymL_p->type == " ")
					{
						SymL_p = SymL_Head[0];
						while (strcmp(SymL_p->name, arrow->name) != 0 && SymL_p->next != NULL)
						{
							SymL_p = SymL_p->next;
						}
						if (SymL_p->type == " " || strcmp(SymL_p->name, arrow->name) != 0)
						{
							printf("调用未定义变量"); exit(0);
						}
					}
					if (strcmp(SymL_p->cat, "v") == 0 || strcmp(SymL_p->cat, "p") == 0)
					{
						SEND("re", "_", SymL_p->name, funcname);
					}
					Next(infile);
					arrow = arrow->next;
					if (arrow->type == 39)//判断;
					{
						Next(infile);
						arrow = arrow->next;
						arrow = yujuliebiao(arrow); //进入 语句列表 子程序
						return arrow;
					}
					else logger("err");
				}
				else {
					if (arrow->type == 1 || arrow->type == 2 || arrow->type == 3)//判断常量
					{
						SEND("re", "_", arrow->name, funcname);
						Next(infile);
						arrow = arrow->next;
						if (arrow->type == 39)//判断;
						{
							Next(infile);
							arrow = arrow->next;
							arrow = yujuliebiao(arrow); //进入 语句列表 子程序
							return arrow;
						}
						else logger("err");
					}
					else logger("err");
				}
			}
			else {
				if (arrow->type == 0)//判断标识符
				{
					string newString = "";
					newString = arrow->name;
					while (arrow->type == 0)//判断标识符
					{
						SymL_p = SymL_Head[rangenum];
						while (strcmp(SymL_p->name, arrow->name) != 0 && SymL_p->next != NULL)
						{
							SymL_p = SymL_p->next;
						}
						if (SymL_p->type == " ")
						{
							SymL_p = SymL_Head[0];
							while (strcmp(SymL_p->name, arrow->name) != 0 && SymL_p->next != NULL)
							{
								SymL_p = SymL_p->next;
							}
							if (SymL_p->type == " " || strcmp(SymL_p->name, arrow->name) != 0)
							{
								printf("调用未定义变量"); exit(0);
							}
						}

						Next(infile);
						arrow = arrow->next;
						if (arrow->type == 42)//判断[
						{
							newString = newString + '[';
							Next(infile);
							arrow = arrow->next;
							if (arrow->type == 3)//判断数字
							{
								newString = newString + (arrow->name);
								Next(infile);
								arrow = arrow->next;
							}
							else logger("err");
							if (arrow->type == 43)//判断]
							{
								newString = newString + ']';
								Next(infile);
								arrow = arrow->next;
							}
							else logger("err");
						}
						if (arrow->type == 27)//判断=
						{
							SEM.push(newString);
							Next(infile);
							arrow = arrow->next;
							if (arrow->type == 1 || arrow->type == 2)//判断常量
							{
								Next(infile);
								arrow = arrow->next;
							}
							else { arrow = suanshubiaodashi(arrow); }//进入 算术表达式 子程序
							ASSI(newString);
							if (arrow->type != 38)//判断,
								break;
						}
						else logger("err");
					}
					if (arrow->type == 39)//判断;
					{
						Next(infile);
						arrow = arrow->next;
						arrow = yujuliebiao(arrow); //进入 语句列表 子程序
						return arrow;
					}
					else  logger("err");
				}

				if (arrow->type == 7 || arrow->type == 5 || arrow->type == 9 || arrow->type == 11)//判断char int bool float
				{
					type = arrow->name;//读type准备写入符号总表
					Next(infile);
					arrow = arrow->next;
					if (arrow->type == 0)//判断标识符
					{
						SymL_p = SymL_Head[rangenum];
						while (strcmp(SymL_p->name, arrow->name) != 0) { SymL_p = SymL_p->next; }
						if (SymL_p->next != NULL) { printf("重复定义"); exit(0); }
						SymL_p->type = type;
						Next(infile);
						arrow = arrow->next;
						if (arrow->type == 40)//判断[
						{
							strcpy_s(SymL_p->cat, "a");
							SymL_p->addr = off;
							//SYNBLp->addr=++AINFaddr;
							AINFLp = new Array_List();
							AINFLp->tp = type;
							AINFLp->low = 0;
							AINFLp->next = NULL;
							ArrL_p2->next = AINFLp;
							ArrL_p2 = ArrL_p2->next;
							//填写数组表
							Next(infile);
							arrow = arrow->next;
							if (arrow->type == 3)//判断数字
							{
								AINFLp->up = atoi(arrow->name);//数字写入up
								if (AINFLp->tp == "int") AINFLp->len = 4 * AINFLp->up;
								if (AINFLp->tp == "float") AINFLp->len = 8 * AINFLp->up;
								if (AINFLp->tp == "bool" || AINFLp->tp == "char") AINFLp->len = AINFLp->up;
								off = off + AINFLp->len;
								Next(infile);
								arrow = arrow->next;
								if (arrow->type == 41)//判断]
								{
									Next(infile);
									arrow = arrow->next;
									arrow = shuzufuzhi(arrow);//进入 数组赋值语句 子程序
									arrow = houjishuzufuzhi(arrow);//进入 后继数组赋值语句 子程序
									arrow = yujuliebiao(arrow); //进入 语句列表 子程序
									return arrow;
								}
								else logger("err");
							}
							else logger("err");
						}
						else {
							if (arrow->type == 25)//判断=
							{
								strcpy_s(SymL_p->cat, "v");
								SymL_p->addr = off;
								if (type == "int") off = off + 4;
								if (type == "float")  off = off + 8;
								if (type == "bool" || AINFLp->tp == "char")  off = off + 1;
								SymL_p->addr = off;                              //地址指向vall
								Next(infile);
								arrow = arrow->next;
								if (arrow->type == 1 || arrow->type == 2)//判断常量
								{
									Next(infile);
									arrow = arrow->next;
									arrow = houjibianliangshengming(arrow); //进入 后继变量声明 子程序
									arrow = yujuliebiao(arrow); //进入 语句列表 子程序
									return arrow;
								}
								else {
									arrow = suanshubiaodashi(arrow);//进入 算术表达式 子程序
									arrow = houjibianliangshengming(arrow); //进入 后继变量声明 子程序
									arrow = yujuliebiao(arrow); //进入 语句列表 子程序
									return arrow;
								}
							}
							else {
								strcpy_s(SymL_p->cat, "v");
								SymL_p->addr = off;
								if (type == "int") off = off + 4;
								if (type == "float")  off = off + 8;
								if (type == "bool" || type == "char")  off = off + 1;                         //地址指向vall
								arrow = houjibianliangshengming(arrow); //进入 后继变量声明 子程序
								arrow = yujuliebiao(arrow); //进入 语句列表 子程序
								return arrow;
							}

						}
					}
					else logger("err");
				}
				if (arrow->type == 19 || arrow->type == 20)
				{
					arrow = shuchu(arrow);
					arrow = shuru(arrow);
					if (arrow->type == 39)//判断;
					{
						Next(infile);
						arrow = arrow->next;
						arrow = yujuliebiao(arrow); //进入 语句列表 子程序
						return arrow;
					}
					else { logger("err"); }
				}
				else return arrow;
			}
		}
	}

}

Term* putongbianliangshengming(Term* arrow)//普通变量声明
{
	if (arrow->type == 0)//判断标识符
	{
		SymL_p = SymL_Head[rangenum];
		while (strcmp(SymL_p->name, arrow->name) != 0) { SymL_p = SymL_p->next; }
		if (SymL_p->next != NULL) { logger("重复定义"); exit(0); }
		SymL_p->type = type;
		Next(infile);
		arrow = arrow->next;
		if (strcmp(arrow->name, "=") == 0)
		{
			Next(infile);
			arrow = arrow->next;
			if ((arrow->type == 1) || (arrow->type == 2))//判断是否是常量(字符串，常量)
			{
				Next(infile);
				arrow = arrow->next;
			}
			else
			{
				arrow = suanshubiaodashi(arrow);//算术表达式
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
	SymL_p->addr = off;
	if (type == "int") off = off + 4;
	if (type == "float")  off = off + 8;
	if (type == "bool" || type == "char")  off = off + 1;
	// SYNBLp->addr=OFF;                                               //地址指向vall
	return arrow;
}
Term* houjibianliangshengming(Term* arrow)//后继变量声明
{
	if (strcmp(arrow->name, ";") == 0)
	{
		Next(infile);
		arrow = arrow->next;
	}
	else if (strcmp(arrow->name, ",") == 0)
	{
		Next(infile);
		arrow = arrow->next;
		arrow = putongbianliangshengming(arrow);//普通变量声明
		arrow = houjibianliangshengming(arrow);//后继变量声明
	}
	else
	{
		logger("err");
	}
	return arrow;
}

Term* canshuliebiao(Term* arrow)//参数列表
{
	if ((strcmp(arrow->name, "int") == 0) || (strcmp(arrow->name, "char") == 0) || (strcmp(arrow->name, "float") == 0) || (strcmp(arrow->name, "bool") == 0))
	{
		fn++;
		arrow = canshushengming(arrow);//参数申明
		arrow = houjicanshushengming(arrow);//后继参数申明
	}
	return arrow;
}

Term* canshushengming(Term* arrow)//参数申明
{
	if ((strcmp(arrow->name, "int") == 0) || (strcmp(arrow->name, "char") == 0) || (strcmp(arrow->name, "float") == 0) || (strcmp(arrow->name, "bool") == 0))
	{
		type = arrow->name;//读type准备写入符号总表

		Next(infile);
		arrow = arrow->next;
		Parameter_List* PARAMp2 = new Parameter_List();
		strcpy_s(PARAMp2->name, arrow->name);
		if (arrow->type == 0)//判断是否是标识符
		{
			SEND("canshu", "_", "_", arrow->name);
			SymL_p = SymL_Head[rangenum];

			while (strcmp(SymL_p->name, arrow->name) != 0) { SymL_p = SymL_p->next; }
			if (SymL_p->next != NULL) { logger("重复定义"); exit(0); }
			SymL_p->type = type;
			SymL_p->addr = off;
			PARAMp2->type = type;
			if (type == "int")off = off + 4;
			if (type == "float")off = off + 8;
			if (type == "bool" || type == "char")off = off + 1;
			Next(infile);
			arrow = arrow->next;
			if (strcmp(arrow->name, "=") == 0)
			{
				strcpy_s(SymL_p->cat, "p");

				strcpy_s(PARAMp2->cat, "p");
				SymL_p->addr = off;
				//地址指向vall
				PARAMp2->next = NULL;
				Para_p->next = PARAMp2;
				Para_p = Para_p->next;
				Next(infile);
				arrow = arrow->next;
				if ((arrow->type == 1) || (arrow->type == 2))//判断是否是常量(字符串，常量)
				{
					Next(infile);
					arrow = arrow->next;
				}
				else
				{
					strcpy_s(SymL_p->cat, "p");

					//地址指向vall
					arrow = suanshubiaodashi(arrow);//算术表达式
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

Term* houjicanshushengming(Term* arrow)//后继参数申明
{
	if (strcmp(arrow->name, ",") == 0)
	{
		Next(infile);
		arrow = arrow->next;
		fn++;
		arrow = canshushengming(arrow);//参数申明
		arrow = houjicanshushengming(arrow);//后继参数申明
	}
	return arrow;
}

Term* luojiyuju(Term* arrow)//逻辑语句
{
	arrow = luojixiang(arrow);
	while (1)
	{
		if (strcmp(arrow->name, "||") == 0)
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

Term* luojixiang(Term* arrow)//逻辑项
{
	arrow = luojiyinzi1(arrow);
	while (1)
	{
		if (strcmp(arrow->name, "&&") == 0)
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

Term* luojiyinzi1(Term* arrow)//逻辑因子1
{
	arrow = luojiyinzi2(arrow);
	while (1)
	{
		if (strcmp(arrow->name, "==") == 0)
		{
			Next(infile);
			arrow = arrow->next;
			arrow = luojiyinzi2(arrow);
			GEQ("==");
		}
		else if (strcmp(arrow->name, "!=") == 0)
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

Term* luojiyinzi2(Term* arrow)//逻辑因子2
{
	arrow = suanshubiaodashi(arrow);
	while (1)
	{
		if (strcmp(arrow->name, ">=") == 0)
		{
			Next(infile);
			arrow = arrow->next;
			arrow = suanshubiaodashi(arrow);//算术表达式
			GEQ(">=");
		}
		else if (strcmp(arrow->name, ">") == 0)
		{
			Next(infile);
			arrow = arrow->next;
			arrow = suanshubiaodashi(arrow);
			GEQ(">");
		}
		else if (strcmp(arrow->name, "<=") == 0)
		{
			Next(infile);
			arrow = arrow->next;
			arrow = suanshubiaodashi(arrow);
			GEQ("<=");
		}
		else if (strcmp(arrow->name, "<") == 0)
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

Term* houjishuzufuzhi(Term* arrow)//后继数组赋值
{
	if (strcmp(arrow->name, ",") == 0)
	{
		Next(infile);
		arrow = arrow->next;
		if (arrow->type == 0)//判断是否是标识符
		{
			while (strcmp(SymL_p->name, arrow->name) != 0) SymL_p = SymL_p->next;
			if (SymL_p->next != NULL) { printf("重复定义"); exit(0); }
			SymL_p->type = type;
			Next(infile);
			arrow = arrow->next;
			if (strcmp(arrow->name, "[") == 0)
			{
				strcpy_s(SymL_p->cat, "a");
				SymL_p->addr = ++Arr_addr;
				Array_List* AINFLp = new Array_List();
				AINFLp->tp = type;
				AINFLp->low = 0;
				AINFLp->next = NULL;
				ArrL_p2->next = AINFLp;
				ArrL_p2 = ArrL_p2->next;
				//填写数组表
				Next(infile);
				arrow = arrow->next;
				if (arrow->type == 3)
				{
					AINFLp->up = atoi(arrow->name);//数字写入up
					if (AINFLp->tp == "int") AINFLp->len = 4 * AINFLp->up;
					if (AINFLp->tp == "float") AINFLp->len = 8 * AINFLp->up;
					if (AINFLp->tp == "bool" || AINFLp->tp == "char") AINFLp->len = AINFLp->up;
					off = off + AINFLp->len;
					Next(infile);
					arrow = arrow->next;
					if (strcmp(arrow->name, "]") == 0)
					{
						Next(infile);
						arrow = arrow->next;
						arrow = shuzufuzhi(arrow);//数组赋值
						arrow = houjishuzufuzhi(arrow);//后继数组赋值
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
	}
	else if (strcmp(arrow->name, ";") == 0)
	{
		Next(infile);
		arrow = arrow->next;
	}
	else
	{
		logger("err");
	}
	return arrow;
}

Term* shuchu(Term* arrow) {
	if (arrow->type == 19)/*判断cout*/
	{
		Next(infile);
		arrow = arrow->next;
		if (arrow->type == 36)/*判断"<<"*/
		{
			Next(infile);
			arrow = arrow->next;
			if (arrow->type == 0)
			{
				SEND("cout", "_", "_", arrow->name);
				Next(infile);
				arrow = arrow->next;
			}
			else { logger("err"); }
		}
		else { logger("err"); }
	}

	return arrow;
}

Term* shuru(Term* arrow) {
	if (arrow->type == 20)/*判断cin*/
	{
		Next(infile);
		arrow = arrow->next;
		if (arrow->type == 37)/*判断">>"*/
		{
			Next(infile);
			arrow = arrow->next;
			if (arrow->type == 0)
			{
				SEND("cin", "_", "_", arrow->name);
				Next(infile);
				arrow = arrow->next;
			}
			else { logger("err"); }
		}
		else { logger("err"); }
	}
	return arrow;
}