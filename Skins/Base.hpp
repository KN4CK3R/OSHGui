/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R https://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#pragma once

#include <vector>

#include "../Drawing/Point.hpp"
#include "../Drawing/Size.hpp"
#include "../Drawing/ColorRectangle.hpp"
#include "../Drawing/Image.hpp"

namespace OSHGui
{
	class Control;

	namespace Drawing
	{
		class Graphics;
	}

	namespace Skins
	{
		class Base
		{
		public:
			virtual ~Base() = default;

			virtual void DrawButton(Drawing::Graphics &g, Control *control, bool isActive) = 0;

			virtual void DrawCheckBox(Drawing::Graphics &g, Control *control, const Drawing::SizeF &boxSize, bool checked) = 0;

			virtual void DrawColorBar(Drawing::Graphics &g, Control *control, const std::vector<Drawing::ColorRectangle> &bars, const std::vector<Drawing::PointI> &sliders) = 0;

			virtual void DrawColorPicker(Drawing::Graphics &g, Control *control, const Drawing::ImagePtr &gradient, const Drawing::PointI &cursor) = 0;

			virtual void DrawComboBoxButton(Drawing::Graphics &g, Control *control, const Drawing::SizeF &size, bool isActive) = 0;

			virtual void DrawForm(Drawing::Graphics &g, Control *control, int captionBarHeight) = 0;
			virtual void DrawCaptionBarButton(Drawing::Graphics &g, Control *control) = 0;

			virtual void DrawGroupBox(Drawing::Graphics &g, Control *control, int captionWidth) = 0;

			virtual void DrawHotkeyControl(Drawing::Graphics &g, Control *control, const Drawing::PointF &button) = 0;

			virtual void DrawListBox(Drawing::Graphics &g, Control *control) = 0;

			virtual void DrawLabel(Drawing::Graphics &g, Control *control) = 0;

			virtual void DrawLinkLabel(Drawing::Graphics &g, Control *control) = 0;

			virtual void DrawPanel(Drawing::Graphics &g, Control *control) = 0;

			virtual void DrawPictureBox(Drawing::Graphics &g, Control *control, const Drawing::ImagePtr &image, bool stretch) = 0;

			virtual void DrawRadioButton(Drawing::Graphics &g, Control *control, const Drawing::SizeF &boxSize, bool checked) = 0;
		};
	}
}