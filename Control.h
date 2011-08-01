#ifndef __OSHGUI_CONTROL_H__
#define __OSHGUI_CONTROL_H__

#include <windows.h>

#include "Drawing\Point.h"
#include "Drawing\Size.h"
#include "Drawing\Rectangle.h"
#include "Drawing\IRenderer.h"

#include "Misc\List.h"
#include "Misc\Strings.h"

#include "Event.h"
#include "MouseEvent.h"
#include "KeyboardEvent.h"
#include "SystemEvent.h"

namespace OSHGui
{
	#ifdef UNICODE
		typedef Misc::String::UnicodeString String;
	#else
		typedef Misc::String::AnsiString String;
	#endif

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
		String s;
		
		CONTROL_TYPE GetType() const;
		
		virtual bool CanHaveFocus();
		virtual bool ContainsPoint(const Drawing::Point &point);
		virtual void OnFocusIn();
		virtual void OnFocusOut();
		virtual void OnMouseEnter();
		virtual void OnMouseLeave();
		
		void SetEnabled(bool enabled);
		bool GetEnabled();
		void SetVisible(bool visible);
		bool GetVisible();
		
		virtual void SetBounds(int x, int y, int w, int h);
		virtual void SetBounds(Drawing::Rectangle &bounds);
		virtual Drawing::Rectangle GetBounds();
		virtual void SetLocation(Drawing::Point &point);
		virtual void SetLocation(int x, int y);
		virtual Drawing::Point GetLocation();
		virtual void SetSize(Drawing::Size &size);
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
		
		void SetFont(Drawing::IFont *font);
		Drawing::IFont* GetFont();
		void SetForeColor(Drawing::Color color);
		Drawing::Color GetForeColor();
		void SetBackColor(Drawing::Color color);
		Drawing::Color GetBackColor();

		void SetOnClick(OnClickFunc clickFunc);
		void SetOnKeyPress(OnKeyPressFunc keyPressFunc);
		void SetOnEnter(OnEnterFunc enterFunc);
		void SetOnLeave(OnLeaveFunc leaveFunc);
		void SetOnChange(OnChangeFunc changeFunc);

		void Invalidate();

		bool IsMouseOver(Control *control);
		void RequestFocus(Control *control);
		void ClearFocus();
		void CaptureMouse(Control *control);
		void ReleaseCapture();
		
		virtual Event::NextEventTypes ProcessEvent(Event *event);
		virtual void Render(Drawing::IRenderer *renderer);
		
		Control* GetParent();
		
		Control *Parent;
		bool mouseOver;
		
	protected:
		virtual void UpdateRects();
		
		CONTROL_TYPE type;
	
		bool enabled,
			 visible,
			 //mouseOver,
			 hasFocus,
			 needRepaint;
			 
		void *tag;
		
		Drawing::Rectangle bounds;
		
		OnClickFunc clickFunc;
		OnKeyPressFunc keyPressFunc;
		OnEnterFunc enterFunc;
		OnLeaveFunc leaveFunc;
		OnChangeFunc changeFunc;
		
		Drawing::Color foreColor,
					   backColor,
					   adjustColor;
		
		Drawing::IFont *font;

		Control *focusControl,
				*captureControl,
				*mouseOverControl;
	public:
		Misc::List<Drawing::ITexture*> texture;
	};
}

#endif