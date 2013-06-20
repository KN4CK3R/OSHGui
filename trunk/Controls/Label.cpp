/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
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
		
		ApplyTheme(Application::Instance()->GetTheme());

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