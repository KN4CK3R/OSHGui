#include <time.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#else //Unix
#include <sys/time.h>
#endif

#include "DateTime.hpp"
#include "Exceptions.hpp"

namespace OSHGui
{
	namespace Misc
	{
		const unsigned long long DateTime::TicksPerMillisecond = 10000ui64;
		const unsigned long long DateTime::TicksPerSecond = DateTime::TicksPerMillisecond * 1000ui64;
		const unsigned long long DateTime::TicksPerMinute = DateTime::TicksPerSecond * 60ui64;
		const unsigned long long DateTime::TicksPerHour = DateTime::TicksPerMinute * 60ui64;
		const unsigned long long DateTime::TicksPerDay = DateTime::TicksPerHour * 24ui64;

		const unsigned int DateTime::MillisPerSecond = 1000;
		const unsigned int DateTime::MillisPerMinute = DateTime::MillisPerSecond * 60;
		const unsigned int DateTime::MillisPerHour = DateTime::MillisPerMinute * 60;
		const unsigned int DateTime::MillisPerDay = DateTime::MillisPerHour * 24;

		const unsigned int DateTime::DaysPerYear = 365;
		const unsigned int DateTime::DaysPer4Years = DateTime::DaysPerYear * 4 + 1;
		const unsigned int DateTime::DaysPer100Years = DateTime::DaysPer4Years * 25 - 1;
		const unsigned int DateTime::DaysPer400Years = DateTime::DaysPer100Years * 4 + 1;

		const unsigned int DateTime::DaysTo1601 = DateTime::DaysPer400Years * 4;
		const unsigned int DateTime::DaysTo1899 = DateTime::DaysPer400Years * 4 + DateTime::DaysPer100Years * 3 - 367;
		const unsigned int DateTime::DaysTo10000 = DateTime::DaysPer400Years * 25 - 366;

		const long long DateTime::MinTicks = 0ui64;
		const long long DateTime::MaxTicks = DateTime::TicksPerDay * DateTime::DaysTo10000 - 1ui64;
		const unsigned long long DateTime::MaxMillis = (unsigned long long)DateTime::DaysTo10000 * DateTime::MillisPerDay;
 
		const unsigned long long DateTime::TimezoneOffset = DateTime::GetTimezoneOffset();
		const unsigned long long DateTime::FileTimeOffset = DateTime::DaysTo1601 * DateTime::TicksPerDay;
		const unsigned long long DateTime::DoubleDateOffset = DateTime::TicksPerDay * DateTime::DaysTo1899;
 
		const int DateTime::DaysToMonth365[13] = {
			0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365
		};
		const int DateTime::DaysToMonth366[13] = {
			0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366
		};

		const DateTime DateTime::MinValue(MinTicks, Unspecified);
		const DateTime DateTime::MaxValue(MaxTicks, Unspecified);

		const unsigned long long DateTime::TicksMask = 0x3FFFFFFFFFFFFFFFui64;
		const unsigned long long DateTime::FlagsMask = 0xC000000000000000ui64;
		const unsigned long long DateTime::LocalMask = 0x8000000000000000ui64;
		const long long DateTime::TicksCeiling = 0x4000000000000000i64;
		const unsigned long long DateTime::KindUnspecified = 0x0000000000000000ui64;
		const unsigned long long DateTime::KindUtc = 0x4000000000000000ui64;
		const unsigned long long DateTime::KindLocal = 0x8000000000000000ui64;
		const unsigned long long DateTime::KindLocalAmbiguousDst = 0xC000000000000000ui64;
		const int DateTime::KindShift = 62;

