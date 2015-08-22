#include "Theme.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		void Theme::SetControlColorTheme(ControlType type, Theme::ControlTheme &&theme)
		{
			controlThemes[type] = std::move(theme);
		}
		//---------------------------------------------------------------------------
		const Theme::ControlTheme& Theme::GetControlColorTheme(ControlType type) const
		{
			auto it = controlThemes.find(type);
			if (it == controlThemes.end())
			{
				return DefaultColor;
			}

			return it->second;
		}
		//---------------------------------------------------------------------------
	}
}