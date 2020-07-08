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
	arrow = Chengxu(arrow);//���� ���� �ӳ���
	arrow = Houjichengxu(arrow);//���� ��̳��� �ӳ���
	return  arrow;
}

Term* Houjichengxu(Term* arrow)
{
	if (arrow->type == 47);//�ж�#,45Ϊ��#����Token���е�˳�����
	else {
		arrow = Chengxu(arrow); //���� ���� �ӳ���
		arrow = Houjichengxu(arrow); //���� ��̳��� �ӳ���

	}
	return arrow;
}

Term* Chengxu(Term* arrow)
{
	if (arrow->type == 4 || arrow->type == 7)//�ж�int ��void
	{
		type = arrow->name;//��word׼��д������ܱ�
		Next(infile);
		arrow = arrow->next;
		if (arrow->type == 17)//�ж�main
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
			//��д������

			Next(infile);
			arrow = arrow->next;
			if (arrow->type == 40)//�жϣ�
			{
				Next(infile);
				arrow = arrow->next;
				if (arrow->type == 41)//�жϣ�
				{

					Next(infile);
					arrow = arrow->next;
					//if (arrow->type == 42)//�ж�{
					//{
					//	Next(infile);
					//	arrow = arrow->next;
					arrow = fuheyuju(arrow);//���� ������� �ӳ���
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
		else if (arrow->type == 0)//�жϱ�ʶ��
		{
			SymL_p = SymL_Head[rangenum];
			while (strcmp(SymL_p->name, arrow->name) != 0)
			{
				SymL_p = SymL_p->next;
			}
			if (SymL_p->next != NULL) { printf("�ظ�����"); exit(0); }
			SymL_p->type = type;
			SymL_p->addr = off;

			Next(infile);
			arrow = arrow->next;
			if (arrow->type == 40)//�жϣ�
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

				//��д������
				Next(infile);
				arrow = arrow->next;
				arrow = canshuliebiao(arrow);//���� �����б� �ӳ���
				if (arrow->type == 41)//�жϣ�
				{
					Next(infile);
					arrow = arrow->next;
					//if (arrow->type == 42)//�ж�{
					//{
					//	Next(infile);
					//	arrow = arrow->next;
					arrow = fuheyuju(arrow);//���� ������� �ӳ���
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
				if (arrow->type == 42)//�ж�[
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

					//��д�����
					Next(infile);
					arrow = arrow->next;
					if (arrow->type == 3)//�ж�����
					{
						AINFLp->up = atoi(arrow->name);//����д��up
						if (AINFLp->tp == "int") AINFLp->len = 4 * AINFLp->up;
						if (AINFLp->tp == "float") AINFLp->len = 8 * AINFLp->up;
						if (AINFLp->tp == "bool" || AINFLp->tp == "char") AINFLp->len = AINFLp->up;
						off = off + AINFLp->len;
						SymL_p->addr = off;

						Next(infile);
						arrow = arrow->next;
						if (arrow->type == 43)//�ж�]
						{
							Next(infile);
							arrow = arrow->next;
							arrow = shuzufuzhi(arrow);//���� ���鸳ֵ��� �ӳ���
							arrow = houjishuzufuzhi(arrow);//���� ������鸳ֵ��� �ӳ���

							return arrow;
						}
						else logger("err");

					}
					else logger("err");
				}
				else {
					if (arrow->type == 27)//�ж�=
					{
						strcpy_s(SymL_p->cat, "v");
						Next(infile);
						arrow = arrow->next;
						if (arrow->type == 1 || arrow->type == 2)
						{
							Next(infile);
							arrow = arrow->next;
							arrow = houjibianliangshengming(arrow);//���� ��̱������� �ӳ���

							return arrow;
						}
						else {
							arrow = suanshubiaodashi(arrow);//���� �������ʽ �ӳ���
							arrow = houjibianliangshengming(arrow);//���� ��̱������� �ӳ���

							return arrow;
						}
					}
					else {
						strcpy_s(SymL_p->cat, "v");
						arrow = houjibianliangshengming(arrow);//���� ��̱������� �ӳ���

						return arrow;
					}

				}//�ж�=
			}//�ж�[

		}//�жϱ�ʶ��
		else logger("err");
	}
	else {
		if (arrow->type == 5 || arrow->type == 9 || arrow->type == 11)//�ж�bool float char
		{
			type = arrow->name;//��word׼��д������ܱ�
			Next(infile);
			arrow = arrow->next;
			if (arrow->type == 0)//�жϱ�ʶ��
			{
				SymL_p = SymL_Head[rangenum];
				while (strcmp(SymL_p->name, arrow->name) != 0)
				{
					SymL_p = SymL_p->next;

				}
				if (SymL_p->next != NULL)
				{
					printf("�ظ�����");
					exit(0);
				}
				SymL_p->type = type;
				Next(infile);
				arrow = arrow->next;
				if (arrow->type == 40)//�жϣ�
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

					//��д������
					Next(infile);
					arrow = arrow->next;
					arrow = canshuliebiao(arrow);//���� �����б� �ӳ���
					if (arrow->type == 41)//�жϣ�
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
					if (arrow->type == 42)//�ж�[
					{
						Array_List* AINFLp = new Array_List();
						AINFLp->tp = type;
						AINFLp->low = 0;
						AINFLp->next = NULL;
						ArrL_p2->next = AINFLp;
						ArrL_p2 = ArrL_p2->next;
						//��д�����
						Next(infile);
						arrow = arrow->next;
						if (arrow->type == 3)//�ж�����
						{
							AINFLp->up = atoi(arrow->name);//����д��up
							if (AINFLp->tp == "int") AINFLp->len = 4 * AINFLp->up;
							if (AINFLp->tp == "float") AINFLp->len = 8 * AINFLp->up;
							if (AINFLp->tp == "bool" || AINFLp->tp == "char") AINFLp->len = AINFLp->up;
							off = off + AINFLp->len;
							Next(infile);
							arrow = arrow->next;
							if (arrow->type == 43)//�ж�]
							{
								Next(infile);
								arrow = arrow->next;
								arrow = shuzufuzhi(arrow);//���� ���鸳ֵ��� �ӳ���
								arrow = houjishuzufuzhi(arrow);//���� ������鸳ֵ��� �ӳ���

								return arrow;
							}
							else logger("err");

						}
						else logger("err");
					}//�ж�[
					else {
						if (arrow->type == 27)//�ж�=
						{
							strcpy_s(SymL_p->cat, "v");
							SymL_p->addr = off;
							if (type == "int") off = off + 4;
							if (type == "float")  off = off + 8;
							if (type == "bool" || AINFLp->tp == "char")  off = off + 1;
							SymL_p->addr = off;                       //��ַָ��vall
							Next(infile);
							arrow = arrow->next;
							if (arrow->type == 1 || arrow->type == 2)//�ж��ַ����ַ�������������
							{
								Next(infile);
								arrow = arrow->next;
								arrow = houjibianliangshengming(arrow);//���� ��̱������� �ӳ���

								return arrow;
							}
							else {
								arrow = suanshubiaodashi(arrow);//���� �������ʽ �ӳ���
								arrow = houjibianliangshengming(arrow);//���� ��̱������� �ӳ���

								return arrow;
							}
						}
						else {
							strcpy_s(SymL_p->cat, "v");
							SymL_p->addr = off;
							if (type == "int") off = off + 4;
							if (type == "float")  off = off + 8;
							if (type == "bool" || type == "char")  off = off + 1;
							SymL_p->addr = off;            //��ַָ��vall

							arrow = houjibianliangshengming(arrow);//���� ��̱������� �ӳ���

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

Term* fuheyuju(Term* arrow) //�������
{
	if (arrow->type == 44)//�ж�{
	{
		Next(infile);
		arrow = arrow->next;
	}
	else logger("err");
	arrow = yujuliebiao(arrow); //���� ����б� �ӳ���
	if (arrow->type == 45)//�ж�}
	{
		Next(infile);
		arrow = arrow->next;
		return arrow;
	}
	else logger("err");
}

Term* shuzufuzhi(Term* arrow)//���鸳ֵ
{
	if (strcmp(arrow->name, "=") == 0)
	{
		Next(infile);
		arrow = arrow->next;
		if (strcmp(arrow->name, "{") == 0)
		{
			Next(infile);
			arrow = arrow->next;
			arrow = zhiliebiao(arrow);//ֵ�б�
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

Term* zhiliebiao(Term* arrow)//ֵ�б�
{
	while (1)
	{
		if (arrow->type == 3)//�ж��Ƿ�������
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

Term* suanshuxiang(Term* arrow)//������
{
	arrow = suanshuyinzi(arrow);//��������
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

Term* suanshuyinzi(Term* arrow)//��������
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

Term* suanshudanyuan(Term* arrow)//������Ԫ
{
	string newString = "";
	funcstring = "";
	if (arrow->type == 0)//�ж��Ƿ��Ǳ�ʶ��
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
				cout << "����δ�������" << endl; exit(0);
			}
		}

		newString = arrow->name;
		funcstring = arrow->name;
		Next(infile);
		arrow = arrow->next;
		if (strcmp(arrow->name, "[") == 0)//�ж��Ƿ���[
		{
			newString = newString + '[';
			Next(infile);
			arrow = arrow->next;
			if (arrow->type == 3)//�ж��Ƿ�������
			{
				newString = newString + (arrow->name);
				Next(infile);
				arrow = arrow->next;
				if (strcmp(arrow->name, "]") == 0)//�ж��Ƿ���]
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
			if (strcmp(arrow->name, "(") == 0)//�ж��Ƿ���(
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
				//�ж��Ƿ���,
				if (strcmp(arrow->name, ")") == 0)//�ж��Ƿ���)
				{
					SEND("diaoyong", "_", "_", funcflag);
					Next(infile);
					arrow = arrow->next;
				}
				else  logger("err");


			}
		}


	}
	else if ((arrow->type == 1) || (arrow->type == 2) || (arrow->type == 3))//����
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

Term* yujuliebiao(Term* arrow) //����б�
{
	if (arrow->type == 12)//�ж�if
	{
		Next(infile);
		arrow = arrow->next;
		if (arrow->type == 40)//�жϣ�
		{
			Next(infile);
			arrow = arrow->next;
			arrow = luojiyuju(arrow); //���� �߼���� �ӳ���
			if (arrow->type == 41)//�жϣ�
			{
				IF();//IF��Ԫʽ����
				Next(infile);
				arrow = arrow->next;
				arrow = fuheyuju(arrow); //���� ������� �ӳ���
				if (arrow->type == 13)//�ж�else
				{
					ELSE();//ELSE��Ԫʽ����
					Next(infile);
					arrow = arrow->next;
					arrow = fuheyuju(arrow); //���� ������� �ӳ���
					IFEND();//IFEND��Ԫʽ����
					arrow = yujuliebiao(arrow); //���� ����б� �ӳ���
					return arrow;
				}
				else {
					IFEND();  //IFEND��Ԫʽ����
					arrow = yujuliebiao(arrow); //���� ����б� �ӳ���
					return arrow;
				}
			}
			else logger("err");
		}
		else logger("err");
	}
	else {
		if (arrow->type == 14)//�ж�while
		{
			WHILE();//����WHILE��Ԫʽ
			Next(infile);
			arrow = arrow->next;
			if (arrow->type == 40)//�жϣ�
			{
				Next(infile);
				arrow = arrow->next;
				arrow = luojiyuju(arrow); //���� �߼���� �ӳ���
				if (arrow->type == 41)//�жϣ�
				{
					DO();//����DO��Ԫʽ
					Next(infile);
					arrow = arrow->next;
					{
						arrow = fuheyuju(arrow); //���� ������� �ӳ���
						WHILEEND();//����WHILEEND��Ԫʽ
						arrow = yujuliebiao(arrow); //���� ����б� �ӳ���
						return arrow;
					}
				}
				else logger("err");
			}
			else logger("err");
		}
		else {
			if (arrow->type == 18)//�ж�return
			{
				Next(infile);
				arrow = arrow->next;
				if (arrow->type == 0)//�жϱ�ʶ��
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
							printf("����δ�������"); exit(0);
						}
					}
					if (strcmp(SymL_p->cat, "v") == 0 || strcmp(SymL_p->cat, "p") == 0)
					{
						SEND("re", "_", SymL_p->name, funcname);
					}
					Next(infile);
					arrow = arrow->next;
					if (arrow->type == 39)//�ж�;
					{
						Next(infile);
						arrow = arrow->next;
						arrow = yujuliebiao(arrow); //���� ����б� �ӳ���
						return arrow;
					}
					else logger("err");
				}
				else {
					if (arrow->type == 1 || arrow->type == 2 || arrow->type == 3)//�жϳ���
					{
						SEND("re", "_", arrow->name, funcname);
						Next(infile);
						arrow = arrow->next;
						if (arrow->type == 39)//�ж�;
						{
							Next(infile);
							arrow = arrow->next;
							arrow = yujuliebiao(arrow); //���� ����б� �ӳ���
							return arrow;
						}
						else logger("err");
					}
					else logger("err");
				}
			}
			else {
				if (arrow->type == 0)//�жϱ�ʶ��
				{
					string newString = "";
					newString = arrow->name;
					while (arrow->type == 0)//�жϱ�ʶ��
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
								printf("����δ�������"); exit(0);
							}
						}

						Next(infile);
						arrow = arrow->next;
						if (arrow->type == 42)//�ж�[
						{
							newString = newString + '[';
							Next(infile);
							arrow = arrow->next;
							if (arrow->type == 3)//�ж�����
							{
								newString = newString + (arrow->name);
								Next(infile);
								arrow = arrow->next;
							}
							else logger("err");
							if (arrow->type == 43)//�ж�]
							{
								newString = newString + ']';
								Next(infile);
								arrow = arrow->next;
							}
							else logger("err");
						}
						if (arrow->type == 27)//�ж�=
						{
							SEM.push(newString);
							Next(infile);
							arrow = arrow->next;
							if (arrow->type == 1 || arrow->type == 2)//�жϳ���
							{
								Next(infile);
								arrow = arrow->next;
							}
							else { arrow = suanshubiaodashi(arrow); }//���� �������ʽ �ӳ���
							ASSI(newString);
							if (arrow->type != 38)//�ж�,
								break;
						}
						else logger("err");
					}
					if (arrow->type == 39)//�ж�;
					{
						Next(infile);
						arrow = arrow->next;
						arrow = yujuliebiao(arrow); //���� ����б� �ӳ���
						return arrow;
					}
					else  logger("err");
				}

				if (arrow->type == 7 || arrow->type == 5 || arrow->type == 9 || arrow->type == 11)//�ж�char int bool float
				{
					type = arrow->name;//��type׼��д������ܱ�
					Next(infile);
					arrow = arrow->next;
					if (arrow->type == 0)//�жϱ�ʶ��
					{
						SymL_p = SymL_Head[rangenum];
						while (strcmp(SymL_p->name, arrow->name) != 0) { SymL_p = SymL_p->next; }
						if (SymL_p->next != NULL) { printf("�ظ�����"); exit(0); }
						SymL_p->type = type;
						Next(infile);
						arrow = arrow->next;
						if (arrow->type == 40)//�ж�[
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
							//��д�����
							Next(infile);
							arrow = arrow->next;
							if (arrow->type == 3)//�ж�����
							{
								AINFLp->up = atoi(arrow->name);//����д��up
								if (AINFLp->tp == "int") AINFLp->len = 4 * AINFLp->up;
								if (AINFLp->tp == "float") AINFLp->len = 8 * AINFLp->up;
								if (AINFLp->tp == "bool" || AINFLp->tp == "char") AINFLp->len = AINFLp->up;
								off = off + AINFLp->len;
								Next(infile);
								arrow = arrow->next;
								if (arrow->type == 41)//�ж�]
								{
									Next(infile);
									arrow = arrow->next;
									arrow = shuzufuzhi(arrow);//���� ���鸳ֵ��� �ӳ���
									arrow = houjishuzufuzhi(arrow);//���� ������鸳ֵ��� �ӳ���
									arrow = yujuliebiao(arrow); //���� ����б� �ӳ���
									return arrow;
								}
								else logger("err");
							}
							else logger("err");
						}
						else {
							if (arrow->type == 25)//�ж�=
							{
								strcpy_s(SymL_p->cat, "v");
								SymL_p->addr = off;
								if (type == "int") off = off + 4;
								if (type == "float")  off = off + 8;
								if (type == "bool" || AINFLp->tp == "char")  off = off + 1;
								SymL_p->addr = off;                              //��ַָ��vall
								Next(infile);
								arrow = arrow->next;
								if (arrow->type == 1 || arrow->type == 2)//�жϳ���
								{
									Next(infile);
									arrow = arrow->next;
									arrow = houjibianliangshengming(arrow); //���� ��̱������� �ӳ���
									arrow = yujuliebiao(arrow); //���� ����б� �ӳ���
									return arrow;
								}
								else {
									arrow = suanshubiaodashi(arrow);//���� �������ʽ �ӳ���
									arrow = houjibianliangshengming(arrow); //���� ��̱������� �ӳ���
									arrow = yujuliebiao(arrow); //���� ����б� �ӳ���
									return arrow;
								}
							}
							else {
								strcpy_s(SymL_p->cat, "v");
								SymL_p->addr = off;
								if (type == "int") off = off + 4;
								if (type == "float")  off = off + 8;
								if (type == "bool" || type == "char")  off = off + 1;                         //��ַָ��vall
								arrow = houjibianliangshengming(arrow); //���� ��̱������� �ӳ���
								arrow = yujuliebiao(arrow); //���� ����б� �ӳ���
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
					if (arrow->type == 39)//�ж�;
					{
						Next(infile);
						arrow = arrow->next;
						arrow = yujuliebiao(arrow); //���� ����б� �ӳ���
						return arrow;
					}
					else { logger("err"); }
				}
				else return arrow;
			}
		}
	}

}

Term* putongbianliangshengming(Term* arrow)//��ͨ��������
{
	if (arrow->type == 0)//�жϱ�ʶ��
	{
		SymL_p = SymL_Head[rangenum];
		while (strcmp(SymL_p->name, arrow->name) != 0) { SymL_p = SymL_p->next; }
		if (SymL_p->next != NULL) { logger("�ظ�����"); exit(0); }
		SymL_p->type = type;
		Next(infile);
		arrow = arrow->next;
		if (strcmp(arrow->name, "=") == 0)
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
	SymL_p->addr = off;
	if (type == "int") off = off + 4;
	if (type == "float")  off = off + 8;
	if (type == "bool" || type == "char")  off = off + 1;
	// SYNBLp->addr=OFF;                                               //��ַָ��vall
	return arrow;
}
Term* houjibianliangshengming(Term* arrow)//��̱�������
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
	if ((strcmp(arrow->name, "int") == 0) || (strcmp(arrow->name, "char") == 0) || (strcmp(arrow->name, "float") == 0) || (strcmp(arrow->name, "bool") == 0))
	{
		fn++;
		arrow = canshushengming(arrow);//��������
		arrow = houjicanshushengming(arrow);//��̲�������
	}
	return arrow;
}

Term* canshushengming(Term* arrow)//��������
{
	if ((strcmp(arrow->name, "int") == 0) || (strcmp(arrow->name, "char") == 0) || (strcmp(arrow->name, "float") == 0) || (strcmp(arrow->name, "bool") == 0))
	{
		type = arrow->name;//��type׼��д������ܱ�

		Next(infile);
		arrow = arrow->next;
		Parameter_List* PARAMp2 = new Parameter_List();
		strcpy_s(PARAMp2->name, arrow->name);
		if (arrow->type == 0)//�ж��Ƿ��Ǳ�ʶ��
		{
			SEND("canshu", "_", "_", arrow->name);
			SymL_p = SymL_Head[rangenum];

			while (strcmp(SymL_p->name, arrow->name) != 0) { SymL_p = SymL_p->next; }
			if (SymL_p->next != NULL) { logger("�ظ�����"); exit(0); }
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
				//��ַָ��vall
				PARAMp2->next = NULL;
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
	if (strcmp(arrow->name, ",") == 0)
	{
		Next(infile);
		arrow = arrow->next;
		fn++;
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

Term* luojixiang(Term* arrow)//�߼���
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

Term* luojiyinzi1(Term* arrow)//�߼�����1
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

Term* luojiyinzi2(Term* arrow)//�߼�����2
{
	arrow = suanshubiaodashi(arrow);
	while (1)
	{
		if (strcmp(arrow->name, ">=") == 0)
		{
			Next(infile);
			arrow = arrow->next;
			arrow = suanshubiaodashi(arrow);//�������ʽ
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

Term* houjishuzufuzhi(Term* arrow)//������鸳ֵ
{
	if (strcmp(arrow->name, ",") == 0)
	{
		Next(infile);
		arrow = arrow->next;
		if (arrow->type == 0)//�ж��Ƿ��Ǳ�ʶ��
		{
			while (strcmp(SymL_p->name, arrow->name) != 0) SymL_p = SymL_p->next;
			if (SymL_p->next != NULL) { printf("�ظ�����"); exit(0); }
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
				//��д�����
				Next(infile);
				arrow = arrow->next;
				if (arrow->type == 3)
				{
					AINFLp->up = atoi(arrow->name);//����д��up
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
						arrow = shuzufuzhi(arrow);//���鸳ֵ
						arrow = houjishuzufuzhi(arrow);//������鸳ֵ
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
	if (arrow->type == 19)/*�ж�cout*/
	{
		Next(infile);
		arrow = arrow->next;
		if (arrow->type == 36)/*�ж�"<<"*/
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
	if (arrow->type == 20)/*�ж�cin*/
	{
		Next(infile);
		arrow = arrow->next;
		if (arrow->type == 37)/*�ж�">>"*/
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