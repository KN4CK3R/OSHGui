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
	Timer::Timer(Control *parent) : Control(parent)
	{
		type = CONTROL_TIMER;

		isEnabled = false;
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
	void Timer::SetEnabled(bool isEnabled)
	{
		if (this->isEnabled != isEnabled)
		{
			if (isEnabled == true)
			{
				Application::timerManager.RegisterTimer(this, Misc::TimeSpan::FromMilliseconds(interval));
			}
			else
			{
				Application::timerManager.UnregisterTimer(this);
			}
			
			this->isEnabled = isEnabled;
		}
	}
	//---------------------------------------------------------------------------
	bool Timer::GetEnabled() const
	{
		return isEnabled;
	}
	//---------------------------------------------------------------------------
	void Timer::SetInterval(long interval)
	{
		if (this->interval != interval)
		{
			if (interval > 0)
			{
				this->interval = interval;
				if (isEnabled)
				{
					Application::timerManager.UnregisterTimer(this);
					Application::timerManager.RegisterTimer(this, Misc::TimeSpan::FromMilliseconds(this->interval));
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