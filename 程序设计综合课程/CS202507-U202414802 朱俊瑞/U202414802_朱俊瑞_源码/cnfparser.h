#pragma once
#ifndef CNFPARSER
#define CNFPARSER
#include<iostream>
#include<cstring>

#define MAXSIZE 20000
using namespace std;

struct unit
{
	int unitnum;
	unit* nextunit;
};

struct clause
{
	unit* firstunit;
	clause* nextclause;
};



struct Clause
{
    int literals[MAXSIZE];  // 子句中的文字
    int size;                    // 文字数量
};

// 子句集合
struct ClauseSet
{
    Clause clauses[MAXSIZE]; // 子句数组
    int count;                   // 子句数量
};

struct Litmap
{
    int* pos;  // 包含v的子句索引列表（子句在clauses数组中的索引）
    int* neg;  // 包含-v的子句索引列表
    int pos_len;  // pos数组长度
    int neg_len;  // neg数组长度
};
struct Solver
{
    int* assignment; // 变量赋值 (1-based索引)
    int* level;      // 变量赋值的决策层
    double* activity;   // 变量活动度，用于选择下一个变量
    int* tried;
    int confictcount;
    int boolnum;                // 变量总数
    int clausenum;             // 子句总数

    Litmap* litmap;

    ClauseSet original;           // 原始子句集
    ClauseSet working;            // 工作子句集，用于传播和化简

    // 用于非递归实现的栈和队列
    int stack[MAXSIZE][3];     // 决策栈 [变量][值]
    int stackptr;                // 栈指针

    int queue[MAXSIZE];        // 传播队列
    int queuehead;               // 队列头指针
    int queuetail;               // 队列尾指针

    int currentlevel;            // 当前决策层
};


void scancnf(string filename, int*& boolcountarr, clause*& clausearr,Solver*& solver);





#endif