struct VS_INPUT
{
   float3 position : POSITION;
   float2 texCoord : TEXCOORD0;
};

struct PS_INPUT
{
   float4 position : SV_POSITION;
   float2 texCoord : TEXCOORD0;
};

cbuffer FXAAParams : register(b0)
{
    float2 screenSize;
};

PS_INPUT main(VS_INPUT input)
{
   PS_INPUT output;
   output.position = float4(input.position, 1.0f);
   output.texCoord = input.texCoord;
   return output;
}