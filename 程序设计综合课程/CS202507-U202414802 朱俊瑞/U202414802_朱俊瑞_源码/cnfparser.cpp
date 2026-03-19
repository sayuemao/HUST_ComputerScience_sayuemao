#include "cnfparser.h"
#include "solver.h"
#include<iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctype.h>
using namespace std;
#ifndef MAXSIZE
#define MAXSIZE 20000
#endif // !MAXSIZE

extern int boolnum, clausenum;

void scancnf(string filename,int*& boolcountarr,clause*& clausearr,Solver*& solver)  //输入标准cnf文件
{
	FILE* fp = fopen(filename.c_str(), "r");
	if (fp == NULL)
	{
		perror("文件打开失败\n");
		return;
	}

	char firstchar;
	char buffer[300];
	do
	{
		firstchar = getc(fp);
		if (firstchar == 'p')
		{
			fgets(buffer, 6, fp);
			break;
		}
		else if (firstchar == 'c')
		{
			char* result = fgets(buffer, sizeof(buffer), fp);
			if (result == NULL)
			{
				if (feof(fp) || ferror(fp))
				{
					perror("该cnf不含有子句");
					break;
				}
			}
		}
		else if (firstchar == EOF)
		{
			if (feof(fp) || ferror(fp))
			{
				perror("该cnf不含有子句");
				break;
			}
		}
	} while (true);

	fscanf(fp,"%d%d", &boolnum, &clausenum);

	boolcountarr = (int*)calloc(2 * boolnum + 5,sizeof(int));

	clause* nowclause = NULL;

	for (int i = 0; i < clausenum; i++)   // 读取子句里的变元
	{
		if (clausearr == NULL)
		{
			clausearr = (clause*)malloc(sizeof(clause));
			nowclause = clausearr;
			nowclause->firstunit = NULL;
			nowclause->nextclause = NULL;
		}
		else
		{			
			clause* tempclause = (clause*)malloc(sizeof(clause));
			tempclause->nextclause = NULL;
			tempclause->firstunit = NULL;
			nowclause->nextclause = tempclause;
			nowclause = tempclause;
		}

		int unitnum;
		fscanf(fp,"%d", &unitnum);
		while (unitnum)
		{
			unit* tempunit = (unit*)malloc(sizeof(unit));
			tempunit->unitnum = unitnum;
			tempunit->nextunit = nowclause->firstunit;
			nowclause->firstunit = tempunit;
			++boolcountarr[unitnum + boolnum];
			fscanf(fp,"%d", &unitnum);
		}	
	}



	//优化非递归DPLL 读取数据 
	initsolver(solver);
	clause* tempclause = clausearr;
	for (int i = 0; i < clausenum; i++)
	{
		if (tempclause == NULL) {
			printf("子句数量不足（预期%d，实际%d）\n", clausenum, i);
			return;
		}
		int* literals = (int*)malloc(sizeof(int)*MAXSIZE);
		int count = 0;
		unit* tempunit = tempclause->firstunit;
		while (tempunit)
		{		
			literals[count++] = tempunit->unitnum;
			int absunit = abs(tempunit->unitnum);
			if (tempunit->unitnum > 0)
			{
				if (solver->litmap[absunit].pos == NULL)
				{
					solver->litmap[absunit].pos = (int*)malloc(sizeof(int) * (clausenum + 1));
					solver->litmap[absunit].pos_len = 0;
				}
				solver->litmap[absunit].pos[solver->litmap[absunit].pos_len++] = i;
			}
			else
			{
				if (solver->litmap[absunit].neg == NULL)
				{
					solver->litmap[absunit].neg = (int*)malloc(sizeof(int) * (clausenum + 1));
					solver->litmap[absunit].neg_len = 0;
				}
				solver->litmap[absunit].neg[solver->litmap[absunit].neg_len++] = i;
			}
			tempunit = tempunit->nextunit;
		}
		addclause(solver,literals,count);
		free(literals);
		tempclause = tempclause->nextclause;
	}

	fclose(fp);
}