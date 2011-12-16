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

		parent = 0;

		location = Drawing::Point(6, 6);
		size = Drawing::Size(0, 0);
		
		isEnabled = true;
		isVisible = true;
		
		isFocused = false;
		isClicked = false;
		isInside = false;
		isFocusable = true;
		hasCaptured = false;

		font = Application::Instance()->GetRenderer()->GetDefaultFont();
		
		cursor = Cursors::Get(Cursors::Default);
		
		mouseOverFocusColor = Drawing::Color(0, 20, 20, 20);
	}
	//---------------------------------------------------------------------------
	Control::~Control()
	{
		if (isInside)
		{
			Application::Instance()->MouseEnteredControl = 0;
		}
		if (isFocused)
		{
			Application::Instance()->FocusedControl = 0;
		}
	}
	//---------------------------------------------------------------------------
	CONTROL_TYPE Control::GetType() const
	{
		return type;
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	bool Control::GetIsFocused() const
	{
		return isFocused;
	}
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
		SetBounds(bounds.GetLocation(), bounds.GetSize());
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
	FocusGotEvent& Control::GetFocusGotEvent()
	{
		return focusGotEvent;
	}
	//---------------------------------------------------------------------------
	FocusLostEvent& Control::GetFocusLostEvent()
	{
		return focusLostEvent;
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
	void Control::Focus()
	{
		if (isFocusable && !isFocused)
		{
			OnGotFocus(this);
		}
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

        MouseEventArgs args(mouse);
		args.Location -= absoluteLocation;
		mouseDownEvent.Invoke(this, args);
	}
	//---------------------------------------------------------------------------
	void Control::OnMouseClick(const MouseMessage &mouse)
	{
		MouseEventArgs args(mouse);
		args.Location -= absoluteLocation;
		mouseClickEvent.Invoke(this, args);
	}
	//---------------------------------------------------------------------------
	void Control::OnMouseUp(const MouseMessage &mouse)
	{
		isClicked = false;

		MouseEventArgs args(mouse);
		args.Location -= absoluteLocation;
		mouseUpEvent.Invoke(this, args);
	}
	//---------------------------------------------------------------------------
	void Control::OnMouseMove(const MouseMessage &mouse)
	{
		MouseEventArgs args(mouse);
		args.Location -= absoluteLocation;
		mouseMoveEvent.Invoke(this, args);		
	}
	//---------------------------------------------------------------------------
	void Control::OnMouseScroll(const MouseMessage &mouse)
	{
		MouseEventArgs args(mouse);
		args.Location -= absoluteLocation;
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

		Application::Instance()->MouseEnteredControl = 0;

		mouseLeaveEvent.Invoke(this);
	}
	//---------------------------------------------------------------------------
	void Control::OnGotMouseCapture()
	{
		Application *app = Application::Instance();
		if (app->CaptureControl != 0)
		{
			app->CaptureControl->OnLostMouseCapture();
		}
		app->CaptureControl = this;
		hasCaptured = true;

		isClicked = false;

		mouseCaptureChangedEvent.Invoke(this);
	}
	//---------------------------------------------------------------------------
	void Control::OnLostMouseCapture()
	{
		hasCaptured = false;

		Application::Instance()->CaptureControl = 0;

		mouseCaptureChangedEvent.Invoke(this);
	}
	//---------------------------------------------------------------------------
	void Control::OnGotFocus(Control *newFocusedControl)
	{
		Application *app = Application::Instance();
		if (newFocusedControl != app->FocusedControl)
		{
			if (app->FocusedControl != 0)
			{
				app->FocusedControl->OnLostFocus(newFocusedControl);
			}

			app->FocusedControl = newFocusedControl;
			isFocused = true;

			focusGotEvent.Invoke(this);
		}
	}
	//---------------------------------------------------------------------------
	void Control::OnLostFocus(Control *newFocusedControl)
	{
		isFocused = isClicked = false;
		Application::Instance()->FocusedControl = 0;

		focusLostEvent.Invoke(this, newFocusedControl);
	}
	//---------------------------------------------------------------------------
	bool Control::OnKeyDown(const KeyboardMessage &keyboard)
	{
		KeyEventArgs args(keyboard);
		keyDownEvent.Invoke(this, args);

		return args.Handled;
	}
	//---------------------------------------------------------------------------
	bool Control::OnKeyPress(const KeyboardMessage &keyboard)
	{
		KeyPressEventArgs args(keyboard);
		keyPressEvent.Invoke(this, args);

		return args.Handled;
	}
	//---------------------------------------------------------------------------
	bool Control::OnKeyUp(const KeyboardMessage &keyboard)
	{
		KeyEventArgs args(keyboard);
		keyUpEvent.Invoke(this, args);

		return args.Handled;
	}
	//---------------------------------------------------------------------------
	bool Control::ProcessMouseMessage(const MouseMessage &mouse)
	{
		switch (mouse.State)
		{
			case MouseMessage::LeftDown:
			case MouseMessage::RightDown:
				if (canRaiseEvents && Intersect(mouse.Location))
				{
					if (mouse.State == MouseMessage::LeftDown && !isClicked && isEnabled)
					{
						OnMouseDown(mouse);
						
						if (isFocusable && !isFocused)
						{
							OnGotFocus(this);
						}
					}

					return true;
				}
				break;
			case MouseMessage::LeftUp:
			case MouseMessage::RightUp:
				if (canRaiseEvents && (hasCaptured || Intersect(mouse.Location)))
				{
					if (isClicked)
					{
						if (mouse.State != MouseMessage::Unknown)
						{
							clickEvent.Invoke(this);

							OnMouseClick(mouse);
						}
					}

					OnMouseUp(mouse);
			
					return true;
				}
				break;
			case MouseMessage::Move:
				if (hasCaptured || Intersect(mouse.Location))
				{
					if (canRaiseEvents)
					{
						if (!isInside)
						{
							OnMouseEnter(mouse);
						}

						OnMouseMove(mouse);
					}

					return true;
				}
				break;
			case MouseMessage::Scroll:
				if (hasCaptured || isFocused)
				{
					if (canRaiseEvents)
					{
						if (mouse.Delta != 0)
						{
							OnMouseScroll(mouse);
						}
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
	void Control::Render(Drawing::IRenderer *renderer)
	{
		return;
	}
	//---------------------------------------------------------------------------
}