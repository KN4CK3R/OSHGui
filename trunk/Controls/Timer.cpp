#include "Timer.hpp"
#include "Control.hpp"
#include "..\Application.hpp"
#include "..\TimerManager.hpp"
#include "..\Misc\Exceptions.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	Timer::Timer()
	{
		type = CONTROL_TIMER;

		isEnabled = false;
		interval = 100LL;
	}
	//---------------------------------------------------------------------------
	Timer::~Timer()
	{
		SetEnabled(false);
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void Timer::SetEnabled(bool isEnabled)
	{
		if (this->isEnabled != isEnabled)
		{
			if (isEnabled == true)
			{
				Application::Instance()->timerManager.RegisterTimer(this, Misc::TimeSpan::FromMilliseconds(interval));
			}
			else
			{
				Application::Instance()->timerManager.UnregisterTimer(this);
			}
			
			Control::SetEnabled(isEnabled);
		}
	}
	//---------------------------------------------------------------------------
	void Timer::SetInterval(long long interval)
	{
		if (this->interval != interval)
		{
			if (interval > 0)
			{
				this->interval = interval;
				if (isEnabled)
				{
					Application::Instance()->timerManager.UnregisterTimer(this);
					Application::Instance()->timerManager.RegisterTimer(this, Misc::TimeSpan::FromMilliseconds(this->interval));
				}
			}
			#ifndef OSHGUI_DONTUSEEXCEPTIONS
			else
			{
				throw Misc::ArgumentOutOfRangeException("interval", __FILE__, __LINE__);
			}
			#endif
		}
	}
	//---------------------------------------------------------------------------
	long long Timer::GetInterval() const
	{
		return interval;
	}
	//---------------------------------------------------------------------------
	TickEvent& Timer::GetTickEvent()
	{
		return tickEvent;
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	void Timer::Start()
	{
		SetEnabled(true);
	}
	//---------------------------------------------------------------------------
	void Timer::Stop()
	{
		SetEnabled(false);
	}
	//---------------------------------------------------------------------------
}