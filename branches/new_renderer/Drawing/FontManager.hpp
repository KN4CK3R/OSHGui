/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_DRAWING_FONTMANAGER_HPP
#define OSHGUI_DRAWING_FONTMANAGER_HPP

#include "../Exports.hpp"
#include "Font.hpp"
#include "../Misc/RawDataContainer.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		/**
		 * Die Klasse ermöglicht das Laden einer Schriftart.
		 */
		class OSHGUI_EXPORT FontManager
		{
		public:
			/**
			 * Lädt die Schriftart mit dem entsprechenden Namen.
			 *
			 * \param name Name der Schriftart
			 * \param pointSize Größe in PT
			 * \param antiAliased Legt fest, ob AntiAliasing verwendet werden soll
			 * \return Die geladene Schriftart oder nullptr, falls die Schriftart nicht gefunden wird.
			 */
			static Drawing::FontPtr LoadFont(const Misc::AnsiString &name, float pointSize, bool antiAliased);
			/**
			 * Lädt die Schriftart aus der angegebenen Datei.
			 *
			 * \param file Pfad zur Datei
			 * \param pointSize Größe in PT
			 * \param antiAliased Legt fest, ob AntiAliasing verwendet werden soll
			 * \return Die geladene Schriftart oder nullptr, falls die Schriftart nicht gefunden wird.
			 */
			static Drawing::FontPtr LoadFontFromFile(const Misc::AnsiString &file, float pointSize, bool antiAliased);
			static Drawing::FontPtr LoadFontFromMemory(const Misc::RawDataContainer &data, float pointSize, bool antiAliased);
		};
	}
}

#endif