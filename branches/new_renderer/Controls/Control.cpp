/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include "Control.hpp"
#include "../Misc/Exceptions.hpp"
#include "../Drawing/FontManager.hpp"
#include "../Drawing/Vector.hpp"
#include "../Misc/ReverseIterator.hpp"
#include "../Misc/Intersection.hpp"
#include <algorithm>

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	Control::Control()
		: type((ControlType)0),
		  parent(nullptr),
		  location(6, 6),
		  size(0, 0),
		  anchor(AnchorStyles::Top | AnchorStyles::Left),
		  isEnabled(true),
		  isVisible(true),
		  isFocused(false),
		  isClicked(false),
		  isInside(false),
		  isFocusable(true),
		  hasCaptured(false),
		  autoSize(false),
		  canRaiseEvents(true),
		  needsRedraw(true),
		  cursor(nullptr),
		  mouseOverFocusColor(Drawing::Color::FromARGB(0, 20, 20, 20)),
		  geometry(Application::Instance().GetRenderer().CreateGeometryBuffer())
	{
		
	}
	//---------------------------------------------------------------------------
	Control::~Control()
	{
		if (isInside)
		{
			Application::Instance().MouseEnteredControl = nullptr;
		}
		if (isFocused)
		{
			Application::Instance().FocusedControl = nullptr;
		}

		for (auto &control : internalControls)
		{
			delete control;
		}
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	ControlType Control::GetType() const
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

		Invalidate();
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
		SetBounds(Drawing::PointF(x, y), Drawing::SizeF(w, h));
	}
	//---------------------------------------------------------------------------
	void Control::SetBounds(const Drawing::PointF &location, const Drawing::SizeF &size)
	{
		SetLocation(location);
		SetSize(size);
	}
	//---------------------------------------------------------------------------
	void Control::SetBounds(const Drawing::RectangleF &bounds)
	{
		SetBounds(bounds.GetLocation(), bounds.GetSize());
	}
	//---------------------------------------------------------------------------
	const Drawing::RectangleF Control::GetBounds() const
	{
		return Drawing::RectangleF(location, size);
	}
	//---------------------------------------------------------------------------
	void Control::SetLocation(const Drawing::PointF &location)
	{
		this->location = location;

		OnLocationChanged();
	}
	//---------------------------------------------------------------------------
	void Control::SetLocation(int x, int y)
	{
		SetLocation(Drawing::PointF(x, y));
	}
	//---------------------------------------------------------------------------
	const Drawing::PointF& Control::GetLocation() const
	{
		return location;
	}
	//---------------------------------------------------------------------------
	void Control::SetSize(const Drawing::SizeF &size)
	{
		#ifndef OSHGUI_DONTUSEEXCEPTIONS
		if (size.Width < 0)
		{
			throw Misc::ArgumentOutOfRangeException("width");
		}
		if (size.Height < 0)
		{
			throw Misc::ArgumentOutOfRangeException("height");
		}
		#endif

		this->size = size;
		
		OnSizeChanged();

		Invalidate();

		auto offset = size - GetSize();
		for (auto &control : controls)
		{
			AnchorStyles anchor = control->GetAnchor();

			if (anchor != (AnchorStyles::Top | AnchorStyles::Left))
			{
				if (anchor == (AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Bottom | AnchorStyles::Right))
				{
					control->SetSize(control->GetSize() + offset);
				}
				else if (anchor == (AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right) || anchor == (AnchorStyles::Bottom | AnchorStyles::Left | AnchorStyles::Right))
				{
					control->SetLocation(control->GetLocation() + Drawing::PointF(0, offset.Height));
					control->SetSize(control->GetSize() + Drawing::SizeF(offset.Width, 0));
				}
				else if (anchor == (AnchorStyles::Top | AnchorStyles::Right) || anchor == (AnchorStyles::Bottom | AnchorStyles::Right))
				{
					control->SetLocation(control->GetLocation() + Drawing::PointF(offset.Width, offset.Height));
				}
			}
		}
	}
	//---------------------------------------------------------------------------
	void Control::SetSize(int width, int height)
	{
		SetSize(Drawing::SizeF(width, height));
	}
	//---------------------------------------------------------------------------
	const Drawing::SizeF& Control::GetSize() const
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
	void Control::SetAnchor(AnchorStyles anchor)
	{
		this->anchor = anchor;
	}
	//---------------------------------------------------------------------------
	AnchorStyles Control::GetAnchor() const
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

		Invalidate();
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

		Invalidate();
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

		Invalidate();
	}
	//---------------------------------------------------------------------------
	Drawing::Color Control::GetMouseOverFocusColor() const
	{
		return mouseOverFocusColor;
	}
	//---------------------------------------------------------------------------
	void Control::SetFont(const Drawing::FontPtr &font)
	{
		#ifndef OSHGUI_DONTUSEEXCEPTIONS
		if (font == nullptr)
		{
			throw Misc::ArgumentNullException("font");
		}
		#endif
		
		this->font = font;

		Invalidate();
	}
	//---------------------------------------------------------------------------
	const Drawing::FontPtr& Control::GetFont() const
	{
		return font ? font : parent ? parent->GetFont() : Application::Instance().GetDefaultFont();
	}
	//---------------------------------------------------------------------------
	void Control::SetCursor(const CursorPtr &cursor)
	{
		this->cursor = std::move(cursor);
	}
	//---------------------------------------------------------------------------
	CursorPtr Control::GetCursor() const
	{
		return cursor ? cursor : GetParent() ? GetParent()->GetCursor() : nullptr;
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
			throw Misc::ArgumentNullException("parent");
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
	const std::deque<Control*>& Control::GetControls() const
	{
		return controls;
	}
	//---------------------------------------------------------------------------
	Control::PostOrderIterator Control::GetPostOrderEnumerator()
	{
		return PostOrderIterator(this);
	}
	//---------------------------------------------------------------------------
	void Control::GetRenderContext(Drawing::RenderContext &context) const
	{
		if (surface)
		{
			context.Surface = surface.get();
			context.Owner = this;
			context.Offset = GetLocation();
			context.QueueType = Drawing::RenderQueueType::Base;
		}
		else if (GetParent())
		{
			GetParent()->GetRenderContext(context);
		}
		else
		{
			context.Surface = &Application::Instance().GetRenderSurface();
			context.Owner = nullptr;
			context.Offset = Drawing::PointF(0.0f, 0.0f);
			context.QueueType = Drawing::RenderQueueType::Base;
		}
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
	bool Control::Intersect(const Drawing::PointF &point) const
	{
		return Intersection::TestRectangle(absoluteLocation, size, point);
	}
	//---------------------------------------------------------------------------
	const Drawing::PointF Control::PointToClient(const Drawing::PointF &point) const
	{
		return point - absoluteLocation;
	}
	//---------------------------------------------------------------------------
	const Drawing::PointF Control::PointToScreen(const Drawing::PointF &point) const
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
		if (parent != nullptr)
		{
			absoluteLocation = parent->absoluteLocation + location;
		}
		else
		{
			absoluteLocation = location;
		}

		geometry->SetTranslation(Drawing::Vector((int)absoluteLocation.X, (int)absoluteLocation.Y, 0.0f));
		//TODO: set clipping here

		for (auto &control : internalControls)
		{
			control->CalculateAbsoluteLocation();
		}
	}
	//---------------------------------------------------------------------------
	void Control::AddControl(Control *control)
	{
		#ifndef OSHGUI_DONTUSEEXCEPTIONS
		if (control == nullptr)
		{
			throw Misc::ArgumentNullException("control");
		}
		#endif

		if (control->GetType() == ControlType::Form)
		{
			return;
		}

		AddSubControl(control);

		controls.push_front(control);
	}
	//---------------------------------------------------------------------------
	void Control::RemoveControl(Control *control)
	{
		if (control != nullptr)
		{
			controls.erase(std::remove(std::begin(controls), std::end(controls), control), std::end(controls));
			internalControls.erase(std::remove(std::begin(internalControls), std::end(internalControls), control), std::end(internalControls));
		}
	}
	//---------------------------------------------------------------------------
	void Control::AddSubControl(Control *subcontrol)
	{
		#ifndef OSHGUI_DONTUSEEXCEPTIONS
		if (subcontrol == nullptr)
		{
			throw Misc::ArgumentNullException("subcontrol");
		}
		#endif

		if (subcontrol->GetType() == ControlType::Form)
		{
			return;
		}

		for (auto &control : internalControls)
		{
			if (subcontrol == control || (!subcontrol->GetName().empty() && subcontrol->GetName() == control->GetName()))
			{
				return;
			}
		}

		subcontrol->SetParent(this);

		internalControls.push_front(subcontrol);
	}
	//---------------------------------------------------------------------------
	Control* Control::GetChildAtPoint(const Drawing::PointF &point) const
	{
		for (auto &control : make_reverse_range(controls))
		{
			if (control->GetEnabled() && control->GetVisible() && control->Intersect(point))
			{
				return control;
			}
		}

		return nullptr;
	}
	//---------------------------------------------------------------------------
	Control* Control::GetChildByName(const Misc::AnsiString &name) const
	{
		for (auto &control : controls)
		{
			if (control->GetName() == name)
			{
				return control;
			}
		}

		return nullptr;
	}
	//---------------------------------------------------------------------------
	void Control::InjectTime(const Misc::DateTime &time)
	{

	}
	//---------------------------------------------------------------------------
	void Control::Invalidate()
	{
		needsRedraw = true;

		Application::Instance().GetRenderSurface().Invalidate();
	}
	//---------------------------------------------------------------------------
	void Control::Render()
	{
		if (!isVisible)
		{
			return;
		}

		using namespace Drawing;

		RenderContext ctx;
		GetRenderContext(ctx);

		// clear geometry from surface if it's ours
		if (ctx.Owner == this)
		{
			ctx.Surface->Reset();
		}

		// redraw if no surface set, or if surface is invalidated
		if (!surface)// || surface->isInvalidated())
		{
			DrawSelf(ctx);

			Control *focusedControl = nullptr;
			for (auto &control : make_reverse_range(controls))
			{
				if (control->GetIsFocused())
				{
					focusedControl = control;
				}
				else
				{
					control->Render();
				}
			}
			if (focusedControl != nullptr)
			{
				focusedControl->Render();
			}
		}

		if (ctx.Owner == this)
		{
			ctx.Surface->Draw();
		}
	}
	//---------------------------------------------------------------------------
	void Control::DrawSelf(Drawing::RenderContext &context)
	{
		BufferGeometry(context);
		QueueGeometry(context);
	}
	//---------------------------------------------------------------------------
	void Control::BufferGeometry(Drawing::RenderContext &context)
	{
		if (needsRedraw)
		{
			geometry->Reset();

			PopulateGeometry();

			needsRedraw = false;
		}
	}
	//---------------------------------------------------------------------------
	void Control::QueueGeometry(Drawing::RenderContext &context)
	{
		context.Surface->AddGeometry(context.QueueType, geometry);
	}
	//---------------------------------------------------------------------------
	void Control::PopulateGeometry()
	{

	}
	//---------------------------------------------------------------------------
	void Control::ApplyTheme(const Drawing::Theme &theme)
	{
		auto &controlTheme = theme.GetControlColorTheme(ControlTypeToString(type));
		SetForeColor(controlTheme.ForeColor);
		SetBackColor(controlTheme.BackColor);

		for (auto &control : controls) //?
		{
			control->ApplyTheme(theme);
		}
	}
	//---------------------------------------------------------------------------
	Misc::AnsiString Control::ControlTypeToString(ControlType controlType)
	{
		switch (controlType)
		{
			case ControlType::Panel: return "panel";
			case ControlType::Form: return "form";
			case ControlType::GroupBox: return "groupbox";
			case ControlType::Label: return "label";
			case ControlType::LinkLabel: return "linklabel";
			case ControlType::Button: return "button";
			case ControlType::CheckBox: return "checkbox";
			case ControlType::RadioButton: return "radiobutton";
			case ControlType::ScrollBar: return "scrollbar";
			case ControlType::ListBox: return "listbox";
			case ControlType::ProgressBar: return "progressbar";
			case ControlType::TrackBar: return "trackbar";
			case ControlType::ComboBox: return "combobox";
			case ControlType::TextBox: return "textbox";
			case ControlType::Timer: return "timer";
			case ControlType::TabControl: return "tabcontrol";
			case ControlType::TabPage: return "tabpage";
			case ControlType::PictureBox: return "picturebox";
			case ControlType::ColorPicker: return "colorpicker";
			case ControlType::ColorBar: return "colorbar";
			case ControlType::HotkeyControl: return "hotkeycontrol";
		}
		throw Misc::Exception();
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

		auto &app = Application::Instance();

		if (app.MouseEnteredControl != nullptr && app.MouseEnteredControl->isInside)
		{
			app.MouseEnteredControl->OnMouseLeave(mouse);
		}
		app.MouseEnteredControl = this;

		mouseEnterEvent.Invoke(this);

		app.SetCursor(GetCursor());

		Invalidate();
	}
	//---------------------------------------------------------------------------
	void Control::OnMouseLeave(const MouseMessage &mouse)
	{
		isInside = false;

		Application::Instance().MouseEnteredControl = nullptr;

		mouseLeaveEvent.Invoke(this);

		Invalidate();
	}
	//---------------------------------------------------------------------------
	void Control::OnGotMouseCapture()
	{
		auto &app = Application::Instance();
		if (app.CaptureControl != nullptr)
		{
			app.CaptureControl->OnLostMouseCapture();
		}
		app.CaptureControl = this;
		hasCaptured = true;

		isClicked = false;

		mouseCaptureChangedEvent.Invoke(this);
	}
	//---------------------------------------------------------------------------
	void Control::OnLostMouseCapture()
	{
		hasCaptured = false;

		Application::Instance().CaptureControl = nullptr;

		mouseCaptureChangedEvent.Invoke(this);
	}
	//---------------------------------------------------------------------------
	void Control::OnGotFocus(Control *newFocusedControl)
	{
		auto &app = Application::Instance();
		if (newFocusedControl != app.FocusedControl)
		{
			if (app.FocusedControl != nullptr)
			{
				app.FocusedControl->OnLostFocus(newFocusedControl);
			}

			app.FocusedControl = newFocusedControl;
			isFocused = true;

			focusGotEvent.Invoke(this);

			Invalidate();
		}
	}
	//---------------------------------------------------------------------------
	void Control::OnLostFocus(Control *newFocusedControl)
	{
		isFocused = isClicked = false;
		
		Application::Instance().FocusedControl = nullptr;

		focusLostEvent.Invoke(this, newFocusedControl);

		Invalidate();
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
	//Control::PostOrderIterator
	//---------------------------------------------------------------------------
	Control::PostOrderIterator::PostOrderIterator(Control *start)
	{
		this->start = start;

		if (this->start->internalControls.empty())
		{
			current = this->start;
		}
		else
		{
			LoopThrough(this->start);
			current = controlStack.back();
			controlStack.pop_back();
		}
	}
	//---------------------------------------------------------------------------
	void Control::PostOrderIterator::LoopThrough(Control *container)
	{
		controlStack.push_back(container);

		for (auto &control : make_reverse_range(container->internalControls))
		{
			if (control->GetVisible() && control->GetEnabled())
			{
				LoopThrough(control);
			}
		}
	}
	//---------------------------------------------------------------------------
	void Control::PostOrderIterator::operator++()
	{
		current = controlStack.back();
		controlStack.pop_back();
	}
	//---------------------------------------------------------------------------
	bool Control::PostOrderIterator::operator()()
	{
		return !controlStack.empty();
	}
	//---------------------------------------------------------------------------
	Control* Control::PostOrderIterator::operator*()
	{
		return current;
	}
	//---------------------------------------------------------------------------
}
