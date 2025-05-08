#include "Common.hlsli"

struct PixelInput
{
    float4 position : SV_Position;
    float3 color : COLOR;
    float2 texCoord : TEXCOORD;
    float3 normal : NORMAL;
    float3 cameraDirection : TEXCOORD1;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
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
Texture2D specularMap : register(t1);
Texture2D normalMap : register(t2);
SamplerState diffuseSampler : register(s0);

float4 main(PixelInput input) : SV_TARGET
{
    // diffuse texture color.
    float4 diffuseTexColor = diffuseMap.Sample(diffuseSampler, input.texCoord);
    
    // specular texture color.
    float4 specularTexColor = specularMap.Sample(diffuseSampler, input.texCoord);
    
    // tangent normal.
    float4 tangentNormal = normalMap.Sample(diffuseSampler, input.texCoord);
    tangentNormal = tangentNormal * 2 - 1;
    
    // tangent to world transformation matrix.
    float3x3 tangentToWorld = float3x3(
        normalize(input.tangent),
        normalize(input.bitangent),
        normalize(input.normal)
    );
    
    // Light Dir.
    //float3 lightDir = -float3(500.0f, 500.0f, -500.0f);
    float3 lightDir = normalize(-lightPosition);
    
    // World Normal.
    float3 worldNormal = normalize(mul(tangentNormal.xyz, tangentToWorld));
    
    // Dot (Lambert cosine law) - diffuse.
    float nDotl = CalcLambert(worldNormal, lightDir);
    
    //float4 ambient = texColor * float4(0.2f, 0.2f, 0.2f, 1);
    //float4 ambient = texColor * float4(CalcAmbient(worldNormal), 1);
    float4 ambient = diffuseTexColor * float4(CalcAmbient(worldNormal), 1);
    float4 diffuse = diffuseTexColor * nDotl;
    float4 finalColor = ambient + diffuse;
    
    // Phong (specular).
    //float specular = CalcPhong(worldNormal, lightDir, input.cameraDirection);
    
    // Blinn-Phong (specular).
    float specular = CalcBlinnPhong(worldNormal, lightDir, input.cameraDirection);
    
    finalColor += specularTexColor * specular;
    return finalColor;
}