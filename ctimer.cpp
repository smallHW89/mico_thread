/********************************************************
*   Copyright (C) 2016 All rights reserved.
*  
*   Filename:ctimer.cpp
*   Author  :huangwei  497225735@qq.com
*   Date    :2016-11-06
*   Describe:
*
********************************************************/

#include <assert.h>

#include "ctimer.h"


TimerIter CTimerMng:: AddTimerObj(CTimerObj  * obj)
{
    assert(NULL != obj);

    TimerIter iter = _timer_list.insert(std::make_pair(obj->_expire_timestamp, obj));   
    obj->_iterator = iter;
    return obj->_iterator;
}

void  CTimerMng::DelTimeObj(CTimerObj *obj )  
{
    assert(NULL != obj);
    
    _timer_list.erase(obj->_iterator); 
}

int  CTimerMng:: GetTimeOutObj(std::vector<CTimerObj *> & objList, uint64_t timeLine)
{
    int i=0;
    TimerIter iter = _timer_list.begin();
    for( ; iter != _timer_list.end(); iter++ )
    {
        if(iter->first <= timeLine ) 
        {
            objList.push_back(iter->second);
            i++;
        }
        else
        {
            break;
        }
    }
    return i;
}
