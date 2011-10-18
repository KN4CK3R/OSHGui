#include "Circle.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	CircleCursor::CircleCursor()
	{
		offset = Drawing::Point(-5, -5);
		CreateCursor();
	}
	//---------------------------------------------------------------------------
	CircleCursor::~CircleCursor()
	{
	
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	void CircleCursor::CreateCursor()
	{
		cursor->BeginUpdate();
		cursor->Clear();
		
		cursor->Fill(3, 0, 5, 11, Drawing::Color::Black());
		cursor->Fill(0, 3, 11, 5, Drawing::Color::Black());
		cursor->Fill(1, 2, 9, 7, Drawing::Color::Black());
		cursor->Fill(2, 1, 7, 9, Drawing::Color::Black());
		
		cursor->Fill(3, 1, 5, 9, Drawing::Color::White());
		cursor->Fill(1, 3, 9, 5, Drawing::Color::White());
		cursor->Fill(2, 2, 7, 7, Drawing::Color::White());

		cursor->Clear(3, 2, 5, 7);
		cursor->Clear(2, 3, 7, 5);
		
		cursor->EndUpdate();
	}
	//---------------------------------------------------------------------------
}