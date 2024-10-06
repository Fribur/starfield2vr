cbuffer Parameters : register(b0)
{
    row_major float4x4 MatrixTransform;
};

float4 SpritePixelShader(float4 color    : COLOR0,
                         float2 texCoord : TEXCOORD0) : SV_Target0
{
    return color;
}