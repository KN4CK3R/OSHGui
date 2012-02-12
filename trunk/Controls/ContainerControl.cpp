/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include "ContainerControl.hpp"
#include "../Misc/Exceptions.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	ContainerControl::ContainerControl()
	{

	}
	//---------------------------------------------------------------------------
	ContainerControl::~ContainerControl()
	{
		controls.clear();

		for (std::list<Control*>::iterator it = internalControls.begin(); it != internalControls.end(); ++it)
		{
			delete *it;
		}
		internalControls.clear();
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	const std::list<Control*>& ContainerControl::GetControls() const
	{
		return controls;
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	void ContainerControl::AddControl(Control *control)
	{
		#ifndef OSHGUI_DONTUSEEXCEPTIONS
		if (control == 0)
		{
			throw Misc::ArgumentNullException("control", __FILE__, __LINE__);
		}
		#endif

		if (control->GetType() == CONTROL_FORM)
		{
			return;
		}

		AddSubControl(control);

		controls.push_front(control);
	}
	//---------------------------------------------------------------------------
	void ContainerControl::RemoveControl(Control *control)
	{
		if (control == 0)
		{
			return;
		}

		controls.remove(control);
		internalControls.remove(control);
	}
	//---------------------------------------------------------------------------
	void ContainerControl::AddSubControl(Control* subcontrol)
	{
		#ifndef OSHGUI_DONTUSEEXCEPTIONS
		if (subcontrol == 0)
		{
			throw Misc::ArgumentNullException("subcontrol", __FILE__, __LINE__);
		}
		#endif

		if (subcontrol->GetType() == CONTROL_FORM)
		{
			return;
		}
		
		for (std::list<Control*>::iterator it = internalControls.begin(); it != internalControls.end(); ++it)
		{
			Control *control = *it;
			if (subcontrol == control || subcontrol->GetName() != "" && subcontrol->GetName() == control->GetName())
			{
				return;
			}
		}

		subcontrol->SetParent(this);

		internalControls.push_front(subcontrol);
	}
	//---------------------------------------------------------------------------
	bool ContainerControl::Intersect(const Drawing::Point &point) const
	{
		return Intersection::TestRectangle(absoluteLocation, size, point);
	}
	//---------------------------------------------------------------------------
	void ContainerControl::CalculateAbsoluteLocation()
	{
		Control::CalculateAbsoluteLocation();
		
		for (std::list<Control*>::iterator it = internalControls.begin(); it != internalControls.end(); ++it)
		{
			(*it)->CalculateAbsoluteLocation();
		}
	}
	//---------------------------------------------------------------------------
	Control* ContainerControl::GetChildAtPoint(const Drawing::Point &point) const
	{
		for (std::list<Control*>::const_reverse_iterator it = controls.rbegin(); it != controls.rend(); ++it)
		{
			Control *control = *it;

			if (control->GetEnabled() && control->GetVisible() && control->Intersect(point))
			{
				return control;
			}
		}

		return 0;
	}
	//---------------------------------------------------------------------------
	Control* ContainerControl::GetChildByName(const Misc::AnsiString &name) const
	{
		for (std::list<Control*>::const_iterator it = controls.begin(); it != controls.end(); ++it)
		{
			Control *control = *it;
			
			if (control->GetName() == name)
			{
				return control;
			}
		}

		return 0;
	}
	//---------------------------------------------------------------------------
	ContainerControl::PostOrderVisibleIterator ContainerControl::GetPostOrderVisibleEnumerator()
	{
		return PostOrderVisibleIterator(this);
	}
	//---------------------------------------------------------------------------
	void ContainerControl::Render(Drawing::IRenderer *renderer)
	{
		Control *focusedControl = 0;
		for (std::list<Control*>::reverse_iterator it = controls.rbegin(); it != controls.rend(); ++it)
		{
			Control *control = *it;
			if (control->GetIsFocused())
			{
				focusedControl = control;
			}
			else
			{
				control->Render(renderer);
			}
		}
		if (focusedControl != 0)
		{
			focusedControl->Render(renderer);
		}
	}
	//---------------------------------------------------------------------------
	//ContainerControl::PostOrderVisibleIterator
	//---------------------------------------------------------------------------
	ContainerControl::PostOrderVisibleIterator::PostOrderVisibleIterator(ContainerControl *start)
	{
		this->start = start;

		if (this->start->internalControls.empty())
		{
			current = this->start;
		}
		else
		{
			LoopThrough(this->start);
			current = controlStack.top();
			controlStack.pop();
		}
	}
	//---------------------------------------------------------------------------
	void ContainerControl::PostOrderVisibleIterator::LoopThrough(ContainerControl *container)
	{
		controlStack.push(container);
		for (std::list<Control*>::reverse_iterator it = container->internalControls.rbegin(); it != container->internalControls.rend(); ++it)
		{
			Control *control = *it;
			if (control->GetVisible())
			{
				ContainerControl *nextContainer = dynamic_cast<ContainerControl*>(control);
				if (nextContainer != 0)
				{
					LoopThrough(nextContainer);
				}
				else
				{
					controlStack.push(control);
				}
			}
		}
	}
	//---------------------------------------------------------------------------
	void ContainerControl::PostOrderVisibleIterator::operator++()
	{
		current = controlStack.top();
		controlStack.pop();
	}
	//---------------------------------------------------------------------------
	bool ContainerControl::PostOrderVisibleIterator::operator()()
	{
		return !controlStack.empty();
	}
	//---------------------------------------------------------------------------
	Control* ContainerControl::PostOrderVisibleIterator::operator*()
	{
		return current;
	}
	//---------------------------------------------------------------------------
}