#ifndef __OSHGUI_CONTROL_H__
#define __OSHGUI_CONTROL_H__

#include <vector>
#include <algorithm>

#include "Application.h"

#include "Drawing\Point.h"
#include "Drawing\Size.h"
#include "Drawing\Rectangle.h"
#include "Drawing\IRenderer.h"

#include "Misc\Strings.h"
#include "Misc\TimeHelper.h"

#include "EventHandler.h"

namespace OSHGui
{
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

	/**
	 * Diese Klasse repräsentiert ein Steuerelement.
	 */
	class Control
	{
	public:
		/**
		 * Konstruktor der Klasse.
		 *
		 * @param parent das Elternsteuerelement
		 */
		Control(Control *parent = NULL);
		virtual ~Control();
		
		/**
		 * Gibt den CONTROL_TYPE des Steuerelemts zurück.
		 *
		 * @return der Typ
		 */
		CONTROL_TYPE GetType() const;
		
		/**
		 * Überprüft, ob das Steuerelement den Fokus übernehmen kann.
		 *
		 * @return ja / nein
		 */
		virtual bool CanHaveFocus();
		/**
		 * Überprüft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * @param point
		 * @return ja / nein
		 */
		virtual bool ContainsPoint(const Drawing::Point &point);
		
		/**
		 * Setzt den enabled-Status. Falls ein Steuerelement disabled ist, bearbeitet es keine Events.
		 *
		 * @param enabled
		 */
		void SetEnabled(bool enabled);
		bool GetEnabled();
		/**
		 * Setzt den visible-Status. Falls ein Steuerelement nicht sichtbar ist,
		 * bearbeitet es keine Events und wird nicht gezeichnet.
		 *
		 * @param visible
		 */
		void SetVisible(bool visible);
		bool GetVisible();
		
		/**
		 * Gibt an, ob ein Steuerelement seine Größe automatisch seinem Inhalt anpasst.
		 *
		 * @param autoSize
		 */
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
		/**
		 * Wandelt den Punkt in ClientKoordinaten um.
		 *
		 * @param point
		 * @return der neue Punkt
		 */
		virtual Drawing::Point PointToClient(const Drawing::Point &point);
		virtual Drawing::Point PointToScreen(const Drawing::Point &point);
		
		/**
		 * Legt die mit dem Steuerelement verknüpften benutzerdefinierten Daten fest.
		 *
		 * @param tag
		 */
		void SetTag(void *tag);
		void* GetTag();
		
		/**
		 * Legt den zum Identifizieren des Steuerelements verwendeten Namen fest.
		 *
		 * @param name
		 */
		void SetName(Misc::UnicodeString &name);
		Misc::UnicodeString& GetName();
		
		/**
		 * Legt die Schriftart des Texts im Steuerelement fest.
		 *
		 * @param font
		 */
		void SetFont(Drawing::IFont *font);
		Drawing::IFont* GetFont();
		/**
		 * Legt die Fordergrundfarbe des Steuerelements fest.
		 *
		 * @param color
		 */
		void SetForeColor(Drawing::Color color);
		Drawing::Color& GetForeColor();
		/**
		 * Legt die Hintergrundfarbe des Steuerelements fest.
		 *
		 * @param color
		 */
		void SetBackColor(Drawing::Color color);
		Drawing::Color& GetBackColor();
		/**
		 * Legt die Farbe für das fokusierte Steuerelement fest.
		 *
		 * @param color
		 */
		void SetMouseOverFocusColor(Drawing::Color color);
		Drawing::Color& GetMouseOverFocusColor();

		ClickEventHandler& GetClickEventHandler();
		KeyPressEventHandler& GetKeyPressEventHandler();
		ChangeEventHandler& GetChangeEventHandler();
		MouseEnterEventHandler& GetMouseEnterEventHandler();
		MouseLeaveEventHandler& GetMouseLeaveEventHandler();
		FocusInEventHandler& GetFocusInEventHandler();
		FocusOutEventHandler& GetFocusOutEventHandler();

		/**
		 * Veranlasst das Steuerelemt seine interne Struktur neu zu berechnen.
		 * Wird außerdem für alle Kindelemente aufgerufen.
		 *
		 * Sollte nicht vom Benutzer aufgerufen werden!
		 */
		virtual void Invalidate();
		void InvalidateChildren();

		/**
		 * Fügt ein untergeordnetes Steuerelement hinzu.
		 *
		 * @param control
		 */
		void AddControl(Control *control);

		void RequestFocus(Control *control);
		void ClearFocus();
		void CaptureMouse(Control *control);
		void ReleaseCapture();
		
		Control* FindControlAtPoint(const Drawing::Point &point);
		Control* FindControlByName(const Misc::UnicodeString &name);
		
		/**
		 * Verarbeitet ein Event und gibt es wenn nötig an Kindelemente weiter.
		 *
		 * @param event
		 * @return NextEventTypes
		 */
		virtual Event::NextEventTypes ProcessEvent(Event *event);
		/**
		 * Wird von ProcessEvent verwendet, um Kindelemete Events verarbeiten zu lassen.
		 *
		 * @param event
		 * @return NextEventTypes
		 */
		Event::NextEventTypes ProcessChildrenEvent(Event *event);
		/**
		 * Zeichnet das Steuerelement mithilfe des übergebenen IRenderers.
		 *
		 * @param renderer
		 */
		virtual void Render(Drawing::IRenderer *renderer);
		
		/**
		 * Gibt das übergeordnete Steuerelement zurück.
		 *
		 * @return parent
		 */
		Control* GetParent();
		/**
		 * Gibt eine Liste der untergeordneten Steuerelemente zurück.
		 *
		 * @return parent
		 */
		const std::vector<Control*>& GetControls();
		
	protected:
		Event::NextEventTypes ContainerProcessEvent(Event *event);

		virtual void SetFocus(bool focus);
		
		CONTROL_TYPE type;
		
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

		std::vector<Control*> controls;

		Control *focusControl,
				*captureControl,
				*mouseOverControl;
	};
}

#endif