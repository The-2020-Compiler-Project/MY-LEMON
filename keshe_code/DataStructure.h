#include <iostream>
#include <fstream>
#include <stdio.h>
#include<vector>
#include <stdlib.h>
#include <string.h>

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