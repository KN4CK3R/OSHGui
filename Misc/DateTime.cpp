/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R https://www.oldschoolhack.me/
 *
 * See license in OSHGui.hpp
 */

#include <time.h>

#include <windows.h>

#include "DateTime.hpp"
#include "Exceptions.hpp"

namespace OSHGui
{
	namespace Misc
	{
		const unsigned long long DateTime::TicksPerMillisecond = 10000ui64;
		const unsigned long long DateTime::TicksPerSecond = TicksPerMillisecond * 1000ui64;
		const unsigned long long DateTime::TicksPerMinute = TicksPerSecond * 60ui64;
		const unsigned long long DateTime::TicksPerHour = TicksPerMinute * 60ui64;
		const unsigned long long DateTime::TicksPerDay = TicksPerHour * 24ui64;

		const unsigned int DateTime::MillisPerSecond = 1000;
		const unsigned int DateTime::MillisPerMinute = MillisPerSecond * 60;
		const unsigned int DateTime::MillisPerHour = MillisPerMinute * 60;
		const unsigned int DateTime::MillisPerDay = MillisPerHour * 24;

		const unsigned int DateTime::DaysPerYear = 365;
		const unsigned int DateTime::DaysPer4Years = DaysPerYear * 4 + 1;
		const unsigned int DateTime::DaysPer100Years = DaysPer4Years * 25 - 1;
		const unsigned int DateTime::DaysPer400Years = DaysPer100Years * 4 + 1;

		const unsigned int DateTime::DaysTo1601 = DaysPer400Years * 4;
		const unsigned int DateTime::DaysTo1899 = DaysPer400Years * 4 + DaysPer100Years * 3 - 367;
		const unsigned int DateTime::DaysTo10000 = DaysPer400Years * 25 - 366;

		const long long DateTime::MinTicks = 0ui64;
		const long long DateTime::MaxTicks = TicksPerDay * DaysTo10000 - 1ui64;
		const unsigned long long DateTime::MaxMillis = static_cast<unsigned long long>(DaysTo10000) * MillisPerDay;
 
		const unsigned long long DateTime::TimezoneOffset = GetTimezoneOffset();
		const unsigned long long DateTime::FileTimeOffset = DaysTo1601 * TicksPerDay;
		const unsigned long long DateTime::DoubleDateOffset = TicksPerDay * DaysTo1899;
 
		const int DateTime::DaysToMonth365[13] = {
			0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365
		};
		const int DateTime::DaysToMonth366[13] = {
			0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366
		};

		const DateTime DateTime::MinValue(MinTicks, DateTimeKind::Unspecified);
		const DateTime DateTime::MaxValue(MaxTicks, DateTimeKind::Unspecified);

		const unsigned long long DateTime::TicksMask = 0x3FFFFFFFFFFFFFFFui64;
		const unsigned long long DateTime::FlagsMask = 0xC000000000000000ui64;
		const unsigned long long DateTime::LocalMask = 0x8000000000000000ui64;
		const long long DateTime::TicksCeiling = 0x4000000000000000i64;
		const unsigned long long DateTime::KindUnspecified = 0x0000000000000000ui64;
		const unsigned long long DateTime::KindUtc = 0x4000000000000000ui64;
		const unsigned long long DateTime::KindLocal = 0x8000000000000000ui64;
		const unsigned long long DateTime::KindLocalAmbiguousDst = 0xC000000000000000ui64;
		const int DateTime::KindShift = 62;

