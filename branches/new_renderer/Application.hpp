/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
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
#include "Drawing/IRenderer.hpp"
#include "Drawing/Theme.hpp"
#include "Misc/DateTime.hpp"
#include "Cursor/Cursor.hpp"
#include "Event/MouseMessage.hpp"
#include "Event/KeyboardMessage.hpp"
#include "Exports.hpp"
#include "FormManager.hpp"
#include "TimerManager.hpp"
#include "Event/Hotkey.hpp"

namespace OSHGui
{
	class Control;
	class Form;
	class Timer;
	
	/**
	 * Stellt Methoden und Eigenschaften f�r die Verwaltung einer
	 * Anwendung zur Verf�gung, z.B. Methoden zum Starten und Beenden einer
	 * Anwendung sowie f�r das Abrufen von Informationen zu einer Anwendung.
	 */
	class OSHGUI_EXPORT Application
	{
		friend Control;
		friend Form;
		friend Timer;

		class GuiRenderSurface;

	public:
		/**
		 * Initialisiert die Application-Klasse.
		 *
		 * @param renderer Instanz des verwendeten Renderers
		 */
		static void Create(Drawing::IRenderer *renderer_, Drawing::RendererPtr renderer = nullptr);
		
		/**
		 * Ruft ab, ob das GUI aktiviert ist.
		 *
		 * return isEnabled
		 */
		const bool IsEnabled() const;
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

		Drawing::RendererPtr GetRenderer_() { return renderer; }
		GuiRenderSurface& GetRenderSurface();
		Drawing::FontPtr& GetDefaultFont();
		void SetDefaultFont(Drawing::FontPtr &defaultFont);
		/**
		 * Ruft die aktuelle Mausposition ab.
		 *
		 * @return cursorLocation
		 */
		const Drawing::PointF& GetCursorLocation() const;
		/**
		 * Ruft den Cursor ab.
		 *
		 * @return cursor
		 */
		const std::shared_ptr<Cursor>& GetCursor() const;
		/**
		 * Legt den Cursor fest.
		 *
		 * @param cursor
		 */
		void SetCursor(const std::shared_ptr<Cursor> &cursor);
		/**
		 * Legt fest, ob der Cursor gezeichnet werden soll.
		 *
		 * @param enabled
		 */
		void SetCursorEnabled(bool enabled);
		void SetTheme(const Drawing::Theme &theme);
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
		 * @param mainForm die Hauptform, die angezeigt wird, sobald das GUI aktiviert wird
		 */
		void Run(const std::shared_ptr<Form> &mainForm);
		/**
		 * Gibt eine MouseMessage an die ge�ffneten Formen weiter.
		 *
		 * @param mouse
		 * @return true, falls die Nachricht verarbeitet wurde
		 */
		bool ProcessMouseMessage(MouseMessage &mouse);
		/**
		 * Gibt eine KeyboardMessage an die ge�ffneten Formen weiter.
		 *
		 * @param keyboard
		 * @return true, falls die Nachricht verarbeitet wurde
		 */
		bool ProcessKeyboardMessage(KeyboardMessage &keyboard);
		/**
		 * Zeichnet die ge�ffneten Formen.
		 */
		void Render();

		/**
		 * Registriert einen neuen Hotkey.
		 *
		 * @param hotkey
		 */
		void RegisterHotkey(const Hotkey &hotkey);
		/**
		 * Entfernt einen Hotkey.
		 *
		 * @param hotkey
		 */
		void UnregisterHotkey(const Hotkey &hotkey);
		
		/**
		 * Ruft die aktuelle Instanz der Application ab.
		 *
		 * @return instance
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

		Drawing::RendererPtr renderer;
		GuiRenderSurface guiSurface;
		Drawing::FontPtr defaultFont;

		Drawing::IRenderer *renderer__;
		
		Drawing::Theme defaultTheme;
		Drawing::Theme currentTheme;
	
		FormManager formManager;
		TimerManager timerManager;
		
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