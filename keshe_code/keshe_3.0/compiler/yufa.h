#pragma once
#include<iostream>
#include <fstream>
#include<string>
#include<sstream>
#include "Symbol_table.h"
#include"main_structure.h"
#include<ui_mainwindow.h>
#include"yuyi.h"
#include"cifa.h"
#include<QString>


using namespace std;
Ui::MainWindow* myui;
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
    myui->textEdit_2->append(QString::fromStdString(x));
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
    arrow = Chengxu(arrow);//  
    arrow = Houjichengxu(arrow);//  
    return  arrow;
}

Term* Houjichengxu(Term* arrow)
{
    if (arrow->type == 47);//§Ø#,45#Token§Ö
    else {
        arrow = Chengxu(arrow); //  
        arrow = Houjichengxu(arrow); //  

    }
    return arrow;
}

Term* Chengxu(Term* arrow)
{
    if (arrow->type == 4 || arrow->type == 7)//§Øint void
    {
        type = arrow->name;//word§Õ
        Next(infile);
        arrow = arrow->next;
        if (arrow->type == 17)//§Ømain
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
            //§Õ

            Next(infile);
            arrow = arrow->next;
            if (arrow->type == 40)//§Ø
            {
                Next(infile);
                arrow = arrow->next;
                if (arrow->type == 41)//§Ø
                {

                    Next(infile);
                    arrow = arrow->next;
                    //if (arrow->type == 42)//§Ø{
                    //{
                    //	Next(infile);
                    //	arrow = arrow->next;
                    arrow = fuheyuju(arrow);//  
                    oldrange = rangenum;
                    rangenum = 0;
                    FuncL_p->off = off;
                    SEND("END", "_", "_", "_");
                    return  arrow;
                    //}
                    //else logger("err");
                }
                else logger("err0");
            }
            else logger("err1");

        }
        else if (arrow->type == 0)//§Ø
        {
            SymL_p = SymL_Head[rangenum];
            while (strcmp(SymL_p->name, arrow->name) != 0)
            {
                SymL_p = SymL_p->next;
            }
            if (SymL_p->next != NULL) { printf(""); exit(0); }
            SymL_p->type = type;
            SymL_p->addr = off;

            Next(infile);
            arrow = arrow->next;
            if (arrow->type == 40)//§Ø
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

                //§Õ
                Next(infile);
                arrow = arrow->next;
                arrow = canshuliebiao(arrow);// §Ò 
                if (arrow->type == 41)//§Ø
                {
                    Next(infile);
                    arrow = arrow->next;
                    //if (arrow->type == 42)//§Ø{
                    //{
                    //	Next(infile);
                    //	arrow = arrow->next;
                    arrow = fuheyuju(arrow);//  
                    oldrange = rangenum;
                    rangenum = 0;
                    FuncL_p->off = off;
                    FuncL_p->fn = fn;

                    SEND("END", "_", "_", "_");
                    return  arrow;
                    /*}
                    else logger("err");*/
                }
                else logger("err2");
            }
            else {
                if (arrow->type == 42)//§Ø[
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

                    //§Õ
                    Next(infile);
                    arrow = arrow->next;
                    if (arrow->type == 3)//§Ø
                    {
                        AINFLp->up = atoi(arrow->name);//§Õup
                        if (AINFLp->tp == "int") AINFLp->len = 4 * AINFLp->up;
                        if (AINFLp->tp == "float") AINFLp->len = 8 * AINFLp->up;
                        if (AINFLp->tp == "bool" || AINFLp->tp == "char") AINFLp->len = AINFLp->up;
                        off = off + AINFLp->len;
                        SymL_p->addr = off;

                        Next(infile);
                        arrow = arrow->next;
                        if (arrow->type == 43)//§Ø]
                        {
                            Next(infile);
                            arrow = arrow->next;
                            arrow = shuzufuzhi(arrow);// Ô§ 
                            arrow = houjishuzufuzhi(arrow);// Ô§ 

                            return arrow;
                        }
                        else logger("err3");

                    }
                    else logger("err4");
                }
                else {
                    if (arrow->type == 27)//§Ø=
                    {
                        strcpy_s(SymL_p->cat, "v");
                        Next(infile);
                        arrow = arrow->next;
                        if (arrow->type == 1 || arrow->type == 2)
                        {
                            Next(infile);
                            arrow = arrow->next;
                            arrow = houjibianliangshengming(arrow);//  

                            return arrow;
                        }
                        else {
                            arrow = suanshubiaodashi(arrow);//  
                            arrow = houjibianliangshengming(arrow);//  

                            return arrow;
                        }
                    }
                    else {
                        strcpy_s(SymL_p->cat, "v");
                        arrow = houjibianliangshengming(arrow);//  

                        return arrow;
                    }

                }//§Ø=
            }//§Ø[

        }//§Ø
        else logger("err5");
    }
    else {
        if (arrow->type == 5 || arrow->type == 9 || arrow->type == 11)//§Øbool float char
        {
            type = arrow->name;//word§Õ
            Next(infile);
            arrow = arrow->next;
            if (arrow->type == 0)//§Ø
            {
                SymL_p = SymL_Head[rangenum];
                while (strcmp(SymL_p->name, arrow->name) != 0)
                {
                    SymL_p = SymL_p->next;

                }
                if (SymL_p->next != NULL)
                {
                    printf("");
                    exit(0);
                }
                SymL_p->type = type;
                Next(infile);
                arrow = arrow->next;
                if (arrow->type == 40)//§Ø
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

                    //§Õ
                    Next(infile);
                    arrow = arrow->next;
                    arrow = canshuliebiao(arrow);// §Ò 
                    if (arrow->type == 41)//§Ø
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
                    else logger("err6");
                }
                else {
                    if (arrow->type == 42)//§Ø[
                    {
                        Array_List* AINFLp = new Array_List();
                        AINFLp->tp = type;
                        AINFLp->low = 0;
                        AINFLp->next = NULL;
                        ArrL_p2->next = AINFLp;
                        ArrL_p2 = ArrL_p2->next;
                        //§Õ
                        Next(infile);
                        arrow = arrow->next;
                        if (arrow->type == 3)//§Ø
                        {
                            AINFLp->up = atoi(arrow->name);//§Õup
                            if (AINFLp->tp == "int") AINFLp->len = 4 * AINFLp->up;
                            if (AINFLp->tp == "float") AINFLp->len = 8 * AINFLp->up;
                            if (AINFLp->tp == "bool" || AINFLp->tp == "char") AINFLp->len = AINFLp->up;
                            off = off + AINFLp->len;
                            Next(infile);
                            arrow = arrow->next;
                            if (arrow->type == 43)//§Ø]
                            {
                                Next(infile);
                                arrow = arrow->next;
                                arrow = shuzufuzhi(arrow);// Ô§ 
                                arrow = houjishuzufuzhi(arrow);// Ô§ 

                                return arrow;
                            }
                            else logger("err7");

                        }
                        else logger("err8");
                    }//§Ø[
                    else {
                        if (arrow->type == 27)//§Ø=
                        {
                            strcpy_s(SymL_p->cat, "v");
                            SymL_p->addr = off;
                            if (type == "int") off = off + 4;
                            if (type == "float")  off = off + 8;
                            if (type == "bool" || AINFLp->tp == "char")  off = off + 1;
                            SymL_p->addr = off;                       //vall
                            Next(infile);
                            arrow = arrow->next;
                            if (arrow->type == 1 || arrow->type == 2)//§Ø
                            {
                                Next(infile);
                                arrow = arrow->next;
                                arrow = houjibianliangshengming(arrow);//  

                                return arrow;
                            }
                            else {
                                arrow = suanshubiaodashi(arrow);//  
                                arrow = houjibianliangshengming(arrow);//  

                                return arrow;
                            }
                        }
                        else {
                            strcpy_s(SymL_p->cat, "v");
                            SymL_p->addr = off;
                            if (type == "int") off = off + 4;
                            if (type == "float")  off = off + 8;
                            if (type == "bool" || type == "char")  off = off + 1;
                            SymL_p->addr = off;            //vall

                            arrow = houjibianliangshengming(arrow);//  

                            return arrow;
                        }

                    }


                }
            }
            else logger("err9");
        }
        else logger("err10");
    }
}

