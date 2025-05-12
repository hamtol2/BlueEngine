#include "Common.hlsli"

struct PixelInput
{
    float4 position : SV_POSITION;
    float3 color : COLOR;
    float2 texCoord : TEXCOORD0;
    float3 normal : NORMAL;
    float3 cameraDirection : TEXCOORD1;
    float4 lightClipPosition : TEXCOORD2;
};

// Light Buffer.
cbuffer LightData : register(b2)
{
    float3 lightPosition;
    float lightDataPadding;
    matrix lightViewMatrix;
    matrix lightProjectionMatrix;
};

// Texture.
Texture2D diffuseMap : register(t0);
Texture2D shadowMap : register(t3);
SamplerState diffuseSampler : register(s0);

float4 main(PixelInput input) : SV_TARGET
{
    // Sampling.
    float4 texColor = diffuseMap.Sample(diffuseSampler, input.texCoord);
    
    // Light Dir.
    //float3 lightDir = -float3(500.0f, 500.0f, -500.0f);
    float3 lightDir = normalize(-lightPosition);
    
    // World Normal.
    float3 worldNormal = normalize(input.normal);
    
    // Dot (Lambert cosine law) - diffuse.
    float nDotl = CalcLambert(worldNormal, lightDir);
    
    // Calculate shadow.
    float shadowFactor = nDotl > 0 ? CalculateShadowFactor(shadowMap, diffuseSampler, 0, 0.00000125f, 0.2f, input.lightClipPosition) : 1;
    
    float4 ambient = texColor * float4(0.2f, 0.2f, 0.2f, 1);
    float4 diffuse = texColor * nDotl;
    //diffuse *= shadowFactor;
    
    float4 finalColor = ambient + diffuse;
    
    // Phong (specular).
    //float specular = CalcPhong(worldNormal, lightDir, input.cameraDirection);
    
    // Blinn-Phong (specular).
    float specular = 0;
    if (shadowFactor > 0.9)
    {
        specular = CalcBlinnPhong(worldNormal, lightDir, input.cameraDirection);
    }
    
    finalColor += float4(0.4f, 0.6f, 0.8f, 1) * specular;
    //return float4(specular, specular, specular, 1);
    return finalColor *= shadowFactor;
}