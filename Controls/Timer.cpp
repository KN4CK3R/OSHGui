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
	void Timer::SetInterval(int interval)
	{
		if (this->interval != interval)
		{
			#ifndef OSHGUI_DONTUSEEXCEPTIONS
			if (interval < 1)
			{
				throw Misc::ArgumentOutOfRangeException("interval", __FILE__, __LINE__);
			}
			#endif
			
			this->interval = interval;
			if (isEnabled)
			{
				Application::Instance()->timerManager.UnregisterTimer(this);
				Application::Instance()->timerManager.RegisterTimer(this, Misc::TimeSpan::FromMilliseconds(this->interval));
			}
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