#include "Timer.h"
#include "Control.h"
#include "Application.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	Timer::Timer(const std::shared_ptr<Control> &parent) : Control(parent)
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
				Application::RegisterTimer(std::static_pointer_cast<Timer>(shared_from_this()), Misc::TimeSpan::FromMilliseconds(interval));
			}
			else
			{
				Application::UnregisterTimer(std::static_pointer_cast<Timer>(shared_from_this()));
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
	void Timer::SetInterval(long interval)
	{
		if (this->interval != interval)
		{
			this->interval = interval > 0 ? interval : 100;
			if (enabled)
			{
				Application::UnregisterTimer(std::static_pointer_cast<Timer>(shared_from_this()));
				Application::RegisterTimer(std::static_pointer_cast<Timer>(shared_from_this()), Misc::TimeSpan::FromMilliseconds(interval));
			}
		}
	}
	//---------------------------------------------------------------------------
	long Timer::GetInterval()
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