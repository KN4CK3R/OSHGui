/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_CONTROL_HPP
#define OSHGUI_CONTROL_HPP

#include <vector>

#include "../Exports.hpp"

#include "../Application.hpp"

#include "../Drawing/Point.hpp"
#include "../Drawing/Size.hpp"
#include "../Drawing/Rectangle.hpp"
#include "../Drawing/IRenderer.hpp"
#include "../Drawing/Theme.hpp"

#include "../Misc/Strings.hpp"
#include "../Misc/Any.hpp"

#include "../Event/KeyboardMessage.hpp"
#include "../Event/MouseMessage.hpp"
#include "../Event/Event.hpp"
#include "../Event/KeyEventArgs.hpp"
#include "../Event/KeyPressEventArgs.hpp"
#include "../Event/MouseEventArgs.hpp"
#include "../Event/ScrollEventArgs.hpp"

#include "../Cursor/Cursors.hpp"

#include "../Intersection.hpp"

namespace OSHGui
{
	/**
	 * Auflistung der Arten von Steuerelementen.
	 */
	const Misc::AnsiString CONTROL_PANEL = "panel";
	const Misc::AnsiString CONTROL_FORM = "form";
	const Misc::AnsiString CONTROL_GROUPBOX = "groupbox";
	const Misc::AnsiString CONTROL_LABEL = "label";
	const Misc::AnsiString CONTROL_LINKLABEL = "linklabel";
	const Misc::AnsiString CONTROL_BUTTON = "button";
	const Misc::AnsiString CONTROL_CHECKBOX = "checkbox";
	const Misc::AnsiString CONTROL_RADIOBUTTON = "radiobutton";
	const Misc::AnsiString CONTROL_SCROLLBAR = "scrollbar";
	const Misc::AnsiString CONTROL_LISTBOX = "listbox";
	const Misc::AnsiString CONTROL_PROGRESSBAR = "progressbar";
	const Misc::AnsiString CONTROL_TRACKBAR = "trackbar";
	const Misc::AnsiString CONTROL_COMBOBOX = "combobox";
	const Misc::AnsiString CONTROL_TEXTBOX = "textbox";
	const Misc::AnsiString CONTROL_TIMER = "timer";
	const Misc::AnsiString CONTROL_TABCONTROL = "tabcontrol";
	const Misc::AnsiString CONTROL_TABPAGE = "tabpage";
	const Misc::AnsiString CONTROL_PICTUREBOX = "picturebox";
	const Misc::AnsiString CONTROL_COLORPICKER = "colorpicker";
	const Misc::AnsiString CONTROL_COLORBAR = "colorbar";

	/**
	 * Auflistung der Anker.
	 */
	enum AnchorStyle
	{
		AnchorTop = 1,
		AnchorBottom = 2,
		AnchorLeft = 4,
		AnchorRight = 8
	};

	inline AnchorStyle operator|(AnchorStyle lhs, AnchorStyle rhs)
	{
		return static_cast<AnchorStyle>(static_cast<int>(lhs)|static_cast<int>(rhs));
	}
	inline AnchorStyle operator&(AnchorStyle lhs, AnchorStyle rhs)
	{
		return static_cast<AnchorStyle>(static_cast<int>(lhs)&static_cast<int>(rhs));
	}
	
	class Control;
	class ContainerControl;
	
