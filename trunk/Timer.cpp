#include "Timer.h"
#include "Control.h"
#include "Application.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	Timer::Timer(Control *parent) : Control(parent)
	{
		type = CONTROL_TIMER;

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
		if (this->enabled != enabled)
		{
			if (enabled == true)
			{
				Application::RegisterTimer(this, Misc::TimeSpan::FromMilliseconds(interval));
			}
			else
			{
				Application::UnregisterTimer(this);
			}
			
			this->enabled = enabled;
		}
	}
	//---------------------------------------------------------------------------
	bool Timer::GetEnabled() const
	{
		return enabled;
	}
	//---------------------------------------------------------------------------
	void Timer::SetInterval(long interval)
	{
		if (this->interval != interval)
		{
			this->interval = interval > 0 ? interval : 100;
			if (enabled)
			{
				Application::UnregisterTimer(this);
				Application::RegisterTimer(this, Misc::TimeSpan::FromMilliseconds(interval));
			}
		}
	}
	//---------------------------------------------------------------------------
	long Timer::GetInterval() const
	{
		return interval;
	}
	//---------------------------------------------------------------------------
	TickEventHandler& Timer::GetTickEventHandler()
	{
		return tickEventHandler;
	}
	//---------------------------------------------------------------------------
}