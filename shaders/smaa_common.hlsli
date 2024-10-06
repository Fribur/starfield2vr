cbuffer SMAAConstants : register(b0)
{
    float4 gWindowReciprocalnSize;
    float gContrastFactor;
    float gThreshold;
    float gDepthThreshold;
    float gPredicationThreshold;
    float gPredicationScale;
    float gPredicationStrength;
    int gMaxSearchSteps;
    int gMaxSearchStepsDiag;
    int gCornerRounding;
    float gNearPlane;
    float gFarPlane;
}

struct EdgeDetectionVertex
{
	float4 position:  SV_POSITION;
	float2 texcoord : TEXCOORD0;
    float4 offset[3] : TEXCOORD1;
};

struct BlendWeightVertex
{
	float4 position:  SV_POSITION;
	float2 texcoord : TEXCOORD0;
    float2 pixcoord :  TEXCOORD1;
    float4 offset[3] : TEXCOORD2;
};

struct ResolveVertex
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD0;
	float4 texcoord1 : TEXCOORD1;
};

void vsScreenQuad(uint vertexId : SV_VERTEXID, out float4 position : SV_POSITION, out float2 texcoord : TEXCOORD0)
{
    float4 positions[6] = {
        { -1.0f, -1.0f, 1.0f, 1.0f },
        { -1.0f,  1.0f, 1.0f, 1.0f },
        {  1.0f, -1.0f, 1.0f, 1.0f },
        {  1.0f, -1.0f, 1.0f, 1.0f },
        { -1.0f,  1.0f, 1.0f, 1.0f },
        {  1.0f,  1.0f, 1.0f, 1.0f }
    };

    float2 texcoords[6] = {
        { 0.0f,  1.0f },
        { 0.0f,  0.0f },
        { 1.0f,  1.0f },
        { 1.0f,  1.0f },
        { 0.0f,  0.0f },
        { 1.0f,  0.0f }
    };

    position = positions[vertexId % 6];
    texcoord = texcoords[vertexId % 6];
}


#define SMAA_RT_METRICS gWindowReciprocalnSize
#define SMAA_THRESHOLD gThreshold
#define SMAA_DEPTH_THRESHOLD gDepthThreshold
#define SMAA_PREDICATION_THRESHOLD gPredicationThreshold
#define SMAA_PREDICATION_SCALE gPredicationScale
#define SMAA_PREDICATION_STRENGTH gPredicationStrength
#define SMAA_MAX_SEARCH_STEPS gMaxSearchSteps
#define SMAA_MAX_SEARCH_STEPS_DIAG gMaxSearchStepsDiag
#define SMAA_CORNER_ROUNDING gCornerRounding
#define SMAA_LOCAL_CONTRAST_ADAPTATION_FACTOR gContrastFactor
#define SMAA_HLSL_4_1 1