Term* fuheyuju(Term* arrow) //
{
    if (arrow->type == 44)//§Ø{
    {
        Next(infile);
        arrow = arrow->next;
    }
    else logger("err11");
    arrow = yujuliebiao(arrow); // §Ò 
    if (arrow->type == 45)//§Ø}
    {
        Next(infile);
        arrow = arrow->next;
        return arrow;
    }
    else logger("err12");
}

Term* shuzufuzhi(Term* arrow)//Ô§
{
    if (strcmp(arrow->name, "=") == 0)
    {
        Next(infile);
        arrow = arrow->next;
        if (strcmp(arrow->name, "{") == 0)
        {
            Next(infile);
            arrow = arrow->next;
            arrow = zhiliebiao(arrow);//§Ò
            if (strcmp(arrow->name, "}") == 0)
            {
                Next(infile);
                arrow = arrow->next;
            }
            else {
                logger("err13");
            }
        }
        else {
            logger("err14");
        }
    }
    return arrow;
}

Term* zhiliebiao(Term* arrow)//§Ò
{
    while (1)
    {
        if (arrow->type == 3)//§Ø
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
            logger("err15");
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

Term* suanshuxiang(Term* arrow)//
{
    arrow = suanshuyinzi(arrow);//
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

Term* suanshuyinzi(Term* arrow)//
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

Term* suanshudanyuan(Term* arrow)//
{
    string newString = "";
    funcstring = "";
    if (arrow->type == 0)//§Ø
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
                cout << "¦Ä" << endl; exit(0);
            }
        }

        newString = arrow->name;
        funcstring = arrow->name;
        Next(infile);
        arrow = arrow->next;
        if (strcmp(arrow->name, "[") == 0)//§Ø[
        {
            newString = newString + '[';
            Next(infile);
            arrow = arrow->next;
            if (arrow->type == 3)//§Ø
            {
                newString = newString + (arrow->name);
                Next(infile);
                arrow = arrow->next;
                if (strcmp(arrow->name, "]") == 0)//§Ø]
                {
                    newString = newString + ']';
                    Next(infile);
                    arrow = arrow->next;
                }
                else
                {
                    logger("err16");
                }
            }
            else
            {
                logger("err17");
            }
        }

        else

        {
            if (strcmp(arrow->name, "(") == 0)//§Ø(
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
                //§Ø,
                if (strcmp(arrow->name, ")") == 0)//§Ø)
                {
                    SEND("diaoyong", "_", "_", funcflag);
                    Next(infile);
                    arrow = arrow->next;
                }
                else  logger("err18");


            }
        }


    }
    else if ((arrow->type == 1) || (arrow->type == 2) || (arrow->type == 3))//
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
            logger("err19");
        }
    }
    else
    {
        logger("err20");
    }
    SEM.push(newString);
    return arrow;
}

