cbuffer CB0 : register(b0)
{
    float2 RcpTextureSize;
    float ContrastThreshold;    // default = 0.2, lower is more expensive
    float SubpixelRemoval;        // default = 0.75, lower blurs less
    uint LastQueueIndex;
    uint2 StartPixel;
}

#define CustomSpriteRS \
"RootFlags ( ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |" \
"            DENY_DOMAIN_SHADER_ROOT_ACCESS |" \
"            DENY_GEOMETRY_SHADER_ROOT_ACCESS |" \
"            DENY_HULL_SHADER_ROOT_ACCESS )," \
"RootConstants(b0, num32BitConstants=7), " \
"DescriptorTable ( SRV(t0), visibility = SHADER_VISIBILITY_PIXEL ),"\
"DescriptorTable ( SRV(t1), visibility = SHADER_VISIBILITY_PIXEL ),"\
"StaticSampler(s0,"\
"           filter = FILTER_MIN_MAG_MIP_LINEAR,"\
"           addressU = TEXTURE_ADDRESS_CLAMP,"\
"           addressV = TEXTURE_ADDRESS_CLAMP,"\
"           addressW = TEXTURE_ADDRESS_CLAMP,"\
"           visibility = SHADER_VISIBILITY_PIXEL )"
