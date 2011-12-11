#include "Button.hpp"
#include "..\Misc\Exceptions.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//static attributes
	//---------------------------------------------------------------------------
	const Drawing::Size Button::DefaultSize(92, 24);
	const Drawing::Point Button::DefaultLabelOffset(6, 5);
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	Button::Button()
	{
		type = CONTROL_BUTTON;

		label = new Label();
		label->SetName("Button_Label");
		label->SetLocation(DefaultLabelOffset);
		label->SetText("Button");
		label->SetAutoSize(false);

		SetSize(DefaultSize);
		
		SetBackColor(Drawing::Color(0xFF4E4E4E));
		SetForeColor(Drawing::Color(0xFFE5E0E4));
	}
	//---------------------------------------------------------------------------
	Button::~Button()
	{
		delete label;
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void Button::SetAutoSize(bool autoSize)
	{
		Control::SetAutoSize(autoSize);

		label->SetAutoSize(autoSize);
	}
	//---------------------------------------------------------------------------
	void Button::SetText(const Misc::AnsiString &text)
	{
		label->SetText(text);
		if (autoSize)
		{
			size = label->GetSize().InflateEx(DefaultLabelOffset.Left * 2, DefaultLabelOffset.Top * 2);
		}
	}
	//---------------------------------------------------------------------------
	const Misc::AnsiString& Button::GetText() const
	{
		return label->GetText();
	}
	//---------------------------------------------------------------------------
	void Button::SetFont(const std::shared_ptr<Drawing::IFont> &font)
	{
		Control::SetFont(font);

		label->SetFont(font);
		if (autoSize)
		{
			size = label->GetSize().InflateEx(DefaultLabelOffset.Left * 2, DefaultLabelOffset.Top * 2);
		}
	}
	//---------------------------------------------------------------------------
	void Button::SetForeColor(Drawing::Color color)
	{
		Control::SetForeColor(color);

		label->SetForeColor(color);
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool Button::Intersect(const Drawing::Point &point) const
	{
		return Intersection::TestRectangle(absoluteLocation, size, point);
	}
	//---------------------------------------------------------------------------
	void Button::CalculateAbsoluteLocation()
	{
		Control::CalculateAbsoluteLocation();

		label->SetParent(this);
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	bool Button::OnKeyUp(const KeyboardMessage &keyboard)
	{
		if (!Control::OnKeyUp(keyboard))
		{
			if (keyboard.KeyCode == Key::Return)
			{
				clickEvent.Invoke(this);
			}
		}

		return true;
	}
	//---------------------------------------------------------------------------
	void Button::Render(Drawing::IRenderer *renderer)
	{
		if (!isVisible)
		{
			return;
		}
		
		Drawing::Color tempColor = backColor;

		if ((isFocused || isInside) && !(isFocused && isClicked))
		{
			tempColor += mouseOverFocusColor;
		}
		
		renderer->SetRenderColor(tempColor + Drawing::Color(0, 10, 10, 10));
		renderer->Fill(absoluteLocation.Left + 1, absoluteLocation.Top, GetWidth() - 2, GetHeight() - 1);
		renderer->Fill(absoluteLocation.Left, absoluteLocation.Top + 1, GetWidth(), GetHeight() - 3);
		renderer->SetRenderColor(tempColor - Drawing::Color(0, 50, 50, 50));
		renderer->Fill(absoluteLocation.Left + 1, absoluteLocation.Top + GetHeight() - 2, GetWidth() - 2, 2);
		renderer->Fill(absoluteLocation.Left + GetWidth() - 1, absoluteLocation.Top + 1, 1, GetHeight() - 2);

		renderer->SetRenderColor(tempColor);
		renderer->FillGradient(absoluteLocation.Left + 1, absoluteLocation.Top + 2, GetWidth() - 2, GetHeight() - 4, backColor - Drawing::Color(0, 20, 20, 20));
		renderer->FillGradient(absoluteLocation.Left + 2, absoluteLocation.Top + 1, GetWidth() - 4, GetHeight() - 2, backColor - Drawing::Color(0, 20, 20, 20));

		label->Render(renderer);
	}
	//---------------------------------------------------------------------------
}