/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include "Cursor.hpp"
#include "../Application.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	Cursor::Cursor()
	{
		cursor = Application::Instance()->GetRenderer()->CreateNewTexture(16, 16);
		CreateCursor();
	}
	//---------------------------------------------------------------------------
	Cursor::~Cursor()
	{
	
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	void Cursor::CreateCursor()
	{
		cursor->BeginUpdate();
		cursor->Clear();
		
		cursor->Fill(0, 0, 1, 12, Drawing::Color::White());
		cursor->Fill(1, 0, 1, 11, Drawing::Color::White());
        cursor->Fill(1, 11, 1, 1, Drawing::Color::White());
        cursor->Fill(2, 1, 1, 1, Drawing::Color::White());
        cursor->Fill(2, 10, 1, 1, Drawing::Color::White());
        cursor->Fill(3, 2, 1, 1, Drawing::Color::White());
        cursor->Fill(3, 9, 1, 1, Drawing::Color::White());
        cursor->Fill(4, 3, 1, 1, Drawing::Color::White());
        cursor->Fill(5, 4, 1, 1, Drawing::Color::White());
        cursor->Fill(6, 5, 1, 1, Drawing::Color::White());
        cursor->Fill(7, 6, 1, 1, Drawing::Color::White());
        cursor->Fill(8, 7, 1, 1, Drawing::Color::White());
        cursor->Fill(4, 8, 4, 1, Drawing::Color::White());

        cursor->Fill(1, 1, 1, 10, Drawing::Color::Black());
        cursor->Fill(2, 2, 1, 8, Drawing::Color::Black());
        cursor->Fill(3, 3, 1, 6, Drawing::Color::Black());
        cursor->Fill(4, 4, 1, 4, Drawing::Color::Black());
        cursor->Fill(5, 5, 1, 3, Drawing::Color::Black());
        cursor->Fill(6, 6, 1, 2, Drawing::Color::Black());
        cursor->Fill(7, 7, 1, 1, Drawing::Color::Black());
		
		cursor->EndUpdate();
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	void Cursor::Render(Drawing::IRenderer *renderer, const Drawing::Point &cursorLocation)
	{
		renderer->SetRenderColor(Drawing::Color::White());
		renderer->RenderTexture(cursor, cursorLocation + offset);
	}
	//---------------------------------------------------------------------------
}