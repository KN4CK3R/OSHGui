/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
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
	void Timer::SetEnabled(bool isEnabled_)
	{
		if (isEnabled != isEnabled_)
		{
			next = Misc::DateTime();
			
			Control::SetEnabled(isEnabled_);
		}
	}
	//---------------------------------------------------------------------------
	void Timer::SetInterval(int interval_)
	{
		#ifndef OSHGUI_DONTUSEEXCEPTIONS
		if (interval_ < 1)
		{
			throw Misc::ArgumentOutOfRangeException("interval");
		}
		#endif
		
		interval = Misc::TimeSpan::FromMilliseconds(interval_);
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
	bool Timer::Intersect(const Drawing::PointI &point) const
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
