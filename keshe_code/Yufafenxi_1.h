#pragma once
#include<iostream>
#include <fstream>
#include<string>
#include<stack>
#include "fuhaobiao.h"
#include"Yuyi.h"

using namespace std;

string funcstring;
stack<string> SEM;
string funcname;

FILE *infile;
string word;
SYNBLTerm* SYNBLp;
int  AINFaddr = 0;
int  PFINFaddr = 0;
int OFF = 0;
int ARR[5] = { 0 };
int g = 0;
int UP;
int FN = 0;
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

//�ķ�����ʽ1
Token *Start(Token *  arrow)
{
	Next(infile);
	arrow = arrow->next;
	arrow = Chengxu(arrow);//���� ���� �ӳ���
	arrow = Houjichengxu(arrow);//���� ��̳��� �ӳ���
	return  arrow;
}

Token *Houjichengxu(Token *  arrow)
{
	if (arrow->type == 45);//�ж�#,45Ϊ��#����Token���е�˳�����
	else {
		arrow = Chengxu(arrow); //���� ���� �ӳ���
		arrow = Houjichengxu(arrow); //���� ��̳��� �ӳ���

	}
	return arrow;
}

Token *Chengxu(Token *  arrow)
{
	if (arrow->type == 4 || arrow->type == 7)//�ж�int ��void
	{
		word = arrow->content;//��word׼��д������ܱ�
		Next(infile);
		arrow = arrow->next;
		if (arrow->type == 17)//�ж�main
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
			//��д������

			Next(infile);
			arrow = arrow->next;
			if (arrow->type == 38)//�жϣ�
			{
				Next(infile);
				arrow = arrow->next;
				if (arrow->type == 39)//�жϣ�
				{

					Next(infile);
					arrow = arrow->next;
					if (arrow->type == 42)//�ж�{
					{
						Next(infile);
						arrow = arrow->next;
						arrow = fuheyuju(arrow);//���� ������� �ӳ���
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
		else if (arrow->type == 0)//�жϱ�ʶ��
		{
			SYNBLp = SYNBLHead[rangenum];
			while (strcmp(SYNBLp->content, arrow->content) != 0)
			{
				SYNBLp = SYNBLp->next;
			}
			if (SYNBLp->next != NULL) { printf("�ظ�����"); exit(0); }
			SYNBLp->type = word;
			SYNBLp->addr = OFF;

			Next(infile);
			arrow = arrow->next;
			if (arrow->type == 38)//�жϣ�
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

				//��д������
				Next(infile);
				arrow = arrow->next;
				arrow = canshuliebiao(arrow);//���� �����б� �ӳ���
				if (arrow->type == 39)//�жϣ�
				{
					Next(infile);
					arrow = arrow->next;
					if (arrow->type == 42)//�ж�{
					{
						Next(infile);
						arrow = arrow->next;
						arrow = fuheyuju(arrow);//���� ������� �ӳ���
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
				if (arrow->type == 40)//�ж�[
				{
					strcpy_s(SYNBLp->cat, "a");
					//SYNBLp->addr=++AINFaddr;
					SYNBLp->addr = OFF;
					AINFLp = new AINFLTerm();
					AINFLp->CTP = word;
					AINFLp->LOW = 0;
					AINFLp->next = NULL;
					AINFLp2->next = AINFLp;
					AINFLp2 = AINFLp2->next;

					//��д�����
					Next(infile);
					arrow = arrow->next;
					if (arrow->type == 3)//�ж�����
					{
						AINFLp->UP = atoi(arrow->content);//����д��up
						if (AINFLp->CTP == "int") AINFLp->CLEN = 4 * AINFLp->UP;
						if (AINFLp->CTP == "float") AINFLp->CLEN = 8 * AINFLp->UP;
						if (AINFLp->CTP == "bool" || AINFLp->CTP == "char") AINFLp->CLEN = AINFLp->UP;
						OFF = OFF + AINFLp->CLEN;
						SYNBLp->addr = OFF;

						Next(infile);
						arrow = arrow->next;
						if (arrow->type == 41)//�ж�]
						{
							Next(infile);
							arrow = arrow->next;
							arrow = shuzufuzhi(arrow);//���� ���鸳ֵ��� �ӳ���
							arrow = houjishuzufuzhi(arrow);//���� ������鸳ֵ��� �ӳ���

							return arrow;
						}
						else error();

					}
					else error();
				}
				else {
					if (arrow->type == 25)//�ж�=
					{
						strcpy_s(SYNBLp->cat, "v");
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
						strcpy_s(SYNBLp->cat, "v");
						arrow = houjibianliangshengming(arrow);//���� ��̱������� �ӳ���

						return arrow;
					}

				}//�ж�=
			}//�ж�[

		}//�жϱ�ʶ��
		else error();
	}
	else {
		if (arrow->type == 5 || arrow->type == 9 || arrow->type == 11)//�ж�bool float char
		{
			word = arrow->content;//��word׼��д������ܱ�
			Next(infile);
			arrow = arrow->next;
			if (arrow->type == 0)//�жϱ�ʶ��
			{
				SYNBLp = SYNBLHead[rangenum];
				while (strcmp(SYNBLp->content, arrow->content) != 0)
				{
					SYNBLp = SYNBLp->next;

				}
				if (SYNBLp->next != NULL)
				{
					printf("�ظ�����");
					exit(0);
				}
				SYNBLp->type = word;
				Next(infile);
				arrow = arrow->next;
				if (arrow->type == 38)//�жϣ�
				{
					SYNBLp->addr = OFF;
					if (word == "int") OFF = OFF + 4;
					if (word == "float")  OFF = OFF + 8;
					if (word == "bool" || AINFLp->CTP == "char")  OFF = OFF + 1;
					funcname = SYNBLp->content;
					SEND("hanshu", "_", "_", SYNBLp->content);
					strcpy_s(SYNBLp->cat, "f");
					SYNBLp->addr = OFF;
					//SYNBLp->addr=++PFINFaddr;
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

					//��д������
					Next(infile);
					arrow = arrow->next;
					arrow = canshuliebiao(arrow);//���� �����б� �ӳ���
					if (arrow->type == 39)//�жϣ�
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
					if (arrow->type == 40)//�ж�[
					{
						AINFLTerm* AINFLp = new AINFLTerm();
						AINFLp->CTP = word;
						AINFLp->LOW = 0;
						AINFLp->next = NULL;
						AINFLp2->next = AINFLp;
						AINFLp2 = AINFLp2->next;
						//��д�����
						Next(infile);
						arrow = arrow->next;
						if (arrow->type == 3)//�ж�����
						{
							AINFLp->UP = atoi(arrow->content);//����д��up
							if (AINFLp->CTP == "int") AINFLp->CLEN = 4 * AINFLp->UP;
							if (AINFLp->CTP == "float") AINFLp->CLEN = 8 * AINFLp->UP;
							if (AINFLp->CTP == "bool" || AINFLp->CTP == "char") AINFLp->CLEN = AINFLp->UP;
							OFF = OFF + AINFLp->CLEN;
							Next(infile);
							arrow = arrow->next;
							if (arrow->type == 41)//�ж�]
							{
								Next(infile);
								arrow = arrow->next;
								arrow = shuzufuzhi(arrow);//���� ���鸳ֵ��� �ӳ���
								arrow = houjishuzufuzhi(arrow);//���� ������鸳ֵ��� �ӳ���

								return arrow;
							}
							else error();

						}
						else error();
					}//�ж�[
					else {
						if (arrow->type == 25)//�ж�=
						{
							strcpy_s(SYNBLp->cat, "v");
							SYNBLp->addr = OFF;
							if (word == "int") OFF = OFF + 4;
							if (word == "float")  OFF = OFF + 8;
							if (word == "bool" || AINFLp->CTP == "char")  OFF = OFF + 1;
							SYNBLp->addr = OFF;                       //��ַָ��vall
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
							strcpy_s(SYNBLp->cat, "v");
							SYNBLp->addr = OFF;
							if (word == "int") OFF = OFF + 4;
							if (word == "float")  OFF = OFF + 8;
							if (word == "bool" || word == "char")  OFF = OFF + 1;
							SYNBLp->addr = OFF;            //��ַָ��vall

							arrow = houjibianliangshengming(arrow);//���� ��̱������� �ӳ���

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

Token *fuheyuju(Token *  arrow) //�������
{
	if (arrow->type == 42)//�ж�{
	{
		Next(infile);
		arrow = arrow->next;
	}
	else error();
	arrow = yujuliebiao(arrow); //���� ����б� �ӳ���
	if (arrow->type == 43)//�ж�}
	{
		Next(infile);
		arrow = arrow->next;
		return arrow;
	}
	else error();
}

Token *shuzufuzhi(Token* arrow)//���鸳ֵ
{
	if (strcmp(arrow->content, "=") == 0)
	{
		Next(infile);
		arrow = arrow->next;
		if (strcmp(arrow->content, "{") == 0)
		{
			Next(infile);
			arrow = arrow->next;
			arrow = zhiliebiao(arrow);//ֵ�б�
			if (strcmp(arrow->content, "}") == 0)
			{
				Next(infile);
				arrow = arrow->next;
			}
			else {
				error();
			}
		}
		else {
			error();
		}
	}
	return arrow;
}

Token* zhiliebiao(Token* arrow)//ֵ�б�
{
	while (1)
	{
		if (arrow->type == 3)//�ж��Ƿ�������
		{
			Next(infile);
			arrow = arrow->next;
			if (strcmp(arrow->content, ",") == 0)
			{
				Next(infile);
				arrow = arrow->next;
			}
			else {
				break;
			}
		}
		else {
			error();
		}
	}
	return arrow;
}

Token* suanshubiadashi(Token* arrow) {
	arrow = suanshuyinzi(arrow);
	while (1)
	{
		if (strcmp(arrow->content, "+") == 0)
		{
			Next(infile);
			arrow = arrow->next;
			arrow = suanshuxiang(arrow);
			GEQ("+");
		}
		else if (strcmp(arrow->content, "-") == 0)
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

Token* suanshuxiang(Token* arrow)//������
{
	arrow = suanshuyinzi(arrow);//��������
	while (1)
	{
		if (strcmp(arrow->content, "*") == 0)
		{
			Next(infile);
			arrow = arrow->next;
			arrow = suanshuyinzi(arrow);
			GEQ("*");
		}
		else if (strcmp(arrow->content, "/") == 0)
		{
			Next(infile);
			arrow = arrow->next;
			arrow = suanshuyinzi(arrow);
			GEQ("/");
		}
		else if (strcmp(arrow->content, "%") == 0)
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

Token* suanshuyinzi(Token* arrow)//��������
{
	while (1) {
		if (strcmp(arrow->content, "+") == 0)
		{
			Next(infile);
			arrow = arrow->next;
			arrow = suanshudanyuan(arrow);
			GEQSingle("+");
		}
		else if (strcmp(arrow->content, "-") == 0)
		{
			Next(infile);
			arrow = arrow->next;
			arrow = suanshudanyuan(arrow);
			GEQSingle("-");
		}
		else if (strcmp(arrow->content, "!") == 0)
		{
			Next(infile);
			arrow = arrow->next;
			arrow = suanshudanyuan(arrow);
			GEQSingle("!");
		}
		else
		{
			break;
		}
	}/*while���*/
	return arrow;
}

Token* suanshudanyuan(Token* arrow)//������Ԫ
{
	string newString = "";
	funcstring = "";
	if (arrow->type == 0)//�ж��Ƿ��Ǳ�ʶ��
	{
		SYNBLp = SYNBLHead[rangenum];
		while (strcmp(SYNBLp->content, arrow->content) != 0 && SYNBLp->next != NULL)
		{
			SYNBLp = SYNBLp->next;
		}
		if (SYNBLp->type == " ")
		{
			SYNBLp = SYNBLHead[0];
			while (strcmp(SYNBLp->content, arrow->content) != 0 && SYNBLp->next != NULL)
			{
				SYNBLp = SYNBLp->next;
			}
			if (SYNBLp->type == " " || strcmp(SYNBLp->content, arrow->content) != 0)
			{
				cout << "����δ�������" << endl; exit(0);
			}
		}

		newString = arrow->content;
		funcstring = arrow->content;
		Next(infile);
		arrow = arrow->next;
		if (strcmp(arrow->content, "[") == 0)//�ж��Ƿ���[
		{
			newString = newString + '[';
			Next(infile);
			arrow = arrow->next;
			if (arrow->type == 3)//�ж��Ƿ�������
			{
				newString = newString + (arrow->content);
				Next(infile);
				arrow = arrow->next;
				if (strcmp(arrow->content, "]") == 0)//�ж��Ƿ���]
				{
					newString = newString + ']';
					Next(infile);
					arrow = arrow->next;
				}
				else
				{
					error();
				}
			}
			else
			{
				error();
			}
		}

		else

		{
			if (strcmp(arrow->content, "(") == 0)//�ж��Ƿ���(
			{
				string funcflag = SYNBLp->content;

				Next(infile);
				arrow = arrow->next;
				if (strcmp(arrow->content, ")") != 0)
				{
					SEND("temp", "_", "_", funcflag);
					arrow = suanshubiaodashi(arrow);
					SEND("xingcan", "_", "_", funcstring);
					if (strcmp(arrow->content, ",") == 0)
					{
						do
						{
							Next(infile);
							arrow = arrow->next;
							arrow = suanshubiaodashi(arrow);
							SEND("xingcan", "_", "_", funcstring);
						} while (strcmp(arrow->content, ",") == 0);
					}
				}
				//�ж��Ƿ���,
				if (strcmp(arrow->content, ")") == 0)//�ж��Ƿ���)
				{
					SEND("diaoyong", "_", "_", funcflag);
					Next(infile);
					arrow = arrow->next;
				}
				else  error();


			}
		}


	}
	else if ((arrow->type == 1) || (arrow->type == 2) || (arrow->type == 3))//����
	{
		newString = arrow->content;
		funcstring = arrow->content;
		Next(infile);
		arrow = arrow->next;
	}

	else if (strcmp(arrow->content, "(") == 0)
	{
		Next(infile);
		arrow = arrow->next;
		arrow = luojiyuju(arrow);
		if (strcmp(arrow->content, ")") == 0)
		{
			Next(infile);
			arrow = arrow->next;
			return arrow;
		}
		else
		{
			error();
		}
	}
	else
	{
		error();
	}
	SEM.push(newString);
	return arrow;
}

Token *yujuliebiao(Token*  arrow) //����б�
{
	if (arrow->type == 12)//�ж�if
	{
		Next(infile);
		arrow = arrow->next;
		if (arrow->type == 38)//�жϣ�
		{
			Next(infile);
			arrow = arrow->next;
			arrow = luojiyuju(arrow); //���� �߼���� �ӳ���
			if (arrow->type == 39)//�жϣ�
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
			else error();
		}
		else error();
	}
	else {
		if (arrow->type == 14)//�ж�while
		{
			WHILE();//����WHILE��Ԫʽ
			Next(infile);
			arrow = arrow->next;
			if (arrow->type == 38)//�жϣ�
			{
				Next(infile);
				arrow = arrow->next;
				arrow = luojiyuju(arrow); //���� �߼���� �ӳ���
				if (arrow->type == 39)//�жϣ�
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
				else error();
			}
			else error();
		}
		else {
			if (arrow->type == 18)//�ж�return
			{
				Next(infile);
				arrow = arrow->next;
				if (arrow->type == 0)//�жϱ�ʶ��
				{
					SYNBLp = SYNBLHead[rangenum];
					while (strcmp(SYNBLp->content, arrow->content) != 0 && SYNBLp->next != NULL)
					{
						SYNBLp = SYNBLp->next;
					}
					if (SYNBLp->type == " ")
					{
						SYNBLp = SYNBLHead[0];
						while (strcmp(SYNBLp->content, arrow->content) != 0 && SYNBLp->next != NULL)
						{
							SYNBLp = SYNBLp->next;
						}
						if (SYNBLp->type == " " || strcmp(SYNBLp->content, arrow->content) != 0)
						{
							printf("����δ�������"); exit(0);
						}
					}
					if (strcmp(SYNBLp->cat, "v") == 0 || strcmp(SYNBLp->cat, "p") == 0)
					{
						SEND("re", "_", SYNBLp->content, funcname);
					}
					Next(infile);
					arrow = arrow->next;
					if (arrow->type == 37)//�ж�;
					{
						Next(infile);
						arrow = arrow->next;
						arrow = yujuliebiao(arrow); //���� ����б� �ӳ���
						return arrow;
					}
					else error();
				}
				else {
					if (arrow->type == 1 || arrow->type == 2 || arrow->type == 3)//�жϳ���
					{
						SEND("re", "_", arrow->content, funcname);
						Next(infile);
						arrow = arrow->next;
						if (arrow->type == 37)//�ж�;
						{
							Next(infile);
							arrow = arrow->next;
							arrow = yujuliebiao(arrow); //���� ����б� �ӳ���
							return arrow;
						}
						else error();
					}
					else error();
				}
			}
			else {
				if (arrow->type == 0)//�жϱ�ʶ��
				{
					string newString = "";
					newString = arrow->content;
					while (arrow->type == 0)//�жϱ�ʶ��
					{
						SYNBLp = SYNBLHead[rangenum];
						while (strcmp(SYNBLp->content, arrow->content) != 0 && SYNBLp->next != NULL)
						{
							SYNBLp = SYNBLp->next;
						}
						if (SYNBLp->type == " ")
						{
							SYNBLp = SYNBLHead[0];
							while (strcmp(SYNBLp->content, arrow->content) != 0 && SYNBLp->next != NULL)
							{
								SYNBLp = SYNBLp->next;
							}
							if (SYNBLp->type == " " || strcmp(SYNBLp->content, arrow->content) != 0)
							{
								printf("����δ�������"); exit(0);
							}
						}

						Next(infile);
						arrow = arrow->next;
						if (arrow->type == 40)//�ж�[
						{
							newString = newString + '[';
							Next(infile);
							arrow = arrow->next;
							if (arrow->type == 3)//�ж�����
							{
								newString = newString + (arrow->content);
								Next(infile);
								arrow = arrow->next;
							}
							else error();
							if (arrow->type == 41)//�ж�]
							{
								newString = newString + ']';
								Next(infile);
								arrow = arrow->next;
							}
							else error();
						}
						if (arrow->type == 25)//�ж�=
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
							if (arrow->type != 36)//�ж�,
								break;
						}
						else error();
					}
					if (arrow->type == 37)//�ж�;
					{
						Next(infile);
						arrow = arrow->next;
						arrow = yujuliebiao(arrow); //���� ����б� �ӳ���
						return arrow;
					}
					else  error();
				}

				if (arrow->type == 7 || arrow->type == 5 || arrow->type == 9 || arrow->type == 11)//�ж�char int bool float
				{
					word = arrow->content;//��type׼��д������ܱ�
					Next(infile);
					arrow = arrow->next;
					if (arrow->type == 0)//�жϱ�ʶ��
					{
						SYNBLp = SYNBLHead[rangenum];
						while (strcmp(SYNBLp->content, arrow->content) != 0) { SYNBLp = SYNBLp->next; }
						if (SYNBLp->next != NULL) { printf("�ظ�����"); exit(0); }
						SYNBLp->type = word;
						Next(infile);
						arrow = arrow->next;
						if (arrow->type == 40)//�ж�[
						{
							strcpy_s(SYNBLp->cat, "a");
							SYNBLp->addr = OFF;
							//SYNBLp->addr=++AINFaddr;
							AINFLp = new AINFLTerm();
							AINFLp->CTP = word;
							AINFLp->LOW = 0;
							AINFLp->next = NULL;
							AINFLp2->next = AINFLp;
							AINFLp2 = AINFLp2->next;
							//��д�����
							Next(infile);
							arrow = arrow->next;
							if (arrow->type == 3)//�ж�����
							{
								AINFLp->UP = atoi(arrow->content);//����д��up
								if (AINFLp->CTP == "int") AINFLp->CLEN = 4 * AINFLp->UP;
								if (AINFLp->CTP == "float") AINFLp->CLEN = 8 * AINFLp->UP;
								if (AINFLp->CTP == "bool" || AINFLp->CTP == "char") AINFLp->CLEN = AINFLp->UP;
								OFF = OFF + AINFLp->CLEN;
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
								else error();
							}
							else error();
						}
						else {
							if (arrow->type == 25)//�ж�=
							{
								strcpy_s(SYNBLp->cat, "v");
								SYNBLp->addr = OFF;
								if (word == "int") OFF = OFF + 4;
								if (word == "float")  OFF = OFF + 8;
								if (word == "bool" || AINFLp->CTP == "char")  OFF = OFF + 1;
								SYNBLp->addr = OFF;                              //��ַָ��vall
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
								strcpy_s(SYNBLp->cat, "v");
								SYNBLp->addr = OFF;
								if (word == "int") OFF = OFF + 4;
								if (word == "float")  OFF = OFF + 8;
								if (word == "bool" || word == "char")  OFF = OFF + 1;                         //��ַָ��vall
								arrow = houjibianliangshengming(arrow); //���� ��̱������� �ӳ���
								arrow = yujuliebiao(arrow); //���� ����б� �ӳ���
								return arrow;
							}

						}
					}
					else error();

				}
				else return arrow;
			}
		}
	}

}
