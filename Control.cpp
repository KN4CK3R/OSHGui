#include "Control.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	Control::Control(Control *parent)
	{
		type = CONTROL_BUTTON;

		Parent = parent;
		
		SetEnabled(true);
		SetVisible(true);
		
		SetTag(NULL);
				
		mouseOver = false;
		hasFocus = false;

		focusControl = NULL;
		mouseOverControl = NULL;
		captureControl = NULL;

		font = Drawing::IRenderer::GetDefaultFont();
		
		adjustColor = Drawing::Color(0, 20, 20, 20);
	}
	//---------------------------------------------------------------------------
	Control::~Control()
	{
		for (unsigned int i = 0, len = controls.size(); i < len; i++)
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
	bool Control::GetEnabled()
	{
		return enabled;
	}
	//---------------------------------------------------------------------------
	void Control::SetVisible(bool visible)
	{
		this->visible = visible;
	}
	//---------------------------------------------------------------------------
	bool Control::GetVisible()
	{
		return visible;
	}
	//---------------------------------------------------------------------------
	void Control::SetAutoSize(bool autoSize)
	{
		this->autoSize = autoSize;
		Invalidate();
	}
	//---------------------------------------------------------------------------
	bool Control::GetAutoSize()
	{
		return autoSize;
	}
	//---------------------------------------------------------------------------
	void Control::SetBounds(int x, int y, int w, int h)
	{
		SetBounds(Drawing::Rectangle(x, y, w, h));
	}
	//---------------------------------------------------------------------------
	void Control::SetBounds(Drawing::Rectangle &bounds)
	{
		this->bounds = bounds;
		Invalidate();
	}
	//---------------------------------------------------------------------------
	Drawing::Rectangle Control::GetBounds()
	{
		return bounds;
	}
	//---------------------------------------------------------------------------
	void Control::SetLocation(int x, int y)
	{
		SetLocation(Drawing::Point(x, y));
	}
	//---------------------------------------------------------------------------
	void Control::SetLocation(Drawing::Point &point)
	{
		bounds.SetLeft(point.Left);
		bounds.SetTop(point.Top);
		Invalidate();
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
	Drawing::Point Control::PointToClient(const Drawing::Point &point)
	{
		return Drawing::Point(point.Left - bounds.GetLeft(), point.Top - bounds.GetTop());
	}
	//---------------------------------------------------------------------------
	Drawing::Point Control::PointToScreen(const Drawing::Point &point)
	{
		return Drawing::Point();
	}
	//---------------------------------------------------------------------------
	int Control::GetHeight()
	{
		return bounds.GetHeight();
	}
	//---------------------------------------------------------------------------
	void Control::SetTag(void *tag)
	{
		this->tag = tag;
	}
	//---------------------------------------------------------------------------
	void* Control::GetTag()
	{
		return tag;
	}
	//---------------------------------------------------------------------------
	void Control::SetName(Misc::UnicodeString &name)
	{
		this->name = name;
	}
	//---------------------------------------------------------------------------
	Misc::UnicodeString& Control::GetName()
	{
		return name;
	}
	//---------------------------------------------------------------------------
	void Control::SetForeColor(Drawing::Color color)
	{
		foreColor = color;
	}
	//---------------------------------------------------------------------------
	Drawing::Color Control::GetForeColor()
	{
		return foreColor;
	}
	//---------------------------------------------------------------------------
	void Control::SetBackColor(Drawing::Color color)
	{
		backColor = color;
	}
	//---------------------------------------------------------------------------
	Drawing::Color Control::GetBackColor()
	{
		return backColor;
	}
	//---------------------------------------------------------------------------
	void Control::SetFont(Drawing::IFont *font)
	{
		if (font != NULL)
		{
			this->font = font;
		}
	}
	//---------------------------------------------------------------------------
	Drawing::IFont* Control::GetFont()
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
	ChangeEventHandler& Control::GetChangeEventHandler()
	{
		return changeEventHandler;
	}
	//---------------------------------------------------------------------------
	Control* Control::GetParent()
	{
		return Parent;
	}
	//---------------------------------------------------------------------------
	const std::vector<Control*>& Control::GetControls()
	{
		return controls;
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool Control::CanHaveFocus()
	{
		return false;
	}
	//---------------------------------------------------------------------------
	bool Control::ContainsPoint(const Drawing::Point &point)
	{
		return false;
	}
	//---------------------------------------------------------------------------
	void Control::Invalidate()
	{
		return;
	}
	//---------------------------------------------------------------------------
	void Control::UpdateRects()
	{
		return;
	}
	//---------------------------------------------------------------------------
	void Control::OnFocusIn()
	{
		hasFocus = true;
	}
	//---------------------------------------------------------------------------
	void Control::OnFocusOut()
	{
		hasFocus = false;
	}
	//---------------------------------------------------------------------------
	void Control::OnMouseEnter()
	{
		mouseOver = true;
	}
	//---------------------------------------------------------------------------
	void Control::OnMouseLeave()
	{
		mouseOver = false;
	}
	//---------------------------------------------------------------------------
	void Control::AddControl(Control *control)
	{
		if (control != NULL)
		{
			controls.push_back(control);
		}
	}
	//---------------------------------------------------------------------------
	void Control::InvalidateChildren()
	{
		for (unsigned int i = 0; i < controls.size(); i++)
		{
			Control *control = controls.at(i);

			if (control == NULL)
			{
				continue;
			}

			control->Invalidate();
		}
	}
	//---------------------------------------------------------------------------
	Control* Control::FindControlAtPoint(const Drawing::Point &point)
	{
		for (unsigned int i = 0; i < controls.size(); i++)
		{
			Control *control = controls.at(i);

			if (control == NULL)
			{
				continue;
			}

			if (control->GetEnabled() && control->GetVisible() && control->ContainsPoint(point))
			{
				return control;
			}
		}

		return NULL;
	}
	//---------------------------------------------------------------------------
	Control* Control::FindControlByName(const Misc::UnicodeString &name)
	{
		for (unsigned int i = 0; i < controls.size(); i++)
		{
			Control *control = controls.at(i);

			if (control == NULL)
			{
				continue;
			}
			
			if (control->GetName() == name)
			{
				return control;
			}
		}

		return NULL;
	}
	//---------------------------------------------------------------------------
	void Control::RequestFocus(Control *control)
	{
		if (control == NULL || !control->CanHaveFocus())
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

		if (baseParent->focusControl != NULL)
		{
			baseParent->focusControl->OnFocusOut();
		}
		
		control->OnFocusIn();
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

		if (baseParent->focusControl != NULL)
		{
			baseParent->focusControl->OnFocusOut();
			baseParent->focusControl = NULL;
		}
	}
	//---------------------------------------------------------------------------
	void Control::CaptureMouse(Control *control)
	{
		captureControl = control;
	}
	//---------------------------------------------------------------------------
	void Control::ReleaseCapture()
	{
		captureControl = NULL;
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
		if (event == NULL)
		{
			return Event::DontContinue;
		}
		
		//someone is focused, so let him handle the event expect the mouse
		if (event->Type != Event::Mouse && focusControl != NULL && focusControl->GetVisible() && focusControl->GetEnabled())
		{
			if (focusControl->ProcessEvent(event) == Event::DontContinue)
			{
				return Event::DontContinue;
			}
		}		
			
		if (event->Type == Event::Mouse)
		{
			MouseEvent *mouse = (MouseEvent*)event;
			
			//someone is capturing the mouse
			if (captureControl != NULL)
			{
				if (captureControl->ProcessEvent(mouse) == Event::DontContinue)
				{
					return Event::DontContinue;
				}
			}
			
			//find mouseOverControl
			Control *control = FindControlAtPoint(mouse->Position);
			if (control != mouseOverControl && mouseOverControl != NULL)
			{
				mouseOverControl->OnMouseLeave();
			}

			if (control != NULL)
			{
				mouseOverControl = control;
				mouseOverControl->OnMouseEnter();
			}
			
			//someone is focused
			if (focusControl != NULL && focusControl->GetEnabled())
			{
				if (focusControl->ProcessEvent(mouse) == Event::DontContinue)
				{
					return Event::DontContinue;
				}
			}
			
			//let mouseOverControl handle the mouse
			if (mouseOverControl != NULL)
			{
				if (mouseOverControl->ProcessEvent(event) == Event::DontContinue)
				{
					return Event::DontContinue;
				}
			}
		}
		else if (event->Type == Event::System)
		{
			SystemEvent *system = (SystemEvent*)event;
			if (system->Type == SystemEvent::ActivateApp)
			{
				if (focusControl != NULL && focusControl->GetVisible() && focusControl->GetEnabled())
				{
					if (system->Value)
					{
						focusControl->OnFocusIn();
					}
					else
					{
						focusControl->OnFocusOut();
					}
					
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