		const AnsiString DateTime::DayNames[7] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
		const AnsiString DateTime::MonthNames[12] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };

		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		DateTime::DateTime()
		{
			dateData_ = (MinTicks | static_cast<unsigned long long>(DateTimeKind::Unspecified) << KindShift);
		}
		//---------------------------------------------------------------------------
		DateTime::DateTime(long long ticks)
		{
			#ifndef OSHGUI_DONTUSEEXCEPTIONS
			if (ticks < MinTicks || ticks > MaxTicks)
			{
				throw ArgumentOutOfRangeException("ticks");
			}
			#endif

			dateData_ = ticks;
		}
		//---------------------------------------------------------------------------
		DateTime::DateTime(unsigned long long dateData)
		{
			dateData_ = dateData;
		}
		//---------------------------------------------------------------------------
		DateTime::DateTime(long long ticks, DateTimeKind kind)
		{
			#ifndef OSHGUI_DONTUSEEXCEPTIONS
			if (ticks < MinTicks || ticks > MaxTicks)
			{
				throw ArgumentOutOfRangeException("ticks");
			}
			if (kind < DateTimeKind::Unspecified || kind > DateTimeKind::Local)
			{
				throw ArgumentException("kind");
			}
			#endif

			dateData_ = static_cast<unsigned long long>(ticks) | (static_cast<unsigned long long>(kind) << KindShift);
		}
		//---------------------------------------------------------------------------
		DateTime::DateTime(long long ticks, DateTimeKind kind, bool isAmbiguousDst)
		{
			#ifndef OSHGUI_DONTUSEEXCEPTIONS
			if (ticks < MinTicks || ticks > MaxTicks)
			{
				throw ArgumentOutOfRangeException("ticks");
			}
			if (kind != DateTimeKind::Local)
			{
				throw ArgumentException("kind", "Internal Constructor is for local times only");	
			}
			#endif
			
			dateData_ = static_cast<unsigned long long>(ticks) | (isAmbiguousDst ? KindLocalAmbiguousDst : KindLocal);
		}
		//---------------------------------------------------------------------------
		DateTime::DateTime(int year, int month, int day)
		{
			dateData_ = static_cast<unsigned long long>(DateToTicks(year, month, day));
		}
		//---------------------------------------------------------------------------
		DateTime::DateTime(int year, int month, int day, int hour, int minute, int second)
		{
			dateData_ = static_cast<unsigned long long>(DateToTicks(year, month, day) + TimeToTicks(hour, minute, second));
		}
		//---------------------------------------------------------------------------
		DateTime::DateTime(int year, int month, int day, int hour, int minute, int second, DateTimeKind kind)
		{
			#ifndef OSHGUI_DONTUSEEXCEPTIONS
			if (kind < DateTimeKind::Unspecified || kind > DateTimeKind::Local)
			{
				throw ArgumentException("kind");
			}
			#endif

			const auto ticks = DateToTicks(year, month, day) + TimeToTicks(hour, minute, second);
			dateData_ = static_cast<unsigned long long>(ticks) | (static_cast<unsigned long long>(kind) << KindShift);
		}
		//---------------------------------------------------------------------------
		DateTime::DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond)
		{
			#ifndef OSHGUI_DONTUSEEXCEPTIONS
			if (millisecond < 0 || millisecond >= static_cast<int>(MillisPerSecond))
			{
				throw ArgumentOutOfRangeException("millisecond");
			}
			#endif

			auto ticks = DateToTicks(year, month, day) + TimeToTicks(hour, minute, second);
			ticks += millisecond * TicksPerMillisecond;
			#ifndef OSHGUI_DONTUSEEXCEPTIONS
			if (ticks < MinTicks || ticks > MaxTicks)
			{
				throw ArgumentOutOfRangeException("ticks");
			}
			#endif

			dateData_ = static_cast<unsigned long long>(ticks);
		}
		//---------------------------------------------------------------------------
		DateTime::DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, DateTimeKind kind)
		{
			#ifndef OSHGUI_DONTUSEEXCEPTIONS
			if (millisecond < 0 || millisecond >= static_cast<int>(MillisPerSecond))
			{
				throw ArgumentOutOfRangeException("millisecond");
			}
			if (kind < DateTimeKind::Unspecified || kind > DateTimeKind::Local)
			{
				throw ArgumentException("kind");
			}
			#endif

			auto ticks = DateToTicks(year, month, day) + TimeToTicks(hour, minute, second);
			ticks += millisecond * TicksPerMillisecond;
			#ifndef OSHGUI_DONTUSEEXCEPTIONS
			if (ticks < MinTicks || ticks > MaxTicks)
			{
				throw ArgumentOutOfRangeException("ticks");
			}
			#endif

			dateData_ = static_cast<unsigned long long>(ticks) | (static_cast<unsigned long long>(kind) << KindShift);
		}
		//---------------------------------------------------------------------------
		//Getter/Setter
		//---------------------------------------------------------------------------
		DateTime DateTime::GetDate() const
		{
			const auto ticks = GetInternalTicks();
			return DateTime(static_cast<unsigned long long>(ticks - ticks % TicksPerDay) | GetInternalKind());
		}
		//---------------------------------------------------------------------------
		TimeSpan DateTime::GetTimeOfDay() const
		{
			return TimeSpan(GetInternalTicks() % TicksPerDay);
		}
		//---------------------------------------------------------------------------
		int DateTime::GetYear() const
		{
			return GetDatePart(DatePart::Year);
		}
		//---------------------------------------------------------------------------
		int DateTime::GetMonth() const
		{
			return GetDatePart(DatePart::Month);
		}
		//---------------------------------------------------------------------------
		int DateTime::GetDay() const
		{
			return GetDatePart(DatePart::Day);
		}
		//---------------------------------------------------------------------------
		DayOfWeek DateTime::GetDayOfWeek() const
		{
			return static_cast<DayOfWeek>((GetInternalTicks() / TicksPerDay + 1) % 7);
		}
		//---------------------------------------------------------------------------
		int DateTime::GetDayOfYear() const
		{
			return GetDatePart(DatePart::DayOfYear);
		}
		//---------------------------------------------------------------------------
		int DateTime::GetHour() const
		{
			return static_cast<int>((GetInternalTicks() / TicksPerHour) % 24);
		}
		//---------------------------------------------------------------------------
		int DateTime::GetMinute() const
		{
			return static_cast<int>((GetInternalTicks() / TicksPerMinute) % 60);
		}
		//---------------------------------------------------------------------------
		int DateTime::GetSecond() const
		{
			return static_cast<int>((GetInternalTicks() / TicksPerSecond) % 60);
		}
		//---------------------------------------------------------------------------
		int DateTime::GetMillisecond() const
		{
			return static_cast<int>((GetInternalTicks() / TicksPerMillisecond) % 1000);
		}
		//---------------------------------------------------------------------------
		long long DateTime::GetTicks() const
		{
			return GetInternalTicks();
		}
		//---------------------------------------------------------------------------
		DateTimeKind DateTime::GetKind() const
		{
			switch (GetInternalKind())
			{
				case KindUnspecified:
					return DateTimeKind::Unspecified;
				case KindUtc:
					return DateTimeKind::Utc;
				default:
					return DateTimeKind::Local;
			}
		}
		//---------------------------------------------------------------------------
		DateTime DateTime::GetNow()
		{
			const auto utc = GetUtcNow();
			const long long tick = utc.GetTicks() + TimezoneOffset;
			if (tick > MaxTicks)
			{
				return DateTime(MaxTicks, DateTimeKind::Local);
			}
			if (tick < MinTicks)
			{
				return DateTime(MinTicks, DateTimeKind::Local);
			}
			return DateTime(tick, DateTimeKind::Local, false);
		}
		//---------------------------------------------------------------------------
		DateTime DateTime::GetUtcNow()
		{
			long long ticks;
			GetSystemTimeAsFileTime(reinterpret_cast<LPFILETIME>(&ticks));
			
			return DateTime(static_cast<unsigned long long>(ticks + FileTimeOffset) | KindUtc);
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
			return static_cast<long long>(dateData_ & TicksMask);
		}
		//---------------------------------------------------------------------------
		unsigned long long DateTime::GetInternalKind() const
		{
			return static_cast<unsigned long long>(dateData_ & FlagsMask);
		}
		//---------------------------------------------------------------------------
		DateTime DateTime::Add(double value, int scale) const
		{
			const auto millis = static_cast<long long>(value * scale + (value >= 0.0 ? 0.5 : -0.5));
			#ifndef OSHGUI_DONTUSEEXCEPTIONS
			if (millis <= -static_cast<long long>(MaxMillis) || millis >= static_cast<long long>(MaxMillis))
			{
				throw ArgumentOutOfRangeException("value");
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
				throw ArgumentOutOfRangeException("years");
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
				throw ArgumentOutOfRangeException("months");
			}
			#endif
			
			auto y = GetDatePart(DatePart::Year);
			auto m = GetDatePart(DatePart::Month);
			auto d = GetDatePart(DatePart::Day);
			const auto i = m - 1 + months;
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
				throw ArgumentOutOfRangeException("year");
			}
			#endif
			const auto days = DaysInMonth(y, m);
			if (d > days)
			{
				d = days;
			}
			
			return DateTime(static_cast<unsigned long long>(DateToTicks(y, m, d) + GetInternalTicks() % TicksPerDay) | GetInternalKind());
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
			const auto ticks = GetInternalTicks() + value;
			#ifndef OSHGUI_DONTUSEEXCEPTIONS
			if (ticks > MaxTicks || ticks < MinTicks)
			{
				throw ArgumentOutOfRangeException("value");
			}
			#endif
			return DateTime(static_cast<unsigned long long>(ticks) | GetInternalKind());
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
		DateTime DateTime::operator - (const TimeSpan &ts) const
		{
			const auto ticks = GetInternalTicks();
			const auto valueTicks = ts.GetTicks();
			#ifndef OSHGUI_DONTUSEEXCEPTIONS
			if (ticks - MinTicks < valueTicks || ticks - MaxTicks > valueTicks)
			{
				throw ArgumentOutOfRangeException("ticks");
			}
			#endif
			return DateTime(static_cast<unsigned long long>(ticks - valueTicks) | GetInternalKind());
		}
		//---------------------------------------------------------------------------
		DateTime DateTime::operator + (const TimeSpan &ts) const
		{
			const auto ticks = GetInternalTicks();
			const auto valueTicks = ts.GetTicks();
			#ifndef OSHGUI_DONTUSEEXCEPTIONS
			if (valueTicks > MaxTicks - ticks || valueTicks < MinTicks - ticks)
			{
				throw ArgumentOutOfRangeException("ticks");
			}
			#endif
			return DateTime(static_cast<unsigned long long>(ticks + valueTicks) | GetInternalKind());
		}
		//---------------------------------------------------------------------------
		TimeSpan DateTime::operator - (const DateTime &time) const
		{
			return TimeSpan(GetInternalTicks() - time.GetInternalTicks());
		}
		//---------------------------------------------------------------------------
		TimeSpan DateTime::operator + (const DateTime &time) const
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
					const auto days = IsLeapYear(year) ? DaysToMonth366 : DaysToMonth365;
					if (day >= 1 && day <= days[month] - days[month - 1])
					{
						const auto y = year - 1;
						const auto n = y * 365 + y / 4 - y / 100 + y / 400 + days[month - 1] + day - 1;
						return n * TicksPerDay;
					}
					#ifndef OSHGUI_DONTUSEEXCEPTIONS
					throw ArgumentOutOfRangeException("day");
					#endif
				}
				#ifndef OSHGUI_DONTUSEEXCEPTIONS
				throw ArgumentOutOfRangeException("month");
				#endif
			}
			#ifndef OSHGUI_DONTUSEEXCEPTIONS
			throw ArgumentOutOfRangeException("year");
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
						const auto totalSeconds = static_cast<long long>(hour) * 3600
							+ static_cast<long long>(minute) * 60
							+ static_cast<long long>(second);
						return totalSeconds * TicksPerSecond;
					}
					#ifndef OSHGUI_DONTUSEEXCEPTIONS
					throw ArgumentOutOfRangeException("second");
					#endif
				}
				#ifndef OSHGUI_DONTUSEEXCEPTIONS
				throw ArgumentOutOfRangeException("minute");
				#endif
			}
			#ifndef OSHGUI_DONTUSEEXCEPTIONS
			throw ArgumentOutOfRangeException("hour");
			#endif
			return 0;
		}
		//---------------------------------------------------------------------------
		int DateTime::DaysInMonth(int year, int month)
		{
			if (month < 1 || month > 12)
			{
				#ifndef OSHGUI_DONTUSEEXCEPTIONS
				throw ArgumentOutOfRangeException("month");
				#endif
				return 0;
			}
			
			const auto days = IsLeapYear(year) ? DaysToMonth366 : DaysToMonth365;
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
				throw ArgumentOutOfRangeException("year");
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
			localtime_s(&local, &now);
			gmtime_s(&utc, &now);
			
			const auto diff = local.tm_hour - utc.tm_hour;

			return static_cast<long long>(TicksPerHour) * diff;
		}
		//---------------------------------------------------------------------------
		int DateTime::GetDatePart(DatePart part) const
		{
			const auto ticks = GetInternalTicks();
			int n = static_cast<int>(ticks / TicksPerDay); //number of days since 1/1/0001
			const auto y400 = n / DaysPer400Years; //number of whole 400-year periods since 1/1/0001
			n -= y400 * DaysPer400Years; //day number within 400-year period
			auto y100 = n / DaysPer100Years; //number of whole 100-year periods within 400-year period
			if (y100 == 4)
			{
				y100 = 3; //last 100-year period has an extra day, so decrement result if 4
			}
			n -= y100 * DaysPer100Years; //day number within 100-year period
			const auto y4 = n / DaysPer4Years; //number of whole 4-year periods within 100-year period
			n -= y4 * DaysPer4Years; //day number within 4-year period
			auto y1 = n / DaysPerYear; //number of whole years within 4-year period
			if (y1 == 4)
			{
				y1 = 3; //last year has an extra day, so decrement result if 4
			}
			if (part == DatePart::Year)
			{
				return y400 * 400 + y100 * 100 + y4 * 4 + y1 + 1;
			}

			n -= y1 * DaysPerYear; //day number within year
			if (part == DatePart::DayOfYear)
			{
				return n + 1;
			}

			const auto leapYear = y1 == 3 && (y4 != 24 || y100 == 3);
			const auto days = leapYear ? DaysToMonth366 : DaysToMonth365;
			int m = (n >> 5) + 1;
			while (n >= days[m])
			{
				++m;
			}
			if (part == DatePart::Month)
			{
				return m;
			}

			return n - days[m - 1] + 1;
		}
		//---------------------------------------------------------------------------
		AnsiString DateTime::ToString() const
		{
			return String::Format("%02u.%02u.%04u %02u:%02u:%02u", GetDay(), GetMonth(), GetYear(), GetHour(), GetMinute(), GetSecond());
		}
		//---------------------------------------------------------------------------
		AnsiString DateTime::ToLongDateString() const
		{
			return String::Format("%s, %02u. %s %04u", DayNames[(int)GetDayOfWeek()].c_str(), GetDay(), MonthNames[(int)GetMonth() - 1].c_str(), GetYear());
		}
		//---------------------------------------------------------------------------
		AnsiString DateTime::ToLongTimeString() const
		{
			return String::Format("%02u:%02u:%02u", GetHour(), GetMinute(), GetSecond());
		}
		//---------------------------------------------------------------------------
		AnsiString DateTime::ToShortDateString() const
		{
			return String::Format("%02u.%02u.%04u", GetDay(), GetMonth(), GetYear());
		}
		//---------------------------------------------------------------------------
		AnsiString DateTime::ToShortTimeString() const
		{
			return String::Format("%02u:%02u", GetHour(), GetMinute());
		}
		//---------------------------------------------------------------------------
	}
}