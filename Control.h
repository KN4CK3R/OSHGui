#ifndef __OSHGUI_CONTROL_H__
#define __OSHGUI_CONTROL_H__

#include <windows.h>

#include "Drawing\Point.h"
#include "Drawing\Size.h"
#include "Drawing\Rectangle.h"
#include "Drawing\IRenderer.h"

#include "Misc\List.h"

#include "Event.h"
#include "MouseEvent.h"
#include "KeyboardEvent.h"
#include "SystemEvent.h"

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
	class Panel;

	typedef void (*OnClickFunc)(Control *sender, MouseEvent *mouse);
	typedef void (*OnKeyPressFunc)(Control *sender, KeyboardEvent *keyboard);
	typedef void (*OnEnterFunc)(Control *sender);
	typedef void (*OnLeaveFunc)(Control *sender);
	typedef void (*OnChangeFunc)(Control *sender);

	class Control
	{
	public:
		Control(Panel *parentPanel = NULL);
		virtual ~Control();
		
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
		void SetBounds(Drawing::Rectangle &bounds);
		Drawing::Rectangle GetBounds();
		void SetLocation(Drawing::Point &point);
		Drawing::Point GetLocation();
		void SetSize(Drawing::Size &size);
		Drawing::Size GetSize();
		void SetTag(void *tag);
		void* GetTag();
		void SetFont(Drawing::IFont *font);
		Drawing::IFont* GetFont();
		void SetForeColor(Drawing::Color color);
		Drawing::Color GetForeColor();
		void SetBackColor(Drawing::Color color);
		Drawing::Color GetBackColor();
		void SetFont(Drawing::IFont *font);
		Drawing::IFont* GetFont();

		void SetOnClick(OnClickFunc clickFunc);
		void SetOnKeyPress(OnKeyPressFunc keyPressFunc);
		void SetOnEnter(OnEnterFunc enterFunc);
		void SetOnLeave(OnLeaveFunc leaveFunc);
		void SetOnChange(OnChangeFunc changeFunc);

		void Invalidate();
		
		virtual Event::NextEventTypes ProcessEvent(Event *event);
		virtual void Render(Drawing::IRenderer *renderer);
		
		Control *Parent;
		Panel *ParentPanel;
		
	protected:
		virtual void UpdateRects();
		
		CONTROL_TYPE type;
	
		bool enabled,
			 visible,
			 mouseOver,
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
					   backColor;
		
		Drawing::IFont *font;
		List<Drawing::ITexture*> texture;
	};
}

#endif