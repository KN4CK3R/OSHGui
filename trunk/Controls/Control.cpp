/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include "Control.hpp"
#include "../Misc/Exceptions.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	Control::Control()
		: type((CONTROL_TYPE)0),
		  parent(nullptr),
		  location(6, 6),
		  size(0, 0),
		  anchor(AnchorTop | AnchorLeft),
		  isEnabled(true),
		  isVisible(true),
		  isFocused(false),
		  isClicked(false),
		  isInside(false),
		  isFocusable(true),
		  hasCaptured(false),
		  autoSize(false),
		  canRaiseEvents(true),
		  font(Application::Instance()->GetRenderer()->GetDefaultFont()),
		  cursor(Cursors::Get(Cursors::Default)),
		  mouseOverFocusColor(0, 20, 20, 20)
	{
		
	}
	//---------------------------------------------------------------------------
	Control::~Control()
	{
		if (isInside)
		{
			Application::Instance()->MouseEnteredControl = nullptr;
		}
		if (isFocused)
		{
			Application::Instance()->FocusedControl = nullptr;
		}
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	CONTROL_TYPE Control::GetType() const
	{
		return type;
	}
	//---------------------------------------------------------------------------
	bool Control::IsContainer() const
	{
		return false;
	}
	//---------------------------------------------------------------------------
	bool Control::GetIsFocused() const
	{
		return isFocused;
	}
	//---------------------------------------------------------------------------
	void Control::SetEnabled(bool isEnabled)
	{
		this->isEnabled = isEnabled;
		if (isEnabled == false && isFocused)
		{
			OnLostFocus(nullptr);
		}
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
		if (isVisible == false && isFocused)
		{
			OnLostFocus(nullptr);
		}
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
	void Control::SetAnchor(AnchorStyle anchor)
	{
		this->anchor = anchor;
	}
	//---------------------------------------------------------------------------
	AnchorStyle Control::GetAnchor() const
	{
		return anchor;
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
	void Control::SetForeColor(Drawing::Color color)
	{
		foreColor = color;
	}
	//---------------------------------------------------------------------------
	Drawing::Color Control::GetForeColor() const
	{
		return foreColor;
	}
	//---------------------------------------------------------------------------
	void Control::SetBackColor(Drawing::Color color)
	{
		backColor = color;
	}
	//---------------------------------------------------------------------------
	Drawing::Color Control::GetBackColor() const
	{
		return backColor;
	}
	//---------------------------------------------------------------------------
	void Control::SetMouseOverFocusColor(Drawing::Color color)
	{
		mouseOverFocusColor = color;
	}
	//---------------------------------------------------------------------------
	Drawing::Color Control::GetMouseOverFocusColor() const
	{
		return mouseOverFocusColor;
	}
	//---------------------------------------------------------------------------
	void Control::SetFont(const std::shared_ptr<Drawing::IFont> &font)
	{
		#ifndef OSHGUI_DONTUSEEXCEPTIONS
		if (font == nullptr)
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
		if (parent == nullptr)
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
		if (parent != nullptr && parent != this)
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

		if (app->MouseEnteredControl != nullptr && app->MouseEnteredControl->isInside)
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

		Application::Instance()->MouseEnteredControl = nullptr;

		mouseLeaveEvent.Invoke(this);
	}
	//---------------------------------------------------------------------------
	void Control::OnGotMouseCapture()
	{
		Application *app = Application::Instance();
		if (app->CaptureControl != nullptr)
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

		Application::Instance()->CaptureControl = nullptr;

		mouseCaptureChangedEvent.Invoke(this);
	}
	//---------------------------------------------------------------------------
	void Control::OnGotFocus(Control *newFocusedControl)
	{
		Application *app = Application::Instance();
		if (newFocusedControl != app->FocusedControl)
		{
			if (app->FocusedControl != nullptr)
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
		Application::Instance()->FocusedControl = nullptr;

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
			case MouseMessage::Down:
				if (canRaiseEvents && Intersect(mouse.Location))
				{
					if (mouse.Button == MouseButton::Left && !isClicked && isEnabled)
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
			case MouseMessage::Up:
				if (canRaiseEvents && (hasCaptured || Intersect(mouse.Location)))
				{
					if (isClicked)
					{
						if (mouse.Button == MouseButton::Left)
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
			switch (keyboard.GetState())
			{
				case KeyboardMessage::KeyDown:
					return OnKeyDown(keyboard);
				case KeyboardMessage::KeyUp:
					return OnKeyUp(keyboard);
				case KeyboardMessage::Character:
					return OnKeyPress(keyboard);
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
	void Control::ApplyTheme(const Drawing::Theme &theme)
	{
		auto &controlTheme = theme.GetControlColorTheme(ControlTypeToString(type));
		SetForeColor(controlTheme.ForeColor);
		SetBackColor(controlTheme.BackColor);
	}
	//---------------------------------------------------------------------------
	Misc::AnsiString Control::ControlTypeToString(CONTROL_TYPE controlType)
	{
		switch (controlType)
		{
			case CONTROL_PANEL: return "panel";
			case CONTROL_FORM: return "form";
			case CONTROL_GROUPBOX: return "groupbox";
			case CONTROL_LABEL: return "label";
			case CONTROL_LINKLABEL: return "linklabel";
			case CONTROL_BUTTON: return "button";
			case CONTROL_CHECKBOX: return "checkbox";
			case CONTROL_RADIOBUTTON: return "radiobutton";
			case CONTROL_SCROLLBAR: return "scrollbar";
			case CONTROL_LISTBOX: return "listbox";
			case CONTROL_PROGRESSBAR: return "progressbar";
			case CONTROL_TRACKBAR: return "trackbar";
			case CONTROL_COMBOBOX: return "combobox";
			case CONTROL_TEXTBOX: return "textbox";
			case CONTROL_TIMER: return "timer";
			case CONTROL_TABCONTROL: return "tabcontrol";
			case CONTROL_TABPAGE: return "tabpage";
			case CONTROL_PICTUREBOX: return "picturebox";
			case CONTROL_COLORPICKER: return "colorpicker";
			case CONTROL_COLORBAR: return "colorbar";
			case CONTROL_HOTKEYCONTROL: return "hotkeycontrol";
		}
		throw 1;
	}
	//---------------------------------------------------------------------------
}