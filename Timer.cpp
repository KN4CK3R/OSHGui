#include "Timer.h"

namespace OSHGui
{
	Misc::HashTable<UINT_PTR, Timer*> Timer::timerTable;
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	Timer::Timer()
	{
		enabled = false;
		interval = 100;
	}
	//---------------------------------------------------------------------------
	Timer::~Timer()
	{
		SetEnabled(false);
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void Timer::SetEnabled(bool enabled)
	{
		static int iid = 0xF;
		if (this->enabled != enabled)
		{
			if (enabled == true)
			{
				timerId = SetTimer(NULL, iid++, interval, (TIMERPROC)TimerCallback);

				Timer *temp = (Timer*)this;
				timerTable.Add(timerId, temp);
			}
			else
			{
				timerTable.Remove(timerId);
				KillTimer(NULL, timerId);
			}
			
			this->enabled = enabled;
		}
	}
	//---------------------------------------------------------------------------
	bool Timer::GetEnabled()
	{
		return enabled;
	}
	//---------------------------------------------------------------------------
	void Timer::SetInterval(int interval)
	{
		this->interval = interval > 0 ? interval : 100;
	}
	//---------------------------------------------------------------------------
	int Timer::GetInterval()
	{
		return interval;
	}
	//---------------------------------------------------------------------------
	void Timer::SetOnTickFunc(OnTick onTickFunc)
	{
		this->onTickFunc = onTickFunc;
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	void Timer::OnTimer()
	{
		if (onTickFunc != NULL)
		{
			(*onTickFunc)();
		}
	}
	//---------------------------------------------------------------------------
	void CALLBACK Timer::TimerCallback(HWND hwnd, UINT message, UINT idTimer, DWORD dwTime)
	{
		if (timerTable.Contains(idTimer))
		{
			timerTable[idTimer]->OnTimer();
		}
	}
	//---------------------------------------------------------------------------
}