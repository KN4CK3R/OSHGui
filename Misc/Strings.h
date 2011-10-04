#ifndef OSHGUI_MISC_STRINGS_H_
#define OSHGUI_MISC_STRINGS_H_

#include <string>
#include <stdarg.h>

namespace OSHGui
{
	namespace Misc
	{
		/**
		 * Stellt Text als eine Reihe von Unicode-Zeichen dar.
		 */
		typedef std::wstring UnicodeString;
		/**
		 * Stellt ein Unicode-Zeichen dar.
		 */
		typedef wchar_t UnicodeChar;
		/**
		 * Stellt Text als eine Reihe von Ansi-Zeichen dar.
		 */
		typedef std::string AnsiString;
		/**
		 * Stellt ein Ansi-Zeichen dar.
		 */
		typedef char AnsiChar;
		
		namespace String
		{
			/**
			 * Wandelt einen AnsiString in einen UnicodeString um.
			 *
			 * @param ansi
			 * @return unicode
			 */
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
			/**
			 * Wandelt einen UnicodeString in einen AnsiString um.
			 *
			 * @param unicode
			 * @return ansi
			 */
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
			
			/**
			 * Ersetzt das Formatelement in einer angegebenen Unicode-Zeichenfolge durch die
			 * Zeichenfolgendarstellung eines entsprechenden Objekts.
			 *
			 * @param *fmt
			 * @param ...
			 * @return UnicodeString
			 */
			UnicodeString Format(const UnicodeChar *fmt, ...);
			/**
			 * Ersetzt das Formatelement in einer angegebenen Ansi-Zeichenfolge durch die
			 * Zeichenfolgendarstellung eines entsprechenden Objekts.
			 *
			 * @param *fmt
			 * @param ...
			 * @return AnsiString
			 */
			AnsiString Format(const AnsiChar *fmt, ...);
		}
	}
}

#endif