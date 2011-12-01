#include "Control.hpp"
#include "..\Misc\Exceptions.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	Control::Control(const Misc::AnsiString &name, const Drawing::Point &location, const Drawing::Size &size)
	{
		type = CONTROL_ROOT;

		parent = 0;

		this->name = name;
		this->location = location;
		this->size = size;
		
		isEnabled = true;
		isVisible = true;
		
		isSubComponent = false;
		mouseOver = false;
		isFocused = false;
		isClicked = false;
		isInside = false;
		isFocusable = true;
		hasCaptured = false;

		focusControl = 0;
		mouseOverControl = 0;

		font = Application::Instance()->GetRenderer()->GetDefaultFont();
		
		cursor = Cursors::Get(Cursors::Default);
		
		mouseOverFocusColor = Drawing::Color(0, 20, 20, 20);
	}
	//---------------------------------------------------------------------------
	Control::~Control()
	{

	}
	//---------------------------------------------------------------------------
	CONTROL_TYPE Control::GetType() const
	{
		return type;
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void Control::SetEnabled(bool isEnabled)
	{
		this->isEnabled = isEnabled;
	}
	//---------------------------------------------------------------------------
	bool Control::GetEnabled() const
	{
		return isEnabled;
	}
	//---------------------------------------------------------------------------
	void Control::SetVisible(bool isVisible)
	{
		this->isVisible = isVisible;
	}
	//---------------------------------------------------------------------------
	bool Control::GetVisible() const
	{
		return isVisible;
	}
	//---------------------------------------------------------------------------
	void Control::SetFocus(bool focus)
	{
		isFocused = focus;
	}
	//---------------------------------------------------------------------------
	void Control::SetMouseOver(bool mouseOver)
	{
		this->mouseOver = mouseOver;
		
		if (mouseOver)
		{
			Application::Instance()->SetCursor(cursor);
		}
	}
	//---------------------------------------------------------------------------
	void Control::SetAutoSize(bool autoSize)
	{
		this->autoSize = autoSize;
	}
	//---------------------------------------------------------------------------
	bool Control::GetAutoSize() const
	{
		return autoSize;
	}
	//---------------------------------------------------------------------------
	void Control::SetBounds(int x, int y, int w, int h)
	{
		SetBounds(Drawing::Point(x, y), Drawing::Size(w, h));
	}
	//---------------------------------------------------------------------------
	void Control::SetBounds(const Drawing::Point &location, const Drawing::Size &size)
	{
		SetLocation(location);
		SetSize(size);
	}
	//---------------------------------------------------------------------------
	void Control::SetBounds(const Drawing::Rectangle &bounds)
	{
		SetBounds(bounds.GetPosition(), bounds.GetSize());
	}
	//---------------------------------------------------------------------------
	const Drawing::Rectangle Control::GetBounds() const
	{
		return Drawing::Rectangle(location, size);
	}
	//---------------------------------------------------------------------------
	void Control::SetLocation(const Drawing::Point &location)
	{
		this->location = location;

		OnLocationChanged();
	}
	//---------------------------------------------------------------------------
	void Control::SetLocation(int x, int y)
	{
		SetLocation(Drawing::Point(x, y));
	}
	//---------------------------------------------------------------------------
	const Drawing::Point& Control::GetLocation() const
	{
		return location;
	}
	//---------------------------------------------------------------------------
	void Control::SetSize(const Drawing::Size &size)
	{
		#ifndef OSHGUI_DONTUSEEXCEPTIONS
		if (size.Width < 0)
		{
			throw Misc::ArgumentOutOfRangeException("width", __FILE__, __LINE__);
		}
		if (size.Height < 0)
		{
			throw Misc::ArgumentOutOfRangeException("height", __FILE__, __LINE__);
		}
		#endif

		this->size = size;
		
		OnSizeChanged();
	}
	//---------------------------------------------------------------------------
	void Control::SetSize(int width, int height)
	{	
		SetSize(Drawing::Size(width, height));
	}
	//---------------------------------------------------------------------------
	const Drawing::Size& Control::GetSize() const
	{
		return size;
	}
	//---------------------------------------------------------------------------
	int Control::GetLeft() const
	{
		return location.Left;
	}
	//---------------------------------------------------------------------------
	int Control::GetTop() const
	{
		return location.Top;
	}
	//---------------------------------------------------------------------------
	int Control::GetRight() const
	{
		return location.Left + size.Width;
	}
	//---------------------------------------------------------------------------
	int Control::GetBottom() const
	{
		return location.Top + size.Height;
	}
	//---------------------------------------------------------------------------
	int Control::GetWidth() const
	{
		return size.Width;
	}
	//---------------------------------------------------------------------------
	int Control::GetHeight() const
	{
		return size.Height;
	}
	//---------------------------------------------------------------------------
	void Control::SetTag(Misc::Any &tag)
	{
		this->tag = tag;
	}
	//---------------------------------------------------------------------------
	const Misc::Any& Control::GetTag() const
	{
		return tag;
	}
	//---------------------------------------------------------------------------
	void Control::SetName(const Misc::AnsiString &name)
	{
		this->name = name;
	}
	//---------------------------------------------------------------------------
	const Misc::AnsiString& Control::GetName() const
	{
		return name;
	}
	//---------------------------------------------------------------------------
	void Control::SetForeColor(const Drawing::Color &color)
	{
		foreColor = color;
	}
	//---------------------------------------------------------------------------
	const Drawing::Color& Control::GetForeColor() const
	{
		return foreColor;
	}
	//---------------------------------------------------------------------------
	void Control::SetBackColor(const Drawing::Color &color)
	{
		backColor = color;
	}
	//---------------------------------------------------------------------------
	const Drawing::Color& Control::GetBackColor() const
	{
		return backColor;
	}
	//---------------------------------------------------------------------------
	void Control::SetMouseOverFocusColor(const Drawing::Color &color)
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
		#ifndef OSHGUI_DONTUSEEXCEPTIONS
		if (font == 0)
		{
			throw Misc::ArgumentNullException("font", __FILE__, __LINE__);
		}
		#endif
		
		this->font = font;
	}
	//---------------------------------------------------------------------------
	const std::shared_ptr<Drawing::IFont>& Control::GetFont() const
	{
		return font;
	}
	//---------------------------------------------------------------------------
	void Control::SetCursor(const std::shared_ptr<Cursor> &cursor)
	{
		this->cursor = cursor;
	}
	//---------------------------------------------------------------------------
	const std::shared_ptr<Cursor>& Control::GetCursor() const
	{
		return cursor;
	}
	//---------------------------------------------------------------------------
	LocationChangedEvent& Control::GetLocationChangedEvent()
	{
		return locationChangedEvent;
	}
	//---------------------------------------------------------------------------
	SizeChangedEvent& Control::GetSizeChangedEvent()
	{
		return sizeChangedEvent;
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
	MouseUpEvent& Control::GetMouseUpEvent()
	{
		return mouseUpEvent;
	}
	//---------------------------------------------------------------------------
	MouseMoveEvent& Control::GetMouseMoveEvent()
	{
		return mouseMoveEvent;
	}
	//---------------------------------------------------------------------------
	MouseScrollEvent& Control::GetMouseScrollEvent()
	{
		return mouseScrollEvent;
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
	KeyDownEvent& Control::GetKeyDownEvent()
	{
		return keyDownEvent;
	}
	//---------------------------------------------------------------------------
	KeyPressEvent& Control::GetKeyPressEvent()
	{
		return keyPressEvent;
	}
	//---------------------------------------------------------------------------
	KeyUpEvent& Control::GetKeyUpEvent()
	{
		return keyUpEvent;
	}
	//---------------------------------------------------------------------------
	void Control::SetParent(Control *parent)
	{
		#ifndef OSHGUI_DONTUSEEXCEPTIONS
		if (parent == 0)
		{
			throw Misc::ArgumentNullException("parent", __FILE__, __LINE__);
		}
		#endif

		this->parent = parent;

		OnLocationChanged();
	}
	//---------------------------------------------------------------------------
	Control* Control::GetParent() const
	{
		return parent;
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool Control::CanHaveFocus() const
	{
		return false;
	}
	//---------------------------------------------------------------------------
	void Control::Invalidate()
	{
		return;
	}
	//---------------------------------------------------------------------------
	void Control::InvalidateChildren()
	{
		/*for (std::list<Control*>::iterator it = controls.begin(); it != controls.end(); ++it)
		{
			Control *control = *it;

			if (control == 0)
			{
				continue;
			}

			control->Invalidate();
		}*/
	}
	//---------------------------------------------------------------------------
	const Drawing::Point Control::PointToClient(const Drawing::Point &point) const
	{
		return point - absoluteLocation;
	}
	//---------------------------------------------------------------------------
	const Drawing::Point Control::PointToScreen(const Drawing::Point &point) const
	{
		#ifndef OSHGUI_DONTUSEEXCEPTIONS
		if (!parent)
		{
			throw Misc::ArgumentNullException("parent");
		}
		#endif
		
		if (parent != this)
		{
			return parent->PointToScreen(point + location);
		}

		return point + location;
	}
	//---------------------------------------------------------------------------
	void Control::CalculateAbsoluteLocation()
	{
		if (parent != 0 && parent != this)
		{
			absoluteLocation = parent->absoluteLocation + location;
		}
		if (parent == this)
		{
			absoluteLocation = location;
		}
	}
	//---------------------------------------------------------------------------
	void Control::RequestFocus(Control *control)
	{
		#ifndef OSHGUI_DONTUSEEXCEPTIONS
		if (control == 0)
		{
			throw Misc::ArgumentNullException("control", __FILE__, __LINE__);
		}
		#endif
		
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
			baseParent->focusControl->focusLostEvent.Invoke(this);
		}
		
		control->SetFocus(true);
		control->focusGotEvent.Invoke(this);
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
			baseParent->focusControl->focusLostEvent.Invoke(this);
			baseParent->focusControl = 0;
		}
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	void Control::OnLocationChanged()
	{
		CalculateAbsoluteLocation();
	
		locationChangedEvent.Invoke(this);
	}
	//---------------------------------------------------------------------------
	void Control::OnSizeChanged()
	{
		sizeChangedEvent.Invoke(this);
	}
	//---------------------------------------------------------------------------
	void Control::OnMouseDown(const MouseMessage &mouse)
	{
		isClicked = true;
		Application::Instance()->ClickedControl = this;

        MouseEventArgs args(mouse);
		mouseDownEvent.Invoke(this, args);
	}
	//---------------------------------------------------------------------------
	void Control::OnMouseClick(const MouseMessage &mouse)
	{
		MouseEventArgs args(mouse);
		mouseClickEvent.Invoke(this, args);
	}
	//---------------------------------------------------------------------------
	void Control::OnMouseUp(const MouseMessage &mouse)
	{
		isClicked = false;
		Application::Instance()->ClickedControl = 0;

		MouseEventArgs args(mouse);
		mouseUpEvent.Invoke(this, args);
	}
	//---------------------------------------------------------------------------
	void Control::OnMouseMove(const MouseMessage &mouse)
	{
		MouseEventArgs args(mouse);
		mouseMoveEvent.Invoke(this, args);		
	}
	//---------------------------------------------------------------------------
	void Control::OnMouseScroll(const MouseMessage &mouse)
	{
		MouseEventArgs args(mouse);
		mouseScrollEvent.Invoke(this, args);
	}
	//---------------------------------------------------------------------------
	void Control::OnMouseEnter(const MouseMessage &mouse)
	{
		isInside = true;

		Application *app = Application::Instance();

		if (app->MouseEnteredControl != 0 && app->MouseEnteredControl->isInside)
		{
			app->MouseEnteredControl->OnMouseLeave(mouse);
		}
		app->MouseEnteredControl = this;

		mouseEnterEvent.Invoke(this);

		app->SetCursor(cursor);
	}
	//---------------------------------------------------------------------------
	void Control::OnMouseLeave(const MouseMessage &mouse)
	{
		isInside = false;

		mouseLeaveEvent.Invoke(this);
	}
	//---------------------------------------------------------------------------
	void Control::OnMouseCaptureChanged()
	{
		Application::Instance()->CaptureControl = 0;
		isClicked = false;

		mouseCaptureChangedEvent.Invoke(this);
	}
	//---------------------------------------------------------------------------
	void Control::OnGotFocus()
	{
		Application *app = Application::Instance();
		if (this != app->FocusedControl)
		{
			if (app->FocusedControl != 0)
			{
				app->FocusedControl->OnLostFocus();
			}

			app->FocusedControl = this;
			isFocused = true;

			focusGotEvent.Invoke(this);
		}
	}
	//---------------------------------------------------------------------------
	void Control::OnLostFocus()
	{
		isFocused = isClicked = false;
		Application::Instance()->FocusedControl = 0;

		focusLostEvent.Invoke(this);
	}
	//---------------------------------------------------------------------------
	void Control::OnKeyDown(const KeyboardMessage &keyboard)
	{
		if (canRaiseEvents)
		{
			KeyEventArgs args(keyboard);
			keyDownEvent.Invoke(this, args);
		}
	}
	//---------------------------------------------------------------------------
	void Control::OnKeyPress(const KeyboardMessage &keyboard)
	{
		if (canRaiseEvents)
		{
			KeyPressEventArgs args(keyboard);
			keyPressEvent.Invoke(this, args);
		}
	}
	//---------------------------------------------------------------------------
	void Control::OnKeyUp(const KeyboardMessage &keyboard)
	{
		if (canRaiseEvents)
		{
			KeyEventArgs args(keyboard);
			keyUpEvent.Invoke(this, args);
		}
	}
	//---------------------------------------------------------------------------
	bool Control::ProcessMouseMessage(MouseMessage &mouse)
	{
		/*for (std::list<Control*>::reverse_iterator it = controls.rbegin(); it != controls.rend(); ++it)
		{
			Control &child = *(*it);
			if (child.ProcessMouseMessage(mouse) == true)
			{
				return true;
			}
		}*/

		switch (mouse.State)
		{
			case MouseMessage::LeftDown:
			case MouseMessage::RightDown:
				if (canRaiseEvents && Intersect(mouse.Position))
				{
					if (mouse.State == MouseMessage::LeftDown && !isClicked && isEnabled)
					{
						OnMouseDown(mouse);
						
						if (isFocusable && !isFocused)
						{
							OnGotFocus();
						}
					}

					return true;
				}
				break;
			case MouseMessage::LeftUp:
			case MouseMessage::RightUp:
				if (canRaiseEvents && (hasCaptured || Intersect(mouse.Position)))
				{
					if (isClicked)
					{
						if (mouse.State != MouseMessage::Unknown)
						{
							OnMouseClick(mouse);
						}
					}

					OnMouseUp(mouse);
			
					return true;
				}
				break;
			case MouseMessage::Move:
			case MouseMessage::Scroll:
				if (hasCaptured || Intersect(mouse.Position))
				{
					if (canRaiseEvents)
					{
						if (!isInside)
						{
							OnMouseEnter(mouse);
						}

						if (mouse.Delta != 0)
						{
							OnMouseScroll(mouse);
						}

						OnMouseMove(mouse);
					}

					return true;
				}
				break;
		}

		return false;
	}
	//---------------------------------------------------------------------------
	bool Control::ProcessKeyboardMessage(KeyboardMessage &keyboard)
	{
		if (canRaiseEvents)
		{
			switch (keyboard.State)
			{
				case KeyboardMessage::KeyDown:
					OnKeyDown(keyboard);
					break;
				case KeyboardMessage::KeyUp:
					OnKeyUp(keyboard);
					break;
				case KeyboardMessage::Character:
					OnKeyPress(keyboard);
					break;
			}
			
			return true;
		}
		
		return false;
	}
	//---------------------------------------------------------------------------
	bool Control::ProcessEvent(IEvent *event)
	{
		return true;
	}
	//---------------------------------------------------------------------------
	bool Control::ChildProcessEvent(IEvent *event)
	{
		#ifndef OSHGUI_DONTUSEEXCEPTIONS
		if (event == 0)
		{
			throw Misc::ArgumentNullException("event", __FILE__, __LINE__);
		}
		#endif
		
		//someone is focused, so let him handle the event expect the mouse
		if (event->Type != IEvent::Mouse && focusControl != 0 && focusControl->GetVisible() && focusControl->GetEnabled())
		{
			if (focusControl->ProcessEvent(event) == true)
			{
				return true;
			}
		}
		
		if (event->Type == IEvent::Mouse)
		{
			MouseMessage *mouse = (MouseMessage*)event;
			
			//find mouseOverControl
			Control *control;// = GetChildAtPoint(mouse->Position);
			if (control != mouseOverControl && mouseOverControl != 0)
			{
				mouseOverControl->SetMouseOver(false);
				//mouseOverControl->mouseLeaveEvent.Invoke(this);
				mouseOverControl = 0;
			}

			if (control != 0 && control != mouseOverControl)
			{
				mouseOverControl = control;
				mouseOverControl->SetMouseOver(true);
				//mouseOverControl->mouseEnterEvent.Invoke(this);
			}
			
			//someone is focused
			if (focusControl != 0 && focusControl->GetEnabled())
			{
				if (!(mouse->State == MouseMessage::LeftDown && focusControl != mouseOverControl))
				{
					if (focusControl->ProcessEvent(mouse) == true)
					{
						return true;
					}
				}
			}
			
			//let mouseOverControl handle the mouse
			if (mouseOverControl != 0)
			{
				if (mouseOverControl->ProcessEvent(event) == true)
				{
					return true;
				}
			}
		}
		
		return false;
	}
	//---------------------------------------------------------------------------
	void Control::Render(Drawing::IRenderer *renderer)
	{
		return;
	}
	//---------------------------------------------------------------------------
	void Control::ChildRender(Drawing::IRenderer *renderer)
	{
		Control *focusedControl = 0;
		/*for (std::list<Control*>::iterator it = controls.begin(); it != controls.end(); ++it)
		{
			Control *control = *it;
			if (control->isFocused)
			{
				focusedControl = control;
			}
			else
			{
				control->Render(renderer);
			}
		}*/
		if (focusedControl != 0)
		{
			focusedControl->Render(renderer);
		}
	}
	//---------------------------------------------------------------------------
}