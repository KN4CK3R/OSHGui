#include "Timer.h"
#include "Control.h"
#include "..\Application.h"
#include "..\Misc\Exceptions.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	Timer::Timer(Control *parent) : Control(parent)
	{
		type = CONTROL_TIMER;

		enabled = false;
		interval = 100L;
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
			if (interval > 0)
			{
				this->interval = interval;
				if (enabled)
				{
					Application::UnregisterTimer(this);
					Application::RegisterTimer(this, Misc::TimeSpan::FromMilliseconds(this->interval));
				}
			}
			else
			{
				throw Misc::ArgumentOutOfRangeException(L"interval", __WFILE__, __LINE__);
			}
		}
	}
	//---------------------------------------------------------------------------
	long Timer::GetInterval() const
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