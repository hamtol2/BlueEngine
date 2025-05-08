#pragma once

#include "Texture.h"
#include "Core/Type.h"
#include <d3d11.h>

namespace Blue
{
    class Shadowmap : public Texture
    {
    public:
        Shadowmap(uint32 width = 2048, uint32 height = 2048 * (800 / 1280));
        ~Shadowmap();

        void Clear();

        ID3D11ShaderResourceView* GetShaderResourceView() const { return textureData->shaderResourceView; }

    private:

        uint32 width = 2048;
        uint32 height = 2048 * (800 / 1280);

        ID3D11Texture2D* shadowMapTexture = nullptr;
        ID3D11DepthStencilView* depthStencilView = nullptr;
        D3D11_VIEWPORT shadowmapViewport;
    };
}