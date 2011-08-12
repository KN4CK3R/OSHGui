#ifndef __OSHGUI_TEXTBOX_H__
#define __OSHGUI_TEXTBOX_H__

#include "Control.h"
#include "Misc\TextHelper.h"

#define TEXTBOX_DEFAULT_HEIGHT 24

namespace OSHGui
{
	class TextBox : public Control
	{
	public:
		TextBox(Control *parent = NULL);
		virtual ~TextBox();
		
		void SetText(const Misc::UnicodeString &text);
		const Misc::UnicodeString& GetText();
		
		virtual bool CanHaveFocus();
		virtual bool ContainsPoint(const Drawing::Point &point);
		
		void ClearText();

		virtual void Invalidate();
		
		Event::NextEventTypes ProcessEvent(Event *event);
		void Render(Drawing::IRenderer *renderer);
	
	protected:	
		void PlaceCaret(int position);
		
		void PasteFromClipboard();
		
		Misc::TextHelper textHelper;
		
		Drawing::Rectangle textRect,
						   caretRect;
		
		double blinkTime;
		double lastBlink;
		int caretPosition;
		int firstVisibleCharacter;
	};
}

#endif