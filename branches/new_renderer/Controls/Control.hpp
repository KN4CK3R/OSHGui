/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
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
#include "../Drawing/Renderer.hpp"
#include "../Drawing/RenderContext.hpp"
#include "../Drawing/Graphics.hpp"
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
	enum class ControlType
	{
		Panel = 1,
		Form,
		GroupBox,
		Label,
		LinkLabel,
		Button,
		CheckBox,
		RadioButton,
		ScrollBar,
		ListBox,
		ProgressBar,
		TrackBar,
		ComboBox,
		TextBox,
		Timer,
		TabControl,
		TabPage,
		PictureBox,
		ColorPicker,
		ColorBar,
		HotkeyControl
	};

	/**
	 * Auflistung der Anker.
	 */
	enum class AnchorStyles : int
	{
		Top = 1,
		Bottom = 2,
		Left = 4,
		Right = 8
	};

	inline AnchorStyles operator|(AnchorStyles lhs, AnchorStyles rhs)
	{
		return static_cast<AnchorStyles>(static_cast<int>(lhs) | static_cast<int>(rhs));
	}
	inline AnchorStyles operator&(AnchorStyles lhs, AnchorStyles rhs)
	{
		return static_cast<AnchorStyles>(static_cast<int>(lhs) & static_cast<int>(rhs));
	}
	
	class Control;
	class ContainerControl;
	
	/**
	 * Tritt ein, wenn die Location-Eigenschaft geändert wird.
	 */
	typedef Event<void(Control*)> LocationChangedEvent;
	typedef EventHandler<void(Control*)> LocationChangedEventHandler;
	/**
	 * Tritt ein, wenn die SizeF-Eigenschaft geändert wird.
	 */
	typedef Event<void(Control*)> SizeChangedEvent;
	typedef EventHandler<void(Control*)> SizeChangedEventHandler;
	/**
	 * Tritt auf, wenn eine Taste gedrückt wird.
	 */
	typedef Event<void(Control*, KeyEventArgs&)> KeyDownEvent;
	typedef EventHandler<void(Control*, KeyEventArgs&)> KeyDownEventHandler;
	/**
	 * Tritt auf, wenn das Steuerelement fokusiert ist und eine Taste gedrückt gehalten wird.
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
	 * Tritt auf, wenn sich der Mauszeiger über dem Steuerlement befindet und eine Maustaste gedrückt wurde.
	 */
	typedef Event<void(Control*, MouseEventArgs&)> MouseDownEvent;
	typedef EventHandler<void(Control*, MouseEventArgs&)> MouseDownEventHandler;
	/**
	 * Tritt auf, wenn sich der Mauszeiger über dem Steuerlement befindet und eine Maustaste losgelassen wurde.
	 */
	typedef Event<void(Control*, MouseEventArgs&)> MouseUpEvent;
	typedef EventHandler<void(Control*, MouseEventArgs&)> MouseUpEventHandler;
	/**
	 * Tritt auf, wenn der Mauszeiger über das Steuerlement bewegt wird.
	 */
	typedef Event<void(Control*, MouseEventArgs&)> MouseMoveEvent;
	typedef EventHandler<void(Control*, MouseEventArgs&)> MouseMoveEventHandler;
	/**
	 * Tritt auf, wenn sich der Mauszeiger über dem Steuerlement befindet und eine Maustaste losgelassen wurde.
	 */
	typedef Event<void(Control*, MouseEventArgs&)> MouseScrollEvent;
	typedef EventHandler<void(Control*, MouseEventArgs&)> MouseScrollEventHandler;
	/**
	 * Tritt ein, wenn die Maus in den Bereich des Steuerlements bewegt wird.
	 */
	typedef Event<void(Control*)> MouseEnterEvent;
	typedef EventHandler<void(Control*)> MouseEnterEventHandler;
	/**
	 * Tritt ein, wenn die Maus den Bereich des Steuerlements verlässt.
	 */
	typedef Event<void(Control*)> MouseLeaveEvent;
	typedef EventHandler<void(Control*)> MouseLeaveEventHandler;
	/**
	 * Tritt ein, nachdem sich die Mausaufzeichnung geändert hat.
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
	 * Definiert die Basisklasse für Steuerelemente, die Komponenten mit visueller Darstellung sind.
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
		 * \return der Typ
		 */
		virtual ControlType GetType() const;

		/**
		 * Ruft ab, ob das Control ein Container ist.
		 *
		 * \return true, falls Container
		 */
		virtual bool IsContainer() const;
		
		/**
		 * Ruft ab, ob das Steuerelement den Fokus besitzt.
		 *
		 * \return isFocused
		 */
		virtual bool GetIsFocused() const;
		/**
		 * Legt fest, ob das Steuerlement auf Benutzerinteraktionen reagieren kann.
		 *
		 * \param isEnabled
		 */
		virtual void SetEnabled(bool isEnabled);
		/**
		 * Ruft ab, ob das Steuerlement auf Benutzerinteraktionen reagieren kann.
		 *
		 * \return isEnabled
		 */
		virtual bool GetEnabled() const;
		/**
		 * Legt fest, ob das Steuerelement und alle untergeordneten Steuerelemente
		 * angezeigt werden.
		 *
		 * \param isVisible
		 */
		virtual void SetVisible(bool isVisible);
		/**
		 * Ruft ab, ob das Steuerelement und alle untergeordneten Steuerelemente
		 * angezeigt werden.
		 *
		 * \return isVisible
		 */
		virtual bool GetVisible() const;
		
		/**
		 * Legt fest, ob sich die Größe des Steuerelements automatisch an dessen Inhalt anpasst.
		 *
		 * \param autoSize
		 */
		virtual void SetAutoSize(bool autoSize);
		/**
		 * Ruft ab, ob die Größe des Steuerelements automatisch an dessen Inhalt anpasst.
		 *
		 * \return autoSize
		 */
		virtual bool GetAutoSize() const;
		/**
		 * Legt die Größe und Position des Steuerelements relativ zum übergeordneten
		 * Steuerelement fest.
		 *
		 * \param bounds
		 */
		virtual void SetBounds(const Drawing::RectangleF &bounds);
		/**
		 * Legt die Größe und Position des Steuerelements relativ zum übergeordneten
		 * Steuerelement fest.
		 *
		 * \param location
		 * \param size
		 */
		virtual void SetBounds(const Drawing::PointF &location, const Drawing::SizeF &size);
		/**
		 * Legt die Größe und Position des Steuerelements relativ zum übergeordneten
		 * Steuerelement fest.
		 *
		 * \param x
		 * \param y
		 * \param w
		 * \param h
		 */
		virtual void SetBounds(int x, int y, int w, int h);
		/**
		 * Ruft die Größe und Position des Steuerelements relativ zum übergeordneten
		 * Steuerelement ab.
		 *
		 * \return bounds
		 */
		virtual const Drawing::RectangleF GetBounds() const;
		/**
		 * Legt die Koordinaten der linken oberen Ecke des Steuerelements relativ zur
		 * linken oberen Ecke des Containers fest.
		 *
		 * \param x
		 * \param y
		 */
		virtual void SetLocation(int x, int y);
		/**
		 * Legt die Koordinaten der linken oberen Ecke des Steuerelements relativ zur
		 * linken oberen Ecke des Containers fest.
		 *
		 * \param location
		 */
		virtual void SetLocation(const Drawing::PointF &location);
		/**
		 * Ruft die Koordinaten der linken oberen Ecke des Steuerelements relativ zur
		 * linken oberen Ecke des Containers ab.
		 *
		 * \return location
		 */
		virtual const Drawing::PointF& GetLocation() const;
		/**
		 * Legt die Höhe und Breite des Steuerelements fest.
		 *
		 * \param width
		 * \param height
		 */
		virtual void SetSize(int width, int height);
		/**
		 * Legt die Höhe und Breite des Steuerelements fest.
		 *
		 * \param size
		 */
		virtual void SetSize(const Drawing::SizeF &size);
		/**
		 * Ruft die Höhe und Breite des Steuerelements ab.
		 *
		 * \return size
		 */
		virtual const Drawing::SizeF& GetSize() const;
		/**
		 * Ruft den Abstand zwischen dem linken Rand des Steuerelements und dem linken
		 * Rand des Clientbereichs des zugehörigen Containers ab.
		 *
		 * \return left
		 */
		virtual int GetLeft() const;
		/**
		 * Ruft den Abstand zwischen dem oberen Rand des Steuerelements und dem oberen
		 * Rand des Clientbereichs des zugehörigen Containers ab.
		 *
		 * \return top
		 */
		virtual int GetTop() const;
		/**
		 * Ruft den Abstand zwischen dem rechten Rand des Steuerelements und dem linken
		 * Rand des Clientbereichs des zugehörigen Containers ab.
		 *
		 * \return right
		 */
		virtual int GetRight() const;
		/**
		 * Ruft den Abstand zwischen dem unteren Rand des Steuerelements und dem oberen
		 * Rand des Clientbereichs des zugehörigen Containers ab.
		 *
		 * \return bottom
		 */
		virtual int GetBottom() const;
		/**
		 * Ruft die Breite des Steuerelements ab.
		 *
		 * \return width
		 */
		virtual int GetWidth() const;
		/**
		 * Ruft die Höhe des Steuerelements ab.
		 *
		 * \return height
		 */
		virtual int GetHeight() const;
		/**
		 * Legt die Ränder des Containers ab, an die ein Steuerelement gebunden ist,
		 * und bestimmt, wie die Größe des Steuerelements mit dessen übergeordnetem Element geändert wird.
		 *
		 * \param anchor
		 */
		virtual void SetAnchor(AnchorStyles anchor);
		/**
		 * Ruft die Ränder des Containers ab, an die ein Steuerelement gebunden ist,
		 * und bestimmt, wie die Größe des Steuerelements mit dessen übergeordnetem Element geändert wird.
		 *
		 * \return anchor
		 */
		virtual AnchorStyles GetAnchor() const;
		/**
		 * Legt die mit dem Steuerelement verknüpften benutzerdefinierten Daten fest.
		 *
		 * \param tag
		 */
		virtual void SetTag(Misc::Any &tag);
		/**
		 * Ruft die mit dem Steuerelement verknüpften benutzerdefinierten Daten ab.
		 *
		 * \return tag
		 */
		virtual const Misc::Any& GetTag() const;
		/**
		 * Legt den zum Identifizieren des Steuerelements verwendeten Namen fest.
		 *
		 * \param name
		 */
		virtual void SetName(const Misc::AnsiString &name);
		/**
		 * Ruft den zum Identifizieren des Steuerelements verwendeten Namen ab.
		 *
		 * \return name
		 */
		virtual const Misc::AnsiString& GetName() const;
		/**
		 * Legt die Schriftart des Texts im Steuerelement fest.
		 *
		 * \param font
		 */
		virtual void SetFont(const Drawing::FontPtr &font);
		/**
		 * Ruft die Schriftart des Texts im Steuerelement ab.
		 *
		 * \return font
		 */
		const Drawing::FontPtr& GetFont() const;
		/**
		 * Legt den angezeigten Cursor fest, wenn sich die Maus über dem Steuerelement befindet.
		 *
		 * \param cursor
		 */
		void SetCursor(const std::shared_ptr<Cursor> &cursor);
		/** 
		 * Ruft den angezeigten Cursor ab, wenn sich die Maus über dem Steuerelement befindet.
		 *
		 * \return cursor
		 */
		virtual const std::shared_ptr<Cursor>& GetCursor() const;
		/**
		 * Legt die Fordergrundfarbe des Steuerelements fest.
		 *
		 * \param color
		 */
		virtual void SetForeColor(Drawing::Color color);
		/**
		 * Ruft die Fordergrundfarbe des Steuerelements ab.
		 *
		 * \return color
		 */
		virtual Drawing::Color GetForeColor() const;
		/**
		 * Legt die Hintergrundfarbe des Steuerelements fest.
		 *
		 * \param color
		 */
		virtual void SetBackColor(Drawing::Color color);
		/**
		 * Ruft die Hintergrundfarbe des Steuerelements ab.
		 *
		 * \return color
		 */
		virtual Drawing::Color GetBackColor() const;
		/**
		 * Legt die Farbe für das fokusierte Steuerelement fest.
		 *
		 * \param color
		 */
		virtual void SetMouseOverFocusColor(Drawing::Color color);
		/**
		 * Ruft die Farbe für das fokusierte Steuerelement ab.
		 *
		 * \return color
		 */
		virtual Drawing::Color GetMouseOverFocusColor() const;
		/**
		 * Ruft das LocationChangedEvent für das Steuerelement ab.
		 *
		 * \return locationChangedEvent
		 */
		LocationChangedEvent& GetLocationChangedEvent();
		/**
		 * Ruft das SizeChangedEvent für das Steuerelement ab.
		 *
		 * \return sizeChangedEvent
		 */
		SizeChangedEvent& GetSizeChangedEvent();
		/**
		 * Ruft das ClickEvent für das Steuerelement ab.
		 *
		 * \return clickEvent
		 */
		ClickEvent& GetClickEvent();
		/**
		 * Ruft das MouseClickEvent für das Steuerelement ab.
		 *
		 * \return mouseClickEvent
		 */
		MouseClickEvent& GetMouseClickEvent();
		/**
		 * Ruft das MouseDownEvent für das Steuerelement ab.
		 *
		 * \return mouseDownEvent
		 */
		MouseDownEvent& GetMouseDownEvent();
		/**
		 * Ruft das MouseUpEvent für das Steuerelement ab.
		 *
		 * \return mouseUpEvent
		 */
		MouseUpEvent& GetMouseUpEvent();
		/**
		 * Ruft das MouseMoveEvent für das Steuerelement ab.
		 *
		 * \return mouseMoveEvent
		 */
		MouseMoveEvent& GetMouseMoveEvent();
		/**
		 * Ruft das MouseScrollEvent für das Steuerelement ab.
		 *
		 * \return mouseScrollEvent
		 */
		MouseScrollEvent& GetMouseScrollEvent();
		/**
		 * Ruft das MouseEnterEvent für das Steuerelement ab.
		 *
		 * \return mouseEnterEvent
		 */
		MouseEnterEvent& GetMouseEnterEvent();
		/**
		 * Ruft das MouseLeaveEvent für das Steuerelement ab.
		 *
		 * \return mouseLeaveEvent
		 */
		MouseLeaveEvent& GetMouseLeaveEvent();
		/**
		 * Ruft das MouseCaptureChangedEvent für das Steuerelement ab.
		 *
		 * \return mouseCaptureChangedEvent
		 */
		MouseCaptureChangedEvent& GetMouseCaptureChangedEvent();
		/**
		 * Ruft das KeyDownEvent für das Steuerelement ab.
		 *
		 * \return keyPressEvent
		 */
		KeyDownEvent& GetKeyDownEvent();
		/**
		 * Ruft das KeyPressEvent für das Steuerelement ab.
		 *
		 * \return keyPressEvent
		 */
		KeyPressEvent& GetKeyPressEvent();
		/**
		 * Ruft das KeyUpEvent für das Steuerelement ab.
		 *
		 * \return keyPressEvent
		 */
		KeyUpEvent& GetKeyUpEvent();
		/**
		 * Ruft das FocusGotEvent für das Steuerelement ab.
		 *
		 * \return forcusInEvent
		 */
		FocusGotEvent& GetFocusGotEvent();
		/**
		 * Ruft das FocusLostEvent für das Steuerelement ab.
		 *
		 * \return forcusOutEvent
		 */
		FocusLostEvent& GetFocusLostEvent();
		/**
		 * Legt das übergeordnete Steuerelement fest.
		 *
		 * \param parent
		 */
		virtual void SetParent(Control *parent);
		/**
		 * Ruft das übergeordnete Steuerelement ab.
		 *
		 * \return parent
		 */
		virtual Control* GetParent() const;

		/**
		 * Setzt den Eingabefokus auf das Steuerelement.
		 */
		virtual void Focus();
		/**
		 * Überprüft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * \param point
		 * \return ja / nein
		 */
		virtual bool Intersect(const Drawing::PointF &point) const = 0;
		/**
		 * Berechnet die absolute Position des Steuerelements.
		 */
		virtual void CalculateAbsoluteLocation();
		/**
		 * Rechnet die Position des angegeben Bildschirmpunkts in Clientkoordinaten um.
		 *
		 * \param point
		 * \return Clientkoordinaten
		 */
		virtual const Drawing::PointF PointToClient(const Drawing::PointF &point) const;
		/**
		 * Rechnet die Position des angegeben Clientpunkts in Bildschirmkoordinaten um.
		 *
		 * \param point
		 * \return Bildschirmkoordinaten
		 */
		virtual const Drawing::PointF PointToScreen(const Drawing::PointF &point) const;
		
		/**
		 * Verarbeitet eine Maus-Nachricht. Sollte nicht vom Benutzer aufgerufen werden.
		 *
		 * \param mouse
		 * \return true, falls die Nachricht verarbeitet wurde
		 */
		bool ProcessMouseMessage(const MouseMessage &mouse);
		/**
		 * Verarbeitet eine Tastatur-Nachricht. Sollte nicht vom Benutzer aufgerufen werden.
		 *
		 * \param keyboard
		 * \return true, falls die Nachricht verarbeitet wurde
		 */
		bool ProcessKeyboardMessage(KeyboardMessage &keyboard);
		/**
		 * Veranlasst das Control sich neu zu zeichnen.
		 */
		void Invalidate();
		/**
		 * Zeichnet das Steuerelement.
		 */
		virtual void Render();
		/**
		 * Veranlasst das Steuerelement, sein Aussehen dem Theme anzupassen.
		 *
		 * \param theme
		 */
		virtual void ApplyTheme(const Drawing::Theme &theme);
	
		/**
		 * Ruft die Stringrepresentation des Controltypes ab.
		 *
		 * \param controlType controltype
		 * \return Stringrepresentation
		 */
		static Misc::AnsiString ControlTypeToString(ControlType controlType);

	private:
		void GetRenderContext(Drawing::RenderContext &context) const;

	protected:
		/**
		 * Konstruktor der Klasse.
		 */
		Control();

		virtual void InjectTime(const Misc::DateTime &time);

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

		virtual void DrawSelf(Drawing::RenderContext &context);
		virtual void BufferGeometry(Drawing::RenderContext &context);
		virtual void QueueGeometry(Drawing::RenderContext &context);
		virtual void PopulateGeometry();
		
		Misc::AnsiString name;
		ControlType type;

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
		
		Drawing::PointF location;
		Drawing::PointF absoluteLocation;
		Drawing::SizeF size;
		
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
		
		Drawing::FontPtr font;
		std::shared_ptr<Cursor> cursor;

		bool needsRedraw;
		Drawing::GeometryBufferPtr geometry;
		std::unique_ptr<Drawing::RenderSurface> surface;

		Control *parent;

	private:
		//copying prohibited
		Control(const Control&) { }
		void operator = (const Control&) { }

		AnchorStyles anchor;
	};
}

#endif