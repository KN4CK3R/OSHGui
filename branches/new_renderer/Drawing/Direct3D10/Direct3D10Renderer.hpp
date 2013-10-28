/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_DRAWING_DIRECT3D10RENDERER_HPP
#define OSHGUI_DRAWING_DIRECT3D10RENDERER_HPP

#include "../Renderer.hpp"

#include <vector>
#include <map>

struct ID3D10Device;
struct ID3D10Effect;
struct ID3D10EffectTechnique;
struct ID3D10InputLayout;
struct ID3D10EffectShaderResourceVariable;
struct ID3D10EffectMatrixVariable;
struct ID3D10ShaderResourceView;
struct ID3D10EffectScalarVariable;
struct D3DXMATRIX;

namespace OSHGui
{
	namespace Drawing
	{
		class Direct3D10Texture;
		class Direct3D10TextureTarget;
		class Direct3D10GeometryBuffer;

		/**
		 * Die Direct3D10 Variante des Renderer.
		 */
		class OSHGUI_EXPORT Direct3D10Renderer : public Renderer
		{
		public:
			/**
			 * Konstruktor der Klasse.
			 *
			 * \param device Direct3D10 Device
			 */
			Direct3D10Renderer(ID3D10Device *device);
			/**
			 * Destruktor der Klasse.
			 */
			~Direct3D10Renderer();

			/**
			 * Hilfsfunktion, die vor einem D3D Reset aufgerufen werden muss.
			 */
			void PreD3DReset();
			/**
			 * Hilfsfunktion, die nach einem D3D Reset aufgerufen werden muss.
			 */
			void PostD3DReset();

			/**
			 * Ruft das Direct3D10 Device ab.
			 */
			ID3D10Device* GetDevice() const;

			void BindTechniquePass(const BlendMode mode, const bool clipped);
			void SetCurrentTextureShaderResource(ID3D10ShaderResourceView* srv);
			void SetProjectionMatrix(D3DXMATRIX &matrix);
			void SetWorldMatrix(D3DXMATRIX &matrix);

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
			 * Ruft die Größe des Viewport ab.
			 *
			 * \return Größe des Viewport
			 */
			SizeF GetViewportSize();

			/**
			 * Entfernt ungültige Referenzen zu Objekten, für die ein Reset ausgeführt werden muss.
			 */
			void RemoveWeakReferences();

			ID3D10Device *device;
			
			SizeF displaySize;
			PointF displayDPI;
			
			RenderTargetPtr defaultTarget;
			
			typedef std::vector<std::weak_ptr<Direct3D10TextureTarget>> TextureTargetList;
			TextureTargetList textureTargets;
			
			typedef std::vector<std::weak_ptr<Direct3D10Texture>> TextureList;
			TextureList textures;
			
			ID3D10Effect* effect;
			ID3D10EffectTechnique* normalClippedTechnique;
			ID3D10EffectTechnique* normalUnclippedTechnique;
			ID3D10EffectTechnique* premultipliedClippedTechnique;
			ID3D10EffectTechnique* premultipliedUnclippedTechnique;
			ID3D10InputLayout* inputLayout;
			
			ID3D10EffectMatrixVariable* worldMatrixVariable;
			ID3D10EffectMatrixVariable* projectionMatrixVariable;
			ID3D10EffectShaderResourceVariable* boundTextureVariable;
			ID3D10EffectScalarVariable* useShaderTextureVariable;
		};
	}
}

#endif