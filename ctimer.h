/********************************************************
*   Copyright (C) 2016 All rights reserved.
*   
*   Filename:ctimer.h
*   Author  :huangwei  497225735@qq.com
*   Date    :2016-11-06
*   Describe: timer , 定时器，用multimap实现
*
********************************************************/
#ifndef _CTIMER_H
#define _CTIMER_H

#include<stdint.h>
#include<stdlib.h>
#include<map>
#include<vector>

class CTimerObj;
class CTimerMng;


typedef int ( * TimeOutFunc ) (void *);
typedef std::multimap<uint64_t, CTimerObj *>::iterator  TimerIter;

class  CTimerObj
{
public:
    CTimerObj():_expire_timestamp(0) {}
    CTimerObj(uint64_t t):_expire_timestamp(t) {}
    void  SetExpireTimestamp(uint64_t t)
    {
        _expire_timestamp = t;
    }
    uint64_t GetExpireTimestamp()
    {
        return _expire_timestamp;
    }

    virtual int  OnTimeOut()/*子类重写该接口*/
    {
        return 0;
    }

    friend  class CTimerMng;

protected:

private:
    uint64_t    _expire_timestamp; //
    TimerIter   _iterator;

};

class CTimerMng
{
public:
    TimerIter AddTimerObj(CTimerObj *);
    void      DelTimeObj( CTimerObj *);
    int       GetTimeOutObj(std::vector<CTimerObj *>  & objList, uint64_t timeLine);

private:
   std::multimap<uint64_t, CTimerObj *>  _timer_list;

};

#endif //CTIMER_H
