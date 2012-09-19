#ifndef OSHGUI_THEME_HPP
#define OSHGUI_THEME_HPP

#include "Color.hpp"
#include <string>
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
				Color ForeColor;
				Color BackColor;
			};
		
			enum ColorStyle
			{
				Text,
				Array,
				Integer
			};

			std::string Name;
			std::string Author;
			ControlTheme DefaultColor;

			void SetControlColorTheme(const std::string &controlClass, const ControlTheme &controlTheme);
			const ControlTheme& GetControlColorTheme(const std::string &controlClass) const;

			void Load(const std::string pathToThemeFile);
			bool Save(const std::string pathToThemeFile, ColorStyle style) const;

		private:
			std::map<std::string, ControlTheme> controlThemes;
		};
	}
}

#endif