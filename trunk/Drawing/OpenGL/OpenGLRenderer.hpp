/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_DRAWING_OPENGLRENDERER_HPP
#define OSHGUI_DRAWING_OPENGLRENDERER_HPP

#include "../Renderer.hpp"

#include "GL.hpp"
#include <vector>
#include <map>

namespace OSHGui
{
	namespace Drawing
	{
		class OpenGLTexture;
		class OpenGLTextureTarget;
		class OpenGLGeometryBuffer;

		/**
		 * Die OpenGL Variante des Renderer.
		 */
		class OSHGUI_EXPORT OpenGLRenderer : public Renderer
		{
		public:
			/**
			 * Konstruktor der Klasse.
			 */
			OpenGLRenderer();
			~OpenGLRenderer();

			/**
			 * Hilfsfunktion, die vor einem Reset aufgerufen werden muss.
			 */
			void PreReset();
			/**
			 * Hilfsfunktion, die nach einem Reset aufgerufen werden muss.
			 */
			void PostReset();

			/**
			 * Rechnet die Gr��e in eine unterst�tzt Gr��e um.
			 *
			 * \param size
			 * \return umgewandelte Gr��e
			 */
			SizeF GetAdjustedSize(const SizeF &size);

			virtual RenderTargetPtr& GetDefaultRenderTarget() override;
			virtual GeometryBufferPtr CreateGeometryBuffer() override;
			virtual TextureTargetPtr CreateTextureTarget() override;
			virtual TexturePtr CreateTexture() override;
			virtual TexturePtr CreateTexture(const Misc::AnsiString &filename) override;
			virtual TexturePtr CreateTexture(const SizeF &size) override;

			virtual void BeginRendering() override;
			virtual void EndRendering() override;
			virtual void SetDisplaySize(const SizeF &sz) override;
			virtual const SizeF& GetDisplaySize() const override;
			virtual const PointF& GetDisplayDPI() const override;
			virtual uint32_t GetMaximumTextureSize() const override;

		private:
			/**
			 * Ruft die Gr��e des Viewport ab.
			 *
			 * \return Gr��e des Viewport
			 */
			SizeF GetViewportSize();
			/**
			 * Berechnet die n�chste Zweierpotenz, falss \a size keine Zweierpotenz ist.
			 *
			 * \param size
			 * \return n�chste Zweierpotenz
			 */
			float GetSizeNextPOT(float size) const;

			/**
			 * Entfernt ung�ltige Referenzen zu Objekten, f�r die ein Reset ausgef�hrt werden muss.
			 */
			void RemoveWeakReferences();

			SizeF displaySize;
			PointF displayDPI;
			
			RenderTargetPtr defaultTarget;
			
			typedef std::vector<std::weak_ptr<OpenGLTextureTarget>> TextureTargetList;
			TextureTargetList textureTargets;
			
			typedef std::vector<std::weak_ptr<OpenGLTexture>> TextureList;
			TextureList textures;
			
			uint32_t maxTextureSize;
		};
	}
}

#endif