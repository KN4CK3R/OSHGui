/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_FORMMANAGER_HPP
#define OSHGUI_FORMMANAGER_HPP

#include <vector>
#include <memory>
#include <functional>
#include "Exports.hpp"

namespace OSHGui
{
	class Form;

	class OSHGUI_EXPORT FormManager
	{
	public:
		class FormIterator;

		/**
		 * Ruft die Fenster ab, das aktuell oben liegt.
		 *
		 * @return das oberste Fenster
		 */
		const std::shared_ptr<Form> GetForeMost() const;
		/**
		 * Ruft das Fenster an Position index ab.
		 *
		 * @return
		 */
		const std::shared_ptr<Form>& operator [] (int index) const;
		/**
		 * Ruft die Anzahl der registrierten Fenster.
		 *
		 * @return Anzahl registrierter Fenster
		 */
		int GetFormCount() const;
		
		/**
		 * Registriert das Hauptfenster.
		 *
		 * @param mainForm das Hauptfenster
		 */
		void RegisterMainForm(const std::shared_ptr<Form> &mainForm);
		/**
		 * Ruft das Hauptfenster ab.
		 *
		 * @return das Hauptfenster
		 */
		const std::shared_ptr<Form>& GetMainForm() const;
		/**
		 * Pr�ft, ob das Fenster schon registriert ist.
		 *
		 * @param form das Fenster
		 * @return true, falls registriert
		 */
		bool IsRegistered(const std::shared_ptr<Form> &form);
		/**
		 * Registriert das Fenster.
		 *
		 * @param form das Fenster
		 */
		void RegisterForm(const std::shared_ptr<Form> &form);
		/**
		 * Registriert das Fenster und legt die Funktion fest, die beim Schlie�en des Fensters aufgerufen werden soll.
		 *
		 * @param form das Fenster
		 * @param closeFunction
		 */
		void RegisterForm(const std::shared_ptr<Form> &form, std::function<void()> closeFunction);
		/**
		 * Entfernt das Fenster.
		 *
		 * @param form das Fenster
		 */
		void UnregisterForm(const std::shared_ptr<Form> &form);
		/**
		 * Entfernt Fenster aus der internen Liste.
		 *
		 * @param form das Fenster
		 */
		void RemoveUnregisteredForms();
		
		/**
		 * Bringt das Fenster in den Vordergrund.
		 *
		 * @param form das Fenster
		 */
		void BringToFront(const std::shared_ptr<Form> &form);
		/**
		 * Ruft einen Iterator f�r die Fenster ab.
		 *
		 * @return Fensteriterator
		 */
		FormIterator GetEnumerator();

	private:
		struct FormInfo
		{
			std::shared_ptr<Form> form;
			std::function<void()> closeFunction;
			bool remove;
		};
		std::vector<FormInfo> forms;

		std::shared_ptr<Form> mainForm;

	public:
		class FormIterator
		{
		public:
			FormIterator(FormManager &fm);

			void operator++();
			bool operator()();
			std::shared_ptr<Form>& operator *();

		private:
			std::vector<FormInfo>::reverse_iterator it;
			std::vector<FormInfo>::reverse_iterator end;
		};
	};
}

#endif