#include "Control.hpp"
#include "..\Misc\Exceptions.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	Control::Control(Control *parent)
	{
		type = CONTROL_ROOT;
		
		this->parent = parent;
		
		SetEnabled(true);
		SetVisible(true);
				
		mouseOver = false;
		hasFocus = false;

		focusControl = 0;
		mouseOverControl = 0;

		font = Application::GetRenderer()->GetDefaultFont();
		
		cursor = Cursors::Get(Cursors::Default);
		
		mouseOverFocusColor = Drawing::Color(0, 20, 20, 20);
	}
	//---------------------------------------------------------------------------
	Control::~Control()
	{
		for (unsigned int i = 0; i < controls.size(); ++i)
		{
			Control *control = controls[i];
			delete control;
		}

		controls.clear();
	}
	//---------------------------------------------------------------------------
	CONTROL_TYPE Control::GetType() const
	{
		return type;
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void Control::SetEnabled(bool enabled)
	{
		this->enabled = enabled;
	}
	//---------------------------------------------------------------------------
	bool Control::GetEnabled() const
	{
		return enabled;
	}
	//---------------------------------------------------------------------------
	void Control::SetVisible(bool visible)
	{
		this->visible = visible;
	}
	//---------------------------------------------------------------------------
	bool Control::GetVisible() const
	{
		return visible;
	}
	//---------------------------------------------------------------------------
	void Control::SetFocus(bool focus)
	{
		hasFocus = focus;
	}
	//---------------------------------------------------------------------------
	void Control::SetMouseOver(bool mouseOver)
	{
		this->mouseOver = mouseOver;
		
		if (mouseOver)
		{
			if (Application::mouse.Cursor != cursor)
			{
				Application::mouse.Cursor = cursor;
			}
		}
	}
	//---------------------------------------------------------------------------
	void Control::SetAutoSize(bool autoSize)
	{
		this->autoSize = autoSize;
		Invalidate();
	}
	//---------------------------------------------------------------------------
	bool Control::GetAutoSize() const
	{
		return autoSize;
	}
	//---------------------------------------------------------------------------
	void Control::SetBounds(int x, int y, int w, int h)
	{
		SetBounds(Drawing::Rectangle(x, y, w, h));
	}
	//---------------------------------------------------------------------------
	void Control::SetBounds(Drawing::Point location, Drawing::Size &size)
	{
		SetBounds(Drawing::Rectangle(location.X, location.Y, size.Width, size.Height));
	}
	//---------------------------------------------------------------------------
	void Control::SetBounds(Drawing::Rectangle &bounds)
	{
		this->bounds = bounds;
		
		static bool isValid = true; //no recursive calls
		if (isValid)
		{
			isValid = false;
			Invalidate();
			isValid = true;
		}
	}
	//---------------------------------------------------------------------------
	const Drawing::Rectangle Control::GetBounds() const
	{
		return bounds;
	}
	//---------------------------------------------------------------------------
	void Control::SetLocation(const Drawing::Point &location)
	{
		SetLocation(location.Left, location.Top);
	}
	//---------------------------------------------------------------------------
	void Control::SetLocation(int x, int y)
	{
		bounds.SetLeft(x);
		bounds.SetTop(y);
		
		static bool isValid = true; //no recursive calls
		if (isValid)
		{
			isValid = false;
			Invalidate();
			isValid = true;
		}
	}
	//---------------------------------------------------------------------------
	const Drawing::Point Control::GetLocation() const
	{
		return bounds.GetPosition();
	}
	//---------------------------------------------------------------------------
	void Control::SetSize(const Drawing::Size &size)
	{
		SetSize(size.Width, size.Height);
	}
	//---------------------------------------------------------------------------
	void Control::SetSize(int width, int height)
	{
		if (width < 0)
		{
			throw Misc::ArgumentOutOfRangeException(L"width", __WFILE__, __LINE__);
		}
		if (height < 0)
		{
			throw Misc::ArgumentOutOfRangeException(L"height", __WFILE__, __LINE__);
		}
		
		bounds.SetWidth(width);
		bounds.SetHeight(height);
		
		static bool isValid = true; 
		if (isValid)
		{
			isValid = false;
			Invalidate();
			isValid = true;
		}
	}
	//---------------------------------------------------------------------------
	const Drawing::Size Control::GetSize() const
	{
		return bounds.GetSize();
	}
	//---------------------------------------------------------------------------
	int Control::GetLeft() const
	{
		return bounds.GetLeft();
	}
	//---------------------------------------------------------------------------
	int Control::GetTop() const
	{
		return bounds.GetTop();
	}
	//---------------------------------------------------------------------------
	int Control::GetRight() const
	{
		return bounds.GetRight();
	}
	//---------------------------------------------------------------------------
	int Control::GetBottom() const
	{
		return bounds.GetBottom();
	}
	//---------------------------------------------------------------------------
	int Control::GetWidth() const
	{
		return bounds.GetWidth();
	}
	//---------------------------------------------------------------------------
	int Control::GetHeight() const
	{
		return bounds.GetHeight();
	}
	//---------------------------------------------------------------------------
	void Control::SetTag(Misc::Any &tag)
	{
		this->tag = tag;
	}
	//---------------------------------------------------------------------------
	Misc::Any& Control::GetTag()
	{
		return tag;
	}
	//---------------------------------------------------------------------------
	void Control::SetName(const Misc::UnicodeString &name)
	{
		this->name = name;
	}
	//---------------------------------------------------------------------------
	const Misc::UnicodeString& Control::GetName() const
	{
		return name;
	}
	//---------------------------------------------------------------------------
	void Control::SetForeColor(Drawing::Color color)
	{
		foreColor = color;
	}
	//---------------------------------------------------------------------------
	const Drawing::Color& Control::GetForeColor() const
	{
		return foreColor;
	}
	//---------------------------------------------------------------------------
	void Control::SetBackColor(Drawing::Color color)
	{
		backColor = color;
	}
	//---------------------------------------------------------------------------
	const Drawing::Color& Control::GetBackColor() const
	{
		return backColor;
	}
	//---------------------------------------------------------------------------
	void Control::SetMouseOverFocusColor(Drawing::Color color)
	{
		mouseOverFocusColor = color;
	}
	//---------------------------------------------------------------------------
	const Drawing::Color& Control::GetMouseOverFocusColor() const
	{
		return mouseOverFocusColor;
	}
	//---------------------------------------------------------------------------
	void Control::SetFont(const std::shared_ptr<Drawing::IFont> &font)
	{
		if (font == 0)
		{
			throw Misc::ArgumentNullException(L"font", __WFILE__, __LINE__);
		}
		
		this->font = font;

		static bool isValid = true; 
		if (isValid)
		{
			isValid = false;
			Invalidate();
			isValid = true;
		}
	}
	//---------------------------------------------------------------------------
	const std::shared_ptr<Drawing::IFont> Control::GetFont() const
	{
		return font;
	}
	//---------------------------------------------------------------------------
	void Control::SetCursor(const std::shared_ptr<Cursor> &cursor)
	{
		this->cursor = cursor;
	}
	//---------------------------------------------------------------------------
	const std::shared_ptr<Cursor> Control::GetCursor() const
	{
		return cursor;
	}
	//---------------------------------------------------------------------------
	ClickEvent& Control::GetClickEvent()
	{
		return clickEvent;
	}
	//---------------------------------------------------------------------------
	MouseClickEvent& Control::GetMouseClickEvent()
	{
		return mouseClickEvent;
	}
	//---------------------------------------------------------------------------
	MouseDownEvent& Control::GetMouseDownEvent()
	{
		return mouseDownEvent;
	}
	//---------------------------------------------------------------------------
	MouseMoveEvent& Control::GetMouseMoveEvent()
	{
		return mouseMoveEvent;
	}
	//---------------------------------------------------------------------------
	MouseUpEvent& Control::GetMouseUpEvent()
	{
		return mouseUpEvent;
	}
	//---------------------------------------------------------------------------
	MouseEnterEvent& Control::GetMouseEnterEvent()
	{
		return mouseEnterEvent;
	}
	//---------------------------------------------------------------------------
	MouseLeaveEvent& Control::GetMouseLeaveEvent()
	{
		return mouseLeaveEvent;
	}
	//---------------------------------------------------------------------------
	Control* Control::GetParent() const
	{
		return parent;
	}
	//---------------------------------------------------------------------------
	const std::vector<Control*>& Control::GetControls() const
	{
		return controls;
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool Control::CanHaveFocus() const
	{
		return false;
	}
	//---------------------------------------------------------------------------
	bool Control::ContainsPoint(const Drawing::Point &point) const
	{
		return false;
	}
	//---------------------------------------------------------------------------
	void Control::Invalidate()
	{
		return;
	}
	//---------------------------------------------------------------------------
	void Control::AddControl(Control *control)
	{
		if (control == 0)
		{
			throw Misc::ArgumentNullException(L"control", __WFILE__, __LINE__);
		}
		
		for (std::vector<Control*>::iterator it = controls.begin(); it != controls.end(); ++it)
		{
			if (control == *it)
			{
				return;
			}
		}
	
		controls.push_back(control);

		Invalidate();
	}
	//---------------------------------------------------------------------------
	void Control::InvalidateChildren()
	{
		for (std::vector<Control*>::iterator it = controls.begin(); it != controls.end(); ++it)
		{
			Control *control = *it;

			if (control == 0)
			{
				continue;
			}

			control->Invalidate();
		}
	}
	//---------------------------------------------------------------------------
	const Drawing::Point Control::PointToClient(const Drawing::Point &point) const
	{
		return Drawing::Point(point.Left - bounds.GetLeft(), point.Top - bounds.GetTop());
	}
	//---------------------------------------------------------------------------
	const Drawing::Point Control::PointToScreen(const Drawing::Point &point) const
	{
		//todo
		return point;
	}
	//---------------------------------------------------------------------------
	Control* Control::GetChildAtPoint(const Drawing::Point &point) const
	{
		for (std::vector<Control*>::const_reverse_iterator it = controls.rbegin(); it != controls.rend(); ++it)
		{
			Control *control = *it;

			if (control->GetEnabled() && control->GetVisible() && control->ContainsPoint(point))
			{
				return control;
			}
		}

		return 0;
	}
	//---------------------------------------------------------------------------
	Control* Control::GetChildByName(const Misc::UnicodeString &name) const
	{
		for (std::vector<Control*>::const_iterator it = controls.begin(); it != controls.end(); ++it)
		{
			Control *control = *it;
			
			if (control->GetName() == name)
			{
				return control;
			}
		}

		throw Misc::ArgumentException(L"No control exists with this name", L"name", __WFILE__, __LINE__);
	}
	//---------------------------------------------------------------------------
	void Control::RequestFocus(Control *control)
	{
		if (control == 0)
		{
			throw Misc::ArgumentNullException(L"control", __WFILE__, __LINE__);
		}
		
		if (!control->CanHaveFocus())
		{
			return;
		}

		//walk up parentStack
		Control *baseParent = this;
		while (baseParent->GetParent() != baseParent)
		{
			baseParent = baseParent->GetParent();
		}

		if (baseParent->focusControl == control)
		{
			return;
		}

		if (baseParent->focusControl != 0)
		{
			baseParent->focusControl->SetFocus(false);
			baseParent->focusControl->focusOutEvent.Invoke(this);
		}
		
		control->SetFocus(true);
		control->focusInEvent.Invoke(this);
		baseParent->focusControl = control;
	}
	//---------------------------------------------------------------------------
	void Control::ClearFocus()
	{
		//walk up parentStack
		Control *baseParent = this;
		while (baseParent->GetParent() != baseParent)
		{
			baseParent = baseParent->GetParent();
		}

		if (baseParent->focusControl != 0)
		{
			baseParent->focusControl->SetFocus(false);
			baseParent->focusControl->focusOutEvent.Invoke(this);
			baseParent->focusControl = 0;
		}
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	IEvent::NextEventTypes Control::ProcessEvent(IEvent *event)
	{
		return IEvent::DontContinue;
	}
	//---------------------------------------------------------------------------
	IEvent::NextEventTypes Control::ChildProcessEvent(IEvent *event)
	{
		if (event == 0)
		{
			throw Misc::ArgumentNullException(L"event", __WFILE__, __LINE__);
		}
		
		//someone is focused, so let him handle the event expect the mouse
		if (event->Type != IEvent::Mouse && focusControl != 0 && focusControl->GetVisible() && focusControl->GetEnabled())
		{
			if (focusControl->ProcessEvent(event) == IEvent::DontContinue)
			{
				return IEvent::DontContinue;
			}
		}
		
		if (event->Type == IEvent::Mouse)
		{
			MouseEvent *mouse = (MouseEvent*)event;
			
			//find mouseOverControl
			Control *control = GetChildAtPoint(mouse->Position);
			if (control != mouseOverControl && mouseOverControl != 0)
			{
				mouseOverControl->SetMouseOver(false);
				mouseOverControl->mouseLeaveEvent.Invoke(this);
				mouseOverControl = 0;
			}

			if (control != 0 && control != mouseOverControl)
			{
				mouseOverControl = control;
				mouseOverControl->SetMouseOver(true);
				mouseOverControl->mouseEnterEvent.Invoke(this);
			}
			
			//someone is focused
			if (focusControl != 0 && focusControl->GetEnabled())
			{
				if (!(mouse->State == MouseEvent::LeftDown && focusControl != mouseOverControl))
				{
					if (focusControl->ProcessEvent(mouse) == IEvent::DontContinue)
					{
						return IEvent::DontContinue;
					}
				}
			}
			
			//let mouseOverControl handle the mouse
			if (mouseOverControl != 0)
			{
				if (mouseOverControl->ProcessEvent(event) == IEvent::DontContinue)
				{
					return IEvent::DontContinue;
				}
			}
		}
		
		return IEvent::Continue;
	}
	//---------------------------------------------------------------------------
	void Control::Render(Drawing::IRenderer *renderer)
	{
		return;
	}
	//---------------------------------------------------------------------------
}