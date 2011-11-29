#include "Control.hpp"
#include "..\Misc\Exceptions.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	Control::Control()
	{
		type = CONTROL_ROOT;
		
		SetEnabled(true);
		SetVisible(true);
		
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
		for (std::list<Control*>::iterator it = controls.begin(); it != controls.end(); ++it)
		{
			delete *it;
		}
		for (std::list<Control*>::iterator it = internalControls.begin(); it != internalControls.end(); ++it)
		{
			delete *it;
		}

		controls.clear();
		internalControls.clear();
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

		absolutePosition = PointToScreen(Drawing::Point(0, 0));
		
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

		absolutePosition = PointToScreen(Drawing::Point(0, 0));
		
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
			throw Misc::ArgumentOutOfRangeException("width", __FILE__, __LINE__);
		}
		if (height < 0)
		{
			throw Misc::ArgumentOutOfRangeException("height", __FILE__, __LINE__);
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
			throw Misc::ArgumentNullException("font", __FILE__, __LINE__);
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
	Control* Control::GetParent() const
	{
		return parent;
	}
	//---------------------------------------------------------------------------
	const std::list<Control*>& Control::GetControls() const
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
		return absolutePosition.Left <= point.Left && absolutePosition.Top <= point.Top
			&& point.Left <= absolutePosition.Left + bounds.GetWidth()
			&& point.Top <= absolutePosition.Top + bounds.GetHeight();
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
			throw Misc::ArgumentNullException("contro", __FILE__, __LINE__);
		}

		if (control->GetType() == CONTROL_FORM)
		{
			return;
		}
		
		for (std::list<Control*>::iterator it = controls.begin(); it != controls.end(); ++it)
		{
			Control *cont = *it;
			if (control == cont || control->name == cont->name)
			{
				return;
			}
		}

		control->parent = this;
	
		controls.push_back(control);
		internalControls.push_back(control);

		Invalidate();
	}
	//---------------------------------------------------------------------------
	void Control::InvalidateChildren()
	{
		for (std::list<Control*>::iterator it = controls.begin(); it != controls.end(); ++it)
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
		return point - absolutePosition;
	}
	//---------------------------------------------------------------------------
	const Drawing::Point Control::PointToScreen(const Drawing::Point &point) const
	{
		if (!parent)
		{
			throw Misc::ArgumentNullException("parent");
		}
		
		if (parent != this)
		{
			return parent->PointToScreen(point + bounds.GetPosition());
		}

		return point + bounds.GetPosition();
	}
	//---------------------------------------------------------------------------
	Control* Control::GetChildAtPoint(const Drawing::Point &point) const
	{
		for (std::list<Control*>::const_reverse_iterator it = controls.rbegin(); it != controls.rend(); ++it)
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
	Control* Control::GetChildByName(const Misc::AnsiString &name) const
	{
		for (std::list<Control*>::const_iterator it = controls.begin(); it != controls.end(); ++it)
		{
			Control *control = *it;
			
			if (control->GetName() == name)
			{
				return control;
			}
		}

		return 0;
	}
	//---------------------------------------------------------------------------
	void Control::RequestFocus(Control *control)
	{
		if (control == 0)
		{
			throw Misc::ArgumentNullException("contro", __FILE__, __LINE__);
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
		absolutePosition = PointToScreen(Drawing::Point(0, 0));
	
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
		for (std::list<Control*>::reverse_iterator it = controls.rbegin(); it != controls.rend(); ++it)
		{
			Control &child = *(*it);
			if (child.ProcessMouseMessage(mouse) == true)
			{
				return true;
			}
		}

		switch (mouse.State)
		{
			case MouseMessage::LeftDown:
			case MouseMessage::RightDown:
				if (canRaiseEvents && ContainsPoint(mouse.Position))
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
				if (canRaiseEvents && (hasCaptured || ContainsPoint(mouse.Position)))
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
				if (hasCaptured || ContainsPoint(mouse.Position))
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
		if (event == 0)
		{
			throw Misc::ArgumentNullException("event", __FILE__, __LINE__);
		}
		
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
			Control *control = GetChildAtPoint(mouse->Position);
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
		for (unsigned int i = 0; i < controls.size(); ++i)
		{
			Control *control = controls[i];
			if (control->isFocused)
			{
				focusedControl = control;
			}
			else
			{
				control->Render(renderer);
			}
		}
		if (focusedControl != 0)
		{
			focusedControl->Render(renderer);
		}
	}
	//---------------------------------------------------------------------------
}