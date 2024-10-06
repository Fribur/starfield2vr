#define SMAA_INCLUDE_VS 1
#define SMAA_INCLUDE_PS 0

#include "smaa_common.hlsli"
#include "SMAA.inc.hlsl"

EdgeDetectionVertex vs_main(uint id: SV_VERTEXID)
{
	EdgeDetectionVertex output;
	vsScreenQuad(id, output.position, output.texcoord);
    SMAAEdgeDetectionVS( output.texcoord, output.offset );
	return output;
}