	/**
	 * Tritt ein, wenn die Form geschlossen werden soll.
	 */
	typedef Event<void(Control*, bool &canClose)> FormClosingEvent;
	typedef EventHandler<void(Control*, bool &canClose)> FormClosingEventHandler;
	/**
	 * Tritt ein, wenn sich der Wert der SelectedIndex-Eigenschaft �ndert.
	 */
	typedef Event<void(Control*)> SelectedIndexChangedEvent;
	typedef EventHandler<void(Control*)> SelectedIndexChangedEventHandler;
	/**
	 * Tritt ein, wenn sich der Wert der Color-Eigenschaft �ndert.
	 */
	typedef Event<void(Control*, Drawing::Color &color)> ColorChangedEvent;
	typedef EventHandler<void(Control*, Drawing::Color &color)> ColorChangedEventHandler;
	/**
	 * Tritt ein, wenn sich der Wert der Checked-Eigenschaft �ndert.
	 */
	typedef Event<void(Control*)> CheckedChangedEvent;
	typedef EventHandler<void(Control*)> CheckedChangedEventHandler;
	/**
	 * Tritt ein, wenn sich der Wert der Text-Eigenschaft �ndert.
	 */
	typedef Event<void(Control*)> TextChangedEvent;
	typedef EventHandler<void(Control*)> TextChangedEventHandler;
	/**
	 * Tritt auf, wenn das angegebene Intervall verstrichen ist.
	 */
	typedef Event<void(Control*)> TickEvent;
	typedef EventHandler<void(Control*)> TickEventHandler;
	/**
	 * Tritt auf, wenn der TrackBar-Schieberegler verschoben wird.
	 */
	typedef Event<void(Control*)> ValueChangedEvent;
	typedef EventHandler<void(Control*)> ValueChangedEventHandler;
	/**
	 * Tritt auf, wenn der ScrollBalken verschoben wird.
	 */
	typedef Event<void(Control*, ScrollEventArgs&)> ScrollEvent;
	typedef EventHandler<void(Control*, ScrollEventArgs&)> ScrollEventHandler;
	/**
	 * Tritt ein, wenn die Location-Eigenschaft ge�ndert wird.
	 */
	typedef Event<void(Control*)> LocationChangedEvent;
	typedef EventHandler<void(Control*)> LocationChangedEventHandler;
	/**
	 * Tritt ein, wenn die Size-Eigenschaft ge�ndert wird.
	 */
	typedef Event<void(Control*)> SizeChangedEvent;
	typedef EventHandler<void(Control*)> SizeChangedEventHandler;
	/**
	 * Tritt auf, wenn eine Taste gedr�ckt wird.
	 */
	typedef Event<void(Control*, KeyEventArgs&)> KeyDownEvent;
	typedef EventHandler<void(Control*, KeyEventArgs&)> KeyDownEventHandler;
	/**
	 * Tritt auf, wenn das Steuerelement fokusiert ist und eine Taste gedr�ckt gehalten wird.
	 */
	typedef Event<void(Control*, KeyPressEventArgs&)> KeyPressEvent;
	typedef EventHandler<void(Control*, KeyPressEventArgs&)> KeyPressEventHandler;
	/**
	 * Tritt auf, wenn eine Taste losgelassen wird.
	 */
	typedef Event<void(Control*, KeyEventArgs&)> KeyUpEvent;
	typedef EventHandler<void(Control*, KeyEventArgs&)> KeyUpEventHandler;
	/**
	 * Tritt auf, wenn auf das Steuerelement geklickt wurde.
	 */
	typedef Event<void(Control*)> ClickEvent;
	typedef EventHandler<void(Control*)> ClickEventHandler;
	/**
	 * Tritt auf, wenn mit der Maus auf das Steuerelement geklickt wurde.
	 */
	typedef Event<void(Control*, MouseEventArgs&)> MouseClickEvent;
	typedef EventHandler<void(Control*, MouseEventArgs&)> MouseClickEventHandler;
	/**
	 * Tritt auf, wenn sich der Mauszeiger �ber dem Steuerlement befindet und eine Maustaste gedr�ckt wurde.
	 */
	typedef Event<void(Control*, MouseEventArgs&)> MouseDownEvent;
	typedef EventHandler<void(Control*, MouseEventArgs&)> MouseDownEventHandler;
	/**
	 * Tritt auf, wenn sich der Mauszeiger �ber dem Steuerlement befindet und eine Maustaste losgelassen wurde.
	 */
	typedef Event<void(Control*, MouseEventArgs&)> MouseUpEvent;
	typedef EventHandler<void(Control*, MouseEventArgs&)> MouseUpEventHandler;
	/**
	 * Tritt auf, wenn der Mauszeiger �ber das Steuerlement bewegt wird.
	 */
	typedef Event<void(Control*, MouseEventArgs&)> MouseMoveEvent;
	typedef EventHandler<void(Control*, MouseEventArgs&)> MouseMoveEventHandler;
	/**
	 * Tritt auf, wenn sich der Mauszeiger �ber dem Steuerlement befindet und eine Maustaste losgelassen wurde.
	 */
	typedef Event<void(Control*, MouseEventArgs&)> MouseScrollEvent;
	typedef EventHandler<void(Control*, MouseEventArgs&)> MouseScrollEventHandler;
	/**
	 * Tritt ein, wenn die Maus in den Bereich des Steuerlements bewegt wird.
	 */
	typedef Event<void(Control*)> MouseEnterEvent;
	typedef EventHandler<void(Control*)> MouseEnterEventHandler;
	/**
	 * Tritt ein, wenn die Maus den Bereich des Steuerlements verl�sst.
	 */
	typedef Event<void(Control*)> MouseLeaveEvent;
	typedef EventHandler<void(Control*)> MouseLeaveEventHandler;
	/**
	 * Tritt ein, nachdem sich die Mausaufzeichnung ge�ndert hat.
	 */
	typedef Event<void(Control*)> MouseCaptureChangedEvent;
	typedef EventHandler<void(Control*)> MouseCaptureChangedEventHandler;
	/**
	 * Tritt auf, wenn das Steuerelement zu dem aktiven Steuerelement auf dem Formular wird.
	 */
	typedef Event<void(Control*)> FocusGotEvent;
	typedef EventHandler<void(Control*)> FocusGotEventHandler;
	/**
	 * Tritt auf, wenn das Steuerelement nicht mehr das aktive Steuerelement auf dem Formular ist.
	 */
	typedef Event<void(Control*, Control*)> FocusLostEvent;
	typedef EventHandler<void(Control*, Control*)> FocusLostEventHandler;

