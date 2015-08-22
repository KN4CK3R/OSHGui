#ifndef OSHGUI_THEME_HPP
#define OSHGUI_THEME_HPP

#include <map>

#include "../Exports.hpp"

#include "../Controls/Types.hpp"

#include "Color.hpp"
#include "../Misc/Strings.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		class OSHGUI_EXPORT Theme
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

			ControlTheme DefaultColor;

			/**
			 * Legt das Farbschema für eine Control Klasse fest. Die Klassennamen sind in der Control.hpp zu finden.
			 *
			 * \param controlClass
			 * \param controlTheme
			 */
			void SetControlColorTheme(ControlType type, ControlTheme &&theme);
			/**
			 * Ruft das Farbschema für eine Control Klasse ab. Die Klassennamen sind in der Control.hpp zu finden.
			 *
			 * \param controlClass
			 * \return controlTheme
			 */
			const ControlTheme& GetControlColorTheme(ControlType type) const;

		private:
			std::map<ControlType, ControlTheme> controlThemes;
		};
	}
}

#endif
