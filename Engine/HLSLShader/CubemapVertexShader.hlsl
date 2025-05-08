struct VertexInput
{
    float3 position : POSITION;
    float3 color : COLOR;
    float2 texCoord : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
};
// ConstantBuffer.
cbuffer Transform : register(b0)
{
    matrix worldMatrix;
};

// ConstantBuffer.
cbuffer Camera : register(b1)
{
    matrix view;
    matrix projection;
    float3 cameraPosition;
    float padding;
};

// Light Buffer.
cbuffer LightData : register(b2)
{
    float3 lightPosition;
    float lightDataPadding;
    matrix lightViewMatrix;
    matrix lightProjectionMatrix;
};

struct VertexOutput
{
    float4 position : SV_POSITION;
    float3 texCoord : TEXCOORD;
};

VertexOutput main(VertexInput input)
{
    VertexOutput output;
    //output.position = float4(input.position, 1);
    output.position = mul(float4(input.position, 1), worldMatrix);
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);
    
    output.texCoord = input.position;
    return output;
}