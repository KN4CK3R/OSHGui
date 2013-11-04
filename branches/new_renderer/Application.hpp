/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_APPLICATION_HPP
#define OSHGUI_APPLICATION_HPP

#include <memory>
#include <vector>
#include "Drawing/Renderer.hpp"
#include "Drawing/RenderContext.hpp"
#include "Drawing/Font.hpp"
#include "Drawing/Theme.hpp"
#include "Misc/DateTime.hpp"
#include "Cursor/Cursor.hpp"
#include "Event/MouseMessage.hpp"
#include "Event/KeyboardMessage.hpp"
#include "Exports.hpp"
#include "FormManager.hpp"
#include "Event/Hotkey.hpp"

namespace OSHGui
{
	class Control;
	class Form;
	
	/**
	 * Stellt Methoden und Eigenschaften f�r die Verwaltung einer
	 * Anwendung zur Verf�gung, z.B. Methoden zum Starten und Beenden einer
	 * Anwendung sowie f�r das Abrufen von Informationen zu einer Anwendung.
	 */
	class OSHGUI_EXPORT Application
	{
		friend Control;
		friend Form;

	public:
		class GuiRenderSurface;

		/**
		 * Initialisiert die Application-Klasse.
		 *
		 * \param renderer Instanz des verwendeten Renderers
		 */
		static void Create(Drawing::RendererPtr renderer);
		
		/**
		 * Ruft ab, ob das GUI aktiviert ist.
		 *
		 * return isEnabled
		 */
		const bool IsEnabled() const;
		/**
		 * Ruft die aktuelle Uhrzeit ab.
		 *
		 * \return DateTime::Now
		 */
		const Misc::DateTime& GetNow() const;
		/**
		 * Ruft den verwendeten Renderer ab.
		 *
		 * \return renderer
		 */
		Drawing::RendererPtr GetRenderer() const;

		/**
		 * Ruft das RenderSurface der Gui ab.
		 *
		 * \return GuiRenderSurface
		 */
		GuiRenderSurface& GetRenderSurface();
		
		/**
		 * Legt die Standardschrift f�r das Gui fest.
		 *
		 * \param font Standardschrift
		 */
		void SetDefaultFont(Drawing::FontPtr &font);
		/**
		 * Ruft die Standardschrift f�r das Gui ab.
		 *
		 * \return Standardschrift
		 */
		Drawing::FontPtr& GetDefaultFont();
		
		/**
		 * Ruft die aktuelle Mausposition ab.
		 *
		 * \return cursorLocation
		 */
		const Drawing::PointF& GetCursorLocation() const;
		/**
		 * Ruft den Cursor ab.
		 *
		 * \return cursor
		 */
		const std::shared_ptr<Cursor>& GetCursor() const;
		/**
		 * Legt den Cursor fest.
		 *
		 * \param cursor
		 */
		void SetCursor(const std::shared_ptr<Cursor> &cursor);
		/**
		 * Legt fest, ob der Cursor gezeichnet werden soll.
		 *
		 * \param enabled
		 */
		void SetCursorEnabled(bool enabled);
		
		/**
		 * Legt das Theme f�r das Gui fest.
		 *
		 * \param theme Theme
		 */
		void SetTheme(const Drawing::Theme &theme);
		/**
		 * Ruft das Theme f�r das Gui ab.
		 *
		 * \return Theme
		 */
		const Drawing::Theme& GetTheme() const;
	
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
		 * \param mainForm die Hauptform, die angezeigt wird, sobald das GUI aktiviert wird
		 */
		void Run(const std::shared_ptr<Form> &mainForm);
		/**
		 * Gibt eine MouseMessage an die ge�ffneten Formen weiter.
		 *
		 * \param mouse
		 * \return true, falls die Nachricht verarbeitet wurde
		 */
		bool ProcessMouseMessage(MouseMessage &mouse);
		/**
		 * Gibt eine KeyboardMessage an die ge�ffneten Formen weiter.
		 *
		 * \param keyboard
		 * \return true, falls die Nachricht verarbeitet wurde
		 */
		bool ProcessKeyboardMessage(KeyboardMessage &keyboard);
		
		/**
		 * Zeichnet die ge�ffneten Formen.
		 */
		void Render();

		/**
		 * Registriert einen neuen Hotkey.
		 *
		 * \param hotkey
		 */
		void RegisterHotkey(const Hotkey &hotkey);
		/**
		 * Entfernt einen Hotkey.
		 *
		 * \param hotkey
		 */
		void UnregisterHotkey(const Hotkey &hotkey);
		
		/**
		 * Ruft die aktuelle Instanz der Application ab.
		 *
		 * \return instance
		 */
		static Application* Instance();

		class GuiRenderSurface : public Drawing::RenderSurface
		{
		public:
			GuiRenderSurface(Drawing::RenderTarget &target);

			void Invalidate();

			virtual void Draw() override;

		private:
			friend void Application::Render();

			bool needsRedraw;
		};

	private:
		static Application *instance;
		Application(Drawing::RendererPtr renderer);

		void InjectTime();

		Drawing::RendererPtr renderer;
		GuiRenderSurface guiSurface;
		Drawing::FontPtr defaultFont;
		
		Drawing::Theme defaultTheme;
		Drawing::Theme currentTheme;
	
		FormManager formManager;
		
		Misc::DateTime now;

		struct
		{
			Drawing::PointF Location;
			std::shared_ptr<Cursor> Cursor;
			bool Enabled;
		} mouse;

		std::vector<Hotkey> hotkeys;

		Control *FocusedControl;
		Control *CaptureControl;
		Control *MouseEnteredControl;

		bool isEnabled;
	};
}

#endif
