#ifndef OSHGUI_FORMMANAGER_H_
#define OSHGUI_FORMMANAGER_H_

#include <vector>
#include <memory>
#include <functional>
#include "Event\IEvent.h"

namespace OSHGui
{
	namespace Drawing {	class IRenderer; }
	class Form;

	class FormManager
	{
	public:
		const std::shared_ptr<Form>& GetForeMost() const;

		const std::shared_ptr<Form>& operator [] (int index) const;

		int GetFormCount() const;

		void RegisterMainForm(const std::shared_ptr<Form> &mainForm);
		const std::shared_ptr<Form>& GetMainForm() const;
		void RegisterForm(const std::shared_ptr<Form> &form);
		void RegisterForm(const std::shared_ptr<Form> &form, std::function<void()> closeFunction);
		void UnregisterForm(const std::shared_ptr<Form> &form);
		void BringToFront(const std::shared_ptr<Form> &form);
		
		IEvent::NextEventTypes ForwardEventToForms(IEvent *event);
		void RenderForms(Drawing::IRenderer *renderer);

	private:
		struct FormInfo
		{
			std::shared_ptr<Form> form;
			std::function<void()> closeFunction;
		};
		std::vector<FormInfo> forms;

		std::shared_ptr<Form> mainForm;
	};
}

#endif