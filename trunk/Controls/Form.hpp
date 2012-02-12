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
	 * Gibt Bezeichner an, die den R�ckgabewert eines Dialogfelds angeben.
	 */
	enum DialogResult
	{
		/**
		 * Der R�ckgabewert des Dialogfelds ist Nothing.
		 */
		ResultNone,
		/**
		 * Der R�ckgabewert des Dialogfelds ist OK (�blicherweise von der Schaltfl�che OK gesendet).
		 */
		ResultOK,
		/**
		 * Der R�ckgabewert des Dialogfelds ist Cancel (�blicherweise von der Schaltfl�che Abbrechen gesendet).
		 */
		ResultCancel,
		/**
		 * Der R�ckgabewert des Dialogfelds ist Abort (�blicherweise von der Schaltfl�che Abbrechen gesendet).
		 */
		ResultAbort,
		/**
		 * Der R�ckgabewert des Dialogfelds ist Retry (�blicherweise von der Schaltfl�che Wiederholen gesendet).
		 */
		ResultRetry,
		/**
		 * Der R�ckgabewert des Dialogfelds ist Ignore (�blicherweise von der Schaltfl�che Ignorieren gesendet).
		 */
		ResultIgnore,
		/**
		 * Der R�ckgabewert des Dialogfelds ist Yes (�blicherweise von der Schaltfl�che Ja gesendet).
		 */
		ResultYes,
		/**
		 * Der R�ckgabewert des Dialogfelds ist No (�blicherweise von der Schaltfl�che Nein gesendet).
		 */
		ResultNo
	};

	class Label;
	class Panel;

	/**
	 * Stellt ein Fenster dar, das die Benutzeroberfl�che bildet.
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
		 * Legt die H�he und Breite des Steuerelements fest.
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
		 * Gibt den Text zur�ck.
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
		 * Ruft das DialogResult f�r das Fenster ab.
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
		 * @param closeFunction diese Funktion wird ausgef�hrt, wenn die Form geschlossen wird (kann 0 sein)
		 */
		void ShowDialog(const std::shared_ptr<Form> &instance, const std::function<void()> &closeFunction);
		/**
		 * Schlie�t die Form.
		 */
		void Close();
		/**
		 * F�gt ein untergeordnetes Steuerelement hinzu.
		 *
		 * @param control
		 */
		virtual void AddControl(Control *control);

		/**
		 * Zeichnet das Steuerelement mithilfe des �bergebenen IRenderers.
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