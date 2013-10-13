/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_CONTAINERCONTROL_HPP
#define OSHGUI_CONTAINERCONTROL_HPP

#include <deque>
#include <vector>
#include "Control.hpp"

namespace OSHGui
{
	class OSHGUI_EXPORT ContainerControl : public Control
	{
		using Control::SetSize;

	public:
		class ControlIterator;
		class PostOrderIterator;

		static const int DefaultBorderPadding = 6;

		/**
		 * Konstruktor der Klasse.
		 */
		ContainerControl();
		~ContainerControl();

		/**
		 * Ruft ab, ob das Control ein Container ist.
		 *
		 * @return true, falls Container
		 */
		virtual bool IsContainer() const override;

		/**
		 * Legt die H�he und Breite des Steuerelements fest.
		 *
		 * @param size
		 */
		virtual void SetSize(const Drawing::SizeF &size) override;
		/**
		 * Gibt eine Liste der untergeordneten Steuerelemente zur�ck.
		 *
		 * @return parent
		 */
		virtual const std::deque<Control*>& GetControls() const;

		/**
		 * F�gt ein untergeordnetes Steuerelement hinzu.
		 *
		 * @param control
		 */
		virtual void AddControl(Control *control);
		/**
		 * Entfernt ein untergeordnetes Steuerelement.
		 *
		 * @param control
		 */
		virtual void RemoveControl(Control *control);
		/**
		 * Ruft das untergeordnete Steuerelement ab, das sich an den angegebenen
		 * Koordinaten befindet.
		 *
		 * @param point
		 * @return 0, falls sich dort kein Steuerelement befindet
		 */
		Control* GetChildAtPoint(const Drawing::PointF &point) const;
		/**
		 * Ruft das untergeordnete Steuerelement mit dem entsprechenden Namen ab.
		 *
		 * @param name
		 * @return 0, falls kein Steuerelement mit diesem Namen existiert
		 */
		Control* GetChildByName(const Misc::AnsiString &name) const;
		/**
		 * �berpr�ft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * @param point
		 * @return ja / nein
		 */
		virtual bool Intersect(const Drawing::PointF &point) const override;
		/**
		 * Berechnet die absolute Position des Steuerelements.
		 */
		virtual void CalculateAbsoluteLocation() override;

		PostOrderIterator GetPostOrderEnumerator();

		/**
		 * Zeichnet das Steuerelement mithilfe des �bergebenen IRenderers.
		 *
		 * @param renderer
		 */
		virtual void Render(Drawing::IRenderer *renderer) override;

		/**
		 * Veranlasst das Steuerelement, sein Aussehen dem Theme anzupassen.
		 *
		 * @param theme
		 */
		virtual void ApplyTheme(const Drawing::Theme &theme) override;

	protected:
		void AddSubControl(Control* subcontrol);

		std::deque<Control*> internalControls;
		std::deque<Control*> controls;

	public:
		class PostOrderIterator
		{
		public:
			PostOrderIterator(ContainerControl *start);

			void operator++();
			bool operator()();
			Control* operator*();

		private:
			void LoopThrough(ContainerControl *container);

			ContainerControl *start;
			Control *current;
			std::vector<Control*> controlStack;
		};
	};
}

#endif