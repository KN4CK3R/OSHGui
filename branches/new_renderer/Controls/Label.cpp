/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#include "Label.hpp"
#include "../Misc/Exceptions.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	Label::Label()
		: textHelper(GetFont())
	{
		type = ControlType::Label;
		
		SetAutoSize(true);
		
		ApplyTheme(Application::Instance().GetTheme());

		canRaiseEvents = false;
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void Label::SetText(const Misc::AnsiString &text)
	{
		textHelper.SetText(text);
		if (autoSize)
		{
			Control::SetSize(textHelper.GetSize());
		}
	}
	//---------------------------------------------------------------------------
	const Misc::AnsiString& Label::GetText() const
	{
		return textHelper.GetText();
	}
	//---------------------------------------------------------------------------
	void Label::SetFont(const Drawing::FontPtr &font)
	{
		Control::SetFont(font);
		textHelper.SetFont(font);
		if (autoSize)
		{
			size = textHelper.GetSize();
		}
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool Label::Intersect(const Drawing::PointF &point) const
	{
		return false;
	}
	//---------------------------------------------------------------------------
	void Label::PopulateGeometry()
	{
		using namespace Drawing;

		Graphics g(*geometry);

		if (backColor.GetAlpha() > 0)
		{
			g.FillRectangle(GetBackColor(), RectangleF(PointF(), GetSize()));
		}
		
		g.DrawString(textHelper.GetText(), GetFont(), GetForeColor(), PointF(0, 0));
	}
	//---------------------------------------------------------------------------
}
