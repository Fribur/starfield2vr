
#define FXAA_PC 1
#define FXAA_HLSL_5 1
#define FXAA_QUALITY__PRESET 29
#define FXAA_GREEN_AS_LUMA 1

#include "FXAA_NVIDIA_311.hlsli"

cbuffer cbFxaa : register(b1) {
    float4 rcpFrame : packoffset(c0);
};

struct FxaaVS_Output {
 float4 Pos : SV_POSITION;
 float2 Tex : TEXCOORD0;
 };

FxaaVS_Output FxaaVS(uint id : SV_VertexID) {
 FxaaVS_Output Output;
 Output.Tex = float2((id << 1) & 2, id & 2);
 Output.Pos = float4(Output.Tex * float2(2.0f, -2.0f) + float2(-1.0f, 1.0f), 0.0f, 1.0f);
 return Output;
}

SamplerState anisotropicSampler : register(s0);
Texture2D inputTexture : register(t0);


float4 FxaaPS(FxaaVS_Output Input) : SV_TARGET {
 FxaaTex tex = { anisotropicSampler, inputTexture };
 return float4(FxaaPixelShader(Input.Tex.xy, tex, rcpFrame.xy), 1.0f);
}