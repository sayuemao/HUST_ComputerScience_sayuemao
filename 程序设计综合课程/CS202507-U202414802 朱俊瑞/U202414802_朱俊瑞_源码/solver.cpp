#include "solver.h"
#include "cnfparser.h"
#include<iostream>
#include <cstdio>
#include <cstring>
#include<ctime>
using namespace std;
extern clock_t begintime,begintime2;
extern double maxtime;
extern bool overtime,overtime2;
extern int boolnum, clausenum;
extern int* finalassignment;
double activity_factor = 1.0;   //增长因子
const double decay_factor = 0.5;  // 衰减因子
//未优化
clause* findsoloclause(clause* tempclause)  //遍历寻找第一个出现的单子句
{
	while (tempclause)
	{
		if (tempclause->firstunit == NULL)
		{
			tempclause = tempclause->nextclause;
			continue;
		}
		if (tempclause->firstunit->nextunit == NULL)
		{
			return tempclause;
		}
		tempclause = tempclause->nextclause;
	}
	return NULL;
}
void simplify(clause*& clausearr,int* boolcountarr,int unitnum)  //化简S
{
	clause* preclause = NULL;
	clause* tempclause = clausearr;
	while (tempclause)   //查找删除含unitnum的子句
	{
		bool shouldelete = false;
		unit* tempunit = tempclause->firstunit;
		while (tempunit)
		{
			if (tempunit->unitnum == unitnum)
			{
				shouldelete = true;
				break;
			}
			tempunit = tempunit->nextunit;
		}
		if (shouldelete)
		{
			unit* tempunit = tempclause->firstunit;
			while (tempunit)
			{
				unit* t = tempunit;
				tempunit = tempunit->nextunit;
				--boolcountarr[t->unitnum + boolnum];
				free(t);
			}
			if (preclause == NULL)
			{
				clausearr = tempclause->nextclause;
				free(tempclause);
				tempclause = clausearr;
			}
			else
			{
				preclause->nextclause = tempclause->nextclause;
				free(tempclause);
				tempclause = preclause->nextclause;
			}
		}
		else
		{
			preclause = tempclause;
			tempclause = tempclause->nextclause;
		}
	}
	if (clausearr == NULL)
	{
		return;
	}
	unitnum = -unitnum;
	tempclause = clausearr;
	while (tempclause)
	{
		unit* tempunit = tempclause->firstunit;
		unit* preunit = NULL;
		while (tempunit)
		{
			if (tempunit->unitnum == unitnum)
			{
				--boolcountarr[unitnum + boolnum];
				if (preunit == NULL)
				{
					tempclause->firstunit = tempunit->nextunit;
					free(tempunit);
					tempunit = tempclause->firstunit;
				}
				else
				{
					preunit->nextunit = tempunit->nextunit;
					free(tempunit);
					tempunit = preunit->nextunit;
				}
			}
			else
			{
				preunit = tempunit;
				tempunit = tempunit->nextunit;
			}
		}
		tempclause = tempclause->nextclause;
	}
}
int chooseordinary(int* boolcountarr)  //遍历寻找出现次数最多的变元 ，此处变元区分符号
{
	int maxunitnum = 1;
	int maxnum = -1;
	for (int i = 0; i <= 2 * boolnum; i++)
	{
		if (i - boolnum == 0) continue;
		if (boolcountarr[i] > maxnum)
		{
			maxnum = boolcountarr[i];
			maxunitnum = i - boolnum;
		}
	}
	return maxunitnum;
}
clause* copy(clause* clausearr)
{
	clause* newclausearr = NULL;
	
	clause* tempclause = clausearr;
	clause* preclause = NULL;
	while (tempclause)
	{
		clause* nowclause = (clause*)malloc(sizeof(clause));
		nowclause->nextclause = NULL;
		nowclause->firstunit = NULL;

		unit* tempunit = tempclause->firstunit;
		unit* preunit = NULL;
		while (tempunit)
		{
			unit* nowunit = (unit*)malloc(sizeof(unit));
			nowunit->nextunit = NULL;
			nowunit->unitnum = tempunit->unitnum;
			if (preunit == NULL)
			{
				nowclause->firstunit = nowunit;
			}
			else
			{
				preunit->nextunit = nowunit;
			}
			preunit = nowunit;
			tempunit = tempunit->nextunit;
		}

		if (preclause == NULL)
		{
			newclausearr = nowclause;
		}
		else
		{
			preclause->nextclause = nowclause;
		}
		preclause = nowclause;
		tempclause = tempclause->nextclause;
	}
	return newclausearr;
}
int* copycountarr(int* boolcountarr)
{
	int total = 2 * boolnum + 5;
	int* newcountarr = (int*)malloc(sizeof(int) * total);
	for (int i = 0; i < total; i++)
	{
		newcountarr[i] = boolcountarr[i];
	}
	return newcountarr;
}
void freeclausearr(clause* clausearr)
{
	while (clausearr)
	{
		unit* t = clausearr->firstunit;
		while (t)
		{
			unit* next = t->nextunit;
			free(t);
			t = next;
		}
		clause* nextclausearr = clausearr->nextclause;
		free(clausearr);
		clausearr = nextclausearr;
	}
}
bool DPLLordinary(clause*& clausearr,int* boolcountarr,int* assignment)   //普通未优化DPLL:采用链表数据结构，使用递归解决问题，遍历寻找单子句和出现次数最多的变元
{
	if (overtime)
	{
		return false;
	}
	time_t nowtime = clock();
	if ( (double)(nowtime-begintime)/CLOCKS_PER_SEC > maxtime)
	{
		overtime = true;
		return false;
	}
	int* nowcountarr = copycountarr(boolcountarr);
	clause* tempclause = findsoloclause(clausearr);
	while (tempclause)
	{
		int unit = tempclause->firstunit->unitnum;
		int absunit = abs(unit);
		int value = (unit > 0) ? 1 : -1;
		assignment[absunit] = value;
		simplify(clausearr, nowcountarr,unit);
		if (clausearr == NULL)
		{
			free(nowcountarr);
			for (int i = 1; i <= boolnum; i++)
			{
				finalassignment[i] = assignment[i];
			}
			return true;
		}
		else    //查找是否有空子句
		{
			clause* t = clausearr;
			while (t)
			{
				if (t->firstunit == NULL)
				{
					free(nowcountarr);
					return false;
				}
				t = t->nextclause;
			}
		}
		tempclause = findsoloclause(clausearr);
	}
	int v = chooseordinary(nowcountarr);
	
	  //复制两份S

	clause* temp = (clause*)malloc(sizeof(clause));
	temp->firstunit = (unit*)malloc(sizeof(unit));
	temp->firstunit->nextunit = NULL;
	temp->firstunit->unitnum = v;
	temp->nextclause = copy(clausearr);
	
	int absv = abs(v);
	int oldvalue = assignment[absv];
	assignment[absv] = (v>0)?1:-1;
	++nowcountarr[v + boolnum];
	if (DPLLordinary(temp,nowcountarr,assignment))
	{
		free(nowcountarr);
		return true;
	}

	assignment[absv] = oldvalue;
	--nowcountarr[v + boolnum];
	freeclausearr(temp);

	clause* temp2 = (clause*)malloc(sizeof(clause));
	temp2->firstunit = (unit*)malloc(sizeof(unit));
	temp2->firstunit->nextunit = NULL;
	temp2->firstunit->unitnum = -v;
	temp2->nextclause = copy(clausearr);

	assignment[absv] = (v > 0) ? -1 : 1;
	++nowcountarr[-v + boolnum];
	bool result = DPLLordinary(temp2,nowcountarr,assignment);
	free(nowcountarr);
	if (result)
	{
		return true;
	}
	else
	{
		assignment[absv] = oldvalue;
		freeclausearr(temp2);
		return false;
	}
}

