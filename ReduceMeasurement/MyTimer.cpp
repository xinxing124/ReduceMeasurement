#include "stdafx.h"  
#include "MyTimer.h"  
  
#include <iostream>  
#include <time.h>  
  
using namespace std;  
  
MyTimer::MyTimer()  
{  
    m_hThread = CreateThread(NULL, 0, ThreadFunc, LPVOID(this), 0, NULL);  
}  
  
MyTimer::~MyTimer()  
{  
    CloseHandle(m_hThread);  
    m_hThread = NULL;  
}  
  
void MyTimer::startTimer(int timeID, unsigned int nElapse, TimerCallBack func)  
{  
    Timer_Info info = {nElapse, time(NULL), func};  
    m_timerList.insert(std::make_pair(timeID, info));       //此处没有判断timeID是否存在就直接insert，是因为map::insert当key存在时，insert会失败。  
}  
  
void MyTimer::endTimer(int timeID)  
{  
    m_timerList.erase(timeID);      //直接erase原理同insert，不存在时， erase失败  
}  
  
DWORD WINAPI MyTimer::ThreadFunc(LPVOID lpParam)  
{  
    MyTimer *pThis = (MyTimer*)(lpParam);  
  
    if (pThis == NULL)  return -1;  
  
    while (true)  
    {  
        if (pThis->m_timerList.size() != 0)      //只在定时器列表中有ID时才作处理  
        {  
            time_t endTime = time(NULL);  
            int diff;  
  
            //在定时器列表中，轮流查询哪个定时器时间到达。  
            for (TIMER_LIST::iterator iter =pThis->m_timerList.begin(); iter != pThis->m_timerList.end(); iter++)  
            {  
                diff = (int)difftime(endTime, iter->second.beginTime);  
                if (diff >= iter->second.nElapse )  
                {  
                    iter->second.onTimer(iter->first);  
                    iter->second.beginTime = endTime;  
                }  
            }  
        }  
          
    }  
}  