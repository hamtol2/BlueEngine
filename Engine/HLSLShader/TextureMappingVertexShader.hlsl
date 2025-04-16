struct VertexInput
{
    float3 position : POSITION;
    float3 color : COLOR;
    float2 texCoord : TEXCOORD;
    float3 normal : NORMAL;
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

struct VertexOutput
{
    float4 position : SV_POSITION;
    float3 color : COLOR;
    float2 texCoord : TEXCOORD;
    float3 normal : NORMAL;
    float3 cameraDirection : TEXCOORD1;
};

VertexOutput main(VertexInput input)
{
    VertexOutput output;
    //output.position = float4(input.position, 1);
    output.position = mul(float4(input.position, 1), worldMatrix);
    float3 worldPosition = output.position.xyz;
    
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);
    
    output.color = input.color;
    output.texCoord = input.texCoord;
    
    output.normal = mul(input.normal, (float3x3)worldMatrix);
    
    output.cameraDirection = normalize(worldPosition - cameraPosition);
    
    return output;
}