	/**
	 * Definiert die Basisklasse f�r Steuerelemente, die Komponenten mit visueller Darstellung sind.
	 */
	class OSHGUI_EXPORT Control
	{
		friend Application;
		friend ContainerControl;

	public:
		virtual ~Control();
		
		/**
		 * Ruft den CONTROL_TYPE des Steuerelemts ab.
		 *
		 * @return der Typ
		 */
		virtual const Misc::AnsiString& GetType() const;
		
		/**
		 * Ruft ab, ob das Steuerelement den Fokus besitzt.
		 *
		 * @return isFocused
		 */
		virtual bool GetIsFocused() const;
		/**
		 * Legt fest, ob das Steuerlement auf Benutzerinteraktionen reagieren kann.
		 *
		 * @param isEnabled
		 */
		virtual void SetEnabled(bool isEnabled);
		/**
		 * Ruft ab, ob das Steuerlement auf Benutzerinteraktionen reagieren kann.
		 *
		 * @return isEnabled
		 */
		virtual bool GetEnabled() const;
		/**
		 * Legt fest, ob das Steuerelement und alle untergeordneten Steuerelemente
		 * angezeigt werden.
		 *
		 * @param isVisible
		 */
		virtual void SetVisible(bool isVisible);
		/**
		 * Ruft ab, ob das Steuerelement und alle untergeordneten Steuerelemente
		 * angezeigt werden.
		 *
		 * @return isVisible
		 */
		virtual bool GetVisible() const;
		
