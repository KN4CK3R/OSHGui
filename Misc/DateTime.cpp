#include "DateTime.h"
#include <time.h>
#include <stdexcept>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

namespace OSHGui
{
	namespace Misc
	{
		const unsigned long long DateTime::TicksPerMillisecond = 10000ULL;
		const unsigned long long DateTime::TicksPerSecond = DateTime::TicksPerMillisecond * 1000ULL;
		const unsigned long long DateTime::TicksPerMinute = DateTime::TicksPerSecond * 60ULL;
		const unsigned long long DateTime::TicksPerHour = DateTime::TicksPerMinute * 60ULL;
		const unsigned long long DateTime::TicksPerDay = DateTime::TicksPerHour * 24ULL;

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

		const unsigned long long DateTime::MinTicks = 0ULL;
		const unsigned long long DateTime::MaxTicks = DateTime::TicksPerDay * DateTime::DaysTo10000 - 1ULL;
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

		const unsigned long long DateTime::TicksMask = 0x3FFFFFFFFFFFFFFFULL;
		const unsigned long long DateTime::FlagsMask = 0xC000000000000000ULL;
		const unsigned long long DateTime::LocalMask = 0x8000000000000000ULL;
		const long long DateTime::TicksCeiling = 0x4000000000000000LL;
		const unsigned long long DateTime::KindUnspecified = 0x0000000000000000ULL;
		const unsigned long long DateTime::KindUtc = 0x4000000000000000ULL;
		const unsigned long long DateTime::KindLocal = 0x8000000000000000ULL;
		const unsigned long long DateTime::KindLocalAmbiguousDst = 0xC000000000000000ULL;
		const int DateTime::KindShift = 62;

		const UnicodeString DateTime::dayNames[7] = { L"Sunday", L"Monday", L"Tuesday", L"Wednesday", L"Thursday", L"Friday", L"Saturday" };
		const UnicodeString DateTime::monthNames[12] = { L"January", L"February", L"March", L"April", L"May", L"June", L"July", L"August", L"September", L"October", L"November", L"December" };

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
			if (ticks < MinTicks || ticks > MaxTicks)
			{
				throw std::out_of_range("Argument out of range: ticks");
			}

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
			if (ticks < MinTicks || ticks > MaxTicks)
			{
				throw std::out_of_range("Argument out of range: ticks");
			}
			if (kind < Unspecified || kind > Local)
			{
				throw std::invalid_argument("Invalid argument: kind");
			}

