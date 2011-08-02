#ifndef __OSHGUI_MISC_STRINGS_H__
#define __OSHGUI_MISC_STRINGS_H__

#include <string>
#include <stdarg.h>

namespace OSHGui
{
	namespace Misc
	{
		typedef std::wstring UnicodeString;
		typedef wchar_t UnicodeChar;
		typedef std::string AnsiString;
		typedef char AnsiChar;
					
		inline UnicodeString AnsiToUnicode(const AnsiString &ansi)
		{
			if (ansi.length() == 0)
			{
				return L"";
			}

			UnicodeString out(ansi.length(), (UnicodeChar)0);

			std::use_facet<std::ctype<UnicodeChar>>(std::locale()).widen(&ansi[0], &ansi[0] + ansi.length(), &out[0]);

			return out; 
		}
		//---------------------------------------------------------------------------
		inline AnsiString UnicodeToAnsi(const UnicodeString &unicode)
		{
			if (unicode.length() == 0)
			{
				return "";
			}

			AnsiString out(unicode.length(), (AnsiChar)0);

			std::use_facet<std::ctype<UnicodeChar>>(std::locale()).narrow(&unicode[0], &unicode[0] + unicode.length(), '?', &out[0]);

			return out;
		}
		//---------------------------------------------------------------------------
		UnicodeString Format(const UnicodeChar *fmt, ...);
		AnsiString Format(const AnsiChar *fmt, ...);
	}
}

#endif