		/**
		 * Legt fest, ob sich die Gr��e des Steuerelements automatisch an dessen Inhalt anpasst.
		 *
		 * @param autoSize
		 */
		virtual void SetAutoSize(bool autoSize);
		/**
		 * Ruft ab, ob die Gr��e des Steuerelements automatisch an dessen Inhalt anpasst.
		 *
		 * @return autoSize
		 */
		virtual bool GetAutoSize() const;
		/**
		 * Legt die Gr��e und Position des Steuerelements relativ zum �bergeordneten
		 * Steuerelement fest.
		 *
		 * @param bounds
		 */
		virtual void SetBounds(const Drawing::Rectangle &bounds);
		/**
		 * Legt die Gr��e und Position des Steuerelements relativ zum �bergeordneten
		 * Steuerelement fest.
		 *
		 * @param location
		 * @param size
		 */
		virtual void SetBounds(const Drawing::Point &location, const Drawing::Size &size);
		/**
		 * Legt die Gr��e und Position des Steuerelements relativ zum �bergeordneten
		 * Steuerelement fest.
		 *
		 * @param x
		 * @param y
		 * @param w
		 * @param h
		 */
		virtual void SetBounds(int x, int y, int w, int h);
		/**
		 * Ruft die Gr��e und Position des Steuerelements relativ zum �bergeordneten
		 * Steuerelement ab.
		 *
		 * @return bounds
		 */
		virtual const Drawing::Rectangle GetBounds() const;
		/**
		 * Legt die Koordinaten der linken oberen Ecke des Steuerelements relativ zur
		 * linken oberen Ecke des Containers fest.
		 *
		 * @param x
		 * @param y
		 */
		virtual void SetLocation(int x, int y);
		/**
		 * Legt die Koordinaten der linken oberen Ecke des Steuerelements relativ zur
		 * linken oberen Ecke des Containers fest.
		 *
		 * @param location
		 */
		virtual void SetLocation(const Drawing::Point &location);
		/**
		 * Ruft die Koordinaten der linken oberen Ecke des Steuerelements relativ zur
		 * linken oberen Ecke des Containers ab.
		 *
		 * @return location
		 */
		virtual const Drawing::Point& GetLocation() const;
		/**
		 * Legt die H�he und Breite des Steuerelements fest.
		 *
		 * @param width
		 * @param height
		 */
		virtual void SetSize(int width, int height);
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
		virtual const Drawing::Size& GetSize() const;
		/**
		 * Ruft den Abstand zwischen dem linken Rand des Steuerelements und dem linken
		 * Rand des Clientbereichs des zugeh�rigen Containers ab.
		 *
		 * @return left
		 */
		virtual int GetLeft() const;
		/**
		 * Ruft den Abstand zwischen dem oberen Rand des Steuerelements und dem oberen
		 * Rand des Clientbereichs des zugeh�rigen Containers ab.
		 *
		 * @return top
		 */
		virtual int GetTop() const;
		/**
		 * Ruft den Abstand zwischen dem rechten Rand des Steuerelements und dem linken
		 * Rand des Clientbereichs des zugeh�rigen Containers ab.
		 *
		 * @return right
		 */
		virtual int GetRight() const;
		/**
		 * Ruft den Abstand zwischen dem unteren Rand des Steuerelements und dem oberen
		 * Rand des Clientbereichs des zugeh�rigen Containers ab.
		 *
		 * @return bottom
		 */
		virtual int GetBottom() const;
		/**
		 * Ruft die Breite des Steuerelements ab.
		 *
		 * @return width
		 */
		virtual int GetWidth() const;
		/**
		 * Ruft die H�he des Steuerelements ab.
		 *
		 * @return height
		 */
		virtual int GetHeight() const;
		/**
		 * Legt die R�nder des Containers ab, an die ein Steuerelement gebunden ist,
		 * und bestimmt, wie die Gr��e des Steuerelements mit dessen �bergeordnetem Element ge�ndert wird.
		 *
		 * @param anchor
		 */
		virtual void SetAnchor(AnchorStyle anchor);
		/**
		 * Ruft die R�nder des Containers ab, an die ein Steuerelement gebunden ist,
		 * und bestimmt, wie die Gr��e des Steuerelements mit dessen �bergeordnetem Element ge�ndert wird.
		 *
		 * @return anchor
		 */
		virtual AnchorStyle GetAnchor() const;
		/**
		 * Legt die mit dem Steuerelement verkn�pften benutzerdefinierten Daten fest.
		 *
		 * @param tag
		 */
		virtual void SetTag(Misc::Any &tag);
		/**
		 * Ruft die mit dem Steuerelement verkn�pften benutzerdefinierten Daten ab.
		 *
		 * @return tag
		 */
		virtual const Misc::Any& GetTag() const;
		/**
		 * Legt den zum Identifizieren des Steuerelements verwendeten Namen fest.
		 *
		 * @param name
		 */
		virtual void SetName(const Misc::AnsiString &name);
		/**
		 * Ruft den zum Identifizieren des Steuerelements verwendeten Namen ab.
		 *
		 * @return name
		 */
		virtual const Misc::AnsiString& GetName() const;
		/**
		 * Legt die Schriftart des Texts im Steuerelement fest.
		 *
		 * @param font
		 */
		virtual void SetFont(const std::shared_ptr<Drawing::IFont> &font);
		/**
		 * Ruft die Schriftart des Texts im Steuerelement ab.
		 *
		 * @return font
		 */
		const std::shared_ptr<Drawing::IFont>& GetFont() const;
		/**
		 * Legt den angezeigten Cursor fest, wenn sich die Maus �ber dem Steuerelement befindet.
		 *
		 * @param cursor
		 */
		void SetCursor(const std::shared_ptr<Cursor> &cursor);
		/** 
		 * Ruft den angezeigten Cursor ab, wenn sich die Maus �ber dem Steuerelement befindet.
		 *
		 * @return cursor
		 */
		virtual const std::shared_ptr<Cursor>& GetCursor() const;
		/**
		 * Legt die Fordergrundfarbe des Steuerelements fest.
		 *
		 * @param color
		 */
		virtual void SetForeColor(const Drawing::Color &color);
		/**
		 * Ruft die Fordergrundfarbe des Steuerelements ab.
		 *
		 * @return color
		 */
		virtual const Drawing::Color& GetForeColor() const;
		/**
		 * Legt die Hintergrundfarbe des Steuerelements fest.
		 *
		 * @param color
		 */
		virtual void SetBackColor(const Drawing::Color &color);
		/**
		 * Ruft die Hintergrundfarbe des Steuerelements ab.
		 *
		 * @return color
		 */
		virtual const Drawing::Color& GetBackColor() const;
		/**
		 * Legt die Farbe f�r das fokusierte Steuerelement fest.
		 *
		 * @param color
		 */
		virtual void SetMouseOverFocusColor(const Drawing::Color &color);
		/**
		 * Ruft die Farbe f�r das fokusierte Steuerelement ab.
		 *
		 * @return color
		 */
		virtual const Drawing::Color& GetMouseOverFocusColor() const;
		/**
		 * Ruft das LocationChangedEvent f�r das Steuerelement ab.
		 *
		 * @return locationChangedEvent
		 */
		LocationChangedEvent& GetLocationChangedEvent();
		/**
		 * Ruft das SizeChangedEvent f�r das Steuerelement ab.
		 *
		 * @return sizeChangedEvent
		 */
		SizeChangedEvent& GetSizeChangedEvent();
		/**
		 * Ruft das ClickEvent f�r das Steuerelement ab.
		 *
		 * @return clickEvent
		 */
		ClickEvent& GetClickEvent();
		/**
		 * Ruft das MouseClickEvent f�r das Steuerelement ab.
		 *
		 * @return mouseClickEvent
		 */
		MouseClickEvent& GetMouseClickEvent();
		/**
		 * Ruft das MouseDownEvent f�r das Steuerelement ab.
		 *
		 * @return mouseDownEvent
		 */
		MouseDownEvent& GetMouseDownEvent();
		/**
		 * Ruft das MouseUpEvent f�r das Steuerelement ab.
		 *
		 * @return mouseUpEvent
		 */
		MouseUpEvent& GetMouseUpEvent();
		/**
		 * Ruft das MouseMoveEvent f�r das Steuerelement ab.
		 *
		 * @return mouseMoveEvent
		 */
		MouseMoveEvent& GetMouseMoveEvent();
		/**
		 * Ruft das MouseScrollEvent f�r das Steuerelement ab.
		 *
		 * @return mouseScrollEvent
		 */
		MouseScrollEvent& GetMouseScrollEvent();
		/**
		 * Ruft das MouseEnterEvent f�r das Steuerelement ab.
		 *
		 * @return mouseEnterEvent
		 */
		MouseEnterEvent& GetMouseEnterEvent();
		/**
		 * Ruft das MouseLeaveEvent f�r das Steuerelement ab.
		 *
		 * @return mouseLeaveEvent
		 */
		MouseLeaveEvent& GetMouseLeaveEvent();
		/**
		 * Ruft das MouseCaptureChangedEvent f�r das Steuerelement ab.
		 *
		 * @return mouseCaptureChangedEvent
		 */
		MouseCaptureChangedEvent& GetMouseCaptureChangedEvent();
		/**
		 * Ruft das KeyDownEvent f�r das Steuerelement ab.
		 *
		 * @return keyPressEvent
		 */
		KeyDownEvent& GetKeyDownEvent();
		/**
		 * Ruft das KeyPressEvent f�r das Steuerelement ab.
		 *
		 * @return keyPressEvent
		 */
		KeyPressEvent& GetKeyPressEvent();
		/**
		 * Ruft das KeyUpEvent f�r das Steuerelement ab.
		 *
		 * @return keyPressEvent
		 */
		KeyUpEvent& GetKeyUpEvent();
		/**
		 * Ruft das FocusGotEvent f�r das Steuerelement ab.
		 *
		 * @return forcusInEvent
		 */
		FocusGotEvent& GetFocusGotEvent();
		/**
		 * Ruft das FocusLostEvent f�r das Steuerelement ab.
		 *
		 * @return forcusOutEvent
		 */
		FocusLostEvent& GetFocusLostEvent();
		/**
		 * Legt das �bergeordnete Steuerelement fest.
		 *
		 * @param parent
		 */
		void SetParent(Control *parent);
		/**
		 * Ruft das �bergeordnete Steuerelement ab.
		 *
		 * @return parent
		 */
		Control* GetParent() const;

