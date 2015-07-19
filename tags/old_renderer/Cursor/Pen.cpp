/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include "Pen.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	PenCursor::PenCursor()
	{
		offset = Drawing::Point(0, -15);
		CreateCursor();
	}
	//---------------------------------------------------------------------------
	PenCursor::~PenCursor()
	{
	
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	void PenCursor::CreateCursor()
	{
		cursor->BeginUpdate();
		cursor->Clear();
		
		cursor->Fill(6, 0, 3, 1, Drawing::Color::Black());
		for (int i = 0; i < 5; ++i)
		{
			cursor->Fill(1 + i, 9 - i * 2, 1, 2, Drawing::Color::Black());
			cursor->Fill(5 + i, 9 - i * 2, 1, 2, Drawing::Color::Black());
			cursor->Fill(0, 11 + i, 5 - i, 1, Drawing::Color::Black());
			cursor->Fill(2 + i, 9 - i * 2, 3, 2, Drawing::Color::White());
		}
		cursor->Fill(2, 11, 2, 1, Drawing::Color::White());

		cursor->EndUpdate();
	}
	//---------------------------------------------------------------------------
}