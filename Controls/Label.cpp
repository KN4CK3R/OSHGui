/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
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
		: textHelper(font)
	{
		type = CONTROL_LABEL;
		
		SetAutoSize(true);
		
		SetBackColor(Drawing::Color::Empty());
		SetForeColor(Drawing::Color(0xFFE5E0E4));

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
	void Label::SetFont(const std::shared_ptr<Drawing::IFont> &font)
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
	bool Label::Intersect(const Drawing::Point &point) const
	{
		return false;
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	void Label::Render(Drawing::IRenderer *renderer)
	{
		if (!isVisible)
		{
			return;
		}
		
		Drawing::Size renderSize = GetParent()->GetSize();
		if (backColor.A != 0)
		{
			renderer->SetRenderColor(backColor);
			renderer->Fill(absoluteLocation, size);
		}
	
		renderer->SetRenderColor(foreColor);
		renderer->RenderText(font, absoluteLocation, renderSize, textHelper.GetText());
	}
	//---------------------------------------------------------------------------
}