		/**
		 * Setzt den Eingabefokus auf das Steuerelement.
		 */
		virtual void Focus();
		/**
		 * �berpr�ft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * @param point
		 * @return ja / nein
		 */
		virtual bool Intersect(const Drawing::Point &point) const = 0;
		/**
		 * Berechnet die absolute Position des Steuerelements.
		 */
		virtual void CalculateAbsoluteLocation();
		/**
		 * Rechnet die Position des angegeben Bildschirmpunkts in Clientkoordinaten um.
		 *
		 * @param point
		 * @return Clientkoordinaten
		 */
		virtual const Drawing::Point PointToClient(const Drawing::Point &point) const;
		/**
		 * Rechnet die Position des angegeben Clientpunkts in Bildschirmkoordinaten um.
		 *
		 * @param point
		 * @return Bildschirmkoordinaten
		 */
		virtual const Drawing::Point PointToScreen(const Drawing::Point &point) const;
		
		/**
		 * Verarbeitet eine Maus-Nachricht. Sollte nicht vom Benutzer aufgerufen werden.
		 *
		 * @param mouse
		 * @return true, falls die Nachricht verarbeitet wurde
		 */
		bool ProcessMouseMessage(const MouseMessage &mouse);
		/**
		 * Verarbeitet eine Tastatur-Nachricht. Sollte nicht vom Benutzer aufgerufen werden.
		 *
		 * @param keyboard
		 * @return true, falls die Nachricht verarbeitet wurde
		 */
		bool ProcessKeyboardMessage(KeyboardMessage &keyboard);
		/**
		 * Zeichnet das Steuerelement mithilfe des �bergebenen IRenderers.
		 *
		 * @param renderer
		 */
		virtual void Render(Drawing::IRenderer *renderer);
		/**
		 * Veranlasst das Steuerelement, sein Aussehen dem Theme anzupassen.
		 *
		 * @param theme
		 */
		virtual void ApplyTheme(const Drawing::Theme &theme);
	
