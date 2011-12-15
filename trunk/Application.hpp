#ifndef OSHGUI_APPLICATION_HPP
#define OSHGUI_APPLICATION_HPP

#include <memory>
#include "Drawing/IRenderer.hpp"
#include "Misc/DateTime.hpp"
#include "Cursor/Cursor.hpp"
#include "Event/IEvent.hpp"
#include "Event/MouseMessage.hpp"
#include "Event/KeyboardMessage.hpp"
#include "Exports.hpp"
#include "FormManager.hpp"
#include "TimerManager.hpp"

namespace OSHGui
{
	class Control;
	class Form;
	class Timer;
	
	/**
	 * Stellt Methoden und Eigenschaften für die Verwaltung einer
	 * Anwendung zur Verfügung, z.B. Methoden zum Starten und Beenden einer
	 * Anwendung sowie für das Abrufen von Informationen zu einer Anwendung.
	 */
	class OSHGUI_EXPORT Application
	{
		friend Control;
		friend Form;
		friend Timer;

	public:
		/**
		 * Initialisiert die Application-Klasse.
		 *
		 * @param renderer Instanz des verwendeten Renderers
		 */
		void Create(Drawing::IRenderer *renderer);
		
		/**
		 * Ruft die aktuelle Uhrzeit ab.
		 *
		 * @return DateTime::Now
		 */
		const Misc::DateTime& GetNow() const;
		/**
		 * Ruft den verwendeten Renderer ab.
		 *
		 * @return renderer
		 */
		Drawing::IRenderer* GetRenderer() const;
		/**
		 * Ruft die aktuelle Mausposition ab.
		 *
		 * @return cursorLocation
		 */
		const Drawing::Point& GetCursorLocation() const;
		/**
		 * Ruft den Cursor ab.
		 *
		 * @cursor
		 */
		const std::shared_ptr<Cursor>& GetCursor() const;
		/**
		 * Legt den Cursor fest.
		 *
		 * @param cursor
		 */
		void SetCursor(const std::shared_ptr<Cursor> &cursor);
	
		/**
		 * Aktiviert das GUI.
		 */
		void Enable();
		/**
		 * Deaktiviert das GUI.
		 */
		void Disable();
		/**
		 * Wechselt zwischen Enabled und Disabled.
		 */
		void Toggle();
		/**
		 * Legt die Hauptform des GUI fest.
		 *
		 * @param mainForm die Hauptform, die angezeigt wird, sobald das GUI aktiviert wird
		 */
		void Run(const std::shared_ptr<Form> &mainForm);
		/**
		 * Gibt eine MouseMessage an die geöffneten Formen weiter.
		 *
		 * @param mouse
		 * @return true, falls die Nachricht verarbeitet wurde
		 */
		bool ProcessMouseMessage(MouseMessage &mouse);
		/**
		 * Gibt eine KeyboardMessage an die geöffneten Formen weiter.
		 *
		 * @param keyboard
		 * @return true, falls die Nachricht verarbeitet wurde
		 */
		bool ProcessKeyboardMessage(KeyboardMessage &keyboard);
		/**
		 * Zeichnet die geöffneten Formen.
		 */
		void Render();
		
		/**
		 * Ruft die aktuelle Instanz der Application ab.
		 *
		 * @return instance
		 */
		static Application* Instance();

	private:
		static Application *instance;
		Application();
			
		Drawing::IRenderer *renderer;
	
		FormManager formManager;
		TimerManager timerManager;
		
		Misc::DateTime now;

		struct
		{
			Drawing::Point Location;
			std::shared_ptr<Cursor> Cursor;
		} mouse;

		Control *FocusedControl;
		Control *ClickedControl;
		Control *CaptureControl;
		Control *MouseEnteredControl;

		bool isEnabled;
	};
}

#endif