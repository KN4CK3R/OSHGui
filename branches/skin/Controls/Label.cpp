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
		: textHelper_(GetFont())
	{
		type_ = ControlType::Label;
		
		SetAutoSize(true);
		
		ApplyStyle(Application::Instance().GetStyle());

		canRaiseEvents_ = false;
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void Label::SetText(const Misc::AnsiString &text)
	{
		textHelper_.SetText(text);
		if (autoSize_)
		{
			Control::SetSize(textHelper_.GetSize());
		}
	}
	//---------------------------------------------------------------------------
	const Misc::AnsiString& Label::GetText() const
	{
		return textHelper_.GetText();
	}
	//---------------------------------------------------------------------------
	void Label::SetFont(const Drawing::FontPtr &font)
	{
		Control::SetFont(font);

		textHelper_.SetFont(font);
		if (autoSize_)
		{
			Control::SetSize(textHelper_.GetSize());
		}
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool Label::Intersect(const Drawing::PointI &point) const
	{
		return false;
	}
	//---------------------------------------------------------------------------
	void Label::PopulateGeometry(Skins::Base &skin)
	{
		skin.DrawLabel(Drawing::Graphics(*geometry_), this);
	}
	//---------------------------------------------------------------------------
}
