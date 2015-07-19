/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include "TextureDX9.hpp"
#include "RendererDX9.hpp"
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
		TextureDX9::TextureDX9(RendererDX9 *renderer, IDirect3DDevice9 *device, const Size &size, int frameCount, const Misc::TimeSpan &frameChangeInterval)
			: renderer(renderer),
			  device(device),
			  frame(0)
		{
			lock.pBits = nullptr;
			
			this->frameChangeInterval = frameChangeInterval;

			Create(size, frameCount);
		}
		//---------------------------------------------------------------------------
		TextureDX9::TextureDX9(RendererDX9 *renderer, IDirect3DDevice9 *device, const Misc::AnsiString &filename)
			: renderer(renderer),
			  device(device),
			  frame(0)
		{
			lock.pBits = nullptr;
			frameChangeInterval = Misc::TimeSpan::FromMilliseconds(125);

			LoadFromFile(filename);
		}
		//---------------------------------------------------------------------------
		TextureDX9::~TextureDX9()
		{
			ClearInternalData();
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
		void TextureDX9::ClearInternalData()
		{
			if (IsLocked())
			{
				#ifndef OSHGUI_DONTUSEEXCEPTIONS
				throw Misc::Exception("Texture is locked.", __FILE__, __LINE__);
				#else
				throw 1;
				#endif
			}

			for (std::size_t i = 0; i < frames.size(); ++i)
			{
				frames[i]->Release();
			}
			frames.clear();
			framesData.clear();
			texture = nullptr;
		}
		//---------------------------------------------------------------------------
		void TextureDX9::Create(const Size &size, int frameCount)
		{
			if (frameCount < 1)
			{
				frameCount = 1;
			}

			this->size = size;
			realSize = renderer->AdjustSize(size);

			for (int i = 0; i < frameCount; ++i)
			{
				if (FAILED(device->CreateTexture(realSize.Width, realSize.Height, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &texture, 0)))
				{
					#ifndef OSHGUI_DONTUSEEXCEPTIONS
					throw Misc::Exception("Cannot create Texture.", __FILE__, __LINE__);
					#else
					throw 1;
					#endif
				}
				frames.push_back(texture);

				D3DLOCKED_RECT lock = { 0 };
				if (SUCCEEDED(texture->LockRect(0, &lock, 0, 0)))
				{
					framesData.push_back(std::vector<Color>(realSize.Height * lock.Pitch / 4));

					texture->UnlockRect(0);
				}
			}
			
			SelectActiveFrame(0);
		}
		//---------------------------------------------------------------------------
		void TextureDX9::LoadFromFile(const Misc::AnsiString &filename)
		{
			ClearInternalData();

			D3DXIMAGE_INFO info;
			if (FAILED(D3DXGetImageInfoFromFileA(filename.c_str(), &info)))
			{
				#ifndef OSHGUI_DONTUSEEXCEPTIONS
				throw Misc::Exception("Cannot get ImageInfo.", __FILE__, __LINE__);
				#else
				throw 1;
				#endif
			}
			
			size = Size(info.Width, info.Height);
			realSize = renderer->AdjustSize(size);
			
			frames.push_back(nullptr);
			framesData.push_back(std::vector<Color>());

			if (FAILED(D3DXCreateTextureFromFileExA(device, filename.c_str(), realSize.Width, realSize.Height, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, nullptr, nullptr, &frames[frame])))
			{
				frames[frame] = nullptr;
				texture = nullptr;
				
				#ifndef OSHGUI_DONTUSEEXCEPTIONS
				throw Misc::Exception("Cannot load Texture.", __FILE__, __LINE__);
				#else
				throw 1;
				#endif
			}

			SelectActiveFrame(frame);
			
			D3DLOCKED_RECT lock = { 0 };
			if (SUCCEEDED(texture->LockRect(0, &lock, 0, 0)))
			{
				Color *raw = (Color*)lock.pBits;
				std::vector<Color> data(raw, raw + realSize.Height * lock.Pitch / 4);
				framesData[frame] = data;

				texture->UnlockRect(0);
			}
		}
		//---------------------------------------------------------------------------
		void TextureDX9::LoadFromWin32Resource(HMODULE module, LPCSTR name)
		{
			ClearInternalData();

			D3DXIMAGE_INFO info;
			if (FAILED(D3DXGetImageInfoFromResourceA(module, name, &info)))
			{
				#ifndef OSHGUI_DONTUSEEXCEPTIONS
				throw Misc::Exception("Cannot get ImageInfo.", __FILE__, __LINE__);
				#else
				throw 1;
				#endif
			}

			size = Drawing::Size(info.Width, info.Height);
			realSize = renderer->AdjustSize(size);

			frames.push_back(nullptr);
			framesData.push_back(std::vector<Color>());
			
			if (FAILED(D3DXCreateTextureFromResourceExA(device, module, name, realSize.Width, realSize.Height, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, nullptr, nullptr, &frames[frame])))
			{
				frames[frame] = nullptr;
				texture = nullptr;

				#ifndef OSHGUI_DONTUSEEXCEPTIONS
				throw Misc::Exception("Cannot load Texture.", __FILE__, __LINE__);
				#else
				throw 1;
				#endif
			}

			SelectActiveFrame(frame);

			D3DLOCKED_RECT lock = { 0 };
			if (SUCCEEDED(texture->LockRect(0, &lock, 0, 0)))
			{
				Color *raw = (Color*)lock.pBits;
				std::vector<Color> data(raw, raw + realSize.Height * lock.Pitch / 4);
				framesData[frame] = data;

				texture->UnlockRect(0);
			}
		}
		//---------------------------------------------------------------------------
		void TextureDX9::BeginUpdate()
		{
			texture->LockRect(0, &lock, nullptr, D3DLOCK_DISCARD);
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

			auto& data = framesData[frame];
			int *raw = static_cast<int*>(lock.pBits);

			for (int j = 0; j < toY; ++j)
			{
				int row = (fromY + j) * lock.Pitch / 4;

				for (int i = 0; i < toX; ++i)
				{
					int index = (fromX + i) + row;

					raw[index] = _color.ARGB;
					data[index] = _color;
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

			auto& data = framesData[frame];
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
						data[index] = newColor;
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
						data[index] = newColor;
					}
				}
			}
		}
		//---------------------------------------------------------------------------
		int TextureDX9::GetFrameCount() const
		{
			return frames.size();
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
			for (std::size_t i = 0; i < frames.size(); ++i)
			{
				frames[i]->Release();
			}
			frames.clear();
		}
		//---------------------------------------------------------------------------
		void TextureDX9::PostReset()
		{
			D3DLOCKED_RECT lock = { 0 };
			for (std::size_t i = 0; i < framesData.size(); ++i)
			{
				if (FAILED(device->CreateTexture(realSize.Width, realSize.Height, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &texture, 0)))
				{
					#ifndef OSHGUI_DONTUSEEXCEPTIONS
					throw Misc::Exception("Cannot create Texture.", __FILE__, __LINE__);
					#else
					throw 1;
					#endif
				}
				
				long result = texture->LockRect(0, &lock, 0, 0);
				if (SUCCEEDED(result))
				{
					std::memcpy(lock.pBits, framesData[i].data(), framesData[i].size() * 4);

					texture->UnlockRect(0);

					frames.push_back(texture);
				}
			}
			
			SelectActiveFrame(0);
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