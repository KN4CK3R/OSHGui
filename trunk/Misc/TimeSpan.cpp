#include "TimeSpan.h"
#include "Exceptions.h"

namespace OSHGui
{
	namespace Misc
	{
		const long long TimeSpan::TicksPerMillisecond = 10000LL;
		const long long TimeSpan::TicksPerSecond = TicksPerMillisecond * 1000LL;
		const long long TimeSpan::TicksPerMinute = TicksPerSecond * 60LL;
		const long long TimeSpan::TicksPerHour = TicksPerMinute * 60LL;
		const long long TimeSpan::TicksPerDay = TicksPerHour * 24LL;
	
		const double TimeSpan::MillisecondsPerTick = 1.0 / TicksPerMillisecond;
		const double TimeSpan::SecondsPerTick = 1.0 / TicksPerSecond;
		const double TimeSpan::MinutesPerTick = 1.0 / TicksPerMinute;
		const double TimeSpan::HoursPerTick = 1.0 / TicksPerHour;
		const double TimeSpan::DaysPerTick = 1.0 / TicksPerDay;
		
		const int TimeSpan::MillisPerSecond = 1000;
		const int TimeSpan::MillisPerMinute = MillisPerSecond * 60;
		const int TimeSpan::MillisPerHour = MillisPerMinute * 60;
		const int TimeSpan::MillisPerDay = MillisPerHour * 24;
		
		const long long TimeSpan::MaxSeconds = 9223372036854775807LL / TicksPerSecond;
		const long long TimeSpan::MinSeconds = -9223372036854775806LL / TicksPerSecond;
		const long long TimeSpan::MaxMilliSeconds = 9223372036854775807LL / TicksPerMillisecond;
		const long long TimeSpan::MinMilliSeconds = -9223372036854775806LL / TicksPerMillisecond;
		
		const long long TimeSpan::TicksPerTenthSecond = TicksPerMillisecond * 100;

