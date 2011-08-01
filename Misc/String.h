#ifndef __OSHGUI_MISC_STRING_H__
#define __OSHGUI_MISC_STRING_H__

#include <string>

namespace OSHGui
{
	namespace Misc
	{
		namespace String
		{
			typedef std::wstring UnicodeString;
			typedef std::string AnsiString;
			typedef wchar_t UnicodeChar;
			typedef char AnsiChar;

			inline AnsiString UnicodeToAnsi(const UnicodeString &in)
			{
				if (in.length() == 0)
				{
					return "";
				}

				AnsiString out(in.length(), (AnsiChar)0);

				std::use_facet<std::ctype<UnicodeChar>>(std::locale()).narrow(&in[0], &in[0] + in.length(), '?', &out[0]);

				return out;
			}

			inline UnicodeString AnsiToUnicode(const AnsiString &in)
			{
				if (in.length() == 0)
				{
					return L"";
				}

				UnicodeString out(in.length(), (UnicodeChar)0);

				std::use_facet<std::ctype<UnicodeChar>>(std::locale()).widen(&in[0], &in[0] + in.length(), &out[0]);

				return out; 
			}
			
			UnicodeString Format(const UnicodeChar *fmt, ...);
			AnsiString Format(const AnsiChar *fmt, ...);
		}
	}
}

#endif