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

		/*!
		\brief
			Enumerated type that contains the valid options that specify the type of
			blending that is to be performed for subsequent rendering operations.
		*/
		enum class BlendMode
		{
			//! Invalid mode indicator.
			Invalid,
			//! Use normal blending mode.
			Normal,
			//! Use blending mode suitable for textures with premultiplied colours.
			RTT_PreMultiplied
		};

		/*!
		\brief
			Abstract class defining the basic required interface for Renderer objects.

			Objects derived from Renderer are the means by which the GUI system
			interfaces with specific rendering technologies.  To use a rendering system
			or API to draw CEGUI imagery requires that an appropriate Renderer object be
			available.
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
			 * @return Standard RenderTarget
			 */
			virtual RenderTargetPtr& GetDefaultRenderTarget() = 0;

			/**
			 * Erzeugt einen neuen GeometryBuffer.
			 *
			 * @return GeometryBuffer
			 */
			virtual GeometryBufferPtr CreateGeometryBuffer() = 0;

			/**
			 * Erzeugt ein neues TextureTarget zum Cachen von Zeichnungen.
			 *
			 * @return TextureTarget oder nullptr, falls nicht unterstützt
			 */
			virtual TextureTargetPtr CreateTextureTarget() = 0;

			/**
			 * Erzeugt eine leere Textur.
			 *
			 * @return Textur
			 */
			virtual TexturePtr CreateTexture() = 0;

			/*!
			\brief
				Create a Texture object using the given image file.

			\param name
				String holding the name for the new texture.  Texture names must be
				unique within the Renderer.

			\param filename
				String object that specifies the path and filename of the image file to
				use when creating the texture.

			\param resourceGroup
				String objet that specifies the resource group identifier to be passed
				to the resource provider when loading the texture file \a filename.

			\return
				A newly created Texture object.  The initial content of the texture
				memory is the requested image file.

			\note
				Due to possible limitations of the underlying hardware, API or engine,
				the final size of the texture may not match the size of the loaded file.
				You can check the ultimate sizes by querying the Texture object
				after creation.

			\exceptions
				- AlreadyExistsException - thrown if a Texture object named \a name
				  already exists within the system.
			*/
			virtual TexturePtr CreateTexture(const Misc::AnsiString &filename) = 0;

			/**
			 * Erzeugt eine leere Textur mit der angegebenen Größe. Eventuell wird die Größe für die Hardware angepasst.
			 *
			 * @param size
			 * @return Textur
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

			/*!
			\brief
				Set the size of the display or host window in pixels for this Renderer
				object.

				This is intended to be called by the System as part of the notification
				process when display size changes are notified to it via the
				System::notifyDisplaySizeChanged function.

			\note
				The Renderer implementation should not use this function other than to
				perform internal state updates on the Renderer and related objects.

			\param size
				SizeF object describing the dimesions of the current or host window in
				pixels.
			*/

			/**
			 * Legt die Größe des Zeichenbereichs fest.
			 *
			 * @param size
			 */
			virtual void SetDisplaySize(const SizeF &size) = 0;

			/*!
			\brief
				Return the size of the display or host window in pixels.

			\return
				SizeF object describing the pixel dimesntions of the current display or
				host window.
			*/
			/**
			 *
			 *
			 * @return
			 */
			virtual const SizeF& GetDisplaySize() const = 0;

			/**
			 * Ruft die Auflösung der Zeichenfläche in DPI ab.
			 *
			 * @return DPI
			 */
			virtual const PointF& GetDisplayDPI() const = 0;

			/**
			 * Ruft die größtmögliche Texturgröße ab.
			 *
			 * @return Texturgröße
			 */
			virtual uint32_t GetMaximumTextureSize() const = 0;
		};

		typedef std::shared_ptr<Renderer> RendererPtr;
	}
}

#endif
