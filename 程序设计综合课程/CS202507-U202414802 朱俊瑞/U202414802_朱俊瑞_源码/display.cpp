#include "display.h"
#include "cnfparser.h"
#include<iostream>
#include <cstdio>
using namespace std;
extern int boolnum, clausenum;

void showclause(clause* clausearr)
{
    printf("变元数量：%d 子句数量：%d\n", boolnum, clausenum);   //已debug可完整输入
    clause* tempclause = clausearr;
    while (tempclause)
    {
        unit* tempunit = tempclause->firstunit;
        while (tempunit)
        {
            printf("%d ", tempunit->unitnum);
            tempunit = tempunit->nextunit;
        }
        printf("\n");
        tempclause = tempclause->nextclause;
    }
}