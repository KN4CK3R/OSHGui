#include "Strings.hpp"

namespace OSHGui
{
	namespace Misc
	{
		namespace String
		{
			UnicodeString Format(const UnicodeChar *fmt, ...)
			{
				UnicodeChar strOut[4096];

				va_list s;
				va_start(s, fmt);
					vswprintf(strOut, sizeof(strOut) / sizeof(UnicodeChar), fmt, s);
				va_end(s);

				UnicodeString str = strOut;
				return str;
			}
			//---------------------------------------------------------------------------
			AnsiString Format(const AnsiChar *fmt, ...)
			{
				AnsiChar strOut[4096];

				va_list s;
				va_start(s, fmt);
					vsnprintf(strOut, sizeof(strOut) / sizeof(AnsiChar), fmt, s);
				va_end(s);

				AnsiString str = strOut;
				return str;
			}
			//---------------------------------------------------------------------------
		}
	}
}