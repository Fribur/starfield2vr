#define SMAA_INCLUDE_VS 1
#define SMAA_INCLUDE_PS 0

#include "smaa_common.hlsli"
#include "SMAA.inc.hlsl"

BlendWeightVertex vs_main(uint id: SV_VERTEXID)
{
	BlendWeightVertex output;
    vsScreenQuad(id, output.position, output.texcoord);
	SMAABlendingWeightCalculationVS( output.texcoord, output.pixcoord, output.offset );
	return output;
}