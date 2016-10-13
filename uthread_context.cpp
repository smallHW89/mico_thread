/*
* ==============================================
*        Author: linushuang   Email:linushuang@tencent.com
*        Create Time: 2016-10-11 20:27
*        Last modified: 2016-10-11 20:27
*        Filename: uthread_context.cpp
*        Description: 
* ==============================================
*/

#include "uthread_context.h" 

extern "C"  int save_context(jmp_buf jbf);
extern "C"  void restore_context(jmp_buf jbf, int ret);
extern "C"  void replace_esp(jmp_buf jbf, void* esp); 

uthread_context * uthread_sched::_cur_thread(NULL);

//分配uthread_pid , 返回非0的
uint32_t uthread_sched::GetNextPid()
{
    _next_pid ++;
    if( _next_pid )
        return _next_pid;
    else       
    {
        _next_pid++;
        return _next_pid;
    }
}

uint32_t uthread_sched::CreateUthread(UthreadEntry func, void *arg  )
{
    uint32_t pid = GetNextPid();
    uthread_context * puthread = new uthread_context;
    puthread->_uthread_pid = pid; 
    char * pstack = new char[_default_stack_size];
    puthread->_stack_addr  = pstack; 
    puthread->_stack_len   = _default_stack_size;
    puthread->_uthread_entry = func;
    puthread->_arg           = arg ;
    puthread->_state         = UTHREAD_STATE_RUNABLE;
    _thread_map[pid] = puthread;
    //=====测试，打乱栈
    char szBuf[1024];
    for(int i=0; i<100; i++ )
        szBuf[i] = i;
    int len = sizeof(szBuf);
    //save context, 保存上下文
    int iret = save_context(puthread->_ctx); 
    //int iret = setjmp(puthread->_ctx );
    if( iret == 0 )
    {
        size_t addr = ((size_t)pstack) +  _default_stack_size;
        printf("create a new thread:%u, stack_addr:%p|%p, len:%u\n", pid, addr, pstack, _default_stack_size);
        //替换jmp_buf中的esp/rsp
        //puthread->_ctx->__jmpbuf[1] = addr;
        replace_esp(puthread->_ctx, (void*)addr);
        _runable_list.push_back(puthread );  
        return pid;
    }  
    else //iret != 0  表示从longjmp跳过来，这里表示协程可以开始运行
    {  //这里直接用 puthread也可以？？, 不可以
       //_cur_thread 可以吗？ 看情况，_cur_thread 是静态成员变量或者全局变量可以，是类的普通成员变量不行
       //why : 类的函数， 有个隐含参数this, 也就是第一个参数this, 这里this是存在栈上，同样， puthread是临时变量，也是栈上
       // 注意：这里创建线程，上面的栈是父线程的栈，代码到这里，是用的子线程的栈，空间不一样，所以呢，用this->xxx肯定也有问题，
       // 也就是代码到这里，不能用前面代码中任何出现在栈上的变量
       // 前面测试时，用puthread ,可以是巧合，只是 恰好 puthread的值在寄存器中，保存上下文时，保存了， 加了上面的 for 句子打乱栈后，就跑不了了
        uthread_context * p  = _cur_thread;
        printf("_cur_thread:%p\n", _cur_thread);        
        if(p->_uthread_entry ) 
             p->_uthread_entry( p->_arg  );
        //协程执行完函数, 回收
        p->_state = UTHREAD_STATE_DONE;
        //_thread_map.erase(puthread->_uthread_pid); 
        delete p ;
        SchedThread();  
        return 0;
    }
    return 0; 
}

//调度线程运行
void uthread_sched::SchedThread()
{
    uthread_context *  puthread  = NULL; 
    if( _runable_list.size() >  0 )     
    { //如果有可运行线程，则选择一个来运行
        puthread = *_runable_list.begin();
        _runable_list.pop_front();
    }
    else
    {//如果没有可运行线程,从yield线程选择一个来运行
        puthread = *_yield_list.begin();
        _yield_list.pop_front();
    }
    
    _cur_thread = puthread;
    _cur_thread->_state = UTHREAD_STATE_RUNNING;
    
    //恢复_cur_thread的上下文
    printf("long jump to pid:%u\n", _cur_thread->_uthread_pid);
    //longjmp(_cur_thread->_ctx, 1);
    restore_context(_cur_thread->_ctx, 1);
}

//当前线程主动放弃
void uthread_sched::Yield() 
{
    printf("Yield\n");
    if(_cur_thread != NULL )
    {
        printf("pid:%u Yield\n", _cur_thread->_uthread_pid );
        _yield_list.push_back(_cur_thread);
        uthread_context *puthread  = _cur_thread;

        int iret = save_context(puthread->_ctx);  

        //int iret = setjmp(puthread->_ctx);  
        if(iret == 0 )
        {
            printf("pid:%u  Yield SchedThread\n", puthread->_uthread_pid);
            SchedThread();
        }
    }
}


