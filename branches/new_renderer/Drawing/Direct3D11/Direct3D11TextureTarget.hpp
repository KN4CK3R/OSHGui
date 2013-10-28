/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_DRAWING_DIRECT3D11TEXTURETARGET_HPP
#define OSHGUI_DRAWING_DIRECT3D11TEXTURETARGET_HPP

#include "Direct3D11RenderTarget.hpp"
#include "../TextureTarget.hpp"

struct ID3D11Texture2D;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;

namespace OSHGui
{
	namespace Drawing
	{
		class Direct3D11Texture;

		/**
		 * Die Direct3D11 Variante des TextureTarget.
		 */
		class OSHGUI_EXPORT Direct3D11TextureTarget : public Direct3D11RenderTarget<TextureTarget>
		{
		public:
			/**
			 * Konstruktor der Klasse.
			 *
			 * \param owner Renderer Objekt, das das RenderTarget erstellt hat
			 */
			Direct3D11TextureTarget(Direct3D11Renderer& owner);
			/**
			 * Destruktor der Klasse.
			 */
			virtual ~Direct3D11TextureTarget();

			/**
			 * Hilfsfunktion, die vor einem D3D Reset vom Renderer aufgerufen wird.
			 */
			void PreD3DReset();
			/**
			 * Hilfsfunktion, die nach einem D3D Reset vom Renderer aufgerufen wird.
			 */
			void PostD3DReset();

			virtual void Activate() override;
			virtual void Deactivate() override;
			
			virtual bool IsImageryCache() const override;
			
			virtual void Clear() override;
			virtual TexturePtr GetTexture() const override;
			virtual void DeclareRenderSize(const SizeF& size) override;
			virtual bool IsRenderingInverted() const override;

		protected:
			static const float DefaultSize;

			/**
			 * Erzeugt die Textur, auf die gezeichnet wird.
			 */
			void InitialiseRenderTexture();
			/**
			 * L�scht die Textur, auf die gezeichnet wird.
			 */
			void CleanupRenderTexture();
			/**
			 * Aktuallisiert die Gr��e der Textur.
			 */
			void ResizeRenderTexture();

			/**
			 * Legt die Textur als Ziel f�r Zeichenoperationen fest.
			 */
			void EnableRenderTexture();
			/**
			 * Stellt das vorherige Ziel f�r Zeichenoperationen wieder her.
			 */
			void DisableRenderTexture();

			ID3D11Texture2D *d3dTexture;
			ID3D11RenderTargetView *renderTargetView;
			
			std::shared_ptr<Direct3D11Texture> texture;
			
			ID3D11RenderTargetView *renderTargetViewBackup;
			ID3D11DepthStencilView *depthStencilViewBackup;
		};
	}
}

#endif