			dateData = ((unsigned long long)ticks | ((unsigned long long)kind << KindShift));
		}
		//---------------------------------------------------------------------------
		DateTime::DateTime(long long ticks, DateTimeKind kind, bool isAmbiguousDst)
		{
			if (ticks < MinTicks || ticks > MaxTicks)
			{
				throw std::out_of_range("Argument out of range: ticks");
			}
			
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
			if (kind < Unspecified || kind > Local)
			{
				throw std::invalid_argument("Invalid argument: kind");
			}

			long long ticks = DateToTicks(year, month, day) + TimeToTicks(hour, minute, second);
			dateData = ((unsigned long long)ticks | ((unsigned long long)kind << KindShift));
		}
		//---------------------------------------------------------------------------
		DateTime::DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond)
		{
			if (millisecond < 0 || millisecond >= MillisPerSecond)
			{
				throw std::out_of_range("Argument out of range: millisecond");
			}

			long long ticks = DateToTicks(year, month, day) + TimeToTicks(hour, minute, second);
			ticks += millisecond * TicksPerMillisecond;
			if (ticks < MinTicks || ticks > MaxTicks)
			{
				throw std::out_of_range("Argument out of range: timerange");
			}

			dateData = (unsigned long long)ticks;
		}
		//---------------------------------------------------------------------------
		DateTime::DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, DateTimeKind kind)
		{
			if (millisecond < 0 || millisecond >= MillisPerSecond)
			{
				throw std::out_of_range("Argument out of range: millisecond");
			}
			if (kind < Unspecified || kind > Local)
			{
				throw std::invalid_argument("Invalid argument: kind");
			}

			long long ticks = DateToTicks(year, month, day) + TimeToTicks(hour, minute, second);
			ticks += millisecond * TicksPerMillisecond;
			if (ticks < MinTicks || ticks > MaxTicks)
			{
				throw std::out_of_range("Argument out of range: timerange");
			}

			dateData = ((unsigned long long)ticks | ((unsigned long long)kind << KindShift));
		}
		//---------------------------------------------------------------------------
		//Runtime-Functions
		//---------------------------------------------------------------------------
		long long DateTime::GetInternalTicks()
		{
			return (long long)(dateData & TicksMask);
		}
		//---------------------------------------------------------------------------
		unsigned long long DateTime::GetInternalKind()
		{
			return (unsigned long long)(dateData & FlagsMask);
		}
		//---------------------------------------------------------------------------
		DateTime DateTime::Add(double value, int scale)
		{
			long long millis = (long long)(value * scale + (value >= 0.0 ? 0.5 : -0.5));
			if (millis <= (long long)-MaxMillis || millis >= (long long)MaxMillis)
			{
				throw std::out_of_range("Argument out of range: add value");
			}
			return AddTicks(millis * TicksPerMillisecond);
		}
		//---------------------------------------------------------------------------
		DateTime DateTime::AddYears(int value)
		{
			if (value < -10000 || value > 10000)
			{
				throw std::out_of_range("Argument out of range: years");
			}
			
			return AddMonths(value * 12);
		}
		//---------------------------------------------------------------------------
		DateTime DateTime::AddMonths(int months)
		{
			if (months < -120000 || months > 120000)
			{
				throw std::out_of_range("Argument out of range: months");
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
				throw std::out_of_range("Argument out of range: months");
			}
			int days = DaysInMonth(y, m);
			if (d > days)
			{
				d = days;
			}
			
			return DateTime((unsigned long long)(DateToTicks(y, m, d) + GetInternalTicks() % TicksPerDay) | GetInternalKind());
		}
		//---------------------------------------------------------------------------
		DateTime DateTime::AddDays(double value)
		{
			return Add(value, MillisPerDay);
		}
		//---------------------------------------------------------------------------
		DateTime DateTime::AddHours(double value)
		{
			return Add(value, MillisPerHour);
		}
		//---------------------------------------------------------------------------
		DateTime DateTime::AddMinutes(double value)
		{
			return Add(value, MillisPerMinute);
		}
		//---------------------------------------------------------------------------
		DateTime DateTime::AddSeconds(double value)
		{
			return Add(value, MillisPerSecond);
		}
		//---------------------------------------------------------------------------
		DateTime DateTime::AddMilliseconds(double value)
		{
			return Add(value, 1);
		}
		//---------------------------------------------------------------------------
		DateTime DateTime::AddTicks(long long value)
		{
			long long ticks = GetInternalTicks() + value;
			if (ticks > MaxTicks || ticks < MinTicks)
			{
				throw std::out_of_range("Argument out of range: ticks");
			}
			return DateTime((unsigned long long)ticks | GetInternalKind());
		}
		//---------------------------------------------------------------------------
		bool DateTime::operator == (DateTime &time)
		{
			return GetInternalTicks() == time.GetInternalTicks();
		}
		//---------------------------------------------------------------------------
		bool DateTime::operator != (DateTime &time)
		{
			return GetInternalTicks() != time.GetInternalTicks();
		}
		//---------------------------------------------------------------------------
		bool DateTime::operator < (DateTime &time)
		{
			return GetInternalTicks() < time.GetInternalTicks();
		}
		//---------------------------------------------------------------------------
		bool DateTime::operator > (DateTime &time)
		{
			return GetInternalTicks() > time.GetInternalTicks();
		}
		//---------------------------------------------------------------------------
		bool DateTime::operator <= (DateTime &time)
		{
			return GetInternalTicks() <= time.GetInternalTicks(); 
		}
		//---------------------------------------------------------------------------
		bool DateTime::operator >= (DateTime &time)
		{		
			return GetInternalTicks() >= time.GetInternalTicks(); 
		}
		//---------------------------------------------------------------------------
		long long DateTime::DateToTicks(int year, int month, int day)
		{
			if (year >= 1 && year <= 9999 && month >= 1 && month <= 12)
			{
				const int *days = IsLeapYear(year) ? DaysToMonth366 : DaysToMonth365;
				if (day >= 1 && day <= days[month] - days[month - 1])
				{
					int y = year - 1;
					int n = y * 365 + y / 4 - y / 100 + y / 400 + days[month - 1] + day - 1;
					return n * TicksPerDay;
				}
			}
			throw std::out_of_range("Argument out of range: year, month, day");
		}
		//---------------------------------------------------------------------------
		long long DateTime::TimeToTicks(int hour, int minute, int second)
		{
			if (hour >= 0 && hour < 24 && minute >= 0 && minute < 60 && second >=0 && second < 60)
			{
				long long totalSeconds = (long long)hour * 3600 + (long long)minute * 60 + (long long)second; 
				return totalSeconds * TicksPerSecond;
			}
			throw std::out_of_range("Argument out of range: hour, minute, second");
		}
		//---------------------------------------------------------------------------
		int DateTime::DaysInMonth(int year, int month)
		{
			if (month < 1 || month > 12)
			{
				throw std::out_of_range("Argument out of range: month");
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
				throw std::out_of_range("Argument out of range: year");
			}
			
			return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
		}
		//---------------------------------------------------------------------------
		unsigned long long DateTime::GetTimezoneOffset()
		{
			time_t now;
			struct tm local, utc;
			
			time(&now);
			local = *localtime(&now);
			utc = *gmtime(&now);
			
			int diff = local.tm_hour - utc.tm_hour;
						
			return TicksPerHour * diff;
		}
		//---------------------------------------------------------------------------
		int DateTime::GetDatePart(DatePart part)
		{
			long long ticks = GetInternalTicks();
			// n = number of days since 1/1/0001
			int n = (int)(ticks / TicksPerDay);
			// y400 = number of whole 400-year periods since 1/1/0001
			int y400 = n / DaysPer400Years;
			// n = day number within 400-year period
			n -= y400 * DaysPer400Years;
			// y100 = number of whole 100-year periods within 400-year period
			int y100 = n / DaysPer100Years;
			// Last 100-year period has an extra day, so decrement result if 4
			if (y100 == 4) y100 = 3;
			// n = day number within 100-year period
			n -= y100 * DaysPer100Years;
			// y4 = number of whole 4-year periods within 100-year period
			int y4 = n / DaysPer4Years;
			// n = day number within 4-year period
			n -= y4 * DaysPer4Years;
			// y1 = number of whole years within 4-year period
			int y1 = n / DaysPerYear;
			// Last year has an extra day, so decrement result if 4
			if (y1 == 4)
			{
				y1 = 3;
			}
			// If year was requested, compute and return it
			if (part == DateTime::Year)
			{
				return y400 * 400 + y100 * 100 + y4 * 4 + y1 + 1;
			}
			// n = day number within year
			n -= y1 * DaysPerYear;
			// If day-of-year was requested, return it
			if (part == DateTime::DayOfYear)
			{
				return n + 1;
			}
			// Leap year calculation looks different from IsLeapYear since y1, y4,
			// and y100 are relative to year 1, not year 0
			bool leapYear = y1 == 3 && (y4 != 24 || y100 == 3);
			const int *days = leapYear ? DaysToMonth366 : DaysToMonth365;
			// All months have less than 32 days, so n >> 5 is a good conservative
			// estimate for the month
			int m = n >> 5 + 1;
			// m = 1-based month number
			while (n >= days[m])
			{
				m++;
			}
			// If month was requested, return it
			if (part == DateTime::Month)
			{
				return m;
			}
			// Return 1-based day-of-month
			return n - days[m - 1] + 1;
		}
		//---------------------------------------------------------------------------
		DateTime DateTime::GetDate()
		{
			long long ticks = GetInternalTicks();
			return DateTime((unsigned long long)(ticks - ticks % TicksPerDay) | GetInternalKind());
		}
		//---------------------------------------------------------------------------
		int DateTime::GetYear()
		{
			return GetDatePart(DateTime::Year);
		}
		//---------------------------------------------------------------------------
		int DateTime::GetMonth()
		{
			return GetDatePart(DateTime::Month);
		}
		//---------------------------------------------------------------------------
		int DateTime::GetDay()
		{
			return GetDatePart(DateTime::Day);
		}
		//---------------------------------------------------------------------------
		DateTime::DayOfWeek DateTime::GetDayOfWeek()
		{
			return (DateTime::DayOfWeek)((GetInternalTicks() / TicksPerDay + 1) % 7);
		}
		//---------------------------------------------------------------------------
		int DateTime::GetDayOfYear()
		{
			return GetDatePart(DateTime::DayOfYear);
		}
		//---------------------------------------------------------------------------
		int DateTime::GetHour()
		{
			return (int)((GetInternalTicks() / TicksPerHour) % 24);
		}
		//---------------------------------------------------------------------------
		int DateTime::GetMinute()
		{
			return (int)((GetInternalTicks() / TicksPerMinute) % 60);
		}
		//---------------------------------------------------------------------------
		int DateTime::GetSecond()
		{
			return (int)((GetInternalTicks() / TicksPerSecond) % 60);
		}
		//---------------------------------------------------------------------------
		int DateTime::GetMillisecond()
		{
			return (int)((GetInternalTicks()/ TicksPerMillisecond) % 1000);
		}
		//---------------------------------------------------------------------------
		long long DateTime::GetTicks()
		{
			return GetInternalTicks();
		}
		//---------------------------------------------------------------------------
		DateTime::DateTimeKind DateTime::GetKind()
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
			GetSystemTimeAsFileTime((LPFILETIME)&ticks);
			
			return DateTime((unsigned long long)(ticks + FileTimeOffset) | DateTime::KindUtc);
		}
		//---------------------------------------------------------------------------
		DateTime DateTime::GetToday()
		{
			return GetNow().GetDate();
		}
		//---------------------------------------------------------------------------
		UnicodeString DateTime::ToString()
		{
			return Format(L"%02u.%02u.%04u %02u:%02u:%02u", GetDay(), GetMonth(), GetYear(), GetHour(), GetMinute(), GetSecond());
		}
		//---------------------------------------------------------------------------
		UnicodeString DateTime::ToLongDateString()
		{
			return Format(L"%s, %02u. %s %04u", dayNames[(int)GetDayOfWeek()].c_str(), GetDay(), monthNames[(int)GetMonth() - 1].c_str(), GetYear());
		}
		//---------------------------------------------------------------------------
		UnicodeString DateTime::ToLongTimeString()
		{
			return Format(L"%02u:%02u:%02u", GetHour(), GetMinute(), GetSecond());
		}
		//---------------------------------------------------------------------------
		UnicodeString DateTime::ToShortDateString()
		{
			return Format(L"%02u.%02u.%04u", GetDay(), GetMonth(), GetYear());
		}
		//---------------------------------------------------------------------------
		UnicodeString DateTime::ToShortTimeString()
		{
			return Format(L"%02u:%02u", GetHour(), GetMinute());
		}
		//---------------------------------------------------------------------------
	}
}