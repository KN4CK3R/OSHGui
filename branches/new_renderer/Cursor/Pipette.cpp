/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include "Pipette.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	PipetteCursor::PipetteCursor()
	{
		offset = Drawing::PointF(0, -15);
		CreateCursor();
	}
	//---------------------------------------------------------------------------
	PipetteCursor::~PipetteCursor()
	{
	
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	void PipetteCursor::CreateCursor()
	{
		cursor->BeginUpdate();
		cursor->Clear();
		
		cursor->Fill(0, 14, 1, 1, Drawing::Color::Black());
		for (int i = 0; i < 2; ++i)
		{
			cursor->Fill(0 + i, 13 - i, 1, 1, Drawing::Color::Black());
			cursor->Fill(1 + i, 14 - i, 1, 1, Drawing::Color::Black());
		}
		for (int i = 0; i < 6; ++i)
		{
			cursor->Fill(1 + i, 11 - i, 1, 1, Drawing::Color::Black());
			cursor->Fill(3 + i, 13 - i, 1, 1, Drawing::Color::Black());

			cursor->Fill(2 + i, 11 - i, 1, 1, Drawing::Color::White());
			cursor->Fill(3 + i, 12 - i, 1, 1, Drawing::Color::White());
			cursor->Fill(2 + i, 12 - i, 1, 1, Drawing::Color::White());
		}
		cursor->Fill(8, 6, 1, 1, Drawing::Color::Black());
		cursor->Fill(7, 3, 6, 3, Drawing::Color::Black());
		cursor->Fill(9, 2, 3, 6, Drawing::Color::Black());
		cursor->Fill(11, 0, 3, 5, Drawing::Color::Black());
		cursor->Fill(10, 1, 5, 3, Drawing::Color::Black());
		
		cursor->EndUpdate();
	}
	//---------------------------------------------------------------------------
}