		const AnsiString DateTime::dayNames[7] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
		const AnsiString DateTime::monthNames[12] = { "January", "February", "March", "Apri", "May", "June", "July", "August", "September", "October", "November", "December" };

		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		DateTime::DateTime()
		{
			dateData = (MinTicks | ((unsigned long long)Unspecified << KindShift));
		}
		//---------------------------------------------------------------------------
		DateTime::DateTime(long long ticks)
		{
			#ifndef OSHGUI_DONTUSEEXCEPTIONS
			if (ticks < MinTicks || ticks > MaxTicks)
			{
				throw ArgumentOutOfRangeException("ticks", __FILE__, __LINE__);
			}
			#endif

			dateData = ticks;
		}
		//---------------------------------------------------------------------------
		DateTime::DateTime(unsigned long long dateData)
		{
			this->dateData = dateData;
		}
		//---------------------------------------------------------------------------
		DateTime::DateTime(long long ticks, DateTimeKind kind)
		{
			#ifndef OSHGUI_DONTUSEEXCEPTIONS
			if (ticks < MinTicks || ticks > MaxTicks)
			{
				throw ArgumentOutOfRangeException("ticks", __FILE__, __LINE__);
			}
			if (kind < Unspecified || kind > Local)
			{
				throw ArgumentException("kind", __FILE__, __LINE__);
			}
			#endif

			dateData = ((unsigned long long)ticks | ((unsigned long long)kind << KindShift));
		}
		//---------------------------------------------------------------------------
		DateTime::DateTime(long long ticks, DateTimeKind kind, bool isAmbiguousDst)
		{
			#ifndef OSHGUI_DONTUSEEXCEPTIONS
			if (ticks < MinTicks || ticks > MaxTicks)
			{
				throw ArgumentOutOfRangeException("ticks", __FILE__, __LINE__);
			}
			if (kind != KindLocal)
			{
				throw ArgumentException("kind", "Internal Constructor is for local times only");	
			}
			#endif
			
			dateData = ((unsigned long long)ticks | (isAmbiguousDst ? KindLocalAmbiguousDst : KindLocal));
		}
		//---------------------------------------------------------------------------
		DateTime::DateTime(int year, int month, int day)
		{
			dateData = (unsigned long long)DateToTicks(year, month, day);
		}
		//---------------------------------------------------------------------------
		DateTime::DateTime(int year, int month, int day, int hour, int minute, int second)
		{
			dateData = (unsigned long long)(DateToTicks(year, month, day) + TimeToTicks(hour, minute, second));
		}
		//---------------------------------------------------------------------------
		DateTime::DateTime(int year, int month, int day, int hour, int minute, int second, DateTimeKind kind)
		{
			#ifndef OSHGUI_DONTUSEEXCEPTIONS
			if (kind < Unspecified || kind > Local)
			{
				throw ArgumentException("kind", __FILE__, __LINE__);
			}
			#endif

			long long ticks = DateToTicks(year, month, day) + TimeToTicks(hour, minute, second);
			dateData = ((unsigned long long)ticks | ((unsigned long long)kind << KindShift));
		}
		//---------------------------------------------------------------------------
		DateTime::DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond)
		{
			#ifndef OSHGUI_DONTUSEEXCEPTIONS
			if (millisecond < 0 || millisecond >= MillisPerSecond)
			{
				throw ArgumentOutOfRangeException("millisecond", __FILE__, __LINE__);
			}
			#endif

			long long ticks = DateToTicks(year, month, day) + TimeToTicks(hour, minute, second);
			ticks += millisecond * TicksPerMillisecond;
			#ifndef OSHGUI_DONTUSEEXCEPTIONS
			if (ticks < MinTicks || ticks > MaxTicks)
			{
				throw ArgumentOutOfRangeException("ticks", __FILE__, __LINE__);
			}
			#endif

			dateData = (unsigned long long)ticks;
		}
		//---------------------------------------------------------------------------
		DateTime::DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, DateTimeKind kind)
		{
			#ifndef OSHGUI_DONTUSEEXCEPTIONS
			if (millisecond < 0 || millisecond >= MillisPerSecond)
			{
				throw ArgumentOutOfRangeException("millisecond", __FILE__, __LINE__);
			}
			if (kind < Unspecified || kind > Local)
			{
				throw ArgumentException("kind", __FILE__, __LINE__);
			}
			#endif

			long long ticks = DateToTicks(year, month, day) + TimeToTicks(hour, minute, second);
			ticks += millisecond * TicksPerMillisecond;
			#ifndef OSHGUI_DONTUSEEXCEPTIONS
			if (ticks < MinTicks || ticks > MaxTicks)
			{
				throw ArgumentOutOfRangeException("ticks", __FILE__, __LINE__);
			}
			#endif

			dateData = ((unsigned long long)ticks | ((unsigned long long)kind << KindShift));
		}
		//---------------------------------------------------------------------------
		//Getter/Setter
		//---------------------------------------------------------------------------
		DateTime DateTime::GetDate() const
		{
			long long ticks = GetInternalTicks();
			return DateTime((unsigned long long)(ticks - ticks % TicksPerDay) | GetInternalKind());
		}
		//---------------------------------------------------------------------------
		TimeSpan DateTime::GetTimeOfDay() const
		{
			return TimeSpan(GetInternalTicks() % TicksPerDay);
		}
		//---------------------------------------------------------------------------
		int DateTime::GetYear() const
		{
			return GetDatePart(DateTime::Year);
		}
		//---------------------------------------------------------------------------
		int DateTime::GetMonth() const
		{
			return GetDatePart(DateTime::Month);
		}
		//---------------------------------------------------------------------------
		int DateTime::GetDay() const
		{
			return GetDatePart(DateTime::Day);
		}
		//---------------------------------------------------------------------------
		DateTime::DayOfWeek DateTime::GetDayOfWeek() const
		{
			return (DateTime::DayOfWeek)((GetInternalTicks() / TicksPerDay + 1) % 7);
		}
		//---------------------------------------------------------------------------
		int DateTime::GetDayOfYear() const
		{
			return GetDatePart(DateTime::DayOfYear);
		}
		//---------------------------------------------------------------------------
		int DateTime::GetHour() const
		{
			return (int)((GetInternalTicks() / TicksPerHour) % 24);
		}
		//---------------------------------------------------------------------------
		int DateTime::GetMinute() const
		{
			return (int)((GetInternalTicks() / TicksPerMinute) % 60);
		}
		//---------------------------------------------------------------------------
		int DateTime::GetSecond() const
		{
			return (int)((GetInternalTicks() / TicksPerSecond) % 60);
		}
		//---------------------------------------------------------------------------
		int DateTime::GetMillisecond() const
		{
			return (int)((GetInternalTicks()/ TicksPerMillisecond) % 1000);
		}
		//---------------------------------------------------------------------------
		long long DateTime::GetTicks() const
		{
			return GetInternalTicks();
		}
		//---------------------------------------------------------------------------
		DateTime::DateTimeKind DateTime::GetKind() const
		{
			switch (GetInternalKind())
			{
				case DateTime::KindUnspecified:
					return DateTime::Unspecified;
				case DateTime::KindUtc:
					return DateTime::Utc;
				default:
					return DateTime::Local;
			}
		}
		//---------------------------------------------------------------------------
		DateTime DateTime::GetNow()
		{
			DateTime utc = GetUtcNow();
			long long tick = utc.GetTicks() + TimezoneOffset;
			if (tick > MaxTicks)
			{
				return DateTime(MaxTicks, Local);
			}
			if (tick < MinTicks)
			{
				return DateTime(MinTicks, Local);
			}
			return DateTime(tick, DateTime::Local, false);
		}
		//---------------------------------------------------------------------------
		DateTime DateTime::GetUtcNow()
		{
			long long ticks;
#ifdef _WIN32
			GetSystemTimeAsFileTime((LPFILETIME)&ticks);
#else //Unix
			timeval tv;
			gettimeofday(&tv, 0);
			ticks = (long long)tv.tv_usec;
			ticks += (long long)(tv.tv_sec / 0.000001);
			ticks += 11644473600000000i64;
			ticks *= 10i64;
#endif
			
			return DateTime((unsigned long long)(ticks + FileTimeOffset) | DateTime::KindUtc);
		}
		//---------------------------------------------------------------------------
		DateTime DateTime::GetToday()
		{
			return GetNow().GetDate();
		}
		//---------------------------------------------------------------------------
		//Runtime-Functions
		//---------------------------------------------------------------------------
		long long DateTime::GetInternalTicks() const
		{
			return (long long)(dateData & TicksMask);
		}
		//---------------------------------------------------------------------------
		unsigned long long DateTime::GetInternalKind() const
		{
			return (unsigned long long)(dateData & FlagsMask);
		}
		//---------------------------------------------------------------------------
		DateTime DateTime::Add(double value, int scale) const
		{
			long long millis = (long long)(value * scale + (value >= 0.0 ? 0.5 : -0.5));
			#ifndef OSHGUI_DONTUSEEXCEPTIONS
			if (millis <= -((long long)MaxMillis) || millis >= (long long)MaxMillis)
			{
				throw ArgumentOutOfRangeException("value", __FILE__, __LINE__);
			}
			#endif
			return AddTicks(millis * TicksPerMillisecond);
		}
		//---------------------------------------------------------------------------
		DateTime DateTime::Add(TimeSpan value) const
		{
			return AddTicks(value.GetTicks());
		}
		//---------------------------------------------------------------------------
		DateTime DateTime::AddYears(int years) const
		{
			#ifndef OSHGUI_DONTUSEEXCEPTIONS
			if (years < -10000 || years > 10000)
			{
				throw ArgumentOutOfRangeException("years", __FILE__, __LINE__);
			}
			#endif
			
			return AddMonths(years * 12);
		}
		//---------------------------------------------------------------------------
		DateTime DateTime::AddMonths(int months) const
		{
			#ifndef OSHGUI_DONTUSEEXCEPTIONS
			if (months < -120000 || months > 120000)
			{
				throw ArgumentOutOfRangeException("months", __FILE__, __LINE__);
			}
			#endif
			
			int y = GetDatePart(Year);
			int m = GetDatePart(Month);
			int d = GetDatePart(Day);
			int i = m - 1 + months;
			if (i >= 0)
			{
				m = i % 12 + 1;
				y = y + i / 12;
			}
			else
			{
				m = 12 + (i + 1) % 12;
				y = y + (i - 11) / 12;
			}
			#ifndef OSHGUI_DONTUSEEXCEPTIONS
			if (y < 1 || y > 9999)
			{
				throw ArgumentOutOfRangeException("year", __FILE__, __LINE__);
			}
			#endif
			int days = DaysInMonth(y, m);
			if (d > days)
			{
				d = days;
			}
			
			return DateTime((unsigned long long)(DateToTicks(y, m, d) + GetInternalTicks() % TicksPerDay) | GetInternalKind());
		}
		//---------------------------------------------------------------------------
		DateTime DateTime::AddDays(double value) const
		{
			return Add(value, MillisPerDay);
		}
		//---------------------------------------------------------------------------
		DateTime DateTime::AddHours(double value) const
		{
			return Add(value, MillisPerHour);
		}
		//---------------------------------------------------------------------------
		DateTime DateTime::AddMinutes(double value) const
		{
			return Add(value, MillisPerMinute);
		}
		//---------------------------------------------------------------------------
		DateTime DateTime::AddSeconds(double value) const
		{
			return Add(value, MillisPerSecond);
		}
		//---------------------------------------------------------------------------
		DateTime DateTime::AddMilliseconds(double value) const
		{
			return Add(value, 1);
		}
		//---------------------------------------------------------------------------
		DateTime DateTime::AddTicks(long long value) const
		{
			long long ticks = GetInternalTicks() + value;
			#ifndef OSHGUI_DONTUSEEXCEPTIONS
			if (ticks > MaxTicks || ticks < MinTicks)
			{
				throw ArgumentOutOfRangeException("value", __FILE__, __LINE__);
			}
			#endif
			return DateTime((unsigned long long)ticks | GetInternalKind());
		}
		//---------------------------------------------------------------------------
		bool DateTime::operator == (const DateTime &time) const
		{
			return GetInternalTicks() == time.GetInternalTicks();
		}
		//---------------------------------------------------------------------------
		bool DateTime::operator != (const DateTime &time) const
		{
			return GetInternalTicks() != time.GetInternalTicks();
		}
		//---------------------------------------------------------------------------
		bool DateTime::operator < (const DateTime &time) const
		{
			return GetInternalTicks() < time.GetInternalTicks();
		}
		//---------------------------------------------------------------------------
		bool DateTime::operator > (const DateTime &time) const
		{
			return GetInternalTicks() > time.GetInternalTicks();
		}
		//---------------------------------------------------------------------------
		bool DateTime::operator <= (const DateTime &time) const
		{
			return GetInternalTicks() <= time.GetInternalTicks();
		}
		//---------------------------------------------------------------------------
		bool DateTime::operator >= (const DateTime &time) const
		{
			return GetInternalTicks() >= time.GetInternalTicks();
		}
		//---------------------------------------------------------------------------
		const DateTime DateTime::operator - (const TimeSpan &ts) const
		{
			long long ticks = GetInternalTicks();
			long long valueTicks = ts.GetTicks();
			#ifndef OSHGUI_DONTUSEEXCEPTIONS
			if (ticks - MinTicks < valueTicks || ticks - MaxTicks > valueTicks)
			{
				throw ArgumentOutOfRangeException("ticks", __FILE__, __LINE__);
			}
			#endif
			return DateTime((unsigned long long)(ticks - valueTicks) | GetInternalKind());
		}
		//---------------------------------------------------------------------------
		const DateTime DateTime::operator + (const TimeSpan &ts) const
		{
			long long ticks = GetInternalTicks();
			long long valueTicks = ts.GetTicks();
			#ifndef OSHGUI_DONTUSEEXCEPTIONS
			if (valueTicks > MaxTicks - ticks || valueTicks < MinTicks - ticks)
			{
				throw ArgumentOutOfRangeException("ticks", __FILE__, __LINE__);
			}
			#endif
			return DateTime((unsigned long long)(ticks + valueTicks) | GetInternalKind());
		}
		//---------------------------------------------------------------------------
		const TimeSpan DateTime::operator - (const DateTime &time) const
		{
			return TimeSpan(GetInternalTicks() - time.GetInternalTicks());
		}
		//---------------------------------------------------------------------------
		const TimeSpan DateTime::operator + (const DateTime &time) const
		{
			return TimeSpan(GetInternalTicks() + time.GetInternalTicks());
		}
		//---------------------------------------------------------------------------
		long long DateTime::DateToTicks(int year, int month, int day)
		{
			if (year >= 1 && year <= 9999)
			{
				if (month >= 1 && month <= 12)
				{
					const int *days = IsLeapYear(year) ? DaysToMonth366 : DaysToMonth365;
					if (day >= 1 && day <= days[month] - days[month - 1])
					{
						int y = year - 1;
						int n = y * 365 + y / 4 - y / 100 + y / 400 + days[month - 1] + day - 1;
						return n * TicksPerDay;
					}
					#ifndef OSHGUI_DONTUSEEXCEPTIONS
					throw ArgumentOutOfRangeException("day", __FILE__, __LINE__);
					#endif
				}
				#ifndef OSHGUI_DONTUSEEXCEPTIONS
				throw ArgumentOutOfRangeException("month", __FILE__, __LINE__);
				#endif
			}
			#ifndef OSHGUI_DONTUSEEXCEPTIONS
			throw ArgumentOutOfRangeException("year", __FILE__, __LINE__);
			#endif
			return 0;
		}
		//---------------------------------------------------------------------------
		long long DateTime::TimeToTicks(int hour, int minute, int second)
		{
			if (hour >= 0 && hour < 24)
			{
				if (minute >= 0 && minute < 60)
				{
					if (second >=0 && second < 60)
					{
						long long totalSeconds = (long long)hour * 3600 + (long long)minute * 60 + (long long)second;
						return totalSeconds * TicksPerSecond;
					}
					#ifndef OSHGUI_DONTUSEEXCEPTIONS
					throw ArgumentOutOfRangeException("second", __FILE__, __LINE__);
					#endif
				}
				#ifndef OSHGUI_DONTUSEEXCEPTIONS
				throw ArgumentOutOfRangeException("minute", __FILE__, __LINE__);
				#endif
			}
			#ifndef OSHGUI_DONTUSEEXCEPTIONS
			throw ArgumentOutOfRangeException("hour", __FILE__, __LINE__);
			#endif
			return 0;
		}
		//---------------------------------------------------------------------------
		int DateTime::DaysInMonth(int year, int month)
		{
			if (month < 1 || month > 12)
			{
				#ifndef OSHGUI_DONTUSEEXCEPTIONS
				throw ArgumentOutOfRangeException("month", __FILE__, __LINE__);
				#endif
				return 0;
			}
			
			const int *days = IsLeapYear(year) ? DaysToMonth366 : DaysToMonth365;
			return days[month] - days[month - 1];
		}
		//---------------------------------------------------------------------------
		DateTime DateTime::SpecifyKind(DateTime value, DateTimeKind kind)
		{
			return DateTime(value.GetInternalTicks(), kind);
		}
		//---------------------------------------------------------------------------
		bool DateTime::IsLeapYear(int year)
		{
			#ifndef OSHGUI_DONTUSEEXCEPTIONS
			if (year < 1 || year > 9999)
			{
				throw ArgumentOutOfRangeException("year", __FILE__, __LINE__);
			}
			#endif
			
			return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
		}
		//---------------------------------------------------------------------------
		long long DateTime::GetTimezoneOffset()
		{
			time_t now;
			struct tm local, utc;
			
			time(&now);
			local = *localtime(&now);
			utc = *gmtime(&now);
			
			int diff = local.tm_hour - utc.tm_hour;
						
			return (long long)TicksPerHour * diff;
		}
		//---------------------------------------------------------------------------
		int DateTime::GetDatePart(DatePart part) const
		{
			long long ticks = GetInternalTicks();
			int n = (int)(ticks / TicksPerDay); //number of days since 1/1/0001
			int y400 = n / DaysPer400Years; //number of whole 400-year periods since 1/1/0001
			n -= y400 * DaysPer400Years; //day number within 400-year period
			int y100 = n / DaysPer100Years; //number of whole 100-year periods within 400-year period
			if (y100 == 4)
			{
				y100 = 3; //last 100-year period has an extra day, so decrement result if 4
			}
			n -= y100 * DaysPer100Years; //day number within 100-year period
			int y4 = n / DaysPer4Years; //number of whole 4-year periods within 100-year period
			n -= y4 * DaysPer4Years; //day number within 4-year period
			int y1 = n / DaysPerYear; //number of whole years within 4-year period
			if (y1 == 4)
			{
				y1 = 3; //last year has an extra day, so decrement result if 4
			}
			if (part == DateTime::Year)
			{
				return y400 * 400 + y100 * 100 + y4 * 4 + y1 + 1;
			}

			n -= y1 * DaysPerYear; //day number within year
			if (part == DateTime::DayOfYear)
			{
				return n + 1;
			}

			bool leapYear = y1 == 3 && (y4 != 24 || y100 == 3);
			const int *days = leapYear ? DaysToMonth366 : DaysToMonth365;
			int m = (n >> 5) + 1;
			while (n >= days[m])
			{
				++m;
			}
			if (part == DateTime::Month)
			{
				return m;
			}

			return n - days[m - 1] + 1;
		}
		//---------------------------------------------------------------------------
		AnsiString DateTime::ToString()
		{
			return String::Format("%02u.%02u.%04u %02u:%02u:%02u", GetDay(), GetMonth(), GetYear(), GetHour(), GetMinute(), GetSecond());
		}
		//---------------------------------------------------------------------------
		AnsiString DateTime::ToLongDateString()
		{
			return String::Format("%s, %02u. %s %04u", dayNames[(int)GetDayOfWeek()].c_str(), GetDay(), monthNames[(int)GetMonth() - 1].c_str(), GetYear());
		}
		//---------------------------------------------------------------------------
		AnsiString DateTime::ToLongTimeString()
		{
			return String::Format("%02u:%02u:%02u", GetHour(), GetMinute(), GetSecond());
		}
		//---------------------------------------------------------------------------
		AnsiString DateTime::ToShortDateString()
		{
			return String::Format("%02u.%02u.%04u", GetDay(), GetMonth(), GetYear());
		}
		//---------------------------------------------------------------------------
		AnsiString DateTime::ToShortTimeString()
		{
			return String::Format("%02u:%02u", GetHour(), GetMinute());
		}
		//---------------------------------------------------------------------------
	}
}