#include "TextureDX9.hpp"
#include <D3dx9tex.h>
#include "..\Misc\Exceptions.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		TextureDX9::TextureDX9(IDirect3DDevice9 *device, const Size &size, int frameCount, Misc::TimeSpan frameChangeInterval)
		{
			this->device = device;
			lock.pBits = 0;
			frame = 0;
			this->frameChangeInterval = frameChangeInterval;

			Create(size, frameCount);
		}
		//---------------------------------------------------------------------------
		TextureDX9::TextureDX9(IDirect3DDevice9 *device, int width, int height, int frameCount, Misc::TimeSpan frameChangeInterval)
		{
			this->device = device;
			lock.pBits = 0;
			frame = 0;
			this->frameChangeInterval = frameChangeInterval;

			Create(Size(width, height), frameCount);
		}
		//---------------------------------------------------------------------------
		TextureDX9::TextureDX9(IDirect3DDevice9 *device, const Misc::UnicodeString &filename)
		{
			this->device = device;
			lock.pBits = 0;
			frame = 0;
			frameChangeInterval = Misc::TimeSpan::FromMilliseconds(125);

			LoadFromFile(filename);
		}
		//---------------------------------------------------------------------------
		TextureDX9::~TextureDX9()
		{
			if (IsLocked())
			{
				EndUpdate();
			}
		
			if (texture != 0)
			{
				texture->Release();
			}
		}
		//---------------------------------------------------------------------------
		//Getter/Setter
		//---------------------------------------------------------------------------
		IDirect3DTexture9* TextureDX9::GetTexture() const
		{
			return texture;
		}
		//---------------------------------------------------------------------------
		bool TextureDX9::IsLocked() const
		{
			return lock.pBits != 0;
		}
		//---------------------------------------------------------------------------
		//Runtime-Functions
		//---------------------------------------------------------------------------
		void TextureDX9::Create(const Size &size, int frameCount)
		{
			if (frameCount < 1)
			{
				frameCount = 1;
			}

			for (int i = 0; i < frameCount; ++i)
			{			
				if (FAILED(device->CreateTexture(size.Width, size.Height, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &texture, 0)))
				{
					throw Misc::Exception(L"Cannot create Texture.", __WFILE__, __LINE__);
				}
				frames.push_back(texture);
			}

			this->size = size;
			
			texture = frames[0];
		}
		//---------------------------------------------------------------------------
		void TextureDX9::LoadFromFile(const Misc::UnicodeString &filename)
		{			
			D3DXIMAGE_INFO info;
			if (FAILED(D3DXGetImageInfoFromFileW(filename.c_str(), &info)))
			{
				throw Misc::Exception(L"Cannot get ImageInfo.", __WFILE__, __LINE__);
			}
			
			size = Drawing::Size(info.Width, info.Height);
			
			if (frames[frame] != 0)
			{
				frames[frame]->Release();
			}
			
			if (FAILED(D3DXCreateTextureFromFileW(device, filename.c_str(), &frames[frame])))
			{
				frames[frame] = 0;
				texture = 0;
				
				throw Misc::Exception(L"Cannot load Texture.", __WFILE__, __LINE__);
			}
			
			texture = frames[frame];
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
			lock.pBits = 0;
		}
		//---------------------------------------------------------------------------
		void TextureDX9::Clear()
		{
			Fill(0, 0, size.Width, size.Height, Color::Empty());
		}
		//---------------------------------------------------------------------------
		void TextureDX9::Clear(const Point &point)
		{
			Fill(point.X, point.Y, 1, 1, Color::Empty());
		}
		//---------------------------------------------------------------------------
		void TextureDX9::Clear(int x, int y)
		{
			Fill(x, y, 1, 1, Color::Empty());
		}
		//---------------------------------------------------------------------------
		void TextureDX9::Clear(const Rectangle &rect)
		{
			Fill(rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight(), Color::Empty());
		}
		//---------------------------------------------------------------------------
		void TextureDX9::Clear(int x, int y, int w, int h)
		{
			Fill(x, y, w, h, Color::Empty());
		}
		//---------------------------------------------------------------------------
		void TextureDX9::Fill(Color color)
		{
			Fill(0, 0, size.Width, size.Height, color);
		}
		//---------------------------------------------------------------------------
		void TextureDX9::Fill(const Point &point, Color color)
		{
			Fill(point.X, point.Y, 1, 1, color);
		}
		//---------------------------------------------------------------------------
		void TextureDX9::Fill(int x, int y, Color color)
		{
			Fill(x, y, 1, 1, color);
		}
		//---------------------------------------------------------------------------
		void TextureDX9::Fill(const Rectangle &rect, Color color)
		{
			Fill(rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight(), color);
		}
		//---------------------------------------------------------------------------
		void TextureDX9::Fill(int x, int y, int w, int h, Color _color)
		{
			BYTE color[4] = { _color.B, _color.G, _color.R, _color.A };
		
			int fromX = w < 0 ? x + w : x;
			int fromY = h < 0 ? y + h : y;
			int toX = std::abs(w);
			int toY = std::abs(h);

			BYTE *raw = (BYTE*)lock.pBits;
			for (int j = 0; j < toY; ++j)
			{
				int row = (fromY + j) * lock.Pitch;
				
				for (int i = 0; i < toX; ++i)
				{
					int index = (fromX + i) * 4 + row;
					
					raw[index] = color[0];
					raw[index + 1] = color[1];
					raw[index + 2] = color[2];
					raw[index + 3] = color[3];
				}
			}
		}
		//---------------------------------------------------------------------------
		void TextureDX9::FillGradient(Color from, Color to, bool updown)
		{
			FillGradient(0, 0, size.Width, size.Height, from, to, updown);
		}
		//---------------------------------------------------------------------------
		void TextureDX9::FillGradient(const Rectangle &rect, Color from, Color to, bool updown)
		{
			FillGradient(rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight(), from, to, updown);
		}
		//---------------------------------------------------------------------------
		void TextureDX9::FillGradient(int j, int i, int w, int h, Color from, Color to, bool updown)
		{
			BYTE color[4] = { from.B, from.G, from.R, from.A };
			float step[4];
		
			BYTE *raw = (BYTE*)lock.pBits;
			int fromX = w < 0 ? j + w : j;
			int fromY = h < 0 ? i + h : i;
			int toX = std::abs(w);
			int toY = std::abs(h);
			
			if (updown)
			{
				step[0] = (to.B - color[0]) / (float)toY;
				step[1] = (to.G - color[1]) / (float)toY;
				step[2] = (to.R - color[2]) / (float)toY;
				step[3] = (to.A - color[3]) / (float)toY;
				
				for (int y = 0; y < toY; ++y)
				{
					int row = (fromY + y) * lock.Pitch;

					BYTE newColor[4] = {
						color[0] + (BYTE)(y * step[0]),
						color[1] + (BYTE)(y * step[1]),
						color[2] + (BYTE)(y * step[2]),
						color[3] + (BYTE)(y * step[3])
					};
					
					for (int x = 0; x < toX; ++x)
					{
						int index = (fromX + x) * 4 + row;
						
						raw[index] = newColor[0];
						raw[index + 1] = newColor[1];
						raw[index + 2] = newColor[2];
						raw[index + 3] = newColor[3];
					}
				}
			}
			else
			{
				step[0] = (to.B - color[0]) / (float)toX;
				step[1] = (to.G - color[1]) / (float)toX;
				step[2] = (to.R - color[2]) / (float)toX;
				step[3] = (to.A - color[3]) / (float)toX;

				for (int x = 0; x < toX; ++x)
				{
					int line = (fromX + x) * 4;

					BYTE newColor[4] = {
						color[0] + (BYTE)(x * step[0]),
						color[1] + (BYTE)(x * step[1]),
						color[2] + (BYTE)(x * step[2]),
						color[3] + (BYTE)(x * step[3])
					};

					for (int y = 0; y < toY; ++y)
					{
						int index = (fromY + y) * lock.Pitch + line;

						raw[index] = newColor[0];
						raw[index + 1] = newColor[1];
						raw[index + 2] = newColor[2];
						raw[index + 3] = newColor[3];
					}
				}
			
				/*for (int y = 0; y < toY; ++y)
				{
					int row = (fromY + y) * lock.Pitch;
					
					for (int x = 0; x < toX; ++x)
					{
						int index = (fromX + x) * 4 + row;
						
						raw[index] = color[0] + (BYTE)(x * step[0]);
						raw[index + 1] = color[1] + (BYTE)(x * step[1]);
						raw[index + 2] = color[2] + (BYTE)(x * step[2]);
						raw[index + 3] = color[3] + (BYTE)(x * step[3]);
					}
				}*/
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
				
				if (!FAILED(device->CreateTexture(tempSize.Width, tempSize.Height, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &temp, 0)))
				{
					size = tempSize;
					tempSize.Inflate(-1, -1);

					temp->LockRect(0, &tempLock, 0, 0);
				
					BYTE *raw = (BYTE*)lock.pBits;
					BYTE *tempRaw = (BYTE*)tempLock.pBits;
					
					if (degrees == 90)
					{
						for (int y = 0; y < bckSize.Height; ++y)
						{
							int row = y * lock.Pitch,
								tempRow = (tempSize.Width - y) * 4;
								
							for (int x = 0; x < bckSize.Width; ++x)
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
						for (int y = 0; y < bckSize.Height; ++y)
						{
							int row = y * lock.Pitch,
								tempRow = (tempSize.Height - y) * tempLock.Pitch;
								
							for (int x = 0; x < bckSize.Width; ++x)
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
						for (int y = 0; y < bckSize.Height; ++y)
						{
							int row = y * lock.Pitch,
								tempRow = y * 4;
								
							for (int x = 0; x < bckSize.Width; ++x)
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
					if (texture != 0)
					{
						texture->Release();
					}
					
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

				float minx = std::min(0.f, std::min(p1x, std::min(p2x, p3x)));
				float miny = std::min(0.f, std::min(p1y, std::min(p2y, p3y)));
				float maxx = std::max(p1x, std::max(p2x, p3x));
				float maxy = std::max(p1y, std::max(p2y, p3y));

				Drawing::Size tempSize((int)ceil(fabs(maxx) - minx), (int)ceil(fabs(maxy) - miny)),
							  bckSize = size;

				if (!FAILED(device->CreateTexture(tempSize.Width, tempSize.Height, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &temp, 0)))
				{
					size = tempSize;

					temp->LockRect(0, &tempLock, 0, 0);
				
					BYTE *raw = (BYTE*)lock.pBits;
					BYTE *tempRaw = (BYTE*)tempLock.pBits;
					
					for(int x = 0; x < tempSize.Width; ++x)
					{
						for(int y = 0; y < tempSize.Height; ++y)
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
					if (texture != 0)
					{
						texture->Release();
					}
					
					texture = temp;
					texture->LockRect(0, &lock, 0, 0);
				}
			}
		}
		//---------------------------------------------------------------------------
		void TextureDX9::Insert(const Point &point, const std::shared_ptr<ITexture> &texture)
		{
			Insert(point.X, point.Y, texture);
		}
		//---------------------------------------------------------------------------
		void TextureDX9::Insert(int j, int i, const std::shared_ptr<ITexture> &texture)
		{
			if (texture == 0 || texture->IsLocked())
			{
				return;
			}
		
			IDirect3DTexture9 *temp = std::static_pointer_cast<TextureDX9>(texture)->GetTexture();
			if (temp == 0)
			{
				return;
			}
			
			Drawing::Size tempSize = texture->GetSize();
			
			if (tempSize.Width + j > size.Width)
			{
				tempSize.Width = size.Width - j;
			}
			if (tempSize.Height + i > size.Height)
			{
				tempSize.Height = size.Height - i;
			}
			
			D3DLOCKED_RECT tempLock;
			temp->LockRect(0, &tempLock, 0, 0);
		
			BYTE *raw = (BYTE*)lock.pBits;
			BYTE *tempRaw = (BYTE*)tempLock.pBits;
			
			for (int y = 0; y < tempSize.Height; ++y)
			{
				int row = (y + i) * lock.Pitch,
					tempRow = y * tempLock.Pitch;
			
				for (int x = 0; x < tempSize.Width; ++x)
				{
					int index = (x + j) * 4 + row,
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
		int TextureDX9::GetFrameCount() const
		{
			return frames.size();
		}
		//---------------------------------------------------------------------------
		const Misc::TimeSpan& TextureDX9::GetFrameChangeInterval() const
		{
			return frameChangeInterval;
		}
		//---------------------------------------------------------------------------
		void TextureDX9::AddFrame(const std::shared_ptr<ITexture> &frame)
		{
			if (frame->GetSize() != GetSize())
			{
				throw 1;
			}

			IDirect3DTexture9 *source = std::static_pointer_cast<TextureDX9>(frame)->GetTexture();
			if (source == 0)
			{
				throw 1;
			}

			IDirect3DTexture9 *copy;
			if (!FAILED(device->CreateTexture(size.Width, size.Height, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &copy, 0)))
			{
				D3DLOCKED_RECT copyLock;
				copy->LockRect(0, &copyLock, 0, 0);
				BYTE *copyRaw = (BYTE*)copyLock.pBits;
				
				D3DLOCKED_RECT sourceLock;
				source->LockRect(0, &sourceLock, 0, 0);
				BYTE *sourceRaw = (BYTE*)sourceLock.pBits;
					
				for (int y = 0; y < size.Height; ++y)
				{
					int row = y * lock.Pitch;
								
					for (int x = 0; x < size.Width; ++x)
					{
						int index = x * 4 + row;
								
						copyRaw[index] = sourceRaw[index];
						copyRaw[index + 1] = sourceRaw[index + 1];
						copyRaw[index + 2] = sourceRaw[index + 2];
						copyRaw[index + 3] = sourceRaw[index + 3];
					}
				}

				source->UnlockRect(0);
				copy->UnlockRect(0);

				frames.push_back(copy);
			}
		}
		//---------------------------------------------------------------------------
		void TextureDX9::SelectActiveFrame(int frame)
		{
			if (frame < 0 || frame >= GetFrameCount())
			{
				frame = 0;
			}
			
			this->frame = frame;

			texture = frames[this->frame];
		}
		//---------------------------------------------------------------------------
	}
}