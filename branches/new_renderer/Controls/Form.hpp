/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_FORM_HPP
#define OSHGUI_FORM_HPP

#include "Control.hpp"

namespace OSHGui
{
	class Label;
	class Panel;

	/**
	 * Tritt ein, wenn die Form geschlossen werden soll.
	 */
	typedef Event<void(Control*, bool &canClose)> FormClosingEvent;
	typedef EventHandler<void(Control*, bool &canClose)> FormClosingEventHandler;

	/**
	 * Gibt Bezeichner an, die den R�ckgabewert eines Dialogfelds angeben.
	 */
	enum class DialogResult
	{
		/**
		 * Der R�ckgabewert des Dialogfelds ist Nothing.
		 */
		None,
		/**
		 * Der R�ckgabewert des Dialogfelds ist OK (�blicherweise von der Schaltfl�che OK gesendet).
		 */
		OK,
		/**
		 * Der R�ckgabewert des Dialogfelds ist Cancel (�blicherweise von der Schaltfl�che Abbrechen gesendet).
		 */
		Cancel,
		/**
		 * Der R�ckgabewert des Dialogfelds ist Abort (�blicherweise von der Schaltfl�che Abbrechen gesendet).
		 */
		Abort,
		/**
		 * Der R�ckgabewert des Dialogfelds ist Retry (�blicherweise von der Schaltfl�che Wiederholen gesendet).
		 */
		Retry,
		/**
		 * Der R�ckgabewert des Dialogfelds ist Ignore (�blicherweise von der Schaltfl�che Ignorieren gesendet).
		 */
		Ignore,
		/**
		 * Der R�ckgabewert des Dialogfelds ist Yes (�blicherweise von der Schaltfl�che Ja gesendet).
		 */
		Yes,
		/**
		 * Der R�ckgabewert des Dialogfelds ist No (�blicherweise von der Schaltfl�che Nein gesendet).
		 */
		No
	};

	/**
	 * Stellt ein Fenster dar, das die Benutzeroberfl�che bildet.
	 */
	class OSHGUI_EXPORT Form : public Control
	{
		class CaptionBar;

	public:
		using Control::SetSize;

		/**
		 * Konstruktor der Klasse.
		 */
		Form();
		
		/**
		 * Ruft ab, ob die Form modal dargestellt wird.
		 *
		 * \return modal
		 */
		bool IsModal() const;
		/**
		 * Legt die H�he und Breite des Steuerelements fest.
		 *
		 * \param size
		 */
		virtual void SetSize(const Drawing::SizeI &size) override;
		/**
		 * Legt den Text fest.
		 *
		 * \param text
		 */
		void SetText(const Misc::AnsiString &text);
		/**
		 * Gibt den Text zur�ck.
		 *
		 * \return der Text
		 */
		const Misc::AnsiString& GetText() const;
		/**
		 * Legt die Fordergrundfarbe des Steuerelements fest.
		 *
		 * \param color
		 */
		virtual void SetForeColor(const Drawing::Color &color) override;
		/**
		 * Gibt eine Liste der untergeordneten Steuerelemente zur�ck.
		 *
		 * \return parent
		 */
		virtual const std::deque<Control*>& GetControls() const override;
		/**
		 * Ruft das DialogResult f�r das Fenster ab.
		 *
		 * \return dialogResult
		 */
		DialogResult GetDialogResult() const;
		/**
		 * Ruft das FormClosingEvent ab.
		 *
		 * \return formClosingEvent
		 */
		FormClosingEvent& GetFormClosingEvent();
		
		/**
		 * Zeigt die Form an.
		 *
		 * \param instance die aktuelle Instanz dieser Form
		 */
		void Show(const std::shared_ptr<Form> &instance);
		/**
		 * Zeigt die Form modal an.
		 *
		 * \param instance die aktuelle Instanz dieser Form
		 */
		void ShowDialog(const std::shared_ptr<Form> &instance);
		/**
		 * Zeigt die Form modal an.
		 *
		 * \param instance die aktuelle Instanz dieser Form
		 * \param closeFunction diese Funktion wird ausgef�hrt, wenn die Form geschlossen wird (kann 0 sein)
		 */
		void ShowDialog(const std::shared_ptr<Form> &instance, const std::function<void()> &closeFunction);
		/**
		 * Schlie�t die Form.
		 */
		void Close();
		/**
		 * F�gt ein untergeordnetes Steuerelement hinzu.
		 *
		 * \param control
		 */
		virtual void AddControl(Control *control) override;

		virtual void DrawSelf(Drawing::RenderContext &context) override;

	protected:
		virtual void PopulateGeometry() override;

		DialogResult dialogResult;

	private:
		static const Drawing::PointI DefaultLocation;
		static const Drawing::SizeI DefaultSize;

		std::weak_ptr<Form> instance;

		CaptionBar *captionBar;
		Panel *containerPanel;

		bool isModal;
		FormClosingEvent formClosingEvent;

		class CaptionBar : public Control
		{
			class CaptionBarButton : public Control
			{
			public:
				static const Drawing::SizeI DefaultSize;

				CaptionBarButton();

				virtual void CalculateAbsoluteLocation() override;

			protected:
				virtual void PopulateGeometry() override;

				virtual void OnMouseUp(const MouseMessage &mouse) override;

			private:
				static const Drawing::PointI DefaultCrossOffset;

				Drawing::PointI crossAbsoluteLocation;
			};

		public:
			static const int DefaultCaptionBarHeight = 17;

			CaptionBar();

			virtual void SetSize(const Drawing::SizeI &size) override;
			void SetText(const Misc::AnsiString &text);
			const Misc::AnsiString& GetText() const;
			virtual void SetForeColor(const Drawing::Color &color) override;

		protected:
			virtual void DrawSelf(Drawing::RenderContext &context) override;

			virtual void OnMouseDown(const MouseMessage &mouse) override;
			virtual void OnMouseMove(const MouseMessage &mouse) override;
			virtual void OnMouseUp(const MouseMessage &mouse) override;

		private:
			static const int DefaultButtonPadding = 6;
			static const Drawing::PointI DefaultTitleOffset;

			bool drag;
			Drawing::PointI dragStart;

			Label *titleLabel;
			CaptionBarButton *closeButton;
		};
	};
}

#endif