/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R https://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#include "STEAM.hpp"

#include "Controls/Label.hpp"

using namespace OSHGui;
using namespace OSHGui::Skins;
using namespace OSHGui::Drawing;

void STEAM::DrawButton(Drawing::Graphics &g, Control *control, bool isActive)
{
	auto tempColor = control->GetBackColor();
	if (isActive)
	{
		tempColor = tempColor + control->GetMouseOverFocusColor();
	}

	auto color = tempColor + Color::FromARGB(0, 10, 10, 10);
	g.FillRectangle(color, RectangleF(PointF(1, 0), control->GetSize() - SizeF(2, 1)));
	g.FillRectangle(color, RectangleF(PointF(0, 1), control->GetSize() - SizeF(0, 3)));
	color = tempColor - Color::FromARGB(0, 50, 50, 50);
	g.FillRectangle(color, RectangleF(PointF(1, control->GetHeight() - 2), SizeF(control->GetWidth() - 2, 2)));
	g.FillRectangle(color, RectangleF(PointF(control->GetWidth() - 1, 1), SizeF(1, control->GetHeight() - 2)));

	ColorRectangle colors(tempColor, control->GetBackColor() - Color::FromARGB(0, 20, 20, 20));
	g.FillRectangleGradient(colors, RectangleF(PointF(1, 2), control->GetSize() - SizeF(2, 4)));
	g.FillRectangleGradient(colors, RectangleF(PointF(2, 1), control->GetSize() - SizeF(4, 2)));
}
//---------------------------------------------------------------------------
void STEAM::DrawCheckBox(Drawing::Graphics &g, Control *control, const Drawing::SizeF &boxSize, bool checked)
{
	g.FillRectangle(control->GetBackColor(), RectangleF(PointF(0, 0), boxSize));

	g.FillRectangleGradient(ColorRectangle(Color::White(), Color::White() - Color::FromARGB(0, 137, 137, 137)), RectangleF(PointF(1, 1), SizeF(15, 15)));
	g.FillRectangleGradient(ColorRectangle(control->GetBackColor(), control->GetBackColor() + Color::FromARGB(0, 55, 55, 55)), RectangleF(PointF(2, 2), SizeF(13, 13)));

	if (checked)
	{
		g.FillRectangle(Color::White(), RectangleF(PointF(5, 5), SizeF(7, 7)));
		g.FillRectangleGradient(ColorRectangle(Color::White(), Color::White() - Color::FromARGB(0, 137, 137, 137)), RectangleF(PointF(6, 6), SizeF(5, 5)));
	}
}
//---------------------------------------------------------------------------
void STEAM::DrawColorBar(Drawing::Graphics &g, Control *control, const std::vector<Drawing::ColorRectangle> &bars, const std::vector<Drawing::PointI> &sliders)
{
	for (int i = 0; i < 3; ++i)
	{
		g.FillRectangleGradient(bars[i], RectangleF(PointI(0, i * 15), SizeI(control->GetWidth(), 8)));

		auto sliderPos = sliders[i] + PointI(1, 0);
		for (int j = 0; j < 3; ++j)
		{
			g.FillRectangle(control->GetForeColor(), RectangleF(PointI(sliderPos.X - j, sliderPos.Y + j), SizeI(1 + j * 2, 1)));
		}
	}
}
//---------------------------------------------------------------------------
void STEAM::DrawColorPicker(Drawing::Graphics &g, Control *control, const Drawing::ImagePtr &gradient, const Drawing::PointI &cursor)
{
	g.DrawImage(gradient, Color::White(), RectangleF(PointF(0, 0), control->GetSize()));

	g.FillRectangle(Color::Black(), RectangleF(cursor - PointF(2, 2), SizeF(4, 4)));
	g.FillRectangle(Color::White(), RectangleF(cursor - PointF(1, 1), SizeF(2, 2)));
}
//---------------------------------------------------------------------------
void STEAM::DrawComboBoxButton(Drawing::Graphics &g, Control *control, const Drawing::SizeF &size, bool isActive)
{
	auto color = isActive ? control->GetBackColor() + control->GetMouseOverFocusColor() : control->GetBackColor();

	g.FillRectangle(color, PointF(0, 1), size - SizeF(0, 2));
	g.FillRectangle(color, PointF(1, 0), size - SizeF(2, 0));
	g.FillRectangleGradient(ColorRectangle(color, color - Color::FromARGB(0, 20, 20, 20)), PointF(1, 1), size - SizeF(2, 2));

	int arrowLeft = size.Width - 9;
	int arrowTop = size.Height - 11;
	for (int i = 0; i < 4; ++i)
	{
		g.FillRectangle(control->GetForeColor(), PointF(arrowLeft - i, arrowTop - i), SizeF(1 + i * 2, 1));
	}
}
//---------------------------------------------------------------------------
void STEAM::DrawForm(Drawing::Graphics &g, Control *control, int captionBarHeight)
{
	g.FillRectangle(control->GetBackColor() - Color::FromARGB(0, 100, 100, 100), RectangleF(PointF(), control->GetSize()));
	auto color = control->GetBackColor() - Color::FromARGB(0, 90, 90, 90);
	g.FillRectangleGradient(ColorRectangle(control->GetBackColor(), control->GetBackColor(), color, color), RectangleF(PointF(1, 1), control->GetSize() - SizeF(2, 2)));
	g.FillRectangle(control->GetBackColor() - Color::FromARGB(0, 50, 50, 50), RectangleF(PointF(5, captionBarHeight + 2), SizeF(control->GetWidth() - 10, 1)));
}
//---------------------------------------------------------------------------
void STEAM::DrawCaptionBarButton(Drawing::Graphics &g, Control *control)
{
	auto color = control->GetParent()->GetForeColor();

	for (int i = 0; i < 4; ++i)
	{
		g.FillRectangle(color, PointF(i, i), SizeF(3, 1));
		g.FillRectangle(color, PointF(6 - i, i), SizeF(3, 1));
		g.FillRectangle(color, PointF(i, 7 - i), SizeF(3, 1));
		g.FillRectangle(color, PointF(6 - i, 7 - i), SizeF(3, 1));
	}
}
//---------------------------------------------------------------------------
void STEAM::DrawGroupBox(Drawing::Graphics &g, Control *control, int captionWidth)
{
	if (control->GetBackColor().GetAlpha() > 0)
	{
		g.FillRectangle(control->GetBackColor(), PointF(0, 0), control->GetSize());
	}

	auto color = control->GetForeColor();
	g.FillRectangle(color, PointF(1, 5), SizeF(3, 1));
	g.FillRectangle(color, PointF(5 + captionWidth, 5), SizeF(control->GetWidth() - captionWidth - 6, 1));
	g.FillRectangle(color, PointF(0, 6), SizeF(1, control->GetHeight() - 7));
	g.FillRectangle(color, PointF(control->GetWidth() - 1, 6), SizeF(1, control->GetHeight() - 7));
	g.FillRectangle(color, PointF(1, control->GetHeight() - 1), SizeF(control->GetWidth() - 2, 1));
}
//---------------------------------------------------------------------------
void STEAM::DrawHotkeyControl(Drawing::Graphics &g, Control *control, const Drawing::PointF &button)
{
	auto color = control->GetForeColor();
	for (int i = 0; i < 4; ++i)
	{
		g.FillRectangle(color, button + PointF(i, i), SizeF(3, 1));
		g.FillRectangle(color, button + PointF(6 - i, i), SizeF(3, 1));
		g.FillRectangle(color, button + PointF(i, 7 - i), SizeF(3, 1));
		g.FillRectangle(color, button + PointF(6 - i, 7 - i), SizeF(3, 1));
	}
}
//---------------------------------------------------------------------------
void STEAM::DrawListBox(Drawing::Graphics &g, Control *control)
{

}
//---------------------------------------------------------------------------
void STEAM::DrawLabel(Drawing::Graphics &g, Control *control)
{
	auto label = static_cast<Label*>(control);

	if (label->GetBackColor().GetAlpha() > 0)
	{
		g.FillRectangle(label->GetBackColor(), RectangleF(PointF(), label->GetSize()));
	}

	g.DrawString(label->GetText(), label->GetFont(), label->GetForeColor(), PointF(0, 0));
}
//---------------------------------------------------------------------------
void STEAM::DrawLinkLabel(Drawing::Graphics &g, Control *control)
{
	DrawLabel(g, control);

	g.FillRectangle(control->GetForeColor(), RectangleF(PointF(0, control->GetHeight()), SizeF(control->GetWidth(), 1)));
}
//---------------------------------------------------------------------------
void STEAM::DrawPanel(Drawing::Graphics &g, Control *control)
{
	if (control->GetBackColor().GetAlpha() > 0)
	{
		g.FillRectangle(control->GetBackColor() - Color::FromARGB(0, 100, 100, 100), RectangleF(PointF(0, 0), control->GetSize()));
		auto color = control->GetBackColor() - Color::FromARGB(0, 90, 90, 90);
		g.FillRectangleGradient(ColorRectangle(control->GetBackColor(), control->GetBackColor(), color, color), RectangleF(PointF(1, 1), control->GetSize() - SizeF(2, 2)));
	}
}
//---------------------------------------------------------------------------
void STEAM::DrawPictureBox(Drawing::Graphics &g, Control *control, const Drawing::ImagePtr &image, bool stretch)
{
	if (control->GetBackColor().GetAlpha() > 0)
	{
		g.FillRectangle(control->GetBackColor(), PointF(0, 0), control->GetSize());
	}

	if (image)
	{
		if (stretch)
		{
			g.DrawImage(image, Color::White(), RectangleF(PointF(0, 0), control->GetSize()));
		}
		else
		{
			g.DrawImage(image, Color::White(), PointF(0, 0), RectangleF(PointF(0, 0), control->GetSize()));
		}
	}
}
//---------------------------------------------------------------------------
void STEAM::DrawRadioButton(Drawing::Graphics &g, Control *control, const Drawing::SizeF &boxSize, bool checked)
{
	g.FillRectangle(control->GetBackColor(), RectangleF(PointF(0, 0), boxSize));

	g.FillRectangleGradient(ColorRectangle(Color::White(), Color::White() - Color::FromARGB(0, 137, 137, 137)), RectangleF(PointF(1, 1), SizeF(15, 15)));
	g.FillRectangleGradient(ColorRectangle(control->GetBackColor(), control->GetBackColor() + Color::FromARGB(0, 55, 55, 55)), RectangleF(PointF(2, 2), SizeF(13, 13)));

	if (checked)
	{
		g.FillRectangle(Color::White() - Color::FromARGB(0, 128, 128, 128), RectangleF(PointF(5, 7), SizeF(7, 3)));
		ColorRectangle colors(Color::White(), Color::White() - Color::FromARGB(0, 137, 137, 137));
		g.FillRectangleGradient(colors, RectangleF(PointF(7, 5), SizeF(3, 7)));
		g.FillRectangleGradient(colors, RectangleF(PointF(6, 6), SizeF(5, 5)));
	}
}
//---------------------------------------------------------------------------
