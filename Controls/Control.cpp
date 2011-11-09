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
		isFocused = false;
		isClicked = false;
		isInside = false;
		isFocusable = true;

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
		return point - bounds.GetPosition();
	}
	//---------------------------------------------------------------------------
	const Drawing::Point Control::PointToScreen(const Drawing::Point &point) const
	{
		if (!parent)
		{
			throw Misc::ArgumentNullException(L"parent");
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
	void Control::OnMouseDown(const MouseEvent &mouse)
	{
		isClicked = true;
		Application::ClickedControl = this;
            
		//UI.CurrentHud.WindowManager.BringToFront(depth.WindowLayer);

        MouseEventArgs args(mouse);
		mouseDownEvent.Invoke(this, args);
	}
	//---------------------------------------------------------------------------
	void Control::OnMouseClick(const MouseEvent &mouse)
	{
		MouseEventArgs args(mouse);
		mouseClickEvent.Invoke(this, args);
	}
	//---------------------------------------------------------------------------
	void Control::OnMouseUp(const MouseEvent &mouse)
	{
		isClicked = false;
		Application::ClickedControl = 0;

		MouseEventArgs args(mouse);
		mouseUpEvent.Invoke(this, args);
	}
	//---------------------------------------------------------------------------
	void Control::OnMouseMove(const MouseEvent &mouse)
	{
		MouseEventArgs args(mouse);
		mouseMoveEvent.Invoke(this, args);		
	}
	//---------------------------------------------------------------------------
	void Control::OnMouseScroll(const MouseEvent &mouse)
	{
		MouseEventArgs args(mouse);
		mouseScrollEvent.Invoke(this, args);
	}
	//---------------------------------------------------------------------------
	void Control::OnMouseEnter(const MouseEvent &mouse)
	{
		isInside = true;

		if (Application::MouseEnteredControl != 0 && Application::MouseEnteredControl->isInside)
		{
			Application::MouseEnteredControl->OnMouseLeave(mouse);
		}
		Application::MouseEnteredControl = this;

		mouseEnterEvent.Invoke(this);
	}
	//---------------------------------------------------------------------------
	void Control::OnMouseLeave(const MouseEvent &mouse)
	{
		isInside = false;

		mouseLeaveEvent.Invoke(this);
	}
	//---------------------------------------------------------------------------
	void Control::OnMouseCaptureChanged()
	{
		Application::CaptureControl = 0;
		isClicked = false;

		mouseCaptureChangedEvent.Invoke(this);
	}
	//---------------------------------------------------------------------------
	void Control::OnGotFocus()
	{
		if (this != Application::FocusedControl)
		{
			if (Application::FocusedControl != 0)
			{
				Application::FocusedControl->OnLostFocus();
			}

			Application::FocusedControl = this;
			isFocused = true;

			focusGotEvent.Invoke(this);
		}
	}
	//---------------------------------------------------------------------------
	void Control::OnLostFocus()
	{
		isFocused = isClicked = false;
		Application::FocusedControl = 0;

		focusLostEvent.Invoke(this);
	}
	//---------------------------------------------------------------------------
	void Control::OnKeyDown(const KeyboardEvent &keyboard)
	{
		if (canRaiseEvents)
		{
			KeyEventArgs args(keyboard);
			keyDownEvent.Invoke(this, args);
		}
	}
	//---------------------------------------------------------------------------
	void Control::OnKeyPress(const KeyboardEvent &keyboard)
	{
		if (canRaiseEvents)
		{
			KeyPressEventArgs args(keyboard);
			keyPressEvent.Invoke(this, args);
		}
	}
	//---------------------------------------------------------------------------
	void Control::OnKeyUp(const KeyboardEvent &keyboard)
	{
		if (canRaiseEvents)
		{
			KeyEventArgs args(keyboard);
			keyUpEvent.Invoke(this, args);
		}
	}
	//---------------------------------------------------------------------------
	IEvent::NextEventTypes Control::ProcessMouseEvent(MouseEvent &mouse)
	{
		Drawing::Point backupMousePosition = mouse.Position;
		mouse.Position = PointToClient(mouse.Position);

		for (std::vector<Control*>::reverse_iterator it = controls.rbegin(); it != controls.rend(); ++it)
		{
			Control &child = *(*it);
			if (child.ProcessMouseEvent(mouse) == IEvent::DontContinue)
			{
				return IEvent::DontContinue;
			}
		}

		switch (mouse.State)
		{
			case MouseEvent::LeftDown:
			case MouseEvent::RightDown:
				if (canRaiseEvents && ContainsPoint(mouse.Position))
				{
					if (mouse.State == MouseEvent::LeftDown && !isClicked && isEnabled)
					{
						OnMouseDown(mouse);
						
						if (isFocusable && !isFocused)
						{
							OnGotFocus();
						}
					}

					return IEvent::DontContinue;
				}
				break;
			case MouseEvent::LeftUp:
			case MouseEvent::RightUp:
				if (canRaiseEvents && (hasCaptured || ContainsPoint(mouse.Position)))
				{
					if (isClicked)
					{
						if (mouse.State != MouseEvent::Unknown)
						{
							OnMouseClick(mouse);
						}
					}

					OnMouseUp(mouse);
			
					return IEvent::DontContinue;
				}
				break;
			case MouseEvent::Move:
			case MouseEvent::Scroll:
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

					return IEvent::DontContinue;
				}
				break;
		}

		mouse.Position = backupMousePosition;

		return IEvent::Continue;
	}
	IEvent::NextEventTypes Control::ProcessEvent(IEvent *event)
	{
		for (std::vector<Control*>::reverse_iterator it = controls.rbegin(); it != controls.rend(); ++it)
		{
			Control &child = *(*it);
			if (child.ProcessEvent(event) == IEvent::DontContinue)
			{
				return IEvent::DontContinue;
			}
		}

		switch (event->Type)
		{
			case IEvent::Mouse:
				{
					MouseEvent &mouse = *(MouseEvent*)event;
					MouseEvent backup = mouse;

					mouse.Position = PointToClient(mouse.Position);

					switch (mouse.State)
					{
						case MouseEvent::LeftDown:
						case MouseEvent::RightDown:
							if (canRaiseEvents && ContainsPoint(mouse.Position))
							{
								if (mouse.State == MouseEvent::LeftDown && !isClicked && isEnabled)
								{
									OnMouseDown(mouse);
									
									if (isFocusable && !isFocused)
									{
										OnGotFocus();
									}
								}

								return IEvent::DontContinue;
							}
							break;
						case MouseEvent::LeftUp:
						case MouseEvent::RightUp:
							if (canRaiseEvents && (hasCaptured || ContainsPoint(mouse.Position)))
							{
								if (isClicked)
								{
									if (mouse.State != MouseEvent::Unknown)
									{
										OnMouseClick(mouse);
									}
								}

								OnMouseUp(mouse);
			
								return IEvent::DontContinue;
							}
							break;
						case MouseEvent::Move:
						case MouseEvent::Scroll:
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

								return IEvent::DontContinue;
							}
							break;
					}

					mouse.Position = backup.Position;
				}
				break;
			case IEvent::Keyboard:
				{
					KeyboardEvent &keyboard = *(KeyboardEvent*)event;
					switch (keyboard.State)
					{
						case KeyboardEvent::KeyDown:
							OnKeyDown(keyboard);
							return IEvent::DontContinue;
						case KeyboardEvent::KeyUp:
							OnKeyUp(keyboard);
							return IEvent::DontContinue;
						case KeyboardEvent::Character:
							OnKeyPress(keyboard);
							return IEvent::DontContinue;
					}
				}
				break;
		}

		return IEvent::Continue;
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