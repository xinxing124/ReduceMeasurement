#pragma once

#include <Windows.h>  
  
#include <map>  
  
typedef void (*TimerCallBack)(int);   
  
struct Timer_Info  
{  
    unsigned int nElapse;  
    time_t beginTime;  
    TimerCallBack onTimer;  
};  
  
typedef std::map<int, Timer_Info> TIMER_LIST;  
  
class MyTimer  
{  
public:  
    MyTimer();  
    ~MyTimer();  
  
    void startTimer(int timeID, unsigned int nElapse, TimerCallBack func);  
    void endTimer(int timeID);    
  
    static DWORD WINAPI ThreadFunc(LPVOID lpParam);  
  
private:  
    TIMER_LIST m_timerList;//定时器列表，存放多个相关定时器信息  
    HANDLE m_hThread;  
};  