//优化后
void initsolver(Solver*& solver)//初始化solver
{
	solver = (Solver*)malloc(sizeof(Solver));
	memset(solver, 0, sizeof(Solver));
	solver->boolnum = boolnum;
	solver->clausenum = clausenum;
	solver->assignment = (int*)malloc(sizeof(int) * (solver->boolnum + 1)); // 变量从1开始
	solver->level = (int*)malloc(sizeof(int) * (solver->boolnum + 1));
	solver->activity = (double*)malloc(sizeof(double) * (solver->boolnum + 1));
	solver->tried = (int*)malloc(sizeof(int) * (solver->boolnum + 1));
	solver->confictcount = 0;
	solver->currentlevel = 0;
	solver->stackptr = 0;
	solver->queuehead = 0;
	solver->queuetail = 0;
	solver->litmap = (Litmap*)malloc(sizeof(Litmap) * (solver->boolnum+1));
	for (int i = 0; i < solver->boolnum + 1; i++)
	{
		solver->litmap[i].pos = solver->litmap[i].neg = NULL;
		solver->litmap[i].pos_len = solver->litmap[i].neg_len = 0;
	}
	for (int i = 1; i <= solver->boolnum; i++)
	{
		solver->assignment[i] = 0;
		solver->level[i] = -1;
		solver->activity[i] = 1.0;
		solver->tried[i] = 0;
	}
}
void decayactivity(Solver* solver) //衰减活动度
{
	for (int i = 1; i <= solver->boolnum; i++) 
	{
		solver->activity[i] *= decay_factor;
	}
	activity_factor *= decay_factor;
}
void conflictoccur(Solver* solver)//冲突发生时增加冲突数量
{
	solver->confictcount++;
	if (solver->confictcount % 50 == 0)
	{
		decayactivity(solver);
	}	
}
void addclause(Solver*& solver, int* literals, int count)//添加子句
{
	memcpy(solver->original.clauses[solver->original.count].literals, literals, count*sizeof(int));
	solver->original.clauses[solver->original.count].size = count;
	solver->original.count++;
}
void copyclauseset(ClauseSet* dest, ClauseSet* src) //复制子句集
{
	dest->count = src->count;
	for (int i = 0; i < src->count; i++)
	{
		dest->clauses[i].size = src->clauses[i].size;
		memcpy(dest->clauses[i].literals, src->clauses[i].literals,src->clauses[i].size * sizeof(int));
	}
}
void enqueue(Solver* solver, int lit) //入队
{
	if ((solver->queuetail + 1) % MAXSIZE == solver->queuehead) {
		cerr << "传播队列已满，无法入队：" << lit << endl;
		return;
	}
	solver->queue[solver->queuetail] = lit;
	solver->queuetail = (solver->queuetail + 1) % MAXSIZE;
}
int dequeue(Solver* solver) //出队
{
	int lit = solver->queue[solver->queuehead];
	solver->queuehead = (solver->queuehead + 1) % MAXSIZE;
	return lit;
}
int popstack(Solver* solver, int* abslit, int* value,int* level)//出栈
{
	if (solver->stackptr <= 0) {
		return 0;
	}
	solver->stackptr--;
	*abslit = solver->stack[solver->stackptr][0];
	*value = solver->stack[solver->stackptr][1];
	*level = solver->stack[solver->stackptr][2];
	return 1;
}
void pushstack(Solver* solver, int abslit, int value,int level) //入栈
{
	solver->stack[solver->stackptr][0] = abslit;
	solver->stack[solver->stackptr][1] = value;
	solver->stack[solver->stackptr][2] = level;
	solver->stackptr++;
}
void assign(Solver* solver, int abslit, int value, int level, int isdecision)//赋值变元 1 || -1
{
	if (solver->assignment[abslit] != 0)
	{
		return; 
	}
	solver->assignment[abslit] = value;
	solver->level[abslit] = level;
	if (isdecision) {
		pushstack(solver, abslit, value, level);
	}
	int opplit = (value == 1) ? -abslit : abslit;
	enqueue(solver, opplit);
}
void unassign(Solver* solver, int abslit) //取消赋值
{
	solver->assignment[abslit] = 0;
	solver->level[abslit] = -1;
}
int unit_propagation(Solver* solver) //单子句传播
{
	while (solver->queuehead != solver->queuetail) 
	{
		int lit = dequeue(solver);
		int abslit = abs(lit);
		int* targetclause;
		int targetlen;
		if (lit > 0)
		{
			targetclause = solver->litmap[abslit].pos;
			targetlen = solver->litmap[abslit].pos_len;
		}
		else
		{
			targetclause = solver->litmap[abslit].neg;
			targetlen = solver->litmap[abslit].neg_len;
		}
		for (int i = 0; i < targetlen; i++)
		{
			Clause* clause = &solver->working.clauses[targetclause[i]];
			int satisfied = 0;
			int unassigned_count = 0;
			int unassigned_lit = 0;
			for (int j = 0; j < clause->size; j++) 
			{
				int l = clause->literals[j];
				int v = abs(l);
				int val = (l > 0) ? 1 :-1;
				if (solver->assignment[v] == 0)
				{
					unassigned_count++;
					unassigned_lit = l;
				}
				else if (solver->assignment[v] == val)
				{
					satisfied = 1;
					break;
				}
			}
			if (satisfied) continue; 
			if (unassigned_count == 1) 
			{
				int u_var = abs(unassigned_lit);
				int u_val = (unassigned_lit > 0) ? 1 : -1;
				if (solver->assignment[u_var] == -u_val) 
				{
					for (int p = 0; p < clause->size; p++)
					{
						int absliteral = abs(clause->literals[p]);
						solver->activity[absliteral]+=activity_factor;
					}
					conflictoccur(solver);
					return 0; 
				}
				assign(solver, u_var, u_val, solver->currentlevel, 0);
			}
			else if (unassigned_count == 0) 
			{
				for (int p = 0; p < clause->size; p++)
				{
					int absliteral = abs(clause->literals[p]);
					solver->activity[absliteral] += activity_factor;
				}
				conflictoccur(solver);
				return 0; 
			}
		}
	}
	return 1; 
}
int select_variable(Solver* solver)//选择新变元
{
	double max_activity = -1;
	int selected_var = -1;
	for (int i = 1; i <= solver->boolnum; i++)
	{
		if (solver->assignment[i] == 0)
		{
			if (solver->activity[i] > max_activity || (solver->activity[i] == max_activity && selected_var == -1))
			{
				max_activity = solver->activity[i];
				selected_var = i;
			}
		}
	}
	return selected_var;
}
int is_satisfied(Solver* solver) //检查是否满足
{
	for (int i = 0; i < solver->working.count; i++) 
	{
		Clause* clause = &solver->working.clauses[i];
		int satisfied = 0;
		for (int j = 0; j < clause->size; j++) 
		{
			int l = clause->literals[j];
			int v = abs(l);
			int val = (l > 0) ? 1 : -1;
			if (solver->assignment[v] == val)
			{
				satisfied = 1;
				break;
			}
		}
		if (!satisfied)
		{
			return 0; 
		}
	}
	return 1; 
}
void backtrack(Solver* solver, int target_level) //回溯上一层状态
{
	for (int i = 1; i <= solver->boolnum; i++) 
	{
		if (solver->level[i] > target_level)
		{
			unassign(solver, i);
		}
	}
	while (solver->stackptr > 0)
	{
		int toplevel = solver->stack[solver->stackptr - 1][2];
		if (toplevel > target_level)
		{
			solver->stackptr--;
		}
		else
		{
			break;
		}
	}
	solver->queuehead = 0;
	solver->queuetail = 0;
	solver->currentlevel = target_level;
}
bool DPLLoptimize(Solver* solver)    //DPLL
{
	copyclauseset(&solver->working,&solver->original);
	for (int i = 0; i < solver->working.count; i++)
	{
		Clause* clause = &solver->working.clauses[i];
		if (clause->size == 1) 
		{ 
			int lit = clause->literals[0];
			int abslit = abs(lit);
			if (solver->assignment[abslit] == 0) 
			{
				int value = (lit > 0) ? 1 : -1;
				assign(solver, abslit, value, solver->currentlevel, 0);
			}
		}
	}
	if (!unit_propagation(solver)) 
	{
		return false; 
	}
	if (is_satisfied(solver)) 
	{
		return true; 
	}
	while (1) 
	{
		clock_t nowtime = clock();
		if ((double)(nowtime - begintime2) / CLOCKS_PER_SEC > maxtime)
		{
			overtime2 = true;
			return false;
		}
		int var = select_variable(solver);
		if (var == -1) 
		{
			if (is_satisfied(solver)) 
			{
				return 1; 
			}
			else 
			{ 
				while (solver->stackptr > 0)
				{
					int back_var, back_val, back_level;
					popstack(solver, &back_var, &back_val, &back_level);
					backtrack(solver, back_level - 1);
					solver->currentlevel = back_level;
					solver->tried[back_var]++;
					if (solver->tried[back_var] % 2 == 0)
					{
						continue;
					}
					int new_val = (back_val == 1) ? -1 : 1;
					assign(solver, back_var, new_val, solver->currentlevel, 1);
					if (unit_propagation(solver))
					{
						if (is_satisfied(solver)) 
							return true;			
						break;
					}
					backtrack(solver, back_level - 1);
					solver->tried[back_var]++;
				}
				if (solver->stackptr == 0) 
					return false; 
			}
		}
		else 
		{
			solver->currentlevel++;
			assign(solver, var, 1, solver->currentlevel, 1);		
			if (unit_propagation(solver))
			{
				if(is_satisfied(solver))
					return 1;	
				continue;
			}
			else 
			{			
				backtrack(solver, solver->currentlevel - 1);
				solver->currentlevel++;
				assign(solver, var, -1, solver->currentlevel, 1);				
				if (!unit_propagation(solver))
				{
					backtrack(solver, solver->currentlevel - 1);			
					while (solver->stackptr > 0) 
					{ 
						int back_var, back_val, back_level;
						popstack(solver, &back_var, &back_val, &back_level);
						backtrack(solver, back_level - 1);
						solver->currentlevel = back_level; 
						solver->tried[back_var]++;
						if (solver->tried[back_var] %2==0)
						{
							continue;
						}
						int new_val = (back_val == 1) ? -1 : 1;
						assign(solver, back_var, new_val, back_level, 1); 
						if (unit_propagation(solver)) 
						{
							if (is_satisfied(solver)) 
								return true;
							break; 
						}
						backtrack(solver, back_level - 1);
						solver->tried[back_var]++;
					}
					if (solver->stackptr == 0) 
					{
						return false;
					}
				}
				else {
					if (is_satisfied(solver)) 
					{
						return 1;
					}
				}
			}
		}
	}

}