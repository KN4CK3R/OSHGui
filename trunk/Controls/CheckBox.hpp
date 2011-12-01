#ifndef OSHGUI_CHECKBOX_HPP_
#define OSHGUI_CHECKBOX_HPP_

#include "Control.hpp"
#include "Label.hpp"

namespace OSHGui
{
	/**
	 * Stellt ein CheckBox-Steuerelement dar.
	 */
	class OSHGUI_EXPORT CheckBox : public Control
	{
	public:
		/**
		 * Konstruktor der Klasse.
		 *
		 * @param name eindeutiger Name des Steuerelements
		 * @param location Position des Steuerelements
		 * @param size Gr��e des Steuerelements
		 * @param text Text des Steuerelemts
		 * @param checked Checked-Eigenschaft
		 */
		CheckBox(const Misc::AnsiString &name, const Drawing::Point &location, const Drawing::Size &size, const Misc::AnsiString &text, bool checked);
		
		/**
		 * Legt den checked-Status fest.
		 *
		 * @param checked
		 */
		virtual void SetChecked(bool checked);
		/**
		 * Gibt den checked-Status zur�ck.
		 *
		 * @return ja / nein
		 */
		virtual bool GetChecked() const;
		/**
		 * Legt den Text fest. Falls autoSize = true, wird die Gr��e automatisch angepasst.
		 *
		 * @param text
		 */
		void SetText(const Misc::AnsiString &text);
		/**
		 * Ruft den Text ab.
		 *
		 * @return der Text
		 */
		const Misc::AnsiString& GetText() const;
		/**
		 * Legt die Schriftart des Texts im Steuerelement fest.
		 *
		 * @param font
		 */
		void SetFont(const std::shared_ptr<Drawing::IFont> &font);
		/**
		 * Legt die Fordergrundfarbe des Steuerelements fest.
		 *
		 * @param color
		 */
		virtual void SetForeColor(Drawing::Color color);
		/**
		 * Ruft das CheckedChangeEvent f�r das Steuerelement ab.
		 *
		 * @return checkedChangeEvent
		 */
		CheckedChangedEvent& GetCheckedChangedEvent();
		
		/**
		 * �berpr�ft, ob das Steuerelement den Fokus �bernehmen kann.
		 *
		 * @return ja / nein
		 */
		virtual bool CanHaveFocus() const;
		
		/**
		 * Verarbeitet ein Event und gibt es wenn n�tig an Kindelemente weiter.
		 *
		 * @param event
		 * @return NextEventTypes
		 */
		//virtual bool ProcessEvent(IEvent *event);
		/**
		 * Zeichnet das Steuerelement mithilfe des �bergebenen IRenderers.
		 *
		 * @param renderer
		 */
		virtual void Render(Drawing::IRenderer *renderer);
	
	protected:
		static const Drawing::Size DefaultLabelOffset;

		virtual void OnMouseClick(const MouseMessage &mouse);
		virtual void OnKeyUp(const KeyboardMessage &keyboard);

		bool checked;
		Drawing::Point checkBoxPosition,
					   textPosition;
					   
		CheckedChangedEvent checkedChangedEvent;

		Label *label;
	};
}

#endif