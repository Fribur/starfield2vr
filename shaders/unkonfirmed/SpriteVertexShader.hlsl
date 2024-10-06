#include "Bloom.hlsli"

[RootSignature(CustomSpriteRS)]
void main(
    inout float4 color    : COLOR0,
    inout float2 texCoord : TEXCOORD0,
    inout float4 position : SV_Position) : SV_Position
{
    position = position;
}