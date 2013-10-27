/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_DRAWING_RENDERER_HPP
#define OSHGUI_DRAWING_RENDERER_HPP

#include "../Misc/Strings.hpp"
#include "Size.hpp"
#include "Point.hpp"
#include <cstdint>
#include <memory>

namespace OSHGui
{
	namespace Drawing
	{
		class Texture;
		typedef std::shared_ptr<Texture> TexturePtr;
		class TextureTarget;
		typedef std::shared_ptr<TextureTarget> TextureTargetPtr;
		class RenderTarget;
		typedef std::shared_ptr<RenderTarget> RenderTargetPtr;
		class GeometryBuffer;
		typedef std::shared_ptr<GeometryBuffer> GeometryBufferPtr;

		/**
		 * Aufz�hlung der verschiedenen BlendMode Typen.
		 */
		enum class BlendMode
		{
			Invalid,
			Normal,
			RTT_PreMultiplied
		};

		/**
		 * Abstrakte Klasse zur Implementierung der Zeichensysteme.
		 */
		class OSHGUI_EXPORT Renderer
		{
		public:
			/**
			 * Destruktor der Klasse.
			 */
			virtual ~Renderer() {}

			/**
			 * Ruft das Standard RenderTarget ab. Dies ist meistens der komplette Bildschirm.
			 *
			 * \return Standard RenderTarget
			 */
			virtual RenderTargetPtr& GetDefaultRenderTarget() = 0;

			/**
			 * Erzeugt einen neuen GeometryBuffer.
			 *
			 * \return GeometryBuffer
			 */
			virtual GeometryBufferPtr CreateGeometryBuffer() = 0;

			/**
			 * Erzeugt ein neues TextureTarget zum Cachen von Zeichnungen.
			 *
			 * \return TextureTarget oder nullptr, falls nicht unterst�tzt
			 */
			virtual TextureTargetPtr CreateTextureTarget() = 0;

			/**
			 * Erzeugt eine leere Textur.
			 *
			 * \return Textur
			 */
			virtual TexturePtr CreateTexture() = 0;

			/**
			 * L�dt eine Textur aus der angegebenen Datei.
			 *
			 * \param path Pfad zur Datei
			 * \return Textur mit dem Inhalt der Datei
			 */
			virtual TexturePtr CreateTexture(const Misc::AnsiString &path) = 0;

			/**
			 * Erzeugt eine leere Textur mit der angegebenen Gr��e. Eventuell wird die Gr��e f�r die Hardware angepasst.
			 *
			 * \param size
			 * \return Textur
			 */
			virtual TexturePtr CreateTexture(const SizeF &size) = 0;

			/**
			 * Erlaubt dem Renderer sich auf das Zeichnen vorzubereiten.
			 */
			virtual void BeginRendering() = 0;

			/**
			 * Erlaubt dem Renderer sich auf das Beenden des Zeichnens vorzubereiten.
			 */
			virtual void EndRendering() = 0;

			/**
			 * Legt die Gr��e des Zeichenbereichs fest.
			 *
			 * \param size
			 */
			virtual void SetDisplaySize(const SizeF &size) = 0;

			/**
			 * Ruft die Gr��e des Zeichenbereichs ab.
			 *
			 * \return Gr��e des Zeichenbereichs
			 */
			virtual const SizeF& GetDisplaySize() const = 0;

			/**
			 * Ruft die Aufl�sung der Zeichenfl�che in DPI ab.
			 *
			 * \return DPI
			 */
			virtual const PointF& GetDisplayDPI() const = 0;

			/**
			 * Ruft die gr��tm�gliche Texturgr��e ab.
			 *
			 * \return Texturgr��e
			 */
			virtual uint32_t GetMaximumTextureSize() const = 0;
		};

		typedef std::shared_ptr<Renderer> RendererPtr;
	}
}

#endif
