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
	/**
	 * Enum �ber die Arten von Steuerelementen.
	 */
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

	/**
	 * Tritt beim Klicken auf das Steuerelement ein.
	 */
	typedef void (*OnClickFunc)(Control *sender, MouseEvent *mouse);
	/**
	 * Tritt ein, wenn eine Taste gedr�ckt wird, w�hrend das Steuerelement den Fokus hat.
	 */
	typedef void (*OnKeyPressFunc)(Control *sender, KeyboardEvent *keyboard);
	/**
	 * Tritt ein, wenn der Mauszeiger den Bereich des Steuerelements betritt.
	 */
	typedef void (*OnEnterFunc)(Control *sender);
	/**
	 * Tritt ein, wenn der Mauszeiger den Bereich des Steuerelements verl�sst.
	 */
	typedef void (*OnLeaveFunc)(Control *sender);
	/**
	 * Tritt ein, wenn der Wert des Steuerelements ver�ndert wird.
	 */
	typedef void (*OnChangeFunc)(Control *sender);

	/**
	 * Diese Klasse repr�sentiert ein Steuerelement.
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
		 * Gibt den CONTROL_TYPE des Steuerelemts zur�ck.
		 *
		 * @return der Typ
		 */
		CONTROL_TYPE GetType() const;
		
		/**
		 * �berpr�ft, ob das Steuerelement den Fokus �bernehmen kann.
		 *
		 * @return ja / nein
		 */
		virtual bool CanHaveFocus();
		/**
		 * �berpr�ft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * @param point
		 * @return ja / nein
		 */
		virtual bool ContainsPoint(const Drawing::Point &point);
		
		/**
		 * Legt fest, ob das Steuerlement auf Benutzerinteraktionen reagieren kann.
		 *
		 * @param enabled
		 */
		void SetEnabled(bool enabled);
		/**
		 * Ruft ab, ob das Steuerlement auf Benutzerinteraktionen reagieren kann.
		 *
		 * @return enabled
		 */
		bool GetEnabled();
		/**
		 * Legt fest, ob das Steuerelement und alle untergeordneten Steuerelemente
		 * angezeigt werden.
		 *
		 * @param visible
		 */
		void SetVisible(bool visible);
		/**
		 * Ruft ab, ob das Steuerelement und alle untergeordneten Steuerelemente
		 * angezeigt werden.
		 *
		 * @return visible
		 */
		bool GetVisible();
		
		/**
		 * Legt fest, ob die Gr��e des Steuerelements automatisch an dessen Inhalt anpasst.
		 *
		 * @param autoSize
		 */
		virtual void SetAutoSize(bool autoSize);
		/**
		 * Ruft ab, ob die Gr��e des Steuerelements automatisch an dessen Inhalt anpasst.
		 *
		 * @return autoSize
		 */
		virtual bool GetAutoSize();
		/**
		 * Legt die Gr��e und Position des Steuerelements relativ zum �bergeordneten
		 * Steuerelement fest.
		 *
		 * @param bounds
		 */
		virtual void SetBounds(Drawing::Rectangle &bounds);
		/**
		 * Legt die Gr��e und Position des Steuerelements relativ zum �bergeordneten
		 * Steuerelement fest.
		 *
		 * @param location
		 * @param size
		 */
		virtual void SetBounds(Drawing::Point location, Drawing::Size &size);
		/**
		 * Legt die Gr��e und Position des Steuerelements relativ zum �bergeordneten
		 * Steuerelement fest.
		 *
		 * @param x
		 * @param y
		 * @param w
		 * @param z
		 */
		virtual void SetBounds(int x, int y, int w, int h);
		/**
		 * Ruft die Gr��e und Position des Steuerelements relativ zum �bergeordneten
		 * Steuerelement ab.
		 *
		 * @return bounds
		 */
		virtual Drawing::Rectangle GetBounds();
		/**
		 * Legt die Koordinaten der linken oberen Ecke des Steuerelements relativ zur
		 * linken oberen Ecke des Containers fest.
		 *
		 * @param location
		 */
		virtual void SetLocation(Drawing::Point &location);
		/**
		 * Legt die Koordinaten der linken oberen Ecke des Steuerelements relativ zur
		 * linken oberen Ecke des Containers fest.
		 *
		 * @param x
		 * @param y
		 */
		virtual void SetLocation(int x, int y);
		/**
		 * Ruft die Koordinaten der linken oberen Ecke des Steuerelements relativ zur
		 * linken oberen Ecke des Containers ab.
		 *
		 * @return location
		 */
		virtual Drawing::Point GetLocation();
		/**
		 * Legt die H�he und Breite des Steuerelements fest.
		 *
		 * @param size
		 */
		virtual void SetSize(const Drawing::Size &size);
		/**
		 * Ruft die H�he und Breite des Steuerelements ab.
		 *
		 * @return size
		 */
		virtual Drawing::Size GetSize();
		/**
		 * Ruft den Abstand zwischen dem linken Rand des Steuerelements und dem linken
		 * Rand des Clientbereichs des zugeh�rigen Containers ab.
		 *
		 * @return left
		 */
		virtual int GetLeft();
		/**
		 * Ruft den Abstand zwischen dem oberen Rand des Steuerelements und dem oberen
		 * Rand des Clientbereichs des zugeh�rigen Containers ab.
		 *
		 * @return top
		 */
		virtual int GetTop();
		/**
		 * Ruft den Abstand zwischen dem rechten Rand des Steuerelements und dem linken
		 * Rand des Clientbereichs des zugeh�rigen Containers ab.
		 *
		 * @return right
		 */
		virtual int GetRight();
		/**
		 * Ruft den Abstand zwischen dem unteren Rand des Steuerelements und dem oberen
		 * Rand des Clientbereichs des zugeh�rigen Containers ab.
		 *
		 * @return bottom
		 */
		virtual int GetBottom();
		/**
		 * Ruft die Breite des Steuerelements ab.
		 *
		 * @return width
		 */
		virtual int GetWidth();
		/**
		 * Ruft die H�he des Steuerelements ab.
		 *
		 * @return height
		 */
		virtual int GetHeight();
		
		/**
		 * Rechnet die Position des angegeben Bildschirmpunkts in Clientkoordinaten um.
		 *
		 * @param point
		 * @return der neue Punkt
		 */
		virtual Drawing::Point PointToClient(const Drawing::Point &point);
		/**
		 * Rechnet die Position des angegeben Clientpunkts in Bildschirmkoordinaten um.
		 *
		 * @param point
		 * @return der neue Punkt
		 */
		virtual Drawing::Point PointToScreen(const Drawing::Point &point);
		
		/**
		 * Legt die mit dem Steuerelement verkn�pften benutzerdefinierten Daten fest.
		 *
		 * @param tag
		 */
		void SetTag(void *tag);
		/**
		 * Gibt die mit dem Steuerelement verkn�pften benutzerdefinierten Daten zur�ck.
		 *
		 * @return tag
		 */
		void* GetTag();
		
		/**
		 * Legt den zum Identifizieren des Steuerelements verwendeten Namen fest.
		 *
		 * @param name
		 */
		void SetName(Misc::UnicodeString &name);
		/**
		 * Gibt den zum Identifizieren des Steuerelements verwendeten Namen zur�ck.
		 *
		 * @return name
		 */
		Misc::UnicodeString& GetName();
		
		/**
		 * Legt die Schriftart des Texts im Steuerelement fest.
		 *
		 * @param font
		 */
		void SetFont(Drawing::IFont *font);
		/**
		 * Gibt die Schriftart des Texts im Steuerelement zur�ck.
		 *
		 * @return font
		 */
		Drawing::IFont* GetFont();
		
		/**
		 * Legt die Fordergrundfarbe des Steuerelements fest.
		 *
		 * @param color
		 */
		void SetForeColor(Drawing::Color color);
		/**
		 * Gibt die Fordergrundfarbe des Steuerelements zur�ck.
		 *
		 * @return color
		 */
		Drawing::Color& GetForeColor();
		/**
		 * Legt die Hintergrundfarbe des Steuerelements fest.
		 *
		 * @param color
		 */
		void SetBackColor(Drawing::Color color);
		/**
		 * Gibt die Hintergrundfarbe des Steuerelements zur�ck.
		 *
		 * @return color
		 */
		Drawing::Color& GetBackColor();
		/**
		 * Legt die Farbe f�r das fokusierte Steuerelement fest.
		 *
		 * @param color
		 */
		void SetMouseOverFocusColor(Drawing::Color color);
		/**
		 * Gibt die Farbe f�r das fokusierte Steuerelement zur�ck.
		 *
		 * @return color
		 */
		Drawing::Color& GetMouseOverFocusColor();

		/**
		 * Ruft den ClickEventHandler f�r das Steuerelement ab.
		 *
		 * @return clickEventHandler
		 */
		ClickEventHandler& GetClickEventHandler();
		/**
		 * Ruft den KeyPressEventHandler f�r das Steuerelement ab.
		 *
		 * @return keyPressEventHandler
		 */
		KeyPressEventHandler& GetKeyPressEventHandler();
		/**
		 * Ruft den ChangeEventHandler f�r das Steuerelement ab.
		 *
		 * @return changeEventHandler
		 */
		ChangeEventHandler& GetChangeEventHandler();
		/**
		 * Ruft den MouseEnterEventHandler f�r das Steuerelement ab.
		 *
		 * @return mouseEnterEventHandler
		 */
		MouseEnterEventHandler& GetMouseEnterEventHandler();
		/**
		 * Ruft den MouseLeaveEventHandler f�r das Steuerelement ab.
		 *
		 * @return mouseLeaveEventHandler
		 */
		MouseLeaveEventHandler& GetMouseLeaveEventHandler();
		/**
		 * Ruft den FocusInEventHandler f�r das Steuerelement ab.
		 *
		 * @return forcusInEventHandler
		 */
		FocusInEventHandler& GetFocusInEventHandler();
		/**
		 * Ruft den FocusOutEventHandler f�r das Steuerelement ab.
		 *
		 * @return forcusOutEventHandler
		 */
		FocusOutEventHandler& GetFocusOutEventHandler();

		/**
		 * Veranlasst das Steuerelemt seine interne Struktur neu zu berechnen.
		 * Wird au�erdem f�r alle Kindelemente aufgerufen.
		 *
		 * Sollte nicht vom Benutzer aufgerufen werden!
		 */
		virtual void Invalidate();
		/**
		 * Ruft Invalidate f�r alle Kindelemente auf.
		 *
		 * Sollte nicht vom Benutzer aufgerufen werden!
		 */
		void InvalidateChildren();

		/**
		 * F�gt ein untergeordnetes Steuerelement hinzu.
		 *
		 * @param control
		 */
		void AddControl(Control *control);

		void RequestFocus(Control *control);
		void ClearFocus();
		void CaptureMouse(Control *control);
		void ReleaseCapture();
		
		/**
		 * Ruft das untergeordnete Steuerelement ab, das sich an den angegebenen
		 * Koordinaten befindet.
		 *
		 * @param point
		 * @return NULL, falls sich dort kein Steuerelement befindet
		 */
		Control* GetChildAtPoint(const Drawing::Point &point);
		/**
		 * Ruft das untergeordnete Steuerelement mit dem entsprechenden Namen ab.
		 *
		 * @param name
		 * @return NULL, falls kein Steuerelement mit diesem Namen existiert
		 */
		Control* GetChildByName(const Misc::UnicodeString &name);
		
		/**
		 * Verarbeitet ein Event und gibt es wenn n�tig an Kindelemente weiter.
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
		 * Zeichnet das Steuerelement mithilfe des �bergebenen IRenderers.
		 *
		 * @param renderer
		 */
		virtual void Render(Drawing::IRenderer *renderer);
		
		/**
		 * Gibt das �bergeordnete Steuerelement zur�ck.
		 *
		 * @return parent
		 */
		Control* GetParent();
		/**
		 * Gibt eine Liste der untergeordneten Steuerelemente zur�ck.
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