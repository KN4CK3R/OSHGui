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
		
		SetOnClick(NULL);
		SetOnKeyPress(NULL);
		SetOnEnter(NULL);
		SetOnLeave(NULL);
		SetOnChange(NULL);
		//SetMouseOver(NULL);
		
		mouseOver = false;
		hasFocus = false;

		font = Drawing::IRenderer::GetDefaultFont();
		
		focusControl = NULL;
		mouseOverControl = NULL;
		captureControl = NULL;

		adjustColor = Drawing::Color(0, 20, 20, 20);
	}
	//---------------------------------------------------------------------------
	Control::~Control()
	{
		/*for (unisgned int i = 0; i < Controls.size(); ++i)
		{
			delete Controls.at(i);
		}
		Controls.clear();*/
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
		if (!enabled)
		{
			Parent->ClearFocus();
		}
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
	Control* Control::GetParent()
	{
		return Parent;
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
	void Control::SetOnClick(OnClickFunc clickFunc)
	{
		this->clickFunc = clickFunc;
	}
	//---------------------------------------------------------------------------
	void Control::SetOnKeyPress(OnKeyPressFunc keyPressFunc)
	{
		this->keyPressFunc = keyPressFunc;
	}
	//---------------------------------------------------------------------------
	void Control::SetOnEnter(OnEnterFunc enterFunc)
	{
		this->enterFunc = enterFunc;
	}
	//---------------------------------------------------------------------------
	void Control::SetOnLeave(OnLeaveFunc leaveFunc)
	{
		this->leaveFunc = leaveFunc;
	}
	//---------------------------------------------------------------------------
	void Control::SetOnChange(OnChangeFunc changeFunc)
	{
		this->changeFunc = changeFunc;
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
	bool Control::IsMouseOver(Control *control)
	{
		return control == mouseOverControl;
	}
	//---------------------------------------------------------------------------
	void Control::RequestFocus(Control *control)
	{
		if (focusControl == control)
		{
			return;
		}

		if (!control->CanHaveFocus())
		{
			return;
		}

		if (focusControl != NULL)
		{
			focusControl->OnFocusOut();
		}

		control->OnFocusIn();
		focusControl = control;
	}
	//---------------------------------------------------------------------------
	void Control::ClearFocus()
	{
		if (focusControl)
		{
			focusControl->OnFocusOut();
			focusControl = NULL;
		}

		ReleaseCapture();
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
		return Event::None;
	}
	//---------------------------------------------------------------------------
	void Control::Render(Drawing::IRenderer *renderer)
	{
		return;
	}
	//---------------------------------------------------------------------------
}