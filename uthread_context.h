/*
* ==============================================
*        Author: huangwei 
*        Create Time: 2016-10-11 20:01
*        Last modified: 2016-10-11 20:01
*        Filename: uthread_context.h
*        Description: 
* ==============================================
*/
#ifndef _UTHREAD_CONTEXT_H_
#define _UTHREAD_CONTEXT_H_

#include <setjmp.h>
#include <stdint.h>
#include <map>
#include <set>
#include <list>

#include <stdio.h>

//协程入口函数
typedef  void (*UthreadEntry) (void *);

typedef  enum{
    UTHREAD_STATE_INIT    = 0,
    UTHREAD_STATE_RUNABLE  = 1,
    UTHREAD_STATE_RUNNING  = 2, 
    UTHREAD_STATE_SLEEPING = 3,
    UTHREAD_STATE_PENDING ,
    UTHREAD_STATE_DONE      
}e_uthread_enum;

//协程的结构
struct uthread_context {
    jmp_buf        _ctx;   
    char     *     _stack_addr;      //协程的栈空间, 基地址 
    uint32_t       _stack_len;       //协程的栈大小
    uint32_t       _uthread_pid;     //协程的pid 
    UthreadEntry   _uthread_entry;   //协程入口函数   
    void     *     _arg;
    e_uthread_enum _state;           //状态
    ~uthread_context()
    {
        if(_stack_addr )
            delete _stack_addr;
        _stack_addr = NULL;
    }
};


//协程管理器、调度器
class uthread_sched
{
public:
    uthread_sched():_next_pid(0), _default_stack_size(4096),  _primo_thread(NULL)
    {
        _primo_thread = new uthread_context; 
        _primo_thread->_state = UTHREAD_STATE_RUNNING;
        _primo_thread->_uthread_pid = 0;
        _cur_thread = _primo_thread;
    }
    //生成新的协程，返回pid, 0表示生成失败
    uint32_t CreateUthread(UthreadEntry func, void * argc );  
    uint32_t GetCurPid()
    {
        if(_cur_thread )
            return _cur_thread->_uthread_pid;
        else
            return 0;
    }

    //从当前运行的协程切走
    void     Yield();


private:
    uint32_t                               _next_pid  ;  //下一个新生成协程的id 
    uint32_t                               _default_stack_size;
    std::map<uint32_t , uthread_context*>  _thread_map;
    uthread_context *                      _primo_thread; 
    
    static uthread_context *                      _cur_thread; //当前运行的协程

    std::list<uthread_context*>  _runable_list;
    std::list<uthread_context*>  _yield_list; 
    
    uint32_t GetNextPid();
    void     SchedThread();   //调度线程策略
    void     SchedPolicy();
};



#endif
