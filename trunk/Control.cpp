#include "Control.h"
#include <Windows.h>

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	Control::Control(Control *parent)
	{
		type = CONTROL_BUTTON;

		SetLocation(Drawing::Point(3, 3));

		Parent = parent;
		
		SetEnabled(true);
		SetVisible(true);
				
		mouseOver = false;
		hasFocus = false;

		focusControl = 0;
		mouseOverControl = 0;

		font = Application::Renderer->GetDefaultFont();
		
		mouseOverFocusColor = Drawing::Color(0, 20, 20, 20);
	}
	//---------------------------------------------------------------------------
	Control::~Control()
	{
		for (unsigned int i = 0; i < controls.size(); i++)
		{
			Control *control = controls.at(i);
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
	Drawing::Rectangle Control::GetBounds() const
	{
		return bounds;
	}
	//---------------------------------------------------------------------------
	void Control::SetLocation(int x, int y)
	{
		SetLocation(Drawing::Point(x, y));
	}
	//---------------------------------------------------------------------------
	void Control::SetLocation(const Drawing::Point &location)
	{
		bounds.SetLeft(location.Left);
		bounds.SetTop(location.Top);
		
		static bool isValid = true; //no recursive calls
		if (isValid)
		{
			isValid = false;
			Invalidate();
			isValid = true;
		}
	}
	//---------------------------------------------------------------------------
	Drawing::Point Control::GetLocation()
	{
		return bounds.GetPosition();
	}
	//---------------------------------------------------------------------------
	void Control::SetSize(const Drawing::Size &size)
	{
		if (size.Width > -1)
		{
			bounds.SetWidth(size.Width);
		}
		if (size.Height > -1)
		{
			bounds.SetHeight(size.Height);
		}

		static bool isValid = true; 
		if (isValid)
		{
			isValid = false;
			Invalidate();
			isValid = true;
		}
	}
	//---------------------------------------------------------------------------
	Drawing::Size Control::GetSize()
	{
		return bounds.GetSize();
	}
	//---------------------------------------------------------------------------
	int Control::GetLeft()
	{
		return bounds.GetLeft();
	}
	//---------------------------------------------------------------------------
	int Control::GetTop()
	{
		return bounds.GetTop();
	}
	//---------------------------------------------------------------------------
	int Control::GetRight()
	{
		return bounds.GetRight();
	}
	//---------------------------------------------------------------------------
	int Control::GetBottom()
	{
		return bounds.GetBottom();
	}
	//---------------------------------------------------------------------------
	int Control::GetWidth()
	{
		return bounds.GetWidth();
	}
	//---------------------------------------------------------------------------
	Drawing::Point Control::PointToClient(const Drawing::Point &point) const
	{
		return Drawing::Point(point.Left - bounds.GetLeft(), point.Top - bounds.GetTop());
	}
	//---------------------------------------------------------------------------
	Drawing::Point Control::PointToScreen(const Drawing::Point &point) const
	{
		return Drawing::Point();
	}
	//---------------------------------------------------------------------------
	int Control::GetHeight()
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
		if (font != 0)
		{
			this->font = font;
		}

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
	ClickEventHandler& Control::GetClickEventHandler()
	{
		return clickEventHandler;
	}
	//---------------------------------------------------------------------------
	KeyPressEventHandler& Control::GetKeyPressEventHandler()
	{
		return keyPressEventHandler;
	}
	//---------------------------------------------------------------------------
	MouseEnterEventHandler& Control::GetMouseEnterEventHandler()
	{
		return mouseEnterEventHandler;
	}
	//---------------------------------------------------------------------------
	MouseLeaveEventHandler& Control::GetMouseLeaveEventHandler()
	{
		return mouseLeaveEventHandler;
	}
	//---------------------------------------------------------------------------
	MouseClickEventHandler& Control::GetMouseClickEventHandler()
	{
		return mouseClickEventHandler;
	}
	//---------------------------------------------------------------------------
	Control* Control::GetParent() const
	{
		return Parent;
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
		if (control != 0)
		{
			controls.push_back(control);

			Invalidate();
		}
	}
	//---------------------------------------------------------------------------
	void Control::InvalidateChildren()
	{
		for (unsigned int i = 0; i < controls.size(); i++)
		{
			Control *control = controls.at(i);

			if (control == 0)
			{
				continue;
			}

			control->Invalidate();
		}
	}
	//---------------------------------------------------------------------------
	Control* Control::GetChildAtPoint(const Drawing::Point &point) const
	{
		for (unsigned int i = 1; i <= controls.size(); ++i)
		{
			Control *control = controls.at(controls.size() - i);

			if (control == 0)
			{
				continue;
			}

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
		for (unsigned int i = 0; i < controls.size(); i++)
		{
			Control *control = controls.at(i);

			if (control == 0)
			{
				continue;
			}
			
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
		if (control == 0 || !control->CanHaveFocus())
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
			baseParent->focusControl->focusOutEventHandler.Invoke(this);
		}
		
		control->SetFocus(true);
		control->focusInEventHandler.Invoke(this);
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
			baseParent->focusControl->focusOutEventHandler.Invoke(this);
			baseParent->focusControl = 0;
		}
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	Event::NextEventTypes Control::ProcessEvent(Event *event)
	{
		return Event::DontContinue;
	}
	//---------------------------------------------------------------------------
	Event::NextEventTypes Control::ProcessChildrenEvent(Event *event)
	{
		if (event == 0)
		{
			return Event::DontContinue;
		}
		
		//someone is focused, so let him handle the event expect the mouse
		if (event->Type != Event::Mouse && focusControl != 0 && focusControl->GetVisible() && focusControl->GetEnabled())
		{
			if (focusControl->ProcessEvent(event) == Event::DontContinue)
			{
				return Event::DontContinue;
			}
		}
		
		if (event->Type == Event::Mouse)
		{
			MouseEvent *mouse = (MouseEvent*)event;
			
			//find mouseOverControl
			Control *control = GetChildAtPoint(mouse->Position);
			if (control != mouseOverControl && mouseOverControl != 0)
			{
				mouseOverControl->mouseOver = false;
				mouseOverControl->mouseLeaveEventHandler.Invoke(this);
				mouseOverControl = 0;
			}

			if (control != 0)
			{
				mouseOverControl = control;
				mouseOverControl->mouseOver = true;
				mouseOverControl->mouseEnterEventHandler.Invoke(this);
			}
			
			//someone is focused
			if (focusControl != 0 && focusControl->GetEnabled())
			{
				if (focusControl->ProcessEvent(mouse) == Event::DontContinue)
				{
					return Event::DontContinue;
				}
			}
			
			//let mouseOverControl handle the mouse
			if (mouseOverControl != 0)
			{
				if (mouseOverControl->ProcessEvent(event) == Event::DontContinue)
				{
					return Event::DontContinue;
				}
			}
		}
		
		return Event::Continue;
	}
	//---------------------------------------------------------------------------
	void Control::Render(Drawing::IRenderer *renderer)
	{
		return;
	}
	//---------------------------------------------------------------------------
}