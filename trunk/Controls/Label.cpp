#include "Label.hpp"
#include "..\Misc\Exceptions.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	Label::Label(const Misc::AnsiString &name, const Drawing::Point &location, const Drawing::Size &size) : Control(name, location, size), textHelper(font)
	{
		type = CONTROL_LABEL;
		
		SetAutoSize(true);
		
		SetText("Label");
		
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
			size = textHelper.GetSize();
		}
	}
	//---------------------------------------------------------------------------
	const Misc::AnsiString& Label::GetText()
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
	bool Label::ContainsPoint(const Drawing::Point &point) const
	{
		return Control::ContainsPoint(point);
	}
	//---------------------------------------------------------------------------
	void Label::Invalidate()
	{
		textHelper.SetFont(font);
		if (autoSize)
		{
			SetSize(textHelper.GetSize());
		}

		clientArea = bounds;

		InvalidateChildren();
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
		
		if (backColor.A != 0)
		{
			renderer->SetRenderColor(backColor);
			renderer->Fill(bounds);
		}
	
		renderer->SetRenderColor(foreColor);
		renderer->RenderText(font, bounds, textHelper.GetText());

		if (controls.size() > 0)
		{
			Drawing::Rectangle renderRect = renderer->GetRenderRectangle();
			renderer->SetRenderRectangle(clientArea + renderRect.GetPosition());
			
			ChildRender(renderer);
			
			renderer->SetRenderRectangle(renderRect);
		}
	}
	//---------------------------------------------------------------------------
}