/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include "TextureDX9.hpp"
#define NOMINMAX
#include <D3dx9tex.h>
#include "../../Misc/Exceptions.hpp"

#include <fstream>

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
			lock.pBits = nullptr;
			frame = 0;
			this->frameChangeInterval = frameChangeInterval;

			Create(size, frameCount);
		}
		//---------------------------------------------------------------------------
		TextureDX9::TextureDX9(IDirect3DDevice9 *device, int width, int height, int frameCount, Misc::TimeSpan frameChangeInterval)
		{
			this->device = device;
			lock.pBits = nullptr;
			frame = 0;
			this->frameChangeInterval = frameChangeInterval;

			Create(Size(width, height), frameCount);
		}
		//---------------------------------------------------------------------------
		TextureDX9::TextureDX9(IDirect3DDevice9 *device, const Misc::AnsiString &filename)
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
		
			for (std::size_t i = 0; i < frames.size(); ++i)
			{
				frames[i]->Release();
			}
			frames.clear();
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
			return lock.pBits != nullptr;
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
				if (FAILED(device->CreateTexture(size.Width, size.Height, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &texture, 0)))
				{
					#ifndef OSHGUI_DONTUSEEXCEPTIONS
					throw Misc::Exception("Cannot create Texture.", __FILE__, __LINE__);
					#else
					throw 1;
					#endif
				}
				frames.push_back(texture);
			}

			this->size = size;
			
			texture = frames[0];
		}
		//---------------------------------------------------------------------------
		void TextureDX9::LoadFromFile(const Misc::AnsiString &filename)
		{
			D3DXIMAGE_INFO info;
			if (FAILED(D3DXGetImageInfoFromFileA(filename.c_str(), &info)))
			{
				#ifndef OSHGUI_DONTUSEEXCEPTIONS
				throw Misc::Exception("Cannot get ImageInfo.", __FILE__, __LINE__);
				#else
				throw 1;
				#endif
			}
			
			size = Drawing::Size(info.Width, info.Height);
			
			if (!frames.empty() && frames[frame] != nullptr)
			{
				frames[frame]->Release();
			}
			else
			{
				frames.push_back(nullptr);
			}
			
			if (FAILED(D3DXCreateTextureFromFileA(device, filename.c_str(), &frames[frame])))
			{
				if (!frames.empty())
				{
					frames[frame] = nullptr;
				}
				texture = nullptr;
				
				#ifndef OSHGUI_DONTUSEEXCEPTIONS
				throw Misc::Exception("Cannot load Texture.", __FILE__, __LINE__);
				#else
				throw 1;
				#endif
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
			lock.pBits = nullptr;
		}
		//---------------------------------------------------------------------------
		void TextureDX9::Fill(int x, int y, int w, int h, Color _color)
		{		
			int fromX = w < 0 ? x + w : x;
			int fromY = h < 0 ? y + h : y;
			int toX = std::abs(w);
			int toY = std::abs(h);

			int *raw = static_cast<int*>(lock.pBits);
			for (int j = 0; j < toY; ++j)
			{
				int row = (fromY + j) * lock.Pitch / 4;

				for (int i = 0; i < toX; ++i)
				{
					int index = (fromX + i) + row;

					raw[index] = _color.ARGB;
				}
			}
		}
		//---------------------------------------------------------------------------
		void TextureDX9::FillGradient(int j, int i, int w, int h, Color from, Color to, bool updown)
		{
			float step[4];
		
			int fromX = w < 0 ? j + w : j;
			int fromY = h < 0 ? i + h : i;
			int toX = std::abs(w);
			int toY = std::abs(h);

			int *raw = static_cast<int*>(lock.pBits);
			
			if (updown)
			{
				step[0] = (to.B - from.B) / (float)toY;
				step[1] = (to.G - from.G) / (float)toY;
				step[2] = (to.R - from.R) / (float)toY;
				step[3] = (to.A - from.A) / (float)toY;
				
				for (int y = 0; y < toY; ++y)
				{
					int row = (fromY + y) * lock.Pitch / 4;

					Drawing::Color newColor(
						from.B + (y * step[0]),
						from.G + (y * step[1]),
						from.R + (y * step[2]),
						from.A + (y * step[3])
					);
					
					for (int x = 0; x < toX; ++x)
					{
						int index = (fromX + x) + row;
						
						raw[index] = newColor.ARGB;
					}
				}
			}
			else
			{
				step[0] = (to.B - from.B) / (float)toX;
				step[1] = (to.G - from.G) / (float)toX;
				step[2] = (to.R - from.R) / (float)toX;
				step[3] = (to.A - from.A) / (float)toX;

				for (int x = 0; x < toX; ++x)
				{
					int column = (fromX + x);

					Drawing::Color newColor(						
						from.A + (x * step[3]),
						from.R + (x * step[2]),
						from.G + (x * step[1]),
						from.B + (x * step[0])
					);

					for (int y = 0; y < toY; ++y)
					{
						int index = (fromY + y) * lock.Pitch / 4 + column;

						raw[index] = newColor.ARGB;
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
				
				if (!FAILED(device->CreateTexture(tempSize.Width, tempSize.Height, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &temp, 0)))
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
					if (texture != nullptr)
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

				if (!FAILED(device->CreateTexture(tempSize.Width, tempSize.Height, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &temp, 0)))
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
					if (texture != nullptr)
					{
						texture->Release();
					}
					
					texture = temp;
					texture->LockRect(0, &lock, 0, 0);
				}
			}
		}
		//---------------------------------------------------------------------------
		void TextureDX9::Insert(int j, int i, const std::shared_ptr<ITexture> &texture)
		{
			if (texture == nullptr || texture->IsLocked())
			{
				return;
			}
		
			IDirect3DTexture9 *temp = std::static_pointer_cast<TextureDX9>(texture)->GetTexture();
			if (temp == nullptr)
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
		void TextureDX9::AddFrame(const std::shared_ptr<ITexture> &frame)
		{
			if (frame->GetSize() != GetSize())
			{
				throw 1;
			}

			IDirect3DTexture9 *source = std::static_pointer_cast<TextureDX9>(frame)->GetTexture();
			if (source == nullptr)
			{
				throw 1;
			}

			IDirect3DTexture9 *copy;
			if (!FAILED(device->CreateTexture(size.Width, size.Height, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &copy, 0)))
			{
				D3DLOCKED_RECT copyLock;
				copy->LockRect(0, &copyLock, 0, 0);
				BYTE *copyRaw = (BYTE*)copyLock.pBits;
				
				D3DLOCKED_RECT sourceLock;
				source->LockRect(0, &sourceLock, 0, 0);
				BYTE *sourceRaw = (BYTE*)sourceLock.pBits;
				
				std::memcpy(copyRaw, sourceRaw, size.Height * lock.Pitch);

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
		void TextureDX9::PreReset()
		{
			D3DLOCKED_RECT lock;
			for (std::size_t i = 0; i < frames.size(); ++i)
			{
				frames[i]->LockRect(0, &lock, 0, 0);
				
				unsigned char *raw = (unsigned char*)lock.pBits;
				std::vector<unsigned char> data(raw, raw + size.Height * lock.Pitch);
				framesReset.push_back(data);
				
				frames[i]->UnlockRect(0);
				frames[i]->Release();
			}
			frames.clear();
		}
		//---------------------------------------------------------------------------
		void TextureDX9::PostReset()
		{
			D3DLOCKED_RECT lock;
			for (std::size_t i = 0; i < framesReset.size(); ++i)
			{
				if (FAILED(device->CreateTexture(size.Width, size.Height, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &texture, 0)))
				{
					#ifndef OSHGUI_DONTUSEEXCEPTIONS
					throw Misc::Exception("Cannot create Texture.", __FILE__, __LINE__);
					#else
					throw 1;
					#endif
				}
				
				texture->LockRect(0, &lock, 0, 0);
			
				std::memcpy(lock.pBits, framesReset[i].data(), size.Height * lock.Pitch);
				
				texture->UnlockRect(0);
				
				frames.push_back(texture);
			}
			
			texture = frames[0];
			
			framesReset.clear();
		}
		//---------------------------------------------------------------------------
		TextureDX9::TextureDX9Iterator::TextureDX9Iterator(const D3DLOCKED_RECT &lock)
			: data(static_cast<int*>(lock.pBits)),
			  pitch(lock.Pitch),
			  position(0)
		{
			
		}
		//---------------------------------------------------------------------------
		void TextureDX9::TextureDX9Iterator::GoTo(int x, int y)
		{
			this->x = x;
			this->y = y;
			position = y * pitch / 4 + x;
		}
		//---------------------------------------------------------------------------
		void TextureDX9::TextureDX9Iterator::NextLine()
		{
			position = ++y * pitch / 4 + x;
		}
		//---------------------------------------------------------------------------
		void TextureDX9::TextureDX9Iterator::NextColumn()
		{
			++x;
			++position;
		}
		//---------------------------------------------------------------------------
		void TextureDX9::TextureDX9Iterator::SetColor(const Drawing::Color &color)
		{
			data[position] = color.ARGB;
		}
		//---------------------------------------------------------------------------
		const Drawing::Color TextureDX9::TextureDX9Iterator::GetColor() const
		{
			return Color(data[position]);
		}
		//---------------------------------------------------------------------------
	}
}