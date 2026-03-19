#include "Sudoku.h"
#include "solver.h"
#include "cnfparser.h"
#include<iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include<ctime>
using namespace std;
extern int boolnum, clausenum;
extern bool overtime, overtime2;
extern clock_t begintime, begintime2;

void shufflenumber(int* num)
{
	for (int i = 9; i >0; i--)
	{
		int j = rand() % i +1;
		swap(num[i], num[j]);
	}
}
bool issatified(int chessboard[11][11],int x,int y,int num)
{
	if (num == 0)
		return true;
	for (int i = 1; i < 10; i++)
	{
		if (chessboard[x][i] == num) return false;
	}
	for (int i = 1; i < 10; i++)
	{
		if (chessboard[i][y] == num) return false;
	}

	int start_row = ((x - 1) / 3) * 3 + 1;  // 宫格起始行：1、4、7
	int start_col = ((y - 1) / 3) * 3 + 1;  // 宫格起始列：1、4、7

	for (int i = 0; i < 3; i++) 
	{
		for (int j = 0; j < 3; j++) 
		{
			if (chessboard[start_row + i][start_col + j] == num) 	return false;
		}
	}

	if (x + y == 10)
	{
		for (int i = 1; i < 10; i++)
		{
			if (chessboard[i][10 - i] == num) return false;
		}
	}
	if ((x > 1 && x < 5) && (y > 1 && y < 5))
	{
		for (int i = 2; i < 5; i++)
		{
			for (int j = 2; j < 5; j++)
			{
				if (chessboard[i][j] == num) return false;
			}
		}
	}
	if ((x > 5 && x < 9) && (y > 5 && y < 9))
	{
		for (int i = 6; i < 9; i++)
		{
			for (int j = 6; j < 9; j++)
			{
				if (chessboard[i][j] == num) return false;
			}
		}
	}
	return true;
}
bool generatebase(int chessboard[11][11])//回溯遍历生成终盘
{
	int x = 0, y=0;
	for (int i = 1; i < 10; i++)
	{
		for (int j = 1; j < 10; j++)
		{
			if (chessboard[i][j] == 0)
			{
				x = i;
				y = j;
				break;
			}
		}
	}
	if (x != 0 && y != 0)
	{
		int num[10] = { 0,1,2,3,4,5,6,7,8,9 };
		shufflenumber(num);
		for (int i = 1; i < 10; i++)
		{
			int number = num[i];
			if (issatified(chessboard,x, y, number))
			{
				chessboard[x][y] = number;
				if (generatebase(chessboard))
					return true;
				chessboard[x][y] = 0;
			}
		}
		return false;
	}
	else
	{
		return true;
	}
}
void shuffleplace(place* places)
{
	for (int i = 81; i > 0; i--)
	{
		int j = rand() % i + 1;
		swap(places[i], places[j]);
	}
}

int findsolutions(int chessboard[11][11])
{
	int x = 0, y = 0;
	for (int i = 1; i < 10; i++)
	{
		for (int j = 1; j < 10; j++)
		{
			if (chessboard[i][j] == 0)
			{
				x = i;
				y = j;
				break;
			}
		}
		if (x != 0) break;
	}
	if (x != 0 && y != 0)
	{
		int count = 0;
		for (int i = 1; i < 10; i++)
		{
			if (issatified(chessboard, x, y, i))
			{
				chessboard[x][y] = i;
				count += findsolutions(chessboard);
				chessboard[x][y] = 0;
				if (count >= 2) return count;
			}
		}
		return count;
	}
	else
	{
		return 1;
	}
}
void generatesudoku(int chessboard[11][11],int leftnumber)//挖洞法
{
	generatebase(chessboard);

	place places[85];
	int len = 0;
	for (int i = 1; i < 10; i++)
	{
		for (int j = 1; j < 10; j++)
		{
			places[++len] = { i,j };
		}
	}
	shuffleplace(places);
	int nowholesnumber = 0;
	int targetholesnumber = 81 - leftnumber;
	for (int i = 1; i < 82; i++)
	{
		if (nowholesnumber >= targetholesnumber)
		{
			break;
		}
		int x = places[i].x;
		int y = places[i].y;
		int temp = chessboard[x][y];
		chessboard[x][y] = 0;
		int copyboard[11][11];
		for (int k = 1; k < 10; k++)
		{
			for (int j = 1; j < 10; j++)
			{
				copyboard[k][j] = chessboard[k][j];
			}
		}
		int solution = findsolutions(copyboard);
		if (solution == 1)
		{
			nowholesnumber++;
		}
		else
		{
			chessboard[x][y] = temp;
		}
	}
}

