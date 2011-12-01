#ifndef OSHGUI_FORM_HPP_
#define OSHGUI_FORM_HPP_

#include <functional>
#include "Panel.hpp"
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

	/**
	 * Stellt ein Fenster dar, das die Benutzeroberfläche bildet.
	 */
	class OSHGUI_EXPORT Form : public Panel
	{
		class CaptionBar;

	public:
		static const Drawing::Point DefaultLocation;
		static const Drawing::Size DefaultSize;

		/**
		 * Konstruktor der Klasse.
		 *
		 * @param name eindeutiger Name der Form
		 * @param location Position der Form
		 * @param size Größe der Form
		 * @param text Text der Form
		 */
		Form(const Misc::AnsiString &name, const Drawing::Point &location, const Drawing::Size &size, const Misc::AnsiString &text);
		virtual ~Form();

		/**
		 * Ruft ab, ob die Form modal dargestellt wird.
		 *
		 * @return modal
		 */
		bool IsModal() const;
		
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
		 * Ruft das DialogResult für das Fenster ab.
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
		 * @param closeFunction diese Funktion wird ausgeführt, wenn die Form geschlossen wird (kann 0 sein)
		 */
		void ShowDialog(const std::shared_ptr<Form> &instance, const std::function<void()> &closeFunction);
		/**
		 * Schließt die Form.
		 */
		void Close();

		/**
		 * Veranlasst das Steuerelemt seine interne Struktur neu zu berechnen.
		 * Wird außerdem für alle Kindelemente aufgerufen.
		 *
		 * Sollte nicht vom Benutzer aufgerufen werden!
		 */
		virtual void Invalidate();
		/**
		 * Fügt ein untergeordnetes Steuerelement hinzu.
		 *
		 * @param control
		 */
		virtual void AddControl(Control *control);

		/**
		 * Verarbeitet ein Event und gibt es wenn nötig an Kindelemente weiter.
		 *
		 * @param event
		 * @return NextEventTypes
		 */
		//bool ProcessEvent(IEvent *event);
		/**
		 * Zeichnet das Steuerelement mithilfe des übergebenen IRenderers.
		 *
		 * @param renderer
		 */
		void Render(Drawing::IRenderer *renderer);

	protected:
		DialogResult dialogResult;

	private:
		Misc::TextHelper textHelper;
		//Drawing::Rectangle captionBar,
		//				   closeRect;
		bool drag;
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

				CaptionBarButton(const Misc::AnsiString &name, const Drawing::Point &location);

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

			CaptionBar(const Misc::AnsiString &name, const Drawing::Point &location, const Drawing::Size &Size, const Misc::AnsiString &text);

			void SetText(const Misc::AnsiString &text);
			const Misc::AnsiString& GetText() const;

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