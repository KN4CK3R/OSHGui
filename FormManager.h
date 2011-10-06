#ifndef OSHGUI_FORMMANAGER_H_
#define OSHGUI_FORMMANAGER_H_

#include <list>
#include <memory>
#include <functional>
#include "Event\IEvent.h"

namespace OSHGui
{
	class Form;

	class FormManager
	{
	public:
		const std::shared_ptr<Form>& GetForeMost() const;

		const std::shared_ptr<Form>& operator [] (int index) const;

		void RegisterForm(const std::shared_ptr<Form> &form);
		void RegisterForm(const std::shared_ptr<Form> &form, std::function<void()> closeFunction);
		void Remove(const std::shared_ptr<Form> &form);
		void BringToFront(const std::shared_ptr<Form> &form);
		
		IEvent::NextEventTypes ForwardEventToForms(IEvent *event);
		void RenderForms(Drawing::IRenderer *renderer);

	private:
		struct FormInfo
		{
			std::shared_ptr<Form> form;
			std::function<void()> closeFunction;
		};
	
		std::list<FormInfo> formList;
	};
}

#endif