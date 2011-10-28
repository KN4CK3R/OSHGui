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
		const unsigned __int64 DateTime::TicksPerMillisecond = 10000ui64;
		const unsigned __int64 DateTime::TicksPerSecond = DateTime::TicksPerMillisecond * 1000ui64;
		const unsigned __int64 DateTime::TicksPerMinute = DateTime::TicksPerSecond * 60ui64;
		const unsigned __int64 DateTime::TicksPerHour = DateTime::TicksPerMinute * 60ui64;
		const unsigned __int64 DateTime::TicksPerDay = DateTime::TicksPerHour * 24ui64;

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

		const __int64 DateTime::MinTicks = 0ui64;
		const __int64 DateTime::MaxTicks = DateTime::TicksPerDay * DateTime::DaysTo10000 - 1ui64;
		const unsigned __int64 DateTime::MaxMillis = (unsigned __int64)DateTime::DaysTo10000 * DateTime::MillisPerDay;
 
		const unsigned __int64 DateTime::TimezoneOffset = DateTime::GetTimezoneOffset();
		const unsigned __int64 DateTime::FileTimeOffset = DateTime::DaysTo1601 * DateTime::TicksPerDay;
		const unsigned __int64 DateTime::DoubleDateOffset = DateTime::TicksPerDay * DateTime::DaysTo1899;
 
		const int DateTime::DaysToMonth365[13] = {
			0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365
		};
		const int DateTime::DaysToMonth366[13] = {
			0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366
		};

		const DateTime DateTime::MinValue(MinTicks, Unspecified);
		const DateTime DateTime::MaxValue(MaxTicks, Unspecified);

		const unsigned __int64 DateTime::TicksMask = 0x3FFFFFFFFFFFFFFFui64;
		const unsigned __int64 DateTime::FlagsMask = 0xC000000000000000ui64;
		const unsigned __int64 DateTime::LocalMask = 0x8000000000000000ui64;
		const __int64 DateTime::TicksCeiling = 0x4000000000000000i64;
		const unsigned __int64 DateTime::KindUnspecified = 0x0000000000000000ui64;
		const unsigned __int64 DateTime::KindUtc = 0x4000000000000000ui64;
		const unsigned __int64 DateTime::KindLocal = 0x8000000000000000ui64;
		const unsigned __int64 DateTime::KindLocalAmbiguousDst = 0xC000000000000000ui64;
		const int DateTime::KindShift = 62;

		const UnicodeString DateTime::dayNames[7] = { L"Sunday", L"Monday", L"Tuesday", L"Wednesday", L"Thursday", L"Friday", L"Saturday" };
		const UnicodeString DateTime::monthNames[12] = { L"January", L"February", L"March", L"April", L"May", L"June", L"July", L"August", L"September", L"October", L"November", L"December" };

		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		DateTime::DateTime()
		{
			dateData = (MinTicks | ((unsigned __int64)Unspecified << KindShift));
		}
		//---------------------------------------------------------------------------
		DateTime::DateTime(__int64 ticks)
		{
			if (ticks < MinTicks || ticks > MaxTicks)
			{
				throw ArgumentOutOfRangeException(L"ticks", __WFILE__, __LINE__);
			}

			dateData = ticks;
		}
		//---------------------------------------------------------------------------
		DateTime::DateTime(unsigned __int64 dateData)
		{
			this->dateData = dateData;
		}
		//---------------------------------------------------------------------------
		DateTime::DateTime(__int64 ticks, DateTimeKind kind)
		{
			if (ticks < MinTicks || ticks > MaxTicks)
			{
				throw ArgumentOutOfRangeException(L"ticks", __WFILE__, __LINE__);
			}
			if (kind < Unspecified || kind > Local)
			{
				throw ArgumentException(L"kind", __WFILE__, __LINE__);
			}

			dateData = ((unsigned __int64)ticks | ((unsigned __int64)kind << KindShift));
		}
		//---------------------------------------------------------------------------
		DateTime::DateTime(__int64 ticks, DateTimeKind kind, bool isAmbiguousDst)
		{
			if (ticks < MinTicks || ticks > MaxTicks)
			{
				throw ArgumentOutOfRangeException(L"ticks", __WFILE__, __LINE__);
			}

			if (kind == KindLocal)
			{
				throw ArgumentException(L"kind", L"Internal Constructor is for local times only");	
			}
			
			dateData = ((unsigned __int64)ticks | (isAmbiguousDst ? KindLocalAmbiguousDst : KindLocal));
		}
		//---------------------------------------------------------------------------
		DateTime::DateTime(int year, int month, int day)
		{
			dateData = (unsigned __int64)DateToTicks(year, month, day);
		}
		//---------------------------------------------------------------------------
		DateTime::DateTime(int year, int month, int day, int hour, int minute, int second)
		{
			dateData = (unsigned __int64)(DateToTicks(year, month, day) + TimeToTicks(hour, minute, second));
		}
		//---------------------------------------------------------------------------
		DateTime::DateTime(int year, int month, int day, int hour, int minute, int second, DateTimeKind kind)
		{
			if (kind < Unspecified || kind > Local)
			{
				throw ArgumentException(L"kind", __WFILE__, __LINE__);
			}

			__int64 ticks = DateToTicks(year, month, day) + TimeToTicks(hour, minute, second);
			dateData = ((unsigned __int64)ticks | ((unsigned __int64)kind << KindShift));
		}
		//---------------------------------------------------------------------------
		DateTime::DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond)
		{
			if (millisecond < 0 || millisecond >= MillisPerSecond)
			{
				throw ArgumentOutOfRangeException(L"millisecond", __WFILE__, __LINE__);
			}

			__int64 ticks = DateToTicks(year, month, day) + TimeToTicks(hour, minute, second);
			ticks += millisecond * TicksPerMillisecond;
			if (ticks < MinTicks || ticks > MaxTicks)
			{
				throw ArgumentOutOfRangeException(L"ticks", __WFILE__, __LINE__);
			}

			dateData = (unsigned __int64)ticks;
		}
		//---------------------------------------------------------------------------
		DateTime::DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, DateTimeKind kind)
		{
			if (millisecond < 0 || millisecond >= MillisPerSecond)
			{
				throw ArgumentOutOfRangeException(L"millisecond", __WFILE__, __LINE__);
			}
			if (kind < Unspecified || kind > Local)
			{
				throw ArgumentException(L"kind", __WFILE__, __LINE__);
			}

			__int64 ticks = DateToTicks(year, month, day) + TimeToTicks(hour, minute, second);
			ticks += millisecond * TicksPerMillisecond;
			if (ticks < MinTicks || ticks > MaxTicks)
			{
				throw ArgumentOutOfRangeException(L"ticks", __WFILE__, __LINE__);
			}

			dateData = ((unsigned __int64)ticks | ((unsigned __int64)kind << KindShift));
		}
		//---------------------------------------------------------------------------
		//Getter/Setter
		//---------------------------------------------------------------------------
		DateTime DateTime::GetDate() const
		{
			__int64 ticks = GetInternalTicks();
			return DateTime((unsigned __int64)(ticks - ticks % TicksPerDay) | GetInternalKind());
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
		__int64 DateTime::GetTicks() const
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
			__int64 tick = utc.GetTicks() + TimezoneOffset;
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
			__int64 ticks;
#ifdef _WIN32
			GetSystemTimeAsFileTime((LPFILETIME)&ticks);
#else //Unix
			timeval tv;
			gettimeofday(&tv, 0);
			ticks = (__int64)tv.tv_usec;
			ticks += (__int64)(tv.tv_sec / 0.000001);
			ticks += 11644473600000000i64;
			ticks *= 10i64;
#endif
			
			return DateTime((unsigned __int64)(ticks + FileTimeOffset) | DateTime::KindUtc);
		}
		//---------------------------------------------------------------------------
		DateTime DateTime::GetToday()
		{
			return GetNow().GetDate();
		}
		//---------------------------------------------------------------------------
		//Runtime-Functions
		//---------------------------------------------------------------------------
		__int64 DateTime::GetInternalTicks() const
		{
			return (__int64)(dateData & TicksMask);
		}
		//---------------------------------------------------------------------------
		unsigned __int64 DateTime::GetInternalKind() const
		{
			return (unsigned __int64)(dateData & FlagsMask);
		}
		//---------------------------------------------------------------------------
		DateTime DateTime::Add(double value, int scale) const
		{
			__int64 millis = (__int64)(value * scale + (value >= 0.0 ? 0.5 : -0.5));
			if (millis <= -((__int64)MaxMillis) || millis >= (__int64)MaxMillis)
			{
				throw ArgumentOutOfRangeException(L"value", __WFILE__, __LINE__);
			}
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
			if (years < -10000 || years > 10000)
			{
				throw ArgumentOutOfRangeException(L"years", __WFILE__, __LINE__);
			}
			
			return AddMonths(years * 12);
		}
		//---------------------------------------------------------------------------
		DateTime DateTime::AddMonths(int months) const
		{
			if (months < -120000 || months > 120000)
			{
				throw ArgumentOutOfRangeException(L"months", __WFILE__, __LINE__);
			}
			
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
			if (y < 1 || y > 9999)
			{
				throw ArgumentOutOfRangeException(L"year", __WFILE__, __LINE__);
			}
			int days = DaysInMonth(y, m);
			if (d > days)
			{
				d = days;
			}
			
			return DateTime((unsigned __int64)(DateToTicks(y, m, d) + GetInternalTicks() % TicksPerDay) | GetInternalKind());
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
		DateTime DateTime::AddTicks(__int64 value) const
		{
			__int64 ticks = GetInternalTicks() + value;
			if (ticks > MaxTicks || ticks < MinTicks)
			{
				throw ArgumentOutOfRangeException(L"value", __WFILE__, __LINE__);
			}
			return DateTime((unsigned __int64)ticks | GetInternalKind());
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
			__int64 ticks = GetInternalTicks();
			__int64 valueTicks = ts.GetTicks();
			if (ticks - MinTicks < valueTicks || ticks - MaxTicks > valueTicks)
			{
				throw ArgumentOutOfRangeException(L"ticks", __WFILE__, __LINE__);
			}
			return DateTime((unsigned __int64)(ticks - valueTicks) | GetInternalKind());
		}
		//---------------------------------------------------------------------------
		const DateTime DateTime::operator + (const TimeSpan &ts) const
		{
			__int64 ticks = GetInternalTicks();
			__int64 valueTicks = ts.GetTicks();
			if (valueTicks > MaxTicks - ticks || valueTicks < MinTicks - ticks)
			{
				throw ArgumentOutOfRangeException(L"ticks", __WFILE__, __LINE__);
			}
			return DateTime((unsigned __int64)(ticks + valueTicks) | GetInternalKind());
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
		__int64 DateTime::DateToTicks(int year, int month, int day)
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
					throw ArgumentOutOfRangeException(L"day", __WFILE__, __LINE__);
				}
				throw ArgumentOutOfRangeException(L"month", __WFILE__, __LINE__);
			}
			throw ArgumentOutOfRangeException(L"year", __WFILE__, __LINE__);
		}
		//---------------------------------------------------------------------------
		__int64 DateTime::TimeToTicks(int hour, int minute, int second)
		{
			if (hour >= 0 && hour < 24)
			{
				if (minute >= 0 && minute < 60)
				{
					if (second >=0 && second < 60)
					{
						__int64 totalSeconds = (__int64)hour * 3600 + (__int64)minute * 60 + (__int64)second;
						return totalSeconds * TicksPerSecond;
					}
					throw ArgumentOutOfRangeException(L"second", __WFILE__, __LINE__);
				}
				throw ArgumentOutOfRangeException(L"minute", __WFILE__, __LINE__);
			}
			throw ArgumentOutOfRangeException(L"hour", __WFILE__, __LINE__);
		}
		//---------------------------------------------------------------------------
		int DateTime::DaysInMonth(int year, int month)
		{
			if (month < 1 || month > 12)
			{
				throw ArgumentOutOfRangeException(L"month", __WFILE__, __LINE__);
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
			if (year < 1 || year > 9999)
			{
				throw ArgumentOutOfRangeException(L"year", __WFILE__, __LINE__);
			}
			
			return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
		}
		//---------------------------------------------------------------------------
		__int64 DateTime::GetTimezoneOffset()
		{
			time_t now;
			struct tm local, utc;
			
			time(&now);
			local = *localtime(&now);
			utc = *gmtime(&now);
			
			int diff = local.tm_hour - utc.tm_hour;
						
			return (__int64)TicksPerHour * diff;
		}
		//---------------------------------------------------------------------------
		int DateTime::GetDatePart(DatePart part) const
		{
			__int64 ticks = GetInternalTicks();
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
			int m = (n >> 5) + 1; //1-based month number
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
		UnicodeString DateTime::ToString()
		{
			return String::Format(L"%02u.%02u.%04u %02u:%02u:%02u", GetDay(), GetMonth(), GetYear(), GetHour(), GetMinute(), GetSecond());
		}
		//---------------------------------------------------------------------------
		UnicodeString DateTime::ToLongDateString()
		{
			return String::Format(L"%s, %02u. %s %04u", dayNames[(int)GetDayOfWeek()].c_str(), GetDay(), monthNames[(int)GetMonth() - 1].c_str(), GetYear());
		}
		//---------------------------------------------------------------------------
		UnicodeString DateTime::ToLongTimeString()
		{
			return String::Format(L"%02u:%02u:%02u", GetHour(), GetMinute(), GetSecond());
		}
		//---------------------------------------------------------------------------
		UnicodeString DateTime::ToShortDateString()
		{
			return String::Format(L"%02u.%02u.%04u", GetDay(), GetMonth(), GetYear());
		}
		//---------------------------------------------------------------------------
		UnicodeString DateTime::ToShortTimeString()
		{
			return String::Format(L"%02u:%02u", GetHour(), GetMinute());
		}
		//---------------------------------------------------------------------------
	}
}