void sudoku()
{
	while (1)
	{
		system("cls");
		printf("------------------------------------------------\n");
		printf("|                  请输入难度:                  |\n");
		printf("|   1.简单     2.中等      3.困难     4.极难    |\n");
		printf("|   0.退出                                      |\n");
		printf("------------------------------------------------\n");
		srand(time(0));
		int nandu = 0,leftnum;
		scanf("%d", &nandu);
		if (nandu == 0) break;
		if (nandu > 0 && nandu < 5)
		{
			srand(time(0));
			leftnum = 30;
			if (nandu == 1)
				leftnum = rand() % (35 - 30 + 1) + 30;
			else if (nandu == 2)
				leftnum = rand() % (30 - 25 + 1) + 25;
			else if (nandu == 3)
				leftnum = rand() % (25 - 20 + 1) + 20;
			else if (nandu == 4)
				leftnum = rand() % (20 - 17 + 1) + 17;
			int chessboard[11][11],answer[11][11];
			memset(chessboard, 0, sizeof(chessboard));
			generatesudoku(chessboard,leftnum);

			int renewchessboard[11][11];
			for (int i = 1; i < 10; i++)
			{
				for (int j = 1; j < 10; j++)
				{
					renewchessboard[i][j] = chessboard[i][j];
				}
			}

			Solver* solver = NULL;
			boolnum = 729;
			clausenum = MAXSIZE;
			initsolver(solver);
			int clausetotal = 0;

			//根据约束添加子句
			//格约束
			int* literals = (int*)malloc(sizeof(int) * 10);
			for (int i = 1; i < 10; i++)//能填
			{
				for (int j = 1; j < 10; j++)
				{
					for (int k = 1; k < 10; k++)
					{
						literals[k-1] = (i-1)*81 + (j-1) * 9 + k;
						int lit = literals[k-1];
						if (solver->litmap[lit].pos == NULL)//添加变元-子句映射表
						{
							solver->litmap[lit].pos = (int*)malloc(sizeof(int) * clausenum);
							solver->litmap[lit].pos_len = 0;
						}
						solver->litmap[lit].pos[solver->litmap[lit].pos_len++] = solver->original.count;
					}
					addclause(solver, literals, 9);
				}
			}		
			for (int i = 1; i < 10; i++)//不能同时填
			{
				for (int j = 1; j < 10; j++)
				{
					for (int k = 1; k < 9; k++)
					{
						literals[0] = -((i - 1) * 81 + (j - 1) * 9 + k);
						int abslit = abs(literals[0]);
						if (solver->litmap[abslit].neg == NULL)//添加变元-子句映射表
						{
							solver->litmap[abslit].neg = (int*)malloc(sizeof(int) * clausenum);
							solver->litmap[abslit].neg_len = 0;
						}
						solver->litmap[abslit].neg[solver->litmap[abslit].neg_len++] = solver->original.count;
						for (int p = k + 1; p < 10; p++)
						{
							literals[1] = -((i - 1) * 81 + (j - 1) * 9 + p);
							int abslit2 = abs(literals[1]);
							if (solver->litmap[abslit2].neg == NULL)//添加变元-子句映射表
							{
								solver->litmap[abslit2].neg = (int*)malloc(sizeof(int) * clausenum);
								solver->litmap[abslit2].neg_len = 0;
							}
							solver->litmap[abslit2].neg[solver->litmap[abslit2].neg_len++] = solver->original.count;
							addclause(solver, literals, 2);
						}
					}
				}
			}
			//行约束
			for (int i = 1; i < 10; i++)
			{
				for (int j = 1; j < 10; j++)
				{
					for (int k = 1; k < 10; k++)
					{
						literals[k-1] = (i - 1) * 81 + (k - 1) * 9 + j;
						int lit = literals[k-1];
						if (solver->litmap[lit].pos == NULL)//添加变元-子句映射表
						{
							solver->litmap[lit].pos = (int*)malloc(sizeof(int) * clausenum);
							solver->litmap[lit].pos_len = 0;
						}
						solver->litmap[lit].pos[solver->litmap[lit].pos_len++] = solver->original.count;
					}
					addclause(solver, literals, 9);
				}
			}
			for (int i = 1; i < 10; i++)
			{
				for (int j = 1; j < 10; j++)
				{
					for (int k = 1; k < 9; k++)
					{
						literals[0] = -((i - 1) * 81 + (k - 1) * 9 + j);
						int abslit = abs(literals[0]);
						if (solver->litmap[abslit].neg == NULL)//添加变元-子句映射表
						{
							solver->litmap[abslit].neg = (int*)malloc(sizeof(int) * clausenum);
							solver->litmap[abslit].neg_len = 0;
						}
						solver->litmap[abslit].neg[solver->litmap[abslit].neg_len++] = solver->original.count;
						for (int p = k + 1; p < 10; p++)
						{
							literals[1] = -((i - 1) * 81 + (p - 1) * 9 + j);
							int abslit2 = abs(literals[1]);
							if (solver->litmap[abslit2].neg == NULL)//添加变元-子句映射表
							{
								solver->litmap[abslit2].neg = (int*)malloc(sizeof(int) * clausenum);
								solver->litmap[abslit2].neg_len = 0;
							}
							solver->litmap[abslit2].neg[solver->litmap[abslit2].neg_len++] = solver->original.count;
							addclause(solver, literals, 2);
						}
					}
				}
			}
			//列约束
			for (int i = 1; i < 10; i++)
			{
				for (int j = 1; j < 10; j++)
				{
					for (int k = 1; k < 10; k++)
					{
						literals[k - 1] = (k - 1) * 81 + (i - 1) * 9 + j;
						int lit = literals[k - 1];
						if (solver->litmap[lit].pos == NULL)//添加变元-子句映射表
						{
							solver->litmap[lit].pos = (int*)malloc(sizeof(int) * clausenum);
							solver->litmap[lit].pos_len = 0;
						}
						solver->litmap[lit].pos[solver->litmap[lit].pos_len++] = solver->original.count;
					}
					addclause(solver, literals, 9);
				}
			}
			for (int i = 1; i < 10; i++)
			{
				for (int j = 1; j < 10; j++)
				{
					for (int k = 1; k < 9; k++)
					{
						literals[0] = -((k - 1) * 81 + (i - 1) * 9 + j);
						int abslit = abs(literals[0]);
						if (solver->litmap[abslit].neg == NULL)//添加变元-子句映射表
						{
							solver->litmap[abslit].neg = (int*)malloc(sizeof(int) * clausenum);
							solver->litmap[abslit].neg_len = 0;
						}
						solver->litmap[abslit].neg[solver->litmap[abslit].neg_len++] = solver->original.count;
						for (int p = k + 1; p < 10; p++)
						{
							literals[1] = -((p - 1) * 81 + (i - 1) * 9 + j);
							int abslit2 = abs(literals[1]);
							if (solver->litmap[abslit2].neg == NULL)//添加变元-子句映射表
							{
								solver->litmap[abslit2].neg = (int*)malloc(sizeof(int) * clausenum);
								solver->litmap[abslit2].neg_len = 0;
							}
							solver->litmap[abslit2].neg[solver->litmap[abslit2].neg_len++] = solver->original.count;
							addclause(solver, literals, 2);
						}
					}
				}
			}
			//宫约束
			for (int hangzengliang = 0; hangzengliang < 9; hangzengliang += 3)
			{
				for (int liezengliang = 0; liezengliang < 9; liezengliang += 3)
				{
					for (int num = 1; num < 10; num++)
					{	
						int count = 0;
						for (int i = 1; i < 4; i++)//能填
						{
							for (int j = 1; j < 4; j++)
							{
								literals[count] = (hangzengliang+i - 1) * 81 + (liezengliang+j - 1) * 9 + num;
								int lit = literals[count];
								if (solver->litmap[lit].pos == NULL)//添加变元-子句映射表
								{
									solver->litmap[lit].pos = (int*)malloc(sizeof(int) * clausenum);
									solver->litmap[lit].pos_len = 0;
								}
								solver->litmap[lit].pos[solver->litmap[lit].pos_len++] = solver->original.count;
								count++;
							}
						}
						addclause(solver, literals, count);
						for (int i = 1; i < 4; i++)//不能填
						{
							for (int j = 1; j < 4; j++)
							{
								if (i == 3 && j == 3)
									break;
								literals[0] = -((hangzengliang + i - 1) * 81 + (liezengliang + j - 1) * 9 + num);
								int abslit = abs(literals[0]);
								if (solver->litmap[abslit].neg == NULL)//添加变元-子句映射表
								{
									solver->litmap[abslit].neg = (int*)malloc(sizeof(int) * clausenum);
									solver->litmap[abslit].neg_len = 0;
								}
								solver->litmap[abslit].neg[solver->litmap[abslit].neg_len++] = solver->original.count;
								for (int k = 1; k < 4; k++)
								{
									for (int p = 1; p < 4; p++)
									{
										if (k < i)
										{
											break;
										}
										else
										{
											if (p <= j)
												continue;
											literals[1] = -((hangzengliang + k - 1) * 81 + (liezengliang + p - 1) * 9 + num);
											int abslit2 = abs(literals[0]);
											if (solver->litmap[abslit2].neg == NULL)//添加变元-子句映射表
											{
												solver->litmap[abslit2].neg = (int*)malloc(sizeof(int) * clausenum);
												solver->litmap[abslit2].neg_len = 0;
											}
											solver->litmap[abslit2].neg[solver->litmap[abslit2].neg_len++] = solver->original.count;
											addclause(solver, literals, 2);
										}
									}
								}
							}
						}
					}
				}
			}
			//百分号约束-----------
			//撇对角线约束
			for (int k = 1; k < 10; k++)
			{
				for (int i = 1; i < 10; i++)//能填
				{
					int j = 10 - i;
					literals[i - 1] = (i - 1) * 81 + (j - 1) * 9 + k;
					int lit = literals[i - 1];
					if (solver->litmap[lit].pos == NULL)//添加变元-子句映射表
					{
						solver->litmap[lit].pos = (int*)malloc(sizeof(int) * clausenum);
						solver->litmap[lit].pos_len = 0;
					}
					solver->litmap[lit].pos[solver->litmap[lit].pos_len++] = solver->original.count;
				}
				addclause(solver, literals, 9);
				for (int i = 1; i < 9; i++)//不能填
				{
					literals[0] = -((i - 1) * 81 + (10-i - 1) * 9 + k);
					int abslit = abs(literals[0]);
					if (solver->litmap[abslit].neg == NULL)//添加变元-子句映射表
					{
						solver->litmap[abslit].neg = (int*)malloc(sizeof(int) * clausenum);
						solver->litmap[abslit].neg_len = 0;
					}
					solver->litmap[abslit].neg[solver->litmap[abslit].neg_len++] = solver->original.count;
					for (int j = 10 - i-1; j > 0; j--)
					{
						literals[1] = -((i - 1) * 81 + (j - 1) * 9 + k);
						int abslit = abs(literals[1]);
						if (solver->litmap[abslit].neg == NULL)//添加变元-子句映射表
						{
							solver->litmap[abslit].neg = (int*)malloc(sizeof(int) * clausenum);
							solver->litmap[abslit].neg_len = 0;
						}
						solver->litmap[abslit].neg[solver->litmap[abslit].neg_len++] = solver->original.count;
						addclause(solver, literals, 2);
					}

				}
			}
			//窗口约束1
			for (int num = 1; num < 10; num++)
			{
				int count = 0;
				for (int i = 1; i < 4; i++)//能填
				{
					for (int j = 1; j < 4; j++)
					{
						literals[count] = (1 + i - 1) * 81 + (1 + j - 1) * 9 + num;
						int lit = literals[count];
						if (solver->litmap[lit].pos == NULL)//添加变元-子句映射表
						{
							solver->litmap[lit].pos = (int*)malloc(sizeof(int) * clausenum);
							solver->litmap[lit].pos_len = 0;
						}
						solver->litmap[lit].pos[solver->litmap[lit].pos_len++] = solver->original.count;
						count++;
					}
				}
				addclause(solver, literals, count);
				for (int i = 1; i < 4; i++)//不能填
				{
					for (int j = 1; j < 4; j++)
					{
						if (i == 3 && j == 3)
							break;
						literals[0] = -((1 + i - 1) * 81 + (1 + j - 1) * 9 + num);
						int abslit = abs(literals[0]);
						if (solver->litmap[abslit].neg == NULL)//添加变元-子句映射表
						{
							solver->litmap[abslit].neg = (int*)malloc(sizeof(int) * clausenum);
							solver->litmap[abslit].neg_len = 0;
						}
						solver->litmap[abslit].neg[solver->litmap[abslit].neg_len++] = solver->original.count;
						for (int k = 1; k < 4; k++)
						{
							for (int p = 1; p < 4; p++)
							{
								if (k < i)
								{
									break;
								}
								else
								{
									if (p <= j)
										continue;
									literals[1] = -((1 + k - 1) * 81 + (1 + p - 1) * 9 + num);
									int abslit2 = abs(literals[0]);
									if (solver->litmap[abslit2].neg == NULL)//添加变元-子句映射表
									{
										solver->litmap[abslit2].neg = (int*)malloc(sizeof(int) * clausenum);
										solver->litmap[abslit2].neg_len = 0;
									}
									solver->litmap[abslit2].neg[solver->litmap[abslit2].neg_len++] = solver->original.count;
									addclause(solver, literals, 2);
								}
							}
						}
					}
				}
			}
			//窗口约束2
			for (int num = 1; num < 10; num++)
			{
				int count = 0;
				for (int i = 1; i < 4; i++)//能填
				{
					for (int j = 1; j < 4; j++)
					{
						literals[count] = (5 + i - 1) * 81 + (5 + j - 1) * 9 + num;
						int lit = literals[count];
						if (solver->litmap[lit].pos == NULL)//添加变元-子句映射表
						{
							solver->litmap[lit].pos = (int*)malloc(sizeof(int) * clausenum);
							solver->litmap[lit].pos_len = 0;
						}
						solver->litmap[lit].pos[solver->litmap[lit].pos_len++] = solver->original.count;
						count++;
					}
				}
				addclause(solver, literals, count);
				for (int i = 1; i < 4; i++)//不能填
				{
					for (int j = 1; j < 4; j++)
					{
						if (i == 3 && j == 3)
							break;
						literals[0] = -((5 + i - 1) * 81 + (5 + j - 1) * 9 + num);
						int abslit = abs(literals[0]);
						if (solver->litmap[abslit].neg == NULL)//添加变元-子句映射表
						{
							solver->litmap[abslit].neg = (int*)malloc(sizeof(int) * clausenum);
							solver->litmap[abslit].neg_len = 0;
						}
						solver->litmap[abslit].neg[solver->litmap[abslit].neg_len++] = solver->original.count;
						for (int k = 1; k < 4; k++)
						{
							for (int p = 1; p < 4; p++)
							{
								if (k < i)
								{
									break;
								}
								else
								{
									if (p <= j)
										continue;
									literals[1] = -((5 + k - 1) * 81 + (5 + p - 1) * 9 + num);
									int abslit2 = abs(literals[0]);
									if (solver->litmap[abslit2].neg == NULL)//添加变元-子句映射表
									{
										solver->litmap[abslit2].neg = (int*)malloc(sizeof(int) * clausenum);
										solver->litmap[abslit2].neg_len = 0;
									}
									solver->litmap[abslit2].neg[solver->litmap[abslit2].neg_len++] = solver->original.count;
									addclause(solver, literals, 2);
								}
							}
						}
					}
				}
			}
			//填数约束
			for (int i = 1; i < 10; i++)
			{
				for (int j = 1; j < 10; j++)
				{
					if (chessboard[i][j] != 0)
					{
						literals[0] = (i - 1) * 81 + (j - 1) * 9 + chessboard[i][j];
						int lit = literals[0];
						if (solver->litmap[lit].pos == NULL)//添加变元-子句映射表
						{
							solver->litmap[lit].pos = (int*)malloc(sizeof(int) * clausenum);
							solver->litmap[lit].pos_len = 0;
						}
						solver->litmap[lit].pos[solver->litmap[lit].pos_len++] = solver->original.count;
						addclause(solver, literals, 1);
					}
				}
			}

			if (!DPLLoptimize(solver))
			{
				printf("此次生成数独失败！\n");
				getchar(); getchar();
				continue;
			}

			//输出到文件
			string newfilename = "C:\\Users\\rui\\Desktop\\暂时使用\\2025秋-程序设计综合课程设计任务及指导学生包\\助教课设演示检查要求\\sudokuresult.cnf";
			FILE* fp = fopen(newfilename.c_str(), "w");

			fprintf(fp, "p cnf %d %d\n", solver->boolnum, solver->original.count);

			for (int i = 0; i < solver->original.count; i++)
			{
				for (int j = 0; j < solver->original.clauses[i].size; j++)
				{
					fprintf(fp, "%d ", solver->original.clauses[i].literals[j]);
				}
				fprintf(fp,"0\n");
			}

			fclose(fp);

			while (1)
			{
				bool isend = true;
				for (int i = 1; i < 10; i++)
				{
					for (int j = 1; j < 10; j++)
					{
						if (chessboard[i][j] == 0)
						{
							isend = false;
							break;
						}
					}
					if (isend == false)
						break;
				}
				if (isend)
				{
					printf("恭喜你成功完成该百分号数独！！！\n");
					getchar(); getchar();
					break;
				}
				system("cls");
				printf("------------------------------------------------\n");
				string nandustr;
				switch (nandu)
				{
				case 1:
					nandustr = "简单";
					break;
				case 2:
					nandustr = "中等";
					break;
				case 3:
					nandustr = "困难";
					break;
				case 4:
					nandustr = "极难";
					break;
				default:
					nandustr = "简单";
					break;
				}
				printf("数独难度:%s，提示数个数:%d个\n", nandustr.c_str(), leftnum);
				printf("+---------+---------+---------+\n");
				for (int i = 1; i < 10; i++)
				{
					printf("|");
					for (int j = 1; j < 10; j++)
					{
						if (chessboard[i][j] != 0)
						{
							if (renewchessboard[i][j] != 0)
								printf("\033[31m %d \033[0m", chessboard[i][j]);
							else
								printf("\033[34m %d \033[0m", chessboard[i][j]);
						}
						else
						{
							printf("\033[37m · \033[0m");
						}
						if (j %3 == 0)
						{
							printf("|");
						}
						if (j == 9)
							printf("\n");
					}
					if (i % 3 == 0)
					{
						printf("+---------+---------+---------+\n");
					}
				}
				printf("------------------------------------------------\n");
				printf("|                 请输入指令：                 |\n");
				printf("| 1.手动填充数字(1-9)  2.生成全部答案  3.重来  |\n");
				printf("| 0.不玩了                                     |\n");
				printf("------------------------------------------------\n");
				int zhiling = 0;
				scanf("%d", &zhiling);
				if (zhiling == 1)
				{
					printf("请依次输入行号，列号和填入数字：\n");
					int x, y, num;
					if (scanf("%d%d%d", &x, &y, &num) == 3 && x>0&&x<10&&y>0&&y<10&&num>=0&&num<10)
					{
						if (renewchessboard[x][y] != 0)
						{
							printf("不能填预设好的位置!\n");
						}
						else
						{
							if (chessboard[x][y] != num)
							{
								if (!issatified(chessboard, x, y, num))
								{
									printf("不符合百分号数独约束！\n");
								}
								else
								{
									chessboard[x][y] = num;
								}
							}
						}
					}
					else
					{
						printf("输入格式错误，请确保行列号合法，填入数字1-9\n");
					}
					
				}
				else if (zhiling == 2)
				{
					for (int i = 1; i <= solver->boolnum; i++)
					{
						if (solver->assignment[i] == 1)
						{
							int x = (i - 1) / 81 + 1;
							int y = (i - (x - 1) * 81 - 1) / 9 + 1;
							int number = i - (x - 1) * 81 - (y - 1) * 9;
							answer[x][y] = number;
						}
					}
					//system("cls");
					printf("------------------------------------------------\n");
					printf("+---------+---------+---------+\n");
					for (int i = 1; i < 10; i++)
					{
						printf("|");
						for (int j = 1; j < 10; j++)
						{
							if (renewchessboard[i][j] != 0)
							{
								printf("\033[31m %d \033[0m", renewchessboard[i][j]);
							}
							else
							{
								printf("\033[34m %d \033[0m", answer[i][j]);
							}
							if (j % 3 == 0)
							{
								printf("|");
							}
							if (j == 9)
								printf("\n");
						}
						if (i % 3 == 0)
						{
							printf("+---------+---------+---------+\n");
						}
					}
				}
				else if (zhiling == 3)
				{
					for (int i = 1; i < 10; i++)
					{
						for (int j = 1; j < 10; j++)
						{
							chessboard[i][j] = renewchessboard[i][j];
						}
					}
				}
				else if (zhiling == 0)
				{
					break;
				}
				getchar();
				getchar();
			}
			free(solver);
		}
		else
		{
			printf("该难度不存在\n");
			getchar();
			getchar();
		}
	}
}