Term* yujuliebiao(Term* arrow) //§Ò
{
    if (arrow->type == 12)//§Øif
    {
        Next(infile);
        arrow = arrow->next;
        if (arrow->type == 40)//§Ø
        {
            Next(infile);
            arrow = arrow->next;
            arrow = luojiyuju(arrow); //  
            if (arrow->type == 41)//§Ø
            {
                IF();//IF
                Next(infile);
                arrow = arrow->next;
                arrow = fuheyuju(arrow); //  
                if (arrow->type == 13)//§Øelse
                {
                    ELSE();//ELSE
                    Next(infile);
                    arrow = arrow->next;
                    arrow = fuheyuju(arrow); //  
                    IFEND();//IFEND
                    arrow = yujuliebiao(arrow); // §Ò 
                    return arrow;
                }
                else {
                    IFEND();  //IFEND
                    arrow = yujuliebiao(arrow); // §Ò 
                    return arrow;
                }
            }
            else logger("err21");
        }
        else logger("err22");
    }
    else {
        if (arrow->type == 14)//§Øwhile
        {
            WHILE();//WHILE
            Next(infile);
            arrow = arrow->next;
            if (arrow->type == 40)//§Ø
            {
                Next(infile);
                arrow = arrow->next;
                arrow = luojiyuju(arrow); //  
                if (arrow->type == 41)//§Ø
                {
                    DO();//DO
                    Next(infile);
                    arrow = arrow->next;
                    {
                        arrow = fuheyuju(arrow); //  
                        WHILEEND();//WHILEEND
                        arrow = yujuliebiao(arrow); // §Ò 
                        return arrow;
                    }
                }
                else logger("err23");
            }
            else logger("err24");
        }
        else {
            if (arrow->type == 18)//§Øreturn
            {
                Next(infile);
                arrow = arrow->next;
                if (arrow->type == 0)//§Ø
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
                            printf("¦Ä"); exit(0);
                        }
                    }
                    if (strcmp(SymL_p->cat, "v") == 0 || strcmp(SymL_p->cat, "p") == 0)
                    {
                        SEND("re", "_", SymL_p->name, funcname);
                    }
                    Next(infile);
                    arrow = arrow->next;
                    if (arrow->type == 39)//§Ø;
                    {
                        Next(infile);
                        arrow = arrow->next;
                        arrow = yujuliebiao(arrow); // §Ò 
                        return arrow;
                    }
                    else logger("err25");
                }
                else {
                    if (arrow->type == 1 || arrow->type == 2 || arrow->type == 3)//§Ø
                    {
                        SEND("re", "_", arrow->name, funcname);
                        Next(infile);
                        arrow = arrow->next;
                        if (arrow->type == 39)//§Ø;
                        {
                            Next(infile);
                            arrow = arrow->next;
                            arrow = yujuliebiao(arrow); // §Ò 
                            return arrow;
                        }
                        else logger("err26");
                    }
                    else logger("err27");
                }
            }
            else {
                if (arrow->type == 0)//§Ø
                {
                    string newString = "";
                    newString = arrow->name;
                    while (arrow->type == 0)//§Ø
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
                                printf("¦Ä"); exit(0);
                            }
                        }

                        Next(infile);
                        arrow = arrow->next;
                        if (arrow->type == 42)//§Ø[
                        {
                            newString = newString + '[';
                            Next(infile);
                            arrow = arrow->next;
                            if (arrow->type == 3)//§Ø
                            {
                                newString = newString + (arrow->name);
                                Next(infile);
                                arrow = arrow->next;
                            }
                            else logger("err28");
                            if (arrow->type == 43)//§Ø]
                            {
                                newString = newString + ']';
                                Next(infile);
                                arrow = arrow->next;
                            }
                            else logger("err29");
                        }
                        if (arrow->type == 27)//§Ø=
                        {
                            SEM.push(newString);
                            Next(infile);
                            arrow = arrow->next;
                            if (arrow->type == 1 || arrow->type == 2)//§Ø
                            {
                                Next(infile);
                                arrow = arrow->next;
                            }
                            else { arrow = suanshubiaodashi(arrow); }//  
                            ASSI(newString);
                            if (arrow->type != 38)//§Ø,
                                break;
                        }
                        else logger("err30");
                    }
                    if (arrow->type == 39)//§Ø;
                    {
                        Next(infile);
                        arrow = arrow->next;
                        arrow = yujuliebiao(arrow); // §Ò 
                        return arrow;
                    }
                    else  logger("err31");
                }

                if (arrow->type == 7 || arrow->type == 5 || arrow->type == 9 || arrow->type == 11)//§Øchar int bool float
                {
                    type = arrow->name;//type§Õ
                    Next(infile);
                    arrow = arrow->next;
                    if (arrow->type == 0)//§Ø
                    {
                        SymL_p = SymL_Head[rangenum];
                        while (strcmp(SymL_p->name, arrow->name) != 0) { SymL_p = SymL_p->next; }
                        if (SymL_p->next != NULL) { printf(""); exit(0); }
                        SymL_p->type = type;
                        Next(infile);
                        arrow = arrow->next;
                        if (arrow->type == 42)//§Ø[
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
                            //§Õ
                            Next(infile);
                            arrow = arrow->next;
                            if (arrow->type == 3)//§Ø
                            {
                                AINFLp->up = atoi(arrow->name);//§Õup
                                if (AINFLp->tp == "int") AINFLp->len = 4 * AINFLp->up;
                                if (AINFLp->tp == "float") AINFLp->len = 8 * AINFLp->up;
                                if (AINFLp->tp == "bool" || AINFLp->tp == "char") AINFLp->len = AINFLp->up;
                                off = off + AINFLp->len;
                                Next(infile);
                                arrow = arrow->next;
                                if (arrow->type == 43)//§Ø]
                                {
                                    Next(infile);
                                    arrow = arrow->next;
                                    arrow = shuzufuzhi(arrow);// Ô§ 
                                    arrow = houjishuzufuzhi(arrow);// Ô§ 
                                    arrow = yujuliebiao(arrow); // §Ò 
                                    return arrow;
                                }
                                else logger("err32");
                            }
                            else logger("err33");
                        }
                        else {
                            if (arrow->type == 25)//§Ø=
                            {
                                strcpy_s(SymL_p->cat, "v");
                                SymL_p->addr = off;
                                if (type == "int") off = off + 4;
                                if (type == "float")  off = off + 8;
                                if (type == "bool" || AINFLp->tp == "char")  off = off + 1;
                                SymL_p->addr = off;                              //vall
                                Next(infile);
                                arrow = arrow->next;
                                if (arrow->type == 1 || arrow->type == 2)//§Ø
                                {
                                    Next(infile);
                                    arrow = arrow->next;
                                    arrow = houjibianliangshengming(arrow); //  
                                    arrow = yujuliebiao(arrow); // §Ò 
                                    return arrow;
                                }
                                else {
                                    arrow = suanshubiaodashi(arrow);//  
                                    arrow = houjibianliangshengming(arrow); //  
                                    arrow = yujuliebiao(arrow); // §Ò 
                                    return arrow;
                                }
                            }
                            else {
                                strcpy_s(SymL_p->cat, "v");
                                SymL_p->addr = off;
                                if (type == "int") off = off + 4;
                                if (type == "float")  off = off + 8;
                                if (type == "bool" || type == "char")  off = off + 1;                         //vall
                                arrow = houjibianliangshengming(arrow); //  
                                arrow = yujuliebiao(arrow); // §Ò 
                                return arrow;
                            }

                        }
                    }
                    else logger("err34");
                }
                if (arrow->type == 19 || arrow->type == 20)
                {
                    arrow = shuchu(arrow);
                    arrow = shuru(arrow);
                    if (arrow->type == 39)//§Ø;
                    {
                        Next(infile);
                        arrow = arrow->next;
                        arrow = yujuliebiao(arrow); // §Ò 
                        return arrow;
                    }
                    else { logger("err35"); }
                }
                else return arrow;
            }
        }
    }

}

