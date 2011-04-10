#include "TextureDX9.h"

#ifndef SAFE_RELEASE
	#define SAFE_RELEASE(p) { if (p) { (p)->Release(); (p) = NULL; } }
#endif

namespace OSHGui
{
	namespace Drawing
	{
		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		TextureDX9::TextureDX9(IDirect3DDevice9 *device)
		{
			this->device = device;
			texture = NULL;
			lock.pBits = NULL;
		}
		//---------------------------------------------------------------------------
		TextureDX9::~TextureDX9()
		{
			SAFE_RELEASE(texture);
		}
		//---------------------------------------------------------------------------
		//Getter/Setter
		//---------------------------------------------------------------------------
		LPDIRECT3DTEXTURE9 TextureDX9::GetTexture()
		{
			return texture;
		}
		//---------------------------------------------------------------------------
		bool TextureDX9::IsLocked()
		{
			return lock.pBits != NULL;
		}
		//---------------------------------------------------------------------------
		//Runtime-Functions
		//---------------------------------------------------------------------------
		bool TextureDX9::Create(const Size &size)
		{
			if (this->size == size)
			{
				//return true; //todo (Reset Problem)
			}
			
			SAFE_RELEASE(texture);
			if (!FAILED(device->CreateTexture(size.Width, size.Height, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &texture, NULL)))
			{
				this->size = size;
				return true;
			}
			return false;
		}
		//---------------------------------------------------------------------------
		void TextureDX9::BeginUpdate()
		{
			texture->LockRect(0, &lock, 0, 0);
		}
		//---------------------------------------------------------------------------
		void TextureDX9::EndUpdate()
		{
			texture->UnlockRect(0);
			lock.pBits = NULL;
		}
		//---------------------------------------------------------------------------
		void TextureDX9::Clear()
		{
			Fill(0, 0, size.Width, size.Height, Color::Empty());
		}
		//---------------------------------------------------------------------------
		void TextureDX9::Clear(int x, int y)
		{
			Fill(x, y, 1, 1, Color::Empty());
		}
		//---------------------------------------------------------------------------
		void TextureDX9::Clear(const Drawing::Point &point)
		{
			Fill(point.X, point.Y, 1, 1, Color::Empty());
		}
		//---------------------------------------------------------------------------
		void TextureDX9::Clear(Drawing::Rectangle &rect)
		{
			Fill(rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight(), Color::Empty());
		}
		//---------------------------------------------------------------------------
		void TextureDX9::Clear(int x, int y, int w, int h)
		{
			Fill(x, y, w, h, Color::Empty());
		}
		//---------------------------------------------------------------------------
		void TextureDX9::Fill(Drawing::Color color)
		{
			Fill(0, 0, size.Width, size.Height, color);
		}
		//---------------------------------------------------------------------------
		void TextureDX9::Fill(int x, int y, Drawing::Color color)
		{
			Fill(x, y, 1, 1, color);
		}
		//---------------------------------------------------------------------------
		void TextureDX9::Fill(const Drawing::Point &point, Drawing::Color color)
		{
			Fill(point.X, point.Y, 1, 1, color);
		}
		//---------------------------------------------------------------------------
		void TextureDX9::Fill(Drawing::Rectangle &rect, Drawing::Color color)
		{
			Fill(rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight(), color);
		}
		//---------------------------------------------------------------------------
		void TextureDX9::Fill(int _x, int _y, int _w, int _h, Drawing::Color _color)
		{
			BYTE color[4] = { _color.B, _color.G, _color.R, _color.A };
		
			int rangeX = _w - _x;
			int rangeY = _h - _y;

			BYTE *raw = (BYTE*)lock.pBits;
			for (int y = 0; y < rangeY; y++)
			{
				int row = (_y + y) * lock.Pitch;
				
				for (int x = 0; x < rangeX; x++)
				{
					int index = (_x + x) * 4 + row;
					
					raw[index] = color[0];
					raw[index + 1] = color[1];
					raw[index + 2] = color[2];
					raw[index + 3] = color[3];
				}
			}
		}
		//---------------------------------------------------------------------------
		void TextureDX9::FillGradient(Drawing::Color from, Drawing::Color to, bool updown)
		{
			FillGradient(0, 0, size.Width, size.Height, from, to, updown);
		}
		//---------------------------------------------------------------------------
		void TextureDX9::FillGradient(Drawing::Rectangle &rect, Drawing::Color from, Drawing::Color to, bool updown)
		{
			FillGradient(rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight(), from, to, updown);
		}
		//---------------------------------------------------------------------------
		void TextureDX9::FillGradient(int _x, int _y, int _w, int _h, Drawing::Color from, Drawing::Color to, bool updown)
		{
			BYTE color[4] = { from.B, from.G, from.R, from.A };
			float step[4];
		
			BYTE *raw = (BYTE*)lock.pBits;
			
			if (updown)
			{
				int rangeX = _w - _x;
				int rangeY = _h - _y;

				step[0] = (to.B - color[0]) / (float)(rangeY - 1);
				step[1] = (to.G - color[1]) / (float)(rangeY - 1);
				step[2] = (to.R - color[2]) / (float)(rangeY - 1);
				step[3] = (to.A - color[3]) / (float)(rangeY - 1);
				
				for (int y = 0; y < rangeY; y++)
				{
					int row = (_y + y) * lock.Pitch;
					
					for (int x = 0; x < rangeX; x++)
					{
						int index = (_x + x) * 4 + row;
						
						raw[index] = color[0] + (BYTE)(y * step[0]);
						raw[index + 1] = color[1] + (BYTE)(y * step[1]);
						raw[index + 2] = color[2] + (BYTE)(y * step[2]);
						raw[index + 3] = color[3] + (BYTE)(y * step[3]);
					}
				}
			}
			else
			{
				int rangeX = _w - _x;
				int rangeY = _h - _y;

				step[0] = (to.B - color[0]) / (float)(rangeX - 1);
				step[1] = (to.G - color[1]) / (float)(rangeX - 1);
				step[2] = (to.R - color[2]) / (float)(rangeX - 1);
				step[3] = (to.A - color[3]) / (float)(rangeX - 1);
			
				for (int y = 0; y < rangeY; y++)
				{
					int row = (_y + y) * lock.Pitch;
					
					for (int x = 0; x < rangeX; x++)
					{
						int index = (_x + x) * 4 + row;
						
						raw[index] = color[0] + (BYTE)(x * step[0]);
						raw[index + 1] = color[1] + (BYTE)(x * step[1]);
						raw[index + 2] = color[2] + (BYTE)(x * step[2]);
						raw[index + 3] = color[3] + (BYTE)(x * step[3]);
					}
				}
			}
		}
		//---------------------------------------------------------------------------
		void TextureDX9::Rotate(int degrees)
		{
			if (degrees == 0 || degrees == 360)
			{
				return;
			}
					
			LPDIRECT3DTEXTURE9 temp;
			D3DLOCKED_RECT tempLock;
			
			if (degrees == 90 || degrees == 180 || degrees == 270)
			{
				Drawing::Size tempSize = size, bckSize = size;

				if (degrees == 90 || degrees == 270)
				{
					int w = tempSize.Width;
					tempSize.Width = tempSize.Height;
					tempSize.Height = w;
				}
				
				if (!FAILED(device->CreateTexture(tempSize.Width, tempSize.Height, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &temp, NULL)))
				{
					size = tempSize;
					tempSize.Inflate(-1, -1);

					temp->LockRect(0, &tempLock, 0, 0);
				
					BYTE *raw = (BYTE*)lock.pBits;
					BYTE *tempRaw = (BYTE*)tempLock.pBits;
					
					if (degrees == 90)
					{
						for (int y = 0; y < bckSize.Height; y++)
						{
							int row = y * lock.Pitch,
								tempRow = (tempSize.Width - y) * 4;
								
							for (int x = 0; x < bckSize.Width; x++)
							{
								int index = x * 4 + row,
									tempIndex = tempRow + x * tempLock.Pitch;
									
								tempRaw[tempIndex] = raw[index];
								tempRaw[tempIndex + 1] = raw[index + 1];
								tempRaw[tempIndex + 2] = raw[index + 2];
								tempRaw[tempIndex + 3] = raw[index + 3];
							}
						}
					}
					else if (degrees == 180)
					{
						for (int y = 0; y < bckSize.Height; y++)
						{
							int row = y * lock.Pitch,
								tempRow = (tempSize.Height - y) * tempLock.Pitch;
								
							for (int x = 0; x < bckSize.Width; x++)
							{
								int index = x * 4 + row,
									tempIndex = (tempSize.Width - x) * 4 + tempRow;
									
								tempRaw[tempIndex] = raw[index];
								tempRaw[tempIndex + 1] = raw[index + 1];
								tempRaw[tempIndex + 2] = raw[index + 2];
								tempRaw[tempIndex + 3] = raw[index + 3];
							}
						}
					}
					else if (degrees == 270)
					{
						for (int y = 0; y < bckSize.Height; y++)
						{
							int row = y * lock.Pitch,
								tempRow = y * 4;
								
							for (int x = 0; x < bckSize.Width; x++)
							{
								int index = x * 4 + row,
									tempIndex = tempRow + (tempSize.Width - x) * tempLock.Pitch;
									
								tempRaw[tempIndex] = raw[index];
								tempRaw[tempIndex + 1] = raw[index + 1];
								tempRaw[tempIndex + 2] = raw[index + 2];
								tempRaw[tempIndex + 3] = raw[index + 3];
							}
						}
					}
					
					texture->UnlockRect(0);
					temp->UnlockRect(0);
					SAFE_RELEASE(texture);
					
					texture = temp;
					texture->LockRect(0, &lock, 0, 0);
				}
			}
			else
			{
				float radians = (2 * 3.1416f * degrees) / 360; 

				float cosine = (float)cos(radians); 
				float sine = (float)sin(radians); 

				float p1x = (-size.Height * sine); 
				float p1y = (size.Height * cosine); 
				float p2x = (size.Width * cosine - size.Height * sine); 
				float p2y = (size.Height * cosine + size.Width * sine); 
				float p3x = (size.Width * cosine); 
				float p3y = (size.Width * sine); 

				float minx = _min(0, _min(p1x, _min(p2x, p3x)));
				float miny = _min(0, _min(p1y, _min(p2y, p3y)));
				float maxx = _max(p1x, _max(p2x, p3x));
				float maxy = _max(p1y, _max(p2y, p3y));

				Drawing::Size tempSize((int)ceil(fabs(maxx) - minx), (int)ceil(fabs(maxy) - miny)),
							  bckSize = size;

				if (!FAILED(device->CreateTexture(tempSize.Width, tempSize.Height, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &temp, NULL)))
				{
					size = tempSize;

					temp->LockRect(0, &tempLock, 0, 0);
				
					BYTE *raw = (BYTE*)lock.pBits;
					BYTE *tempRaw = (BYTE*)tempLock.pBits;
					
					for(int x = 0; x < tempSize.Width; x++) 
					{
						for(int y = 0; y < tempSize.Height; y++) 
						{ 
							int srcX = (int)((x + minx) * cosine + (y + miny) * sine); 
							int srcY = (int)((y + miny) * cosine - (x + minx) * sine); 
							
							int tempIndex = x * 4 + y * tempLock.Pitch;

							if (srcX >=0 && srcX < bckSize.Width && srcY >= 0 && srcY < bckSize.Height) 
							{
								int index = srcX * 4 + srcY * lock.Pitch;
									
								tempRaw[tempIndex] = raw[index];
								tempRaw[tempIndex + 1] = raw[index + 1];
								tempRaw[tempIndex + 2] = raw[index + 2];
								tempRaw[tempIndex + 3] = raw[index + 3];
							}
							else
							{
								tempRaw[tempIndex] = 0;
								tempRaw[tempIndex + 1] = 0;
								tempRaw[tempIndex + 2] = 0;
								tempRaw[tempIndex + 3] = 0;
							}
						} 
					}
					
					texture->UnlockRect(0);
					temp->UnlockRect(0);
					SAFE_RELEASE(texture);
					
					texture = temp;
					texture->LockRect(0, &lock, 0, 0);
				}
			}
		}
		//---------------------------------------------------------------------------
		void TextureDX9::Insert(const Drawing::Point &point, Drawing::TextureDX9 *texture)
		{
			Insert(point.X, point.Y, texture);
		}
		//---------------------------------------------------------------------------
		void TextureDX9::Insert(int _x, int _y, Drawing::ITexture *texture)
		{
			if (texture == NULL || texture->IsLocked())
			{
				return;
			}
		
			LPDIRECT3DTEXTURE9 temp = ((TextureDX9*)texture)->GetTexture();
			
			if (temp == NULL)
			{
				return;
			}
			
			Drawing::Size tempSize = texture->GetSize();
			
			if (tempSize.Width + _x > size.Width || tempSize.Height + _y > size.Height)
			{
				return;
			}
			
			D3DLOCKED_RECT tempLock;
			temp->LockRect(0, &tempLock, 0, 0);
		
			BYTE *raw = (BYTE*)lock.pBits;
			BYTE *tempRaw = (BYTE*)tempLock.pBits;
			
			for (int y = 0; y < tempSize.Height; y++)
			{
				int row = (y + _y) * lock.Pitch,
					tempRow = y * tempLock.Pitch;
			
				for (int x = 0; x < tempSize.Width; x++)
				{
					int index = (x + _x) * 4 + row,
						tempIndex = x * 4 + tempRow;
						
					if (tempRaw[tempIndex + 3] > 0)
					{
						raw[index] = tempRaw[tempIndex];
						raw[index + 1] = tempRaw[tempIndex + 1];
						raw[index + 2] = tempRaw[tempIndex + 2];
						raw[index + 3] = tempRaw[tempIndex + 3];
					}
				}
			}
			
			temp->UnlockRect(0);
		}
		//---------------------------------------------------------------------------
	}
}