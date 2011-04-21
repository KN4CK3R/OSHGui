#ifndef __OSHGUI_TEXTBOX_H__
#define __OSHGUI_TEXTBOX_H__

#include "Control.h"
#include "Misc\StringBuffer.h"

#define TEXTBOX_DEFAULT_HEIGHT 24

namespace OSHGui
{
	class TextBox : public Control
	{
	public:
		TextBox(Panel *parentPanel = NULL);
		virtual ~TextBox();
		
		void SetText(const char *text);
		const char* GetText();
		
		virtual bool CanHaveFocus();
		virtual bool ContainsPoint(const Drawing::Point &point);
		
		int GetTextLength();
		bool GetTextCopy(char *dest, int count);
		void ClearText();
		
		Event::NextEventTypes ProcessEvent(Event *event);
		void Render(Drawing::IRenderer *renderer);
	
	protected:
		virtual void UpdateRects();
	
		void PlaceCaret(int position);
		int CharacterToPosition(int charIndex);
		int PositionToCharacterIndex(int position);
		
		void PasteFromClipboard();
		
		Misc::StringBuffer buffer;
		
		Drawing::Rectangle textRect;
		
		double blinkTime;
		double lastBlink;
		int caretPosition;
		int firstVisibleCharacter;
	};
}

#endif