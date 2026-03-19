#pragma once
#ifndef SOLVER
#define SOLVER
#include "cnfparser.h"

clause* findsoloclause(clause* tempclause);
void simplify(clause*& clausearr,int* boolcountarr, int unitnum);
int chooseordinary(int* boolcountarr);
clause* copy(clause* clausearr);
int* copycountarr(int* boolcountarr);
void freeclausearr(clause* clausearr);
bool DPLLordinary(clause*& clausearr, int* boolcountarr, int* assignmengt);

void initsolver(Solver*& solver);
void decayactivity(Solver* solver);
void conflictoccur(Solver* solver);
void addclause(Solver*& solver,int* literals,int count);
void copyclauseset(ClauseSet* dest, ClauseSet* src);
void enqueue(Solver* solver, int lit);
int dequeue(Solver* solver);
int popstack(Solver* solver, int* abslit, int* value,int* level);
void pushstack(Solver* solver, int abslit, int value,int level);
void assign(Solver* solver, int abslit, int value, int level, int isdecision);
void unassign(Solver* solver, int abslit);
int unit_propagation(Solver* solver);
int select_variable(Solver* solver);
int is_satisfied(Solver* solver);
void backtrack(Solver* solver, int target_level);
bool DPLLoptimize(Solver* solver);
#endif