#define SMAA_INCLUDE_VS 0
#define SMAA_INCLUDE_PS 1

#include "smaa_common.hlsli"
#include "SMAA.inc.hlsl"

Texture2D t_SmaaEdges		: register(t2);
Texture2D t_SmaaArea		: register(t4);
Texture2D t_SmaaSearch		: register(t5);

float4 ps_main(BlendWeightVertex inp) : SV_TARGET {
    float4 subsampleindexes = float4(0.0, 0.0, 0.0, 0.0);
//    float4 test =  t_SmaaEdges.Sample(PointSampler, inp.texcoord);
//    test += t_SmaaArea.Sample(PointSampler, inp.texcoord);
//     test += t_SmaaSearch.Sample(PointSampler, inp.texcoord);
//     return test;
   return SMAABlendingWeightCalculationPS(inp.texcoord, inp.pixcoord, inp.offset, t_SmaaEdges, t_SmaaArea, t_SmaaSearch, subsampleindexes );
}