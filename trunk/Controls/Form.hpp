#ifndef OSHGUI_FORM_HPP_
#define OSHGUI_FORM_HPP_

#include <functional>
#include "Panel.hpp"
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

	/**
	 * Stellt ein Fenster dar, das die Benutzeroberfl�che bildet.
	 */
	class OSHGUI_EXPORT Form : public Panel
	{
		class CaptionBar;

	public:
		static const Drawing::Point DefaultLocation;
		static const Drawing::Size DefaultSize;

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
		virtual void SetForeColor(Drawing::Color color);
		/**
		 * Ruft das DialogResult f�r das Fenster ab.
		 *
		 * @return dialogResult
		 */
		DialogResult GetDialogResult() const;
		
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
		bool isModal;

		std::weak_ptr<Form> instance;

		CaptionBar *captionBar;
		Panel *containerPanel;

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
			void SetForeColor(Drawing::Color color);

			void Render(Drawing::IRenderer *renderer);

		protected:
			void OnMouseDown(const MouseMessage &mouse);
			void OnMouseMove(const MouseMessage &mouse);
			void OnMouseClick(const MouseMessage &mouse);

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