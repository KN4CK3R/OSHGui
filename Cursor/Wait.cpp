#include "Wait.h"
#include "..\Application.h"

namespace OSHGui
{
	const long WaitCursor::interval = 125L;
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	WaitCursor::WaitCursor()
	{
		totalFrames = 14;
		frames.push_back(cursor);
		for (int i = 0; i < 13; ++i)
		{
			std::shared_ptr<Drawing::ITexture> frame = Application::Renderer->CreateNewTexture();
			frame->Create(16, 16);
			frames.push_back(frame);
		}
		
		nextFrame = 1;
		nextFrameTime = Application::Now;
	
		CreateCursor();
	}
	//---------------------------------------------------------------------------
	WaitCursor::~WaitCursor()
	{
	
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	void WaitCursor::CreateCursor()
	{
		int[][][] data =
		{
			{
				{ 0, 0 },
				{ 1, 1 },
				{ 2, 2 },
				{ 3, 3 },
				{ 4, 4 },
				{ 5, 5 }
			},
			{
				{ 3, 0 },
				{ 2, 1 },
				{ 1, 2 },
				{ 0, 3 },
				{ 4, 4 },
				{ 5, 5 }
			},
			{
				{ 5, 0 },
				{ 4, 1 },
				{ 3, 2 },
				{ 2, 3 },
				{ 1, 4 },
				{ 0, 5 }
			},
			{
				{ 7, 0 },
				{ 6, 1 },
				{ 5, 2 },
				{ 4, 3 },
				{ 3, 4 },
				{ 2, 5 }
			},
			{
				{ 9, 0 },
				{ 8, 1 },
				{ 7, 2 },
				{ 6, 3 },
				{ 5, 4 },
				{ 4, 5 }
			},
			{
				{ 11, 0 },
				{ 10, 1 },
				{ 9, 2 },
				{ 8, 3 },
				{ 7, 4 },
				{ 6, 5 }
			},
			{
				{ 13, 0 },
				{ 12, 1 },
				{ 11, 2 },
				{ 10, 3 },
				{ 9, 4 },
				{ 8, 5 }
			},
			{
				{ 12, 0 },
				{ 13, 1 },
				{ 11, 2 },
				{ 10, 3 },
				{ 9, 4 },
				{ 8, 5 }
			},
			{
				{ 10, 0 },
				{ 11, 1 },
				{ 12, 2 },
				{ 13, 3 },
				{ 13, 3 },
				{ 13, 3 }
			},
			{
				{ 8, 0 },
				{ 9, 1 },
				{ 10, 2 },
				{ 11, 3 },
				{ 12, 4 },
				{ 13, 5 }
			},
			{
				{ 6, 0 },
				{ 7, 1 },
				{ 8, 2 },
				{ 9, 3 },
				{ 10, 4 },
				{ 11, 5 }
			},
			{
				{ 4, 0 },
				{ 5, 1 },
				{ 6, 2 },
				{ 7, 3 },
				{ 8, 4 },
				{ 9, 5 }
			},
			{
				{ 2, 0 },
				{ 3, 1 },
				{ 4, 2 },
				{ 5, 3 },
				{ 6, 4 },
				{ 7, 5 }
			},
			{
				{ 0, 0 },
				{ 1, 1 },
				{ 2, 2 },
				{ 3, 3 },
				{ 4, 4 },
				{ 5, 5 }
			}
		};
		
		Drawing::Color colors[] = {
			Drawing::Color(114, 114, 114),
			Drawing::Color(130, 130, 130),
			Drawing::Color(168, 168, 168),
			Drawing::Color(188, 188, 188),
			Drawing::Color(206, 206, 206),
			Drawing::Color(222, 222, 222)
		};
	
		for (int frameNum = 0; frameNum < totalFrames; ++frameNum)
		{
			std::shared_ptr<Drawing::ITexture> frame = frames[frameNum];
		
			frameNum ->BeginUpdate();
			frameNum ->Clear();
			
			//Cursor
			frame->Fill(0, 0, 1, 12, Drawing::Color::White());
			frame->Fill(1, 0, 1, 11, Drawing::Color::White());
			frame->Fill(1, 11, 1, 1, Drawing::Color::White());
			frame->Fill(2, 1, 1, 1, Drawing::Color::White());
			frame->Fill(2, 10, 1, 1, Drawing::Color::White());
			frame->Fill(3, 2, 1, 1, Drawing::Color::White());
			frame->Fill(3, 9, 1, 1, Drawing::Color::White());
			frame->Fill(4, 3, 1, 1, Drawing::Color::White());
			frame->Fill(5, 4, 1, 1, Drawing::Color::White());
			frame->Fill(6, 5, 1, 1, Drawing::Color::White());
			frame->Fill(7, 6, 1, 1, Drawing::Color::White());
			frame->Fill(8, 7, 1, 1, Drawing::Color::White());
			frame->Fill(4, 8, 4, 1, Drawing::Color::White());

			frame->Fill(1, 1, 1, 10, Drawing::Color::Black());
			frame->Fill(2, 2, 1, 8, Drawing::Color::Black());
			frame->Fill(3, 3, 1, 6, Drawing::Color::Black());
			frame->Fill(4, 4, 1, 4, Drawing::Color::Black());
			frame->Fill(5, 5, 1, 3, Drawing::Color::Black());
			frame->Fill(6, 6, 1, 2, Drawing::Color::Black());
			frame->Fill(7, 7, 1, 1, Drawing::Color::Black());

			//loading
			frame->Fill(0, 8, 16, 8, Drawing::Color::Black());
			frame->Fill(1, 9, 14, 6, Drawing::Color::White());
			
			for (int bar = 0; bar < 6; ++bar)
			{
				frame->Fill(1 + data[frameNum][bar][0], 9, 1, 6, colors[data[frameNum][bar][1]]);
			}
			
			frameNum ->EndUpdate();
		}
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	void WaitCursor::Render(Drawing::IRenderer *renderer, const Drawing::Point &cursorPosition)
	{
		if (Application::Now > nextFrameTime)
		{
			cursor = frames[nextFrame++];
			if (nextFrame >= totalFrames)
			{
				nextFrame = 0;
			}
			nextFrameTime = Application::Now.Add(interval);
		}
		Cursor::Render(renderer, cursorPosition);
	}
	//---------------------------------------------------------------------------
}