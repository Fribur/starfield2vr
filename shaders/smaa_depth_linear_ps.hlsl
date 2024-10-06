#include "smaa_common.hlsli"

Texture2D<float> depthSource : register(t6);

SamplerState LinearSampler : register(s0);
SamplerState PointSampler : register(s1);

float GetLinearizedDepth(float depth)
{
//
// #if RESHADE_DEPTH_INPUT_IS_LOGARITHMIC
// 		const float C = 0.01;
// 		depth = (exp(depth * log(C + 1.0)) - 1.0) / C;
// #endif

        depth = 1.0 - depth;
		depth /= gFarPlane - depth * (gFarPlane - gNearPlane);
		return depth;
}

float4 ps_main(ResolveVertex inp) : SV_TARGET {
    float depth =  depthSource.Sample(PointSampler, inp.texcoord);
    depth =  GetLinearizedDepth(depth);
    return float4(depth, depth, depth, 1.0);
}