		const TimeSpan TimeSpan::Zero(0);
		const TimeSpan TimeSpan::MaxValue(9223372036854775807LL);
		const TimeSpan TimeSpan::MinValue(-9223372036854775806LL);
		
		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		TimeSpan::TimeSpan()
		{
			ticks = 0;
		}
		//---------------------------------------------------------------------------
		TimeSpan::TimeSpan(long long ticks)
		{
			this->ticks = ticks;
		}
		//---------------------------------------------------------------------------
		TimeSpan::TimeSpan(int hours, int minutes, int seconds)
		{
			ticks = TimeToTicks(hours, minutes, seconds);
		}
		//---------------------------------------------------------------------------
		TimeSpan::TimeSpan(int days, int hours, int minutes, int seconds)
		{
			long long totalMilliSeconds = ((long long)days * 3600 * 24 + (long long)hours * 3600 + (long long)minutes * 60 + seconds) * 1000;
			if (totalMilliSeconds > MaxMilliSeconds || totalMilliSeconds < MinMilliSeconds)
			{
				throw ArgumentOutOfRangeException(L"milliseconds", __WFILE__, __LINE__);
			}
			ticks = totalMilliSeconds * TicksPerMillisecond;
		}
		//---------------------------------------------------------------------------
		TimeSpan::TimeSpan(int days, int hours, int minutes, int seconds, int milliseconds)
		{
			long long totalMilliSeconds = ((long long)days * 3600 * 24 + (long long)hours * 3600 + (long long)minutes * 60 + seconds) * 1000 + milliseconds;
			if (totalMilliSeconds > MaxMilliSeconds || totalMilliSeconds < MinMilliSeconds)
			{
				throw ArgumentOutOfRangeException(L"milliseconds", __WFILE__, __LINE__);
			}
			ticks = totalMilliSeconds * TicksPerMillisecond;
		}
		//---------------------------------------------------------------------------
		//Getter/Setter
		//---------------------------------------------------------------------------
		int TimeSpan::GetDays()
		{
			return (int)(ticks / TicksPerDay);
		}
		//---------------------------------------------------------------------------
		int TimeSpan::GetHours()
		{
			return (int)((ticks / TicksPerHour) % 24);
		}
		//---------------------------------------------------------------------------
		int TimeSpan::GetMinutes()
		{
			return (int)((ticks / TicksPerMinute) % 60);
		}
		//---------------------------------------------------------------------------
		int TimeSpan::GetSeconds()
		{
			return (int)((ticks / TicksPerSecond) % 60);
		}
		//---------------------------------------------------------------------------
		int TimeSpan::GetMilliseconds()
		{
			return (int)((ticks / TicksPerMillisecond) % 1000);
		}
		//---------------------------------------------------------------------------
		long long TimeSpan::GetTicks() const
		{
			return ticks;
		}
		//---------------------------------------------------------------------------
		double TimeSpan::GetTotalDays()
		{
			return (double)ticks * DaysPerTick;
		}
		//---------------------------------------------------------------------------
		double TimeSpan::GetTotalHours()
		{
			return (double)ticks * HoursPerTick;
		}
		//---------------------------------------------------------------------------
		double TimeSpan::GetTotalMinutes()
		{
			return (double)ticks * MinutesPerTick;
		}
		//---------------------------------------------------------------------------
		double TimeSpan::GetTotalSeconds()
		{
			return (double)ticks * SecondsPerTick;
		}
		//---------------------------------------------------------------------------
		double TimeSpan::GetTotalMilliseconds()
		{
			double temp = (double)ticks * MillisecondsPerTick;
			if (temp > MaxMilliSeconds)
			{
				return (double)MaxMilliSeconds;
			}
			if (temp < MinMilliSeconds)
			{
				return (double)MinMilliSeconds;
			}
			return temp;
		}
		//---------------------------------------------------------------------------
		//Runtime-Functions
		//---------------------------------------------------------------------------
		const TimeSpan TimeSpan::Add(TimeSpan ts) const
		{
			long long result = ticks + ts.ticks;
			if ((ticks >> 63 == ts.ticks >> 63) && (ticks >> 63 != result >> 63))
			{
				throw ArgumentOutOfRangeException(L"ticks", __WFILE__, __LINE__);
			}
			return TimeSpan(result);
		}
		//---------------------------------------------------------------------------
		const TimeSpan TimeSpan::Subtract(TimeSpan ts) const
		{
			long long result = ticks - ts.ticks;
			if ((ticks >> 63 != ts.ticks >> 63) && (ticks >> 63 != result >> 63))
			{
				throw ArgumentOutOfRangeException(L"ticks", __WFILE__, __LINE__);
			}
			return TimeSpan(result);
		}
		//---------------------------------------------------------------------------
		bool TimeSpan::operator == (const TimeSpan &ts) const
		{
			return ticks == ts.ticks;
		}
		//---------------------------------------------------------------------------
		bool TimeSpan::operator != (const TimeSpan &ts) const
		{
			return ticks != ts.ticks;
		}
		//---------------------------------------------------------------------------
		bool TimeSpan::operator < (const TimeSpan &ts) const
		{
			return ticks < ts.ticks;
		}
		//---------------------------------------------------------------------------
		bool TimeSpan::operator > (const TimeSpan &ts) const
		{
			return ticks > ts.ticks;
		}
		//---------------------------------------------------------------------------
		bool TimeSpan::operator <= (const TimeSpan &ts) const
		{
			return ticks <= ts.ticks;
		}
		//---------------------------------------------------------------------------
		bool TimeSpan::operator >= (const TimeSpan &ts) const
		{
			return ticks >= ts.ticks;
		}
		//---------------------------------------------------------------------------
		const TimeSpan TimeSpan::operator-() const
		{
			return TimeSpan(-ticks);
		}
		//---------------------------------------------------------------------------
		const TimeSpan TimeSpan::operator+() const
		{
			return *this;
		}
		//---------------------------------------------------------------------------
		const TimeSpan TimeSpan::operator-(const TimeSpan &ts) const
		{
			return Subtract(ts);
		}
		//---------------------------------------------------------------------------
		const TimeSpan TimeSpan::operator+(const TimeSpan &ts) const
		{
			return Add(ts);
		}
		//---------------------------------------------------------------------------
		TimeSpan TimeSpan::Duration()
		{
			if (ticks == MinValue.ticks)
			{
				throw ArgumentOutOfRangeException(L"ticks", __WFILE__, __LINE__);
			}
			return TimeSpan(ticks >= 0 ? ticks : -ticks);
		}
		//---------------------------------------------------------------------------
		TimeSpan TimeSpan::Negate()
		{
			if (ticks == MinValue.ticks)
			{
				throw ArgumentOutOfRangeException(L"ticks", __WFILE__, __LINE__);
			}
			return TimeSpan(-ticks);
		}
		//---------------------------------------------------------------------------
		TimeSpan TimeSpan::FromDays(double value)
		{
			return Interval(value, MillisPerDay);
		}
		//---------------------------------------------------------------------------
		TimeSpan TimeSpan::FromHours(double value)
		{
			return Interval(value, MillisPerHour);
		}
		//---------------------------------------------------------------------------
		TimeSpan TimeSpan::FromMinutes(double value)
		{
			return Interval(value, MillisPerMinute);
		}
		//---------------------------------------------------------------------------
		TimeSpan TimeSpan::FromSeconds(double value)
		{
			return Interval(value, MillisPerSecond);
		}
		//---------------------------------------------------------------------------
		TimeSpan TimeSpan::FromMilliseconds(double value)
		{
			return Interval(value, 1);
		}
		//---------------------------------------------------------------------------
		TimeSpan TimeSpan::FromTicks(long long value)
		{
			return TimeSpan(value);
		}
		//---------------------------------------------------------------------------
		TimeSpan TimeSpan::Interval(double value, int scale)
		{
			//if (value == 0.0 / 0.0)
			//{
			//	throw ArgumentException(L"Invalid argument: value is NAN", __WFILE__, __LINE__);
			//}
			double temp = value * scale;
			double millis = temp + (value >= 0.0 ? 0.5 : -0.5);
			if ((millis > MaxMilliSeconds) || (millis < MinMilliSeconds))
			{
				throw ArgumentOutOfRangeException(L"value", __WFILE__, __LINE__);
			}
			return TimeSpan((long long)millis * TicksPerMillisecond);
		}
		//---------------------------------------------------------------------------
		long long TimeSpan::TimeToTicks(int hour, int minute, int second)
		{
			long long totalSeconds = (long long)hour * 3600 + (long long)minute * 60 + (long long)second;
			if (totalSeconds > MaxSeconds || totalSeconds < MinSeconds)
			{
				throw ArgumentOutOfRangeException(L"totalSeconds", __WFILE__, __LINE__);
			}
			return totalSeconds * TicksPerSecond;
		}
		//---------------------------------------------------------------------------
		UnicodeString TimeSpan::ToString()
		{
			int days = GetDays();
			int milliseconds = GetMilliseconds();
			if (milliseconds != 0)
			{
				if (days != 0)
				{
					return String::Format(L"%u.%02u:%02u:%02u.%03u0000", days, GetHours(), GetMinutes(), GetSeconds(), milliseconds);
				}
				return String::Format(L"%02u:%02u:%02u.%03u0000", GetHours(), GetMinutes(), GetSeconds(), milliseconds);
			}
			return String::Format(L"%02u:%02u:%02u", GetHours(), GetMinutes(), GetSeconds());
		}
		//---------------------------------------------------------------------------
	}
}
