#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdio>
#include <sstream>
#include<ctime>
#include "cnfparser.h"
#include "display.h"
#include "solver.h"
#include "Sudoku.h"
using namespace std;

int boolnum, clausenum;
int* finalassignment;
clock_t begintime,begintime2;
double maxtime = 600; //限制最多运行秒数
bool overtime = false;
bool overtime2 = false;
void init()
{
    boolnum = clausenum = 0;
    overtime = overtime2 = false;
}
int main()
{
    while (1)
    {
        system("cls");
        printf("------------------------------------------------\n");
        printf("|                请输入命令编号：               |\n");
        printf("|         1:读取演示要求的cnf文件进行求解       |\n");
        printf("|         2:读取基准算例的cnf文件进行求解       |\n");
        printf("|         3:运行百分号数独游戏                  |\n");
        printf("|         0:结束程序                            |\n");
        printf("------------------------------------------------\n");
        int command,cnfnumber;
        stringstream ss;
        string fileName;
        scanf("%d", &command);
        if(command == 1)
        {
            printf("请输入要读取第几个cnf文件（演示要求）\n");
            scanf("%d", &cnfnumber);
            ss << cnfnumber;
            fileName = "..\\助教课设演示检查要求\\";
            if ((cnfnumber >= 1 && cnfnumber <= 3) || (cnfnumber >= 5 && cnfnumber <= 6) || cnfnumber == 10 || cnfnumber == 12)
            {
                fileName = fileName + ss.str() + ".cnf";
            }
            else if (cnfnumber == 4 || (cnfnumber >= 7 && cnfnumber <= 9) || cnfnumber == 11)
            {
                fileName = fileName + ss.str() + "（unsatisfied）" + ".cnf";
            }
            else
            {
                printf("没有这个文件请重新开始\n");
                break;
            }
            init();
            clause* clausearr = NULL;
            int* boolcountarr = NULL;
            Solver* solver = NULL;

            string filename = fileName;
            scancnf(filename, boolcountarr, clausearr, solver);

            showclause(clausearr);       //输出 输入的数据供人工检验

            begintime2 = clock();
            clock_t time1 = clock();
            bool result = DPLLoptimize(solver);
            clock_t time2 = clock();
            printf("优化后：\n");
            if (overtime2)
            {
                printf("s -1\n");
            }
            else
            {
                printf("s %d\n", result ? 1 : 0);
                if (result)
                {
                    printf("v");
                    for (int i = 1; i <= boolnum; i++)
                    {
                        printf(" %d", solver->assignment[i] * i);
                    }
                    printf("\n");
                }
            }
            printf("t %.0lf\n", (double)(time2 - time1) / CLOCKS_PER_SEC * 1000);
            //输出到文件
            string newfilename = filename.erase(filename.length() - 4, 4);
            newfilename += ".res";
            FILE* fp = fopen(newfilename.c_str(), "w");
            if (overtime2)
            {
                fprintf(fp,"s -1\n");
            }
            else
            {
                fprintf(fp,"s %d\n", result ? 1 : 0);
                if (result)
                {
                    fprintf(fp,"v");
                    for (int i = 1; i <= boolnum; i++)
                    {
                        fprintf(fp," %d", solver->assignment[i] * i);
                    }
                    fprintf(fp,"\n");
                }
            }
            fprintf(fp,"t %.0lf\n", (double)(time2 - time1) / CLOCKS_PER_SEC * 1000);

            fclose(fp);

            free(solver);

            printf("//////////////////////////////////\n");

            int* assignment = (int*)calloc(boolnum + 1, sizeof(int));

            finalassignment = (int*)calloc(boolnum + 1, sizeof(int));

            begintime = clock();
            clock_t starttime = clock();
            bool issolved = DPLLordinary(clausearr, boolcountarr, assignment);
            clock_t endtime = clock();
            printf("优化前：\n");
            if (overtime)
            {
                printf("s -1\n");
            }
            else
            {
                printf("s %d\n", issolved ? 1 : 0);
                if (issolved)
                {
                    printf("v");
                    for (int i = 1; i <= boolnum; i++)
                    {
                        printf(" %d", finalassignment[i] * i);
                    }
                    printf("\n");
                }
            }

            printf("t %.0lf\n", (double)(endtime - starttime) / CLOCKS_PER_SEC * 1000);

            free(assignment);
            free(finalassignment);
            if (boolcountarr != NULL)
                free(boolcountarr);

            if (result && issolved)
            {
                printf("优化率：%.1lf\n", (double)(((double)(endtime - starttime) / CLOCKS_PER_SEC - (double)(time2 - time1) / CLOCKS_PER_SEC) / ((double)(endtime - starttime) / CLOCKS_PER_SEC)) * 100);
            }

               
        }
        else if (command == 2)
        {
            printf("继续输入命令编号：\n");
            printf("1.功能测试:sat-20.cnf\n");
            printf("2.功能测试:unsat-5cnf-30.cnf\n");
            printf("3.性能测试:ais10.cnf\n");
            printf("4.性能测试:sud00009.cnf\n");
            scanf("%d", &cnfnumber);
            fileName = "..\\SAT测试备选算例\\基准算例\\";
            if (cnfnumber == 1)
            {
                fileName = fileName + "功能测试\\" + "sat-20.cnf";
            }
            else if (cnfnumber == 2)
            {
                fileName = fileName + "功能测试\\" + "unsat-5cnf-30.cnf";
            }
            else if (cnfnumber == 3)
            {
                fileName = fileName + "性能测试\\" + "ais10.cnf";
            }
            else if (cnfnumber == 4)
            {
                fileName = fileName + "性能测试\\" + "sud00009.cnf";
            }
            else
            {
                printf("没有这个文件请重新开始\n");
            }
            init();
            clause* clausearr = NULL;
            int* boolcountarr = NULL;
            Solver* solver = NULL;

            string filename = fileName;
            scancnf(filename, boolcountarr, clausearr, solver);

            showclause(clausearr);       //输出 输入的数据供人工检验

            begintime2 = clock();
            clock_t time1 = clock();
            bool result = DPLLoptimize(solver);
            clock_t time2 = clock();
            if (overtime2)
            {
                printf("s -1\n");
            }
            else
            {
                printf("s %d\n", result ? 1 : 0);
                if (result)
                {
                    printf("v");
                    for (int i = 1; i <= boolnum; i++)
                    {
                        printf(" %d", solver->assignment[i] * i);
                    }
                    printf("\n");
                }
            }
            printf("t %.0lf\n", (double)(time2 - time1) / CLOCKS_PER_SEC * 1000);

            //输出到文件
            string newfilename = filename.erase(filename.length() - 4, 4);
            newfilename += ".res";
            FILE* fp = fopen(newfilename.c_str(), "w");
            if (overtime2)
            {
                fprintf(fp, "s -1\n");
            }
            else
            {
                fprintf(fp, "s %d\n", result ? 1 : 0);
                if (result)
                {
                    fprintf(fp, "v");
                    for (int i = 1; i <= boolnum; i++)
                    {
                        fprintf(fp, " %d", solver->assignment[i] * i);
                    }
                    fprintf(fp, "\n");
                }
            }
            fprintf(fp, "t %.0lf\n", (double)(time2 - time1) / CLOCKS_PER_SEC * 1000);

            fclose(fp);

            free(solver);

            printf("//////////////////////////////////\n");

            int* assignment = (int*)calloc(boolnum + 1, sizeof(int));

            finalassignment = (int*)calloc(boolnum + 1, sizeof(int));

            begintime = clock();
            clock_t starttime = clock();
            bool issolved = DPLLordinary(clausearr, boolcountarr, assignment);
            clock_t endtime = clock();
            if (overtime)
            {
                printf("s -1\n");
            }
            else
            {
                printf("s %d\n", issolved ? 1 : 0);
                if (issolved)
                {
                    printf("v");
                    for (int i = 1; i <= boolnum; i++)
                    {
                        printf(" %d", finalassignment[i] * i);
                    }
                    printf("\n");
                }
            }

            printf("t %.0lf\n", (double)(endtime - starttime) / CLOCKS_PER_SEC * 1000);

            free(assignment);
            free(finalassignment);
            if (boolcountarr != NULL)
                free(boolcountarr);

            if (result && issolved)
            {
                printf("优化率：%.1lf\n", (double)(((double)(endtime - starttime) / CLOCKS_PER_SEC - (double)(time2 - time1) / CLOCKS_PER_SEC) / ((double)(endtime - starttime) / CLOCKS_PER_SEC)) * 100);
            }
        }
        else if (command == 0)
        {
            break;
        }
        else if (command == 3)
        {
            init();
            sudoku();
        }
        /*//else if (command == 4)//个人测试使用
        //{
        //    fileName = "C:\\Users\\rui\\Desktop\\暂时使用\\2025秋-程序设计综合课程设计任务及指导学生包\\程序设计综合课程设计任务及指导学生包\\SAT测试备选算例\\不满足算例\\u-problem7-50.cnf";
        //    init();
        //    clause* clausearr = NULL;
        //    int* boolcountarr = NULL;
        //    Solver* solver = NULL;

        //    string filename = fileName;
        //    scancnf(filename, boolcountarr, clausearr, solver);

        //    //showclause(clausearr);       //输出 输入的数据供人工检验

        //    begintime2 = clock();
        //    clock_t time1 = clock();
        //    bool result = DPLLoptimize(solver);
        //    clock_t time2 = clock();
        //    if (overtime2)
        //    {
        //        printf("s -1\n");
        //    }
        //    else
        //    {
        //        printf("s %d\n", result ? 1 : 0);
        //        if (result)
        //        {
        //            printf("v");
        //            for (int i = 1; i <= boolnum; i++)
        //            {
        //                printf(" %d", solver->assignment[i] * i);
        //            }
        //            printf("\n");
        //        }
        //    }
        //    printf("t %.0lf\n", (double)(time2 - time1) / CLOCKS_PER_SEC * 1000);

        //    free(solver);

        //    printf("//////////////////////////////////\n");

        //    int* assignment = (int*)calloc(boolnum + 1, sizeof(int));

        //    finalassignment = (int*)calloc(boolnum + 1, sizeof(int));

        //    begintime = clock();
        //    clock_t starttime = clock();
        //    bool issolved = DPLLordinary(clausearr, boolcountarr, assignment);
        //    clock_t endtime = clock();
        //    if (overtime)
        //    {
        //        printf("s -1\n");
        //    }
        //    else
        //    {
        //        printf("s %d\n", issolved ? 1 : 0);
        //        if (issolved)
        //        {
        //            printf("v");
        //            for (int i = 1; i <= boolnum; i++)
        //            {
        //                printf(" %d", finalassignment[i] * i);
        //            }
        //            printf("\n");
        //        }
        //    }

        //    printf("t %.0lf\n", (double)(endtime - starttime) / CLOCKS_PER_SEC * 1000);

        //    free(assignment);
        //    free(finalassignment);
        //    if (boolcountarr != NULL)
        //        free(boolcountarr);

        //    if (result && issolved)
        //    {
        //        printf("优化率：%.1lf\n", (double)(((double)(endtime - starttime) / CLOCKS_PER_SEC - (double)(time2 - time1) / CLOCKS_PER_SEC) / ((double)(endtime - starttime) / CLOCKS_PER_SEC)) * 100);
        //    }
        //}*/
        else
        {
            printf("没有这个命令请重新开始\n");
        }   
        getchar();
        getchar();
    }
    return 0;
}
