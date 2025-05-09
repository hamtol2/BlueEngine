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

// Camera Buffer.
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

// VS -> PS.
struct VertexOutput
{
    float4 position : SV_POSITION;
    float4 clipPosition : TEXCOORD1;
};

VertexOutput main(VertexInput input)
{
    VertexOutput output;
    
    //float3 dirZ = -normalize(lightPosition);
    //float3 up = float3(0, 1, 0);
    //float3 dirX = cross(up, dirZ);
    //float3 dirY = cross(dirZ, dirX);
    
    //float4x4 viewMatrix = float4x4(
    //    float4(dirX, -dot(lightPosition, dirX)),
    //    float4(dirY, -dot(lightPosition, dirY)),
    //    float4(dirZ, -dot(lightPosition, dirZ)),
    //    float4(0, 0, 0, 1)
    //);
    
    //viewMatrix = transpose(viewMatrix);
    
    output.position = mul(float4(input.position, 1), worldMatrix);
    output.position = mul(output.position, lightViewMatrix);
    output.position = mul(output.position, lightProjectionMatrix);
    
    output.clipPosition = output.position;
    return output;
}