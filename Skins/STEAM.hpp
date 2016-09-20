/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R https://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#pragma once

#include "Base.hpp"

namespace OSHGui
{
	namespace Skins
	{
		class STEAM : public Base
		{
		public:
			virtual void DrawButton(Drawing::Graphics &g, Control *control, bool isActive) override;

			virtual void DrawCheckBox(Drawing::Graphics &g, Control *control, const Drawing::SizeF &boxSize, bool checked) override;

			virtual void DrawColorBar(Drawing::Graphics &g, Control *control, const std::vector<Drawing::ColorRectangle> &bars, const std::vector<Drawing::PointI> &sliders) override;

			virtual void DrawColorPicker(Drawing::Graphics &g, Control *control, const Drawing::ImagePtr &gradient, const Drawing::PointI &cursor) override;

			virtual void DrawComboBoxButton(Drawing::Graphics &g, Control *control, const Drawing::SizeF &size, bool isActive) override;

			virtual void DrawForm(Drawing::Graphics &g, Control *control, int captionBarHeight) override;
			virtual void DrawCaptionBarButton(Drawing::Graphics &g, Control *control) override;

			virtual void DrawGroupBox(Drawing::Graphics &g, Control *control, int captionWidth) override;

			virtual void DrawHotkeyControl(Drawing::Graphics &g, Control *control, const Drawing::PointF &button) override;

			virtual void DrawListBox(Drawing::Graphics &g, Control *control) override;

			virtual void DrawLabel(Drawing::Graphics &g, Control *control) override;

			virtual void DrawLinkLabel(Drawing::Graphics &g, Control *control) override;

			virtual void DrawPanel(Drawing::Graphics &g, Control *control) override;

			virtual void DrawPictureBox(Drawing::Graphics &g, Control *control, const Drawing::ImagePtr &image, bool stretch) override;

			virtual void DrawRadioButton(Drawing::Graphics &g, Control *control, const Drawing::SizeF &boxSize, bool checked) override;
		};
	}
}
