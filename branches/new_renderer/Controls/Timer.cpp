/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include "Timer.hpp"
#include "../Misc/Exceptions.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	Timer::Timer()
		: interval(Misc::TimeSpan::FromMilliseconds(100))
	{
		type = ControlType::Timer;
	
		SetEnabled(false);
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void Timer::SetEnabled(bool _isEnabled)
	{
		if (isEnabled != _isEnabled)
		{
			next = Misc::DateTime();
			
			Control::SetEnabled(isEnabled);
		}
	}
	//---------------------------------------------------------------------------
	void Timer::SetInterval(int _interval)
	{
		#ifndef OSHGUI_DONTUSEEXCEPTIONS
		if (_interval < 1)
		{
			throw Misc::ArgumentOutOfRangeException("interval");
		}
		#endif
		
		interval = Misc::TimeSpan::FromMilliseconds(_interval);
	}
	//---------------------------------------------------------------------------
	int Timer::GetInterval() const
	{
		return interval.GetTotalMilliseconds();
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
	bool Timer::Intersect(const Drawing::PointF &point) const
	{
		return false;
	}
	//---------------------------------------------------------------------------
	void Timer::InjectTime(const Misc::DateTime &time)
	{
		if (time >= next)
		{
			next = time.Add(interval);
			
			tickEvent.Invoke(this);
		}
	}
	//---------------------------------------------------------------------------
}
