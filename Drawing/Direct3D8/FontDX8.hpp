/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_DRAWING_FONTDX8_HPP
#define OSHGUI_DRAWING_FONTDX8_HPP

#include "../IFont.hpp"

struct ID3DXFont;
struct IDirect3DDevice8;

namespace OSHGui
{
	namespace Drawing
	{
		class Color;

		class OSHGUI_EXPORT FontDX8 : public IFont
		{
		public:
			/**
			 * Konstruktor der Klasse.
			 *
			 * @param device Zeiger auf ein initialisiertes IDirect3DDevice9-Objekt.
			 */
			FontDX8(IDirect3DDevice8 *device, const Misc::AnsiString &name, int size, bool bold, bool italic);
			virtual ~FontDX8();

			/**
			 * Ruft die Maﬂe des Texts mit dieser Schriftart ab.
			 *
			 * @param str der Text
			 * @return die Maﬂe
			 */
			virtual const Size MeasureText(const Misc::AnsiString &str);

			/**
			 * Zeichnet den angegebenen Text.
			 *
			 * @param x X Position
			 * @param y Y Position
			 * @param color Farbe
			 * @text der Text
			 */
			void RenderText(int x, int y, Color color, const Misc::AnsiString &text);

			void PreReset();
			void PostReset();

		private:
			void Initialize();
			void Destroy();

			IDirect3DDevice8 *device;
			IDirect3DVertexBuffer8 *vertexBuffer;
			struct {
				float Scale;
				int Space;
				Size Size;
				float Coordinates[128 - 32][4];
				IDirect3DTexture8 *Data;
			} Texture;

			static const int maxVertices = 300;
			struct Vertex2D
			{
				float x;
				float y;
				float z;
				float rhw;
				DWORD color;
				float tu;
				float tv;

				Vertex2D(float x, float y, DWORD color, float tu, float tv)
					: x(x),
					  y(y),
					  z(0.9f),
					  rhw(1.0f),
					  color(color),
					  tu(tu),
					  tv(tv)
				{

				}
			};

			DWORD backupStateBlock;
			DWORD drawStateBlock;
		};
	}
}

#endif