Term* putongbianliangshengming(Term* arrow)//
{
    if (arrow->type == 0)//§Ø
    {
        SymL_p = SymL_Head[rangenum];
        while (strcmp(SymL_p->name, arrow->name) != 0) { SymL_p = SymL_p->next; }
        if (SymL_p->next != NULL) { logger(""); exit(0); }
        SymL_p->type = type;
        Next(infile);
        arrow = arrow->next;
        if (strcmp(arrow->name, "=") == 0)
        {
            Next(infile);
            arrow = arrow->next;
            if ((arrow->type == 1) || (arrow->type == 2))//§Ø()
            {
                Next(infile);
                arrow = arrow->next;
            }
            else
            {
                arrow = suanshubiaodashi(arrow);//
            }
        }
        else {

        }
    }
    else
    {
        logger("err36");
    }
    strcpy_s(SymL_p->cat, "v");
    SymL_p->addr = off;
    if (type == "int") off = off + 4;
    if (type == "float")  off = off + 8;
    if (type == "bool" || type == "char")  off = off + 1;
    // SYNBLp->addr=OFF;                                               //vall
    return arrow;
}
Term* houjibianliangshengming(Term* arrow)//
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
        arrow = putongbianliangshengming(arrow);//
        arrow = houjibianliangshengming(arrow);//
    }
    else
    {
        logger("err37");
    }
    return arrow;
}

