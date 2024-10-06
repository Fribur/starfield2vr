#define SMAA_INCLUDE_VS 0
#define SMAA_INCLUDE_PS 1

#define SMAA_PREDICATION 1
#include "smaa_common.hlsli"
#include "SMAA.inc.hlsl"


Texture2D inputColorGamma : register(t0);
Texture2D depthTex : register(t7);


float4 ps_main(EdgeDetectionVertex inp) : SV_TARGET {
   float4 color = float4( 0.0, 0.0, 0.0, 0.0 );
    color.rg = SMAALumaEdgeDetectionPS(inp.texcoord, inp.offset, inputColorGamma, depthTex);
    return color;
}