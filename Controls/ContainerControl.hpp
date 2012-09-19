/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
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
	class ContainerControl : public Control
	{
		using Control::SetSize;

	public:
		class PostOrderVisibleIterator;

		static const int DefaultBorderPadding = 6;

		/**
		 * Konstruktor der Klasse.
		 */
		ContainerControl();
		~ContainerControl();

		/**
		 * Legt die Höhe und Breite des Steuerelements fest.
		 *
		 * @param size
		 */
		virtual void SetSize(const Drawing::Size &size);
		/**
		 * Gibt eine Liste der untergeordneten Steuerelemente zurück.
		 *
		 * @return parent
		 */
		const virtual std::deque<Control*>& GetControls() const;

		/**
		 * Fügt ein untergeordnetes Steuerelement hinzu.
		 *
		 * @param control
		 */
		virtual void AddControl(Control *control);
		/**
		 * Entfern ein untergeordnetes Steuerelement.
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
		Control* GetChildAtPoint(const Drawing::Point &point) const;
		/**
		 * Ruft das untergeordnete Steuerelement mit dem entsprechenden Namen ab.
		 *
		 * @param name
		 * @return 0, falls kein Steuerelement mit diesem Namen existiert
		 */
		Control* GetChildByName(const Misc::AnsiString &name) const;
		/**
		 * Überprüft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * @param point
		 * @return ja / nein
		 */
		virtual bool Intersect(const Drawing::Point &point) const;
		/**
		 * Berechnet die absolute Position des Steuerelements.
		 */
		virtual void CalculateAbsoluteLocation();

		PostOrderVisibleIterator GetPostOrderVisibleEnumerator();

		virtual void Render(Drawing::IRenderer *renderer);

		virtual void ApplyTheme(const Drawing::Theme &theme);

	protected:
		void AddSubControl(Control* subcontrol);

		std::deque<Control*> internalControls;
		std::deque<Control*> controls;

	public:
		class PostOrderVisibleIterator
		{
		public:
			PostOrderVisibleIterator(ContainerControl *start);

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