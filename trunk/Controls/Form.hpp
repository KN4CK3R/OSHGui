/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_FORM_HPP
#define OSHGUI_FORM_HPP

#include <functional>
#include "ContainerControl.hpp"
#include "../Misc/TextHelper.hpp"

namespace OSHGui
{
	/**
	 * Gibt Bezeichner an, die den Rückgabewert eines Dialogfelds angeben.
	 */
	enum DialogResult
	{
		/**
		 * Der Rückgabewert des Dialogfelds ist Nothing.
		 */
		ResultNone,
		/**
		 * Der Rückgabewert des Dialogfelds ist OK (üblicherweise von der Schaltfläche OK gesendet).
		 */
		ResultOK,
		/**
		 * Der Rückgabewert des Dialogfelds ist Cancel (üblicherweise von der Schaltfläche Abbrechen gesendet).
		 */
		ResultCancel,
		/**
		 * Der Rückgabewert des Dialogfelds ist Abort (üblicherweise von der Schaltfläche Abbrechen gesendet).
		 */
		ResultAbort,
		/**
		 * Der Rückgabewert des Dialogfelds ist Retry (üblicherweise von der Schaltfläche Wiederholen gesendet).
		 */
		ResultRetry,
		/**
		 * Der Rückgabewert des Dialogfelds ist Ignore (üblicherweise von der Schaltfläche Ignorieren gesendet).
		 */
		ResultIgnore,
		/**
		 * Der Rückgabewert des Dialogfelds ist Yes (üblicherweise von der Schaltfläche Ja gesendet).
		 */
		ResultYes,
		/**
		 * Der Rückgabewert des Dialogfelds ist No (üblicherweise von der Schaltfläche Nein gesendet).
		 */
		ResultNo
	};

	class Label;
	class Panel;

	/**
	 * Stellt ein Fenster dar, das die Benutzeroberfläche bildet.
	 */
	class OSHGUI_EXPORT Form : public ContainerControl
	{
		class CaptionBar;

	public:
		using ContainerControl::SetSize;

		/**
		 * Konstruktor der Klasse.
		 */
		Form();
		virtual ~Form();

		/**
		 * Ruft ab, ob die Form modal dargestellt wird.
		 *
		 * @return modal
		 */
		bool IsModal() const;		
		/**
		 * Legt die Höhe und Breite des Steuerelements fest.
		 *
		 * @param size
		 */
		virtual void SetSize(const Drawing::Size &size);
		/**
		 * Legt den Text fest.
		 *
		 * @param text
		 */
		void SetText(const Misc::AnsiString &text);
		/**
		 * Gibt den Text zurück.
		 *
		 * @return der Text
		 */
		const Misc::AnsiString& GetText() const;
		/**
		 * Legt die Fordergrundfarbe des Steuerelements fest.
		 *
		 * @param color
		 */
		virtual void SetForeColor(const Drawing::Color &color);
		/**
		 * Gibt eine Liste der untergeordneten Steuerelemente zurück.
		 *
		 * @return parent
		 */
		virtual const std::vector<Control*>& GetControls() const;
		/**
		 * Ruft das DialogResult für das Fenster ab.
		 *
		 * @return dialogResult
		 */
		DialogResult GetDialogResult() const;
		/**
		 * Ruft das FormClosingEvent ab.
		 *
		 * @return formClosingEvent
		 */
		FormClosingEvent& GetFormClosingEvent();
		
		/**
		 * Zeigt die Form an.
		 *
		 * @param instance die aktuelle Instanz dieser Form
		 */
		void Show(const std::shared_ptr<Form> &instance);
		/**
		 * Zeigt die Form modal an.
		 *
		 * @param instance die aktuelle Instanz dieser Form
		 */
		void ShowDialog(const std::shared_ptr<Form> &instance);
		/**
		 * Zeigt die Form modal an.
		 *
		 * @param instance die aktuelle Instanz dieser Form
		 * @param closeFunction diese Funktion wird ausgeführt, wenn die Form geschlossen wird (kann 0 sein)
		 */
		void ShowDialog(const std::shared_ptr<Form> &instance, const std::function<void()> &closeFunction);
		/**
		 * Schließt die Form.
		 */
		void Close();
		/**
		 * Fügt ein untergeordnetes Steuerelement hinzu.
		 *
		 * @param control
		 */
		virtual void AddControl(Control *control);

		/**
		 * Zeichnet das Steuerelement mithilfe des übergebenen IRenderers.
		 *
		 * @param renderer
		 */
		void Render(Drawing::IRenderer *renderer);

	protected:
		DialogResult dialogResult;

	private:
		static const Drawing::Point DefaultLocation;
		static const Drawing::Size DefaultSize;

		std::weak_ptr<Form> instance;

		CaptionBar *captionBar;
		Panel *containerPanel;

		bool isModal;
		FormClosingEvent formClosingEvent;

		class CaptionBar : public ContainerControl
		{
			class CaptionBarButton : public Control
			{
			public:
				static const int DefaultButtonWidth = 17;
				static const int DefaultButtonHeight = 17;

				CaptionBarButton();

				virtual void CalculateAbsoluteLocation();
				virtual bool Intersect(const Drawing::Point &point) const;

				void Render(Drawing::IRenderer *renderer);

			protected:
				virtual void OnMouseUp(const MouseMessage &mouse);

			private:
				static const Drawing::Point DefaultCrossOffset;

				Drawing::Point crossAbsoluteLocation;
			};

		public:
			static const int DefaultCaptionBarHeight = 17;

			CaptionBar();

			void SetSize(const Drawing::Size &size);
			void SetText(const Misc::AnsiString &text);
			const Misc::AnsiString& GetText() const;
			void SetForeColor(const Drawing::Color &color);

			void Render(Drawing::IRenderer *renderer);

		protected:
			void OnMouseDown(const MouseMessage &mouse);
			void OnMouseMove(const MouseMessage &mouse);
			void OnMouseUp(const MouseMessage &mouse);

		private:
			static const int DefaultButtonPadding = 6;
			static const Drawing::Point DefaultTitleOffset;

			bool drag;
			Drawing::Point dragStart;

			Label *titleLabel;
			CaptionBarButton *closeButton;
		};
	};
}

#endif