	protected:
		/**
		 * Konstruktor der Klasse.
		 */
		Control();

		virtual void OnLocationChanged();
		virtual void OnSizeChanged();
		virtual void OnMouseDown(const MouseMessage &mouse);
		virtual void OnMouseClick(const MouseMessage &mouse);
		virtual void OnMouseUp(const MouseMessage &mouse);
		virtual void OnMouseMove(const MouseMessage &mouse);
		virtual void OnMouseScroll(const MouseMessage &mouse);
		virtual void OnMouseEnter(const MouseMessage &mouse);
		virtual void OnMouseLeave(const MouseMessage &mouse);
		virtual void OnGotMouseCapture();
		virtual void OnLostMouseCapture();
		virtual void OnGotFocus(Control *newFocusedControl);
		virtual void OnLostFocus(Control *newFocusedControl);
		virtual bool OnKeyDown(const KeyboardMessage &keyboard);
		virtual bool OnKeyPress(const KeyboardMessage &keyboard);
		virtual bool OnKeyUp(const KeyboardMessage &keyboard);
		
		Misc::AnsiString name;
		Misc::AnsiString type;

		bool canRaiseEvents;
		bool isEnabled;
		bool isVisible;
		bool isInside;
		bool isClicked;
		bool isFocusable;
		bool isFocused;
		bool hasCaptured;
		bool autoSize;
			 
		Misc::Any tag;
		
		Drawing::Point location;
		Drawing::Point absoluteLocation;
		Drawing::Size size;
		
		LocationChangedEvent locationChangedEvent;
		SizeChangedEvent sizeChangedEvent;
		ClickEvent clickEvent;
		MouseClickEvent mouseClickEvent;
		MouseDownEvent mouseDownEvent;
		MouseUpEvent mouseUpEvent;
		MouseMoveEvent mouseMoveEvent;
		MouseScrollEvent mouseScrollEvent;
		MouseEnterEvent mouseEnterEvent;
		MouseLeaveEvent mouseLeaveEvent;
		MouseCaptureChangedEvent mouseCaptureChangedEvent;
		KeyDownEvent keyDownEvent;
		KeyPressEvent keyPressEvent;
		KeyUpEvent keyUpEvent;
		FocusGotEvent focusGotEvent;
		FocusLostEvent focusLostEvent;
		
		Drawing::Color foreColor;
		Drawing::Color backColor;
		Drawing::Color mouseOverFocusColor;
		
		std::shared_ptr<Drawing::IFont> font;
		std::shared_ptr<Cursor> cursor;

		Control *parent;

	private:
		//copying prohibited
		Control(const Control&);
		void operator = (const Control&);

		AnchorStyle anchor;
	};
}

#endif