Term* canshuliebiao(Term* arrow)//§Ò
{
    if ((strcmp(arrow->name, "int") == 0) || (strcmp(arrow->name, "char") == 0) || (strcmp(arrow->name, "float") == 0) || (strcmp(arrow->name, "bool") == 0))
    {
        fn++;
        arrow = canshushengming(arrow);//
        arrow = houjicanshushengming(arrow);//
    }
    return arrow;
}

Term* canshushengming(Term* arrow)//
{
    if ((strcmp(arrow->name, "int") == 0) || (strcmp(arrow->name, "char") == 0) || (strcmp(arrow->name, "float") == 0) || (strcmp(arrow->name, "bool") == 0))
    {
        type = arrow->name;//type§Õ

        Next(infile);
        arrow = arrow->next;
        Parameter_List* PARAMp2 = new Parameter_List();
        strcpy_s(PARAMp2->name, arrow->name);
        if (arrow->type == 0)//§Ø
        {
            SEND("canshu", "_", "_", arrow->name);
            SymL_p = SymL_Head[rangenum];

            while (strcmp(SymL_p->name, arrow->name) != 0) { SymL_p = SymL_p->next; }
            if (SymL_p->next != NULL) { logger(""); exit(0); }
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
                //vall
                PARAMp2->next = NULL;
                Para_p->next = PARAMp2;
                Para_p = Para_p->next;
                Next(infile);
                arrow = arrow->next;
                if ((arrow->type == 1) || (arrow->type == 2))//§Ø()
                {
                    Next(infile);
                    arrow = arrow->next;
                }
                else
                {
                    strcpy_s(SymL_p->cat, "p");

                    //vall
                    arrow = suanshubiaodashi(arrow);//
                }
            }
            else { strcpy_s(SymL_p->cat, "p"); }
        }
        else
        {
            logger("err38");
        }
    }
    else
    {
        logger("err39");
    }
    return arrow;
}

