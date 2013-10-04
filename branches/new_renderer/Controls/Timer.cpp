/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include "Timer.hpp"
#include "Control.hpp"
#include "../Application.hpp"
#include "../TimerManager.hpp"
#include "../Misc/Exceptions.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	Timer::Timer()
		: interval(100)
	{
		type = CONTROL_TIMER;
	
		isEnabled = false;
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
	bool Timer::Intersect(const Drawing::Point &point) const
	{
		return false;
	}
	//---------------------------------------------------------------------------
}