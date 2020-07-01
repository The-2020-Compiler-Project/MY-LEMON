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
	if (arrow->type == 45);//�ж�#,45Ϊ��#����Token���е�˳�����
	else {
		arrow = Chengxu(arrow); 
		arrow = Houjichengxu(arrow); 

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
			if (arrow->type == 0)//�жϱ�ʶ��
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
					arrow = canshuliebiao(arrow);
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

			}
			else error();

		}//�жϱ�ʶ��
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