/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include "Hand.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	HandCursor::HandCursor()
	{
		offset = Drawing::Point(-5, 0);
		CreateCursor();
	}
	//---------------------------------------------------------------------------
	HandCursor::~HandCursor()
	{
	
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	void HandCursor::CreateCursor()
	{
		cursor->BeginUpdate();
		cursor->Clear();
		
		cursor->Fill(6, 5, 7, 9, Drawing::Color::Black());
		cursor->Fill(0, 7, 1, 2, Drawing::Color::White());
		cursor->Fill(1, 6, 1, 1, Drawing::Color::White());
		for (int i = 0; i < 3; ++i)
		{
			cursor->Fill(2 + i, 6 + i, 1, 1, Drawing::Color::White());
		}
		for (int i = 0; i < 4; ++i)
		{
			cursor->Fill(1 + i, 9 + i, 1, 1, Drawing::Color::White());
			cursor->Fill(1 + i, 7 + i, 1, 1, Drawing::Color::Black());
			cursor->Fill(2 + i, 7 + i, 1, 1, Drawing::Color::Black());
			cursor->Fill(1 + i, 8 + i, 1, 1, Drawing::Color::Black());
			cursor->Fill(3 + i, 11 + i, 1, 1, Drawing::Color::Black());
		}
		cursor->Fill(4, 1, 1, 7, Drawing::Color::White());
		cursor->Fill(5, 0, 2, 1, Drawing::Color::White());
		cursor->Fill(5, 15, 5, 1, Drawing::Color::White());
		cursor->Fill(7, 1, 1, 6, Drawing::Color::White());
		cursor->Fill(7, 9, 1, 4, Drawing::Color::White());
		cursor->Fill(9, 9, 1, 4, Drawing::Color::White());
		cursor->Fill(11, 9, 1, 4, Drawing::Color::White());
		cursor->Fill(8, 4, 4, 1, Drawing::Color::White());
		cursor->Fill(10, 5, 1, 2, Drawing::Color::White());
		cursor->Fill(12, 5, 1, 2, Drawing::Color::White());
		cursor->Fill(13, 5, 1, 1, Drawing::Color::White());
		cursor->Fill(14, 6, 1, 5, Drawing::Color::White());
		cursor->Fill(13, 11, 1, 2, Drawing::Color::White());
		cursor->Fill(12, 13, 1, 3, Drawing::Color::White());
		cursor->Fill(5, 1, 2, 13, Drawing::Color::Black());
		cursor->Fill(7, 14, 3, 1, Drawing::Color::Black());
		cursor->Fill(13, 6, 1, 5, Drawing::Color::Black());
		cursor->Fill(11, 14, 1, 1, Drawing::Color::Black());
		for (int i = 0; i < 2; ++i)
		{
			cursor->Fill(10 + i, 14 + i, 1, 1, Drawing::Color::White());
			cursor->Fill(4 + i, 13 + i, 1, 1, Drawing::Color::White());
		}
		
		cursor->EndUpdate();
	}
	//---------------------------------------------------------------------------
}