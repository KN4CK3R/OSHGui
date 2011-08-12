#ifndef __OSHGUI_CONTROL_H__
#define __OSHGUI_CONTROL_H__

#include <vector>

#include "Drawing\Point.h"
#include "Drawing\Size.h"
#include "Drawing\Rectangle.h"
#include "Drawing\IRenderer.h"

#include "Misc\Strings.h"
#include "Misc\TimeHelper.h"

#include "EventHandler.h"

namespace OSHGui
{	
	#ifndef _max
		#define _max(a, b) (a > b ? a : b)
	#endif
	#ifndef _min
		#define _min(a, b) (a < b ? a : b)
	#endif

	enum CONTROL_TYPE
	{
		CONTROL_PANEL,
		CONTROL_FORM,
		CONTROL_GROUPBOX,
		CONTROL_LABEL,
		CONTROL_LINKLABEL,
		CONTROL_BUTTON,
		CONTROL_CHECKBOX,
		CONTROL_RADIOBUTTON,
		CONTROL_SCROLLBAR,
		CONTROL_LISTBOX,
		CONTROL_PROGRESSBAR,
		CONTROL_TRACKBAR,
		CONTROL_COMBOBOX,
		CONTROL_TEXTBOX
	};

	class Control;

	typedef void (*OnClickFunc)(Control *sender, MouseEvent *mouse);
	typedef void (*OnKeyPressFunc)(Control *sender, KeyboardEvent *keyboard);
	typedef void (*OnEnterFunc)(Control *sender);
	typedef void (*OnLeaveFunc)(Control *sender);
	typedef void (*OnChangeFunc)(Control *sender);

	class Control
	{
	public:
		Control(Control *parent = NULL);
		virtual ~Control();
		
		CONTROL_TYPE GetType() const;
		
		virtual bool CanHaveFocus();
		virtual bool ContainsPoint(const Drawing::Point &point);
		
		void SetEnabled(bool enabled);
		bool GetEnabled();
		void SetVisible(bool visible);
		bool GetVisible();
		
		virtual void SetAutoSize(bool autoSize);
		virtual bool GetAutoSize();
		virtual void SetBounds(int x, int y, int w, int h);
		virtual void SetBounds(Drawing::Rectangle &bounds);
		virtual Drawing::Rectangle GetBounds();
		virtual void SetLocation(Drawing::Point &point);
		virtual void SetLocation(int x, int y);
		virtual Drawing::Point GetLocation();
		virtual void SetSize(const Drawing::Size &size);
		virtual Drawing::Size GetSize();
		virtual int GetLeft();
		virtual int GetTop();
		virtual int GetRight();
		virtual int GetBottom();
		virtual int GetWidth();
		virtual int GetHeight();
		virtual Drawing::Point PointToClient(const Drawing::Point &point);
		virtual Drawing::Point PointToScreen(const Drawing::Point &point);
		
		void SetTag(void *tag);
		void* GetTag();
		
		void SetName(Misc::UnicodeString &name);
		Misc::UnicodeString& GetName();
		
		void SetFont(Drawing::IFont *font);
		Drawing::IFont* GetFont();
		void SetForeColor(Drawing::Color color);
		Drawing::Color& GetForeColor();
		void SetBackColor(Drawing::Color color);
		Drawing::Color& GetBackColor();
		void SetMouseOverFocusColor(Drawing::Color color);
		Drawing::Color& GetMouseOverFocusColor();

		ClickEventHandler& GetClickEventHandler();
		KeyPressEventHandler& GetKeyPressEventHandler();
		ChangeEventHandler& GetChangeEventHandler();
		MouseEnterEventHandler& GetMouseEnterEventHandler();
		MouseLeaveEventHandler& GetMouseLeaveEventHandler();
		FocusInEventHandler& GetFocusInEventHandler();
		FocusOutEventHandler& GetFocusOutEventHandler();

		virtual void Invalidate();
		void InvalidateChildren();

		void AddControl(Control *control);

		void RequestFocus(Control *control);
		void ClearFocus();
		void CaptureMouse(Control *control);
		void ReleaseCapture();
		
		Control* FindControlAtPoint(const Drawing::Point &point);
		Control* FindControlByName(const Misc::UnicodeString &name);
		
		virtual Event::NextEventTypes ProcessEvent(Event *event);
		Event::NextEventTypes ProcessChildrenEvent(Event *event);
		virtual void Render(Drawing::IRenderer *renderer);
		
		Control* GetParent();
		const std::vector<Control*>& GetControls();
		
	protected:
		CONTROL_TYPE type;

		virtual void SetFocus(bool focus);
		
		Misc::UnicodeString name;
	
		bool enabled,
			 needRepaint,
			 visible,
			 mouseOver,
			 hasFocus,
			 autoSize;
			 
		void *tag;
		
		
		Drawing::Rectangle bounds,
						   clientArea;
		
		ClickEventHandler clickEventHandler;
		KeyPressEventHandler keyPressEventHandler;
		ChangeEventHandler changeEventHandler;
		MouseEnterEventHandler mouseEnterEventHandler;
		MouseLeaveEventHandler mouseLeaveEventHandler;
		FocusInEventHandler focusInEventHandler;
		FocusOutEventHandler focusOutEventHandler;

		OnClickFunc clickFunc;
		OnKeyPressFunc keyPressFunc;
		OnEnterFunc enterFunc;
		OnLeaveFunc leaveFunc;
		OnChangeFunc changeFunc;
		
		Drawing::Color foreColor,
					   backColor,
					   mouseOverFocusColor;
		
		Drawing::IFont *font;

		Control *Parent;

		Event::NextEventTypes ContainerProcessEvent(Event *event);

		std::vector<Control*> controls;

		Control *focusControl,
				*captureControl,
				*mouseOverControl;

	public:
		//Misc::List<Drawing::ITexture*> texture;
	};
}

#endif