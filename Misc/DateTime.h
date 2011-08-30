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
			/**
			 * Gibt an, ob ein DateTime-Objekt eine lokale Zeit, eine koordinate Weltzeit
			 * (UTC) angibt oder weder lokale Zeit noch UTC angegeben sind.
			 */
			enum DateTimeKind
			{
				Unspecified = 0,
				Utc,
				Local
			};
		
			/**
			 * Gibt den Wochentag an.
			 */
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

		private:
			enum DatePart
			{
				Year = 0,
				DayOfYear,
				Month,
				Day
			};
		
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

			static const long long MinTicks;
			static const long long MaxTicks;
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
			/**
			 * Stellt den kleinstm�glichen Wert von DateTime dar.
			 */
			static const DateTime MinValue;
			/**
			 * Stellt den gr��tm�glichen Wert von DateTime dar.
			 */
			static const DateTime MaxValue;

		private:
			DateTime(unsigned long long dateData);
			DateTime(long long ticks, DateTimeKind kind, bool isAmbiguousDst);
			
			long long GetInternalTicks();
			unsigned long long GetInternalKind();
			
			DateTime Add(double value, int scale);
			
			static long long DateToTicks(int year, int month, int day);
			static long long TimeToTicks(int hour, int minute, int second);
			
			int GetDatePart(DatePart part);
			
		public:
			/**
			 * Initialisiert eine neue Instanz der DateTime-Struktur als MinValue.
			 */
			DateTime();
			/**
			 * Initialisiert eine neue Instanz der DateTime-Struktur mit der angegebenen Anzahl an Ticks.
			 *
			 * @param ticks Eine in der Einheit 100 Nanosekunden ausgedr�ckte Datums- und Uhrzeitangabe
			 */
			DateTime(long long ticks);
			/**
			 * Initialisiert eine neue Instanz der DateTime-Struktur mit der angegebenen Anzahl an Ticks
			 * und koordinierter Weltzeit (UTC) oder lokaler Zeit.
			 *
			 * @param ticks Eine in der Einheit 100 Nanosekunden ausgedr�ckte Datums- und Uhrzeitangabe
			 * @param kind Einer der DateTimeKind-Werte, der angibt, ob ticks eine lokale Zeit, UTC angibt oder keine Angabe enth�lt
			 */
			DateTime(long long ticks, DateTimeKind kind);
			/**
			 * Initialisiert eine neue Instanz der DateTime-Struktur mit dem angegebenen Jahr, Monat und Tag.
			 *
			 * @param year das Jahr (1 bis 9999)
			 * @param month der Monat (1 bis 12)
			 * @param day der Tag (1 bis zur Anzahl der Tage in month)
			 */
			DateTime(int year, int month, int day);
			/**
			 * Initialisiert eine neue Instanz der DateTime-Struktur mit dem angegebenen Jahr, Monat, Tag, Stunde, Minute und Sekunde.
			 *
			 * @param year das Jahr (1 bis 9999)
			 * @param month der Monat (1 bis 12)
			 * @param day der Tag (1 bis zur Anzahl der Tage in month)
			 * @param hour die Stunde (0 bis 23)
			 * @param minute die Minute (0 bis 59)
			 * @param second die Sekunde (0 bis 59)
			 */
			DateTime(int year, int month, int day, int hour, int minute, int second);
			/**
			 * Initialisiert eine neue Instanz der DateTime-Struktur mit dem angegebenen Jahr, Monat, Tag, Stunde, Minute, Sekunde
			 * und koordinierter Weltzeit (UTC) oder lokaler Zeit.
			 *
			 * @param year das Jahr (1 bis 9999)
			 * @param month der Monat (1 bis 12)
			 * @param day der Tag (1 bis zur Anzahl der Tage in month)
			 * @param hour die Stunden (0 bis 23)
			 * @param minute die Minuten (0 bis 59)
			 * @param second die Sekunden (0 bis 59)
			 * @param kind Einer der DateTimeKind-Werte, der angibt, ob ticks eine lokale Zeit, UTC angibt oder keine Angabe enth�lt
			 */
			DateTime(int year, int month, int day, int hour, int minute, int second, DateTimeKind kind);
			/**
			 * Initialisiert eine neue Instanz der DateTime-Struktur mit dem angegebenen Jahr, Monat, Tag, Stunde, Minute, Sekunde
			 * und Milisekunden.
			 *
			 * @param year das Jahr (1 bis 9999)
			 * @param month der Monat (1 bis 12)
			 * @param day der Tag (1 bis zur Anzahl der Tage in month)
			 * @param hour die Stunden (0 bis 23)
			 * @param minute die Minuten (0 bis 59)
			 * @param second die Sekunden (0 bis 59)
			 * @param millisecond die Millisekunden (0 bis 999)
			 */
			DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond);
			/**
			 * Initialisiert eine neue Instanz der DateTime-Struktur mit dem angegebenen Jahr, Monat, Tag, Stunde, Minute, Sekunde,
			 * Milisekunden und koordinierter Weltzeit (UTC) oder lokaler Zeit.
			 *
			 * @param year das Jahr (1 bis 9999)
			 * @param month der Monat (1 bis 12)
			 * @param day der Tag (1 bis zur Anzahl der Tage in month)
			 * @param hour die Stunden (0 bis 23)
			 * @param minute die Minuten (0 bis 59)
			 * @param second die Sekunden (0 bis 59)
			 * @param millisecond die Millisekunden (0 bis 999)
			 * @param kind Einer der DateTimeKind-Werte, der angibt, ob ticks eine lokale Zeit, UTC angibt oder keine Angabe enth�lt
			 */
			DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, DateTimeKind kind);
			
			/**
			 * Addiert die angegebene Anzahl von Jahren zum Wert dieser Instanz und gibt die neue DateTime-Struktur zur�ck.
			 *
			 * @param value Eine Anzahl von Jahren. value kann negativ sein.
			 * @return die neue DateTime-Struktur
			 */
			DateTime AddYears(int value);
			/**
			 * Addiert die angegebene Anzahl von Monaten zum Wert dieser Instanz und gibt die neue DateTime-Struktur zur�ck.
			 *
			 * @param value Eine Anzahl von Monaten. value kann negativ sein.
			 * @return die neue DateTime-Struktur
			 */
			DateTime AddMonths(int months);
			/**
			 * Addiert die angegebene Anzahl von Tagen zum Wert dieser Instanz und gibt die neue DateTime-Struktur zur�ck.
			 *
			 * @param value Eine Zahl, die aus ganzen Tagen und dem Bruchteil eines Tages besteht. value kann negativ sein.
			 * @return die neue DateTime-Struktur
			 */
			DateTime AddDays(double value);
			/**
			 * Addiert die angegebene Anzahl von Stunden zum Wert dieser Instanz und gibt die neue DateTime-Struktur zur�ck.
			 *
			 * @param value Eine Zahl, die aus ganzen Stunden und dem Bruchteil einer Stunde besteht. value kann negativ sein.
			 * @return die neue DateTime-Struktur
			 */
			DateTime AddHours(double value);
			/**
			 * Addiert die angegebene Anzahl von Minuten zum Wert dieser Instanz und gibt die neue DateTime-Struktur zur�ck.
			 *
			 * @param value Eine Zahl, die aus ganzen Minuten und dem Bruchteil einer Minute besteht. value kann negativ sein.
			 * @return die neue DateTime-Struktur
			 */
			DateTime AddMinutes(double value);
			/**
			 * Addiert die angegebene Anzahl von Sekunden zum Wert dieser Instanz und gibt die neue DateTime-Struktur zur�ck.
			 *
			 * @param value Eine Zahl, die aus ganzen Sekunden und dem Bruchteil einer Sekunde besteht. value kann negativ sein.
			 * @return die neue DateTime-Struktur
			 */
			DateTime AddSeconds(double value);
			/**
			 * Addiert die angegebene Anzahl von Millisekunden zum Wert dieser Instanz und gibt die neue DateTime-Struktur zur�ck.
			 *
			 * @param value Eine Zahl, die aus ganzen Millisekunden und dem Bruchteil einer Millisekunde besteht. value kann negativ sein.
			 * @return die neue DateTime-Struktur
			 */
			DateTime AddMilliseconds(double value);
			/**
			 * Addiert die angegebene Anzahl von Ticks zum Wert dieser Instanz und gibt die neue DateTime-Struktur zur�ck.
			 *
			 * @param value Eine Anzahl von 100-Nanosekunden-Ticks. value kann negativ sein.
			 * @return die neue DateTime-Struktur
			 */
			DateTime AddTicks(long long value);
			
			bool operator == (DateTime &time);
			bool operator != (DateTime &time);
			bool operator < (DateTime &time);
			bool operator > (DateTime &time);
			bool operator <= (DateTime &time);
			bool operator >= (DateTime &time);
			
			/**
			 * Ruft die Anzahl der Tage im angegeben Monat und Jahr ab.
			 *
			 * @param year das Jahr (1 bis 9999)
			 * @param month der Monat (1 bis 12)
			 * @return die Anzahl der Tage
			 */
			static int DaysInMonth(int year, int month);
			
			/**
			 * Erstellt ein neues DateTime-Objekt, das die gleiche Zeit wie die angegebene DateTime darstellt, aber entsprechend dem
			 * DateTimeKind-Wert in Ortszeit, koordinierter Weltzeit (UTC) oder keinem von beidem angegeben ist.
			 *
			 * @param value die DateTime-Struktur
			 * @param kind Einer der DateTimeKind-Werte, der angibt, ob ticks eine lokale Zeit, UTC angibt oder keine Angabe enth�lt
			 * @return die neue DateTime-Struktur
			 */
			static DateTime SpecifyKind(DateTime value, DateTimeKind kind);
			
			/**
			 * Ruft ab, ob das angegebene Jahr ein Schaltjahr ist.
			 *
			 * @param year das Jahr (1 bis 9999)
			 * @return ja/nein
			 */
			static bool IsLeapYear(int year);
			
			/**
			 * Ruft den Unterschied der aktuellen Zeitzone zur UTC ab.
			 *
			 * @return der Zeitzonenunterschied
			 */
			static unsigned long long GetTimezoneOffset();
			
			/**
			 * Ruft die Datumskomponente dieser Instanz ab.
			 *
			 * @return die Datumskomponente
			 */
			DateTime GetDate();
			/**
			 * Ruft das Jahr ab, das durch diese Instanz dargestellt wird.
			 *
			 * @return das Jahr
			 */
			int GetYear();
			/**
			 * Ruft den Monat ab, der durch diese Instanz dargestellt wird.
			 *
			 * @return der Monat
			 */
			int GetMonth();
			/**
			 * Ruft den Tag des Monats ab, der durch diese Instanz dargestellt wird.
			 *
			 * @return der Tag
			 */
			int GetDay();
			/**
			 * Ruft den Wochentag ab, der durch diese Instanz dargestellt wird.
			 *
			 * @return der Wochentag
			 */
			DayOfWeek GetDayOfWeek();
			/**
			 * Ruft den Tag des Jahres ab, der durch diese Instanz dargestellt wird.
			 *
			 * @return der Tag
			 */
			int GetDayOfYear();
			/**
			 * Ruft die Komponente f�r die Stunden des Datums ab, das durch diese Instanz dargestellt wird.
			 *
			 * @return die Stunden
			 */
			int GetHour();
			/**
			 * Ruft die Komponente f�r die Minuten des Datums ab, das durch diese Instanz dargestellt wird.
			 *
			 * @return die Minuten
			 */
			int GetMinute();
			/**
			 * Ruft die Komponente f�r die Sekunden des Datums ab, das durch diese Instanz dargestellt wird.
			 *
			 * @return die Sekunden
			 */
			int GetSecond();
			/**
			 * Ruft die Komponente f�r die Millisekunden des Datums ab, das durch diese Instanz dargestellt wird.
			 *
			 * @return die Millisekunden
			 */
			int GetMillisecond();
			/**
			 * Ruft die Anzahl der Ticks ab, die Datum und Uhrzeit dieser Instanz dargestellen.
			 *
			 * @return der Tag
			 */
			long long GetTicks();
			
			/**
			 * Ruft einen Wert ab, der angibt, ob die durch diese Instanz dargestellte Zeit
			 * auf lokaler Zeit, koordinierter Weltzeit (UTC) oder keinem von beidem basiert.
			 *
			 * @return der DateTimeKind-Wert
			 */
			DateTimeKind GetKind();
			
			/**
			 * Ruft ein DateTime-Objekt ab, das auf das aktuelle Datum und die aktuelle Zeit
			 * auf dem lokalen Rechner als lokale Zeit festgelegt ist.
			 *
			 * @return das DateTime-Objekt
			 */
			static DateTime GetNow();
			/**
			 * Ruft ein DateTime-Objekt ab, das auf das aktuelle Datum und die aktuelle Zeit
			 * auf dem lokalen Rechner als lokale Zeit festgelegt ist.
			 *
			 * @return das DateTime-Objekt
			 */
			static DateTime GetUtcNow();
			/**
			 * Ruft das aktuelle Datum ab.
			 *
			 * @return das aktuelle Datum
			 */
			static DateTime GetToday();

			/**
			 * Gibt eine Zeichenfolgendarstellung des DateTime-Objekts zur�ck.
			 * Format: d.m.Y H:i:s => 23.04.2011 02:23:48
			 *
			 * @return die Zeichenfolgendarstellung
			 */
			UnicodeString ToString();
			/**
			 * Gibt eine Zeichenfolgendarstellung des DateTime-Objekts zur�ck.
			 * Format: l, d. F Y => Saturday, 23. April 2011
			 *
			 * @return die Zeichenfolgendarstellung
			 */
			UnicodeString ToLongDateString();
			/**
			 * Gibt eine Zeichenfolgendarstellung des DateTime-Objekts zur�ck.
			 * Format: H:i:s => 02:23:48
			 *
			 * @return die Zeichenfolgendarstellung
			 */
			UnicodeString ToLongTimeString();
			/**
			 * Gibt eine Zeichenfolgendarstellung des DateTime-Objekts zur�ck.
			 * Format: d.m.Y => 23.04.2011
			 *
			 * @return die Zeichenfolgendarstellung
			 */
			UnicodeString ToShortDateString();
			/**
			 * Gibt eine Zeichenfolgendarstellung des DateTime-Objekts zur�ck.
			 * Format: H:i => 02:23
			 *
			 * @return die Zeichenfolgendarstellung
			 */
			UnicodeString ToShortTimeString();
		};
	}
}

#endif
