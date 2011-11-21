#ifndef OSHGUI_FORMMANAGER_HPP_
#define OSHGUI_FORMMANAGER_HPP_

#include <vector>
#include <memory>
#include <functional>
#include "Event\IEvent.hpp"
#include "Exports.hpp"

namespace OSHGui
{
	namespace Drawing {	class IRenderer; }
	class Form;

	class OSHGUI_EXPORT FormManager
	{
	public:
		class FormIterator;

		/**
		 * Ruft die 
		 */
		const std::shared_ptr<Form> GetForeMost() const;

		const std::shared_ptr<Form>& operator [] (int index) const;

		int GetFormCount() const;

		void RegisterMainForm(const std::shared_ptr<Form> &mainForm);
		const std::shared_ptr<Form>& GetMainForm() const;
		bool IsRegistered(const std::shared_ptr<Form> &form);
		void RegisterForm(const std::shared_ptr<Form> &form);
		void RegisterForm(const std::shared_ptr<Form> &form, std::function<void()> closeFunction);
		void UnregisterForm(const std::shared_ptr<Form> &form);
		void BringToFront(const std::shared_ptr<Form> &form);
		
		bool ForwardEventToForms(IEvent *event);
		void RenderForms(Drawing::IRenderer *renderer);

		FormIterator GetEnumerator();

	private:
		struct FormInfo
		{
			std::shared_ptr<Form> form;
			std::function<void()> closeFunction;
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