Term* houjicanshushengming(Term* arrow)//
{
    if (strcmp(arrow->name, ",") == 0)
    {
        Next(infile);
        arrow = arrow->next;
        fn++;
        arrow = canshushengming(arrow);//
        arrow = houjicanshushengming(arrow);//
    }
    return arrow;
}

Term* luojiyuju(Term* arrow)//
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

Term* luojixiang(Term* arrow)//
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

Term* luojiyinzi1(Term* arrow)//1
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

Term* luojiyinzi2(Term* arrow)//2
{
    arrow = suanshubiaodashi(arrow);
    while (1)
    {
        if (strcmp(arrow->name, ">=") == 0)
        {
            Next(infile);
            arrow = arrow->next;
            arrow = suanshubiaodashi(arrow);//
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

Term* houjishuzufuzhi(Term* arrow)//Ô§
{
    if (strcmp(arrow->name, ",") == 0)
    {
        Next(infile);
        arrow = arrow->next;
        if (arrow->type == 0)//§Ø
        {
            while (strcmp(SymL_p->name, arrow->name) != 0) SymL_p = SymL_p->next;
            if (SymL_p->next != NULL) { printf(""); exit(0); }
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
                //§Õ
                Next(infile);
                arrow = arrow->next;
                if (arrow->type == 3)
                {
                    AINFLp->up = atoi(arrow->name);//§Õup
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
                        arrow = shuzufuzhi(arrow);//Ô§
                        arrow = houjishuzufuzhi(arrow);//Ô§
                    }
                    else
                    {
                        logger("err40");
                    }
                }
                else
                {
                    logger("err41");
                }
            }
            else
            {
                logger("err42");
            }
        }
        else
        {
            logger("err43");
        }
    }
    else if (strcmp(arrow->name, ";") == 0)
    {
        Next(infile);
        arrow = arrow->next;
    }
    else
    {
        logger("err44");
    }
    return arrow;
}

Term* shuchu(Term* arrow) {
    if (arrow->type == 19)/*§Øcout*/
    {
        Next(infile);
        arrow = arrow->next;
        if (arrow->type == 36)/*§Ø"<<"*/
        {
            Next(infile);
            arrow = arrow->next;
            if (arrow->type == 0)
            {
                SEND("cout", "_", "_", arrow->name);
                Next(infile);
                arrow = arrow->next;
            }
            else { logger("err45"); }
        }
        else { logger("err46"); }
    }

    return arrow;
}

Term* shuru(Term* arrow) {
    if (arrow->type == 20)/*§Øcin*/
    {
        Next(infile);
        arrow = arrow->next;
        if (arrow->type == 37)/*§Ø">>"*/
        {
            Next(infile);
            arrow = arrow->next;
            if (arrow->type == 0)
            {
                SEND("cin", "_", "_", arrow->name);
                Next(infile);
                arrow = arrow->next;
            }
            else { logger("err47"); }
        }
        else { logger("err48"); }
    }
    return arrow;
}
