/*
* ==============================================
*        Author: linushuang   Email:linushuang@tencent.com
*        Create Time: 2016-10-12 09:42
*        Last modified: 2016-10-12 09:42
*        Filename: test_app.c
*        Description: 
* ==============================================
*/

#include "uthread_context.h"

#include "stdio.h"
#include "stdlib.h"

uthread_sched  g_sched;

void uthread_funA(void *arg)
{

    int num = 0;
    while( 1 )
    {
        printf("in uthread_funA, num:%u\n", num);
        
        int pid = g_sched.GetCurPid();

        printf("funA cur uthread pid:%u, num:%u\n", pid, num);
        
        num++;
        g_sched.Yield();
    }
}

void uthread_funB(void *arg)
{

    int num = 0;
    while( 1 )
    {
        printf("in uthread_funB, num:%u\n", num);
        
        int pid = g_sched.GetCurPid();

        printf("funcB cur uthread pid:%u, num:%u\n", pid, num);
        
        num++;
        g_sched.Yield();
    }
}

void  uthread_funC(void *arg)
{

    int num = 0;
    while( 1 )
    {
        printf("in uthread_funC, num:%u\n", num);
        
        int pid = g_sched.GetCurPid();

        printf("funcC cur uthread pid:%u, num:%u\n", pid, num);
        
        num++;
        g_sched.Yield();
    }
}


jmp_buf jmp;

int main()
{
    setjmp(jmp);

    printf("uthread test app begin\n");
    int pid;
    pid = g_sched.CreateUthread(uthread_funA, NULL);
    printf("create uthread:%u\n", pid);
   
    pid = g_sched.CreateUthread(uthread_funB, NULL);
    printf("create uthread:%u\n", pid);  

    pid = g_sched.CreateUthread(uthread_funC, NULL);
    printf("create uthread:%u\n", pid);  

    pid = g_sched.CreateUthread(uthread_funC, NULL);
    printf("create uthread:%u\n", pid);  



    while(1)
    {
        g_sched.Yield();     
    }

    printf("uthread test app end\n");
    
    return 0;
}
