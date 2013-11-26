/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#include "ScrollBar.hpp"
#include "../Misc/Exceptions.hpp"
#include "../Misc/Intersection.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//static attributes
	//---------------------------------------------------------------------------
	const Drawing::SizeF ScrollBar::DefaultSize(14, 110);
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	ScrollBar::ScrollBar()
		: drag(false),
		  value(0),
		  pixelsPerTick(1.0f),
		  maximum(0)
	{
		type = ControlType::ScrollBar;
		
		upButton = new ScrollBarButton(0);
		upButton->GetClickEvent() += ClickEventHandler([this](Control *control)
		{
			if (value > 0)
			{
				SetValueInternal(value - 1);
			}
		});
		AddSubControl(upButton);
		downButton = new ScrollBarButton(1);
		downButton->GetClickEvent() += ClickEventHandler([this](Control *control)
		{
			if (value < maximum)
			{
				SetValueInternal(value + 1);
			}
		});
		AddSubControl(downButton);

		SetSize(DefaultSize);
		trackLocation = Drawing::PointF(0, upButton->GetBottom() + 1);

		ApplyTheme(Application::Instance()->GetTheme());

		SetMaximum(0); //indirect init
		isFocusable = false;
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void ScrollBar::SetSize(const Drawing::SizeF &size)
	{
		Control::SetSize(size);

		upButton->SetSize(Drawing::SizeF(size.Width, upButton->GetHeight()));
		downButton->SetSize(Drawing::SizeF(size.Width, downButton->GetHeight()));

		downButton->SetLocation(0, size.Height - downButton->GetHeight());

		trackSize = Drawing::SizeF(size.Width, size.Height - 2 - upButton->GetHeight() * 2);

		sliderSize.Width = size.Width;
		sliderSize.Height = trackSize.Height / (maximum + 1);
		if (sliderSize.Height < MinimumSliderHeight)
		{
			sliderSize.Height = MinimumSliderHeight;
		}

		SetValueInternal(value);
	}
	//---------------------------------------------------------------------------
	void ScrollBar::SetForeColor(Drawing::Color color)
	{
		Control::SetForeColor(color);

		upButton->SetForeColor(color);
		downButton->SetForeColor(color);
	}
	//---------------------------------------------------------------------------
	void ScrollBar::SetMaximum(int maximum)
	{
		if (maximum < 0)
		{
			#ifndef OSHGUI_DONTUSEEXCEPTIONS
			throw Misc::ArgumentOutOfRangeException("maximum");
			#endif
			return;
		}

		this->maximum = maximum;

		sliderSize.Height = trackSize.Height / (maximum + 1);
		if (sliderSize.Height < MinimumSliderHeight)
		{
			sliderSize.Height = MinimumSliderHeight;
		}

		SetValueInternal(value > maximum ? maximum : value);
	}
	//---------------------------------------------------------------------------
	int ScrollBar::GetMaximum() const
	{
		return maximum;
	}
	//---------------------------------------------------------------------------
	void ScrollBar::SetValue(int value)
	{
		SetValueInternal(value);
	}
	//---------------------------------------------------------------------------
	int ScrollBar::GetValue() const
	{
		return value;
	}
	//---------------------------------------------------------------------------
	ScrollEvent& ScrollBar::GetScrollEvent()
	{
		return scrollEvent;
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	void ScrollBar::SetValueInternal(int value)
	{
		pixelsPerTick = (trackSize.Height - sliderSize.Height) / (maximum > 0 ? (float)maximum : 1.0f);

		if (value < 0)
		{
			value = 0;
		}
		if (value > maximum)
		{
			value = maximum;
		}
		
		if (this->value != value)
		{
			ScrollEventArgs args(value, this->value);

			this->value = value;
			
			scrollEvent.Invoke(this, args);
		}

		sliderLocation.Top = trackLocation.Top + value * pixelsPerTick;
		sliderAbsoluteLocation.Top = absoluteLocation.Top + sliderLocation.Top;

		Invalidate();
	}
	//---------------------------------------------------------------------------
	bool ScrollBar::Intersect(const Drawing::PointF &point) const
	{
		return Intersection::TestRectangle(absoluteLocation.OffsetEx(0, upButton->GetHeight()), size.InflateEx(0, -upButton->GetHeight() * 2), point);
	}
	//---------------------------------------------------------------------------
	void ScrollBar::CalculateAbsoluteLocation()
	{
		Control::CalculateAbsoluteLocation();

		sliderAbsoluteLocation = absoluteLocation + sliderLocation;
		trackAbsoluteLocation = absoluteLocation + trackLocation;
	}
	//---------------------------------------------------------------------------
	void ScrollBar::ScrollToTop()
	{
		SetValue(0);
	}
	//---------------------------------------------------------------------------
	void ScrollBar::ScrollToBottom()
	{
		SetValue(GetMaximum());
	}
	//---------------------------------------------------------------------------
	void ScrollBar::DrawSelf(Drawing::RenderContext &context)
	{
		Control::DrawSelf(context);

		upButton->Render();
		downButton->Render();
	}
	//---------------------------------------------------------------------------
	void ScrollBar::PopulateGeometry()
	{
		using namespace Drawing;

		Graphics g(*geometry);

		g.FillRectangle(GetBackColor(), sliderLocation + PointF(1, 1), sliderSize - SizeF(2, 2));
		g.FillRectangle(GetBackColor(), sliderLocation + PointF(sliderSize.Width - 1, 1), SizeF(1, sliderSize.Height - 2));
		g.FillRectangle(GetBackColor(), sliderLocation + PointF(1, sliderSize.Height - 1), SizeF(sliderSize.Width - 2, 1));
		g.FillRectangle(GetBackColor(), sliderLocation + PointF(0, 1), SizeF(1, sliderSize.Height - 2));
		g.FillRectangle(GetBackColor(), sliderLocation + PointF(1, 0), SizeF(sliderSize.Width - 2, 1));

		auto color = isInside ? GetForeColor() + Color(0, 50, 50, 50) : GetForeColor();
		int sliderHalfHeight = sliderLocation.Y + sliderSize.Height / 2 - 3;
		int sliderLeftPos = sliderLocation.X + 5;
		for (int i = 0; i < 3; ++i)
		{
			g.FillRectangle(color, PointF(sliderLeftPos, sliderHalfHeight + i * 3), SizeF(5, 1));
		}
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	void ScrollBar::OnMouseDown(const MouseMessage &mouse)
	{
		Control::OnMouseDown(mouse);

		if (Intersection::TestRectangle(sliderAbsoluteLocation, sliderSize, mouse.Location))
		{
			drag = true;
			OnGotMouseCapture();
		}
	}
	//---------------------------------------------------------------------------
	void ScrollBar::OnMouseMove(const MouseMessage &mouse)
	{
		Control::OnMouseMove(mouse);

		if (drag)
		{
			if (maximum >= 1)
			{
				float valuePerPixel = (float)maximum / (trackSize.Height - sliderSize.Height);

				int yPos = mouse.Location.Y - trackAbsoluteLocation.Top - sliderSize.Height / 2;
				if (yPos < 0)
				{
					yPos = 0;
				}
				else if (yPos + sliderSize.Height + trackLocation.Top > trackSize.Height)
				{
					yPos = trackSize.Height - sliderSize.Height;
				}

				SetValueInternal(yPos * valuePerPixel + 0.5f);
			}
		}
	}
	//---------------------------------------------------------------------------
	void ScrollBar::OnMouseUp(const MouseMessage &mouse)
	{
		Control::OnMouseUp(mouse);

		if (drag)
		{
			drag = false;
			OnLostMouseCapture();
		}
	}
	//---------------------------------------------------------------------------
	void ScrollBar::OnMouseClick(const MouseMessage &mouse)
	{
		Control::OnMouseClick(mouse);

		if (!drag && maximum > 1)
		{
			if (Intersection::TestRectangle(trackAbsoluteLocation, trackSize, mouse.Location))
			{
				float valuePerPixel = (float)maximum / (trackSize.Height - sliderSize.Height);
				int yPos = mouse.Location.Y - trackAbsoluteLocation.Top - sliderSize.Height / 2;
				SetValueInternal(yPos * valuePerPixel + 0.5f);
			}
		}
	}
	//---------------------------------------------------------------------------
	void ScrollBar::OnMouseScroll(const MouseMessage &mouse)
	{
		Control::OnMouseScroll(mouse);

		SetValueInternal(value + mouse.Delta);
	}
	//---------------------------------------------------------------------------
	//ScrollBar::ScrollBarButton
	//---------------------------------------------------------------------------
	const Drawing::SizeF ScrollBar::ScrollBarButton::DefaultButtonSize(14, 14);
	//---------------------------------------------------------------------------
	ScrollBar::ScrollBarButton::ScrollBarButton(int _direction)
		: direction(_direction)
	{
		SetLocation(0, 0);
		SetSize(DefaultButtonSize);

		isFocusable = false;
	}
	//---------------------------------------------------------------------------
	void ScrollBar::ScrollBarButton::SetSize(const Drawing::SizeF &size)
	{
		Control::SetSize(size);

		iconLocation = Drawing::PointF(size.Width / 2, size.Height / 2);
	}
	//---------------------------------------------------------------------------
	void ScrollBar::ScrollBarButton::PopulateGeometry()
	{
		using namespace Drawing;

		Graphics g(*geometry);

		auto color = isInside ? GetForeColor() + Color(0, 50, 50, 50) : GetForeColor();

		if (direction == 0)
		{
			for (int i = 0; i < 4; ++i)
			{
				g.FillRectangle(color, iconLocation + PointF(-i, i), SizeF(1 + i * 2, 1));
			}
		}
		else
		{
			for (int i = 0; i < 4; ++i)
			{
				g.FillRectangle(color, iconLocation - PointF(i, i), SizeF(1 + i * 2, 1));
			}
		}
	}
	//---------------------------------------------------------------------------
}
