#include "smaa_common.hlsli"

Texture2D gammaTexture : register(t8);

SamplerState LinearSampler : register(s0);
SamplerState PointSampler : register(s1);

float4 ps_main(ResolveVertex inp) : SV_TARGET {
    return gammaTexture.Sample(PointSampler, inp.texcoord);
}
