#include "Control.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	Control::Control(Panel *parentPanel)
	{
		type = CONTROL_BUTTON;

		ParentPanel = NULL;
	
		SetEnabled(true);
		SetVisible(true);
		SetBounds(Rectangle());
		
		SetTag(NULL);
		
		SetOnClick(NULL);
		SetOnKeyPress(NULL);
		SetOnEnter(NULL);
		SetOnLeave(NULL);
		SetOnChange(NULL);
		SetMouseOver(NULL);
		
		mouseOver = false;
		hasFocus = false;
	}
	//---------------------------------------------------------------------------
	Control::~Control()
	{
		/*for (int i = 0; i < Controls.GetSize(); i++)
		{
			delete Controls.Get(i);
		}
		Controls.Clear();*/
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
	void Control::SetBounds(Drawing::Rectangle &bounds)
	{
		this->bounds = bounds;
	}
	//---------------------------------------------------------------------------
	Drawing::Rectangle Control::GetBounds()
	{
		return bounds;
	}
	//---------------------------------------------------------------------------
	void Control::SetLocation(Drawing::Point &point)
	{
		bounds.SetLeft(point.Left);
		bounds.SetTop(point.Top);
	}
	//---------------------------------------------------------------------------
	Drawing::Point Control::GetLocation()
	{
		return bounds.GetPosition();
	}
	//---------------------------------------------------------------------------
	void Control::SetSize(Drawing::Size &size)
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
	void SetFont(Drawing::IFont *font)
	{
		if (font != NULL)
		{
			this->font = font;
		}
	}
	//---------------------------------------------------------------------------
	Drawing::IFont* GetFont()
	{
		return font;
	}
	//---------------------------------------------------------------------------
	void SetForeColor(Drawing::Color color)
	{
		foreColor = color;
	}
	//---------------------------------------------------------------------------
	Drawing::Color GetForeColor()
	{
		return foreColor;
	}
	//---------------------------------------------------------------------------
	void SetBackColor(Drawing::Color color)
	{
		backColor = color;
	}
	//---------------------------------------------------------------------------
	Drawing::Color GetBackColor()
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
	void Control::Invalidate()
	{
		needRepaint = true;
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