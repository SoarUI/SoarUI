//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

struct PSInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};
cbuffer cbworldViewProject : register(b0)
{
    float4x4 worldViewProj;
}
cbuffer cbfontColor : register(b1)
{
    float3 maskrgb;
}
Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);
PSInput VSFontMain(float4 position : POSITION, float4 uv : TEXCOORD)
{
    PSInput result;

    result.position = mul(position, worldViewProj);
    result.uv = uv;
    return result;
}

float4 PSFontMain(PSInput input) : SV_TARGET
{
    return float4(maskrgb, g_texture.Sample(g_sampler, input.uv).a);
}
