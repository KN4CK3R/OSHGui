#ifndef __OSHGUI_MISC_DATETIME_H__
#define __OSHGUI_MISC_DATETIME_H__

#include "Strings.h"

namespace OSHGui
{
	namespace Misc
	{	
		class DateTime
		{
		public:
			enum DateTimeKind
			{
				Unspecified = 0,
				Utc,
				Local
			};
		
			enum DayOfWeek
			{
				Sunday = 0,
				Monday,
				Tuesday,
				Wednesday,
				Thursday,
				Friday,
				Saturday
			};

			enum DatePart
			{
				Year = 0,
				DayOfYear,
				Month,
				Day
			};

		private:
			//Number of 100ns ticks per time unit
			static const unsigned long long TicksPerMillisecond;
			static const unsigned long long TicksPerSecond;
			static const unsigned long long TicksPerMinute;
			static const unsigned long long TicksPerHour;
			static const unsigned long long TicksPerDay;

			// Number of milliseconds per time unit 
			static const unsigned int MillisPerSecond;
			static const unsigned int MillisPerMinute;
			static const unsigned int MillisPerHour;
			static const unsigned int MillisPerDay;

			// Number of days in a non-leap year
			static const unsigned int DaysPerYear;
			// Number of days in 4 years
			static const unsigned int DaysPer4Years;
			// Number of days in 100 years
			static const unsigned int DaysPer100Years;
			// Number of days in 400 years
			static const unsigned int DaysPer400Years;

			// Number of days from 1/1/0001 to 12/31/1600
			static const unsigned int DaysTo1601;
			// Number of days from 1/1/0001 to 12/30/1899
			static const unsigned int DaysTo1899;
			// Number of days from 1/1/0001 to 12/31/9999
			static const unsigned int DaysTo10000;

			static const unsigned long long MinTicks;
			static const unsigned long long MaxTicks;
			static const unsigned long long MaxMillis;
 
			static const unsigned long long TimezoneOffset;
			static const unsigned long long FileTimeOffset;
			static const unsigned long long DoubleDateOffset;

			static const int DaysToMonth365[13];
			static const int DaysToMonth366[13];
 
			static const unsigned long long TicksMask;
			static const unsigned long long FlagsMask;
			static const unsigned long long LocalMask;
			static const long long TicksCeiling;
			static const unsigned long long KindUnspecified;
			static const unsigned long long KindUtc;
			static const unsigned long long KindLocal;
			static const unsigned long long KindLocalAmbiguousDst;
			static const int KindShift;

			static const UnicodeString dayNames[7];
			static const UnicodeString monthNames[12];

			unsigned long long dateData;

		public:
			static const DateTime MinValue;
			static const DateTime MaxValue;

		private:
			DateTime(unsigned long long dateData);
			DateTime(unsigned long long ticks, DateTimeKind kind, bool isAmbiguousDst);
			
			long long GetInternalTicks();
			unsigned long long GetInternalKind();
			
			DateTime Add(double value, int scale);
			
			static long long DateToTicks(int year, int month, int day);
			static long long TimeToTicks(int hour, int minute, int second);
			
			int GetDatePart(DatePart part);
			
			static bool TryCreate(int year, int month, int day, int hour, int minute, int second, int millisecond, DateTime *result);

		public:
			DateTime(long long ticks);
			DateTime(unsigned long long ticks, DateTimeKind kind);
			DateTime(int year, int month, int day);
			DateTime(int year, int month, int day, int hour, int minute, int second);
			DateTime(int year, int month, int day, int hour, int minute, int second, DateTimeKind kind);
			DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond);
			DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, DateTimeKind kind);
			
			DateTime AddDays(double value);
			DateTime AddHours(double value);
			DateTime AddMilliseconds(double value);
			DateTime AddMinutes(double value);
			DateTime AddMonths(int months);
			DateTime AddSeconds(double value);
			DateTime AddTicks(long long value);
			DateTime AddYears(int value);
			
			bool operator == (DateTime &time);
			bool operator != (DateTime &time);
			bool operator < (DateTime &time);
			bool operator > (DateTime &time);
			bool operator <= (DateTime &time);
			bool operator >= (DateTime &time);
			
			static int DaysInMonth(int year, int month);
			
			static DateTime SpecifyKind(DateTime value, DateTimeKind kind);
			
			static bool IsLeapYear(int year);
			
			static unsigned long long GetTimezoneOffset();
			
			DateTime GetDate();
			int GetYear();
			int GetMonth();
			int GetDay();
			DayOfWeek GetDayOfWeek();
			int GetDayOfYear();
			int GetHour();
			int GetMinute();
			int GetSecond();
			int GetMillisecond();
			long long GetTicks();
			
			DateTimeKind GetKind();
			
			static DateTime GetNow();
			static DateTime GetUtcNow();			
			static DateTime GetToday();

			UnicodeString ToString();
			UnicodeString ToLongDateString();
			UnicodeString ToLongTimeString();
			UnicodeString ToShortDateString();
			UnicodeString ToShortTimeString();
		};
	}
}

#endif
