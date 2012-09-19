#ifndef OSHGUI_THEME_HPP
#define OSHGUI_THEME_HPP

#include "Color.hpp"
#include "../Misc/Strings.hpp"
#include <map>

namespace OSHGui
{
	namespace Drawing
	{
		class Theme
		{
		public:
			struct ControlTheme
			{
				ControlTheme()
				{

				}

				ControlTheme(const Color &foreColor, const Color &backColor)
					: ForeColor(foreColor),
					  BackColor(backColor)
				{

				}

				Color ForeColor;
				Color BackColor;
			};
		
			enum ColorStyle
			{
				Text,
				Array,
				Integer
			};

			Misc::AnsiString Name;
			Misc::AnsiString Author;
			ControlTheme DefaultColor;

			void SetControlColorTheme(const Misc::AnsiString &controlClass, const ControlTheme &controlTheme);
			const ControlTheme& GetControlColorTheme(const Misc::AnsiString &controlClass) const;

			void Load(const Misc::AnsiString pathToThemeFile);
			bool Save(const Misc::AnsiString pathToThemeFile, ColorStyle style) const;

		private:
			std::map<Misc::AnsiString, ControlTheme> controlThemes;
		};
	}
}

#endif