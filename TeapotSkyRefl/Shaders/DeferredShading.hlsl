#include "Common.hlsl"

// Constant Buffers

// Model vertex shader constants
cbuffer cbPerObjectVS : register(b0) 
{
    float4x4 WorldViewProjection    : packoffset(c0);
    float4x4 World                  : packoffset(c4);
}

// Model pixel shader constants
cbuffer cbPerObjectPS : register(b0) 
{
	float4 diffuseColor			: packoffset(c0);
    float specExp				: packoffset(c1);
    float specIntensity			: packoffset(c1.y);
	bool useDiffuseTexture		: packoffset(c1.z);
	bool useSpecularTexture		: packoffset(c1.w);
	bool useNormalMapTexture	: packoffset(c2.x);
	bool useAlphaTexture		: packoffset(c2.y);
	float pad					: packoffset(c2.z);
}

// Diffuse texture and linear sampler
Texture2D DiffuseTexture    : register(t0);
SamplerState LinearSampler  : register(s0);


// shader input/output structure
struct VS_INPUT
{
    float4 Position : POSITION;
    float3 Normal   : NORMAL;
    float2 UV       : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float2 UV       : TEXCOORD0;
    float3 Normal   : TEXCOORD1;
};

// Vertex shader
VS_OUTPUT RenderSceneVS(VS_INPUT input)
{
    VS_OUTPUT Output;
    float3 vNormalWorldSpace;
    
	// Transform position from object space to homogeneous projection space
    Output.Position = mul(input.Position, WorldViewProjection);

    Output.UV = input.UV;

	// Transform the normal to world space
	Output.Normal = mul(input.Normal, (float3x3) World);
    
    return Output;
}


// Pixel shader
struct PS_GBUFFER_OUT
{
    float4 ColorSpecInt : SV_TARGET0;
    float4 Normal       : SV_TARGET1;
    float4 SpecPow      : SV_TARGET2;
};


// Packs all the data into the GBuffer structure
PS_GBUFFER_OUT PackGBuffer(float3 BaseColor, float3 Normal, float SpecIntensity, float SpecPower)
{
    PS_GBUFFER_OUT Out;

	// Normalize the specular power
    float SpecPowerNorm = max(0.0001, (SpecPower - g_SpecPowerRange.x) / g_SpecPowerRange.y);

    Out.ColorSpecInt = float4(BaseColor.rgb, SpecIntensity);
    Out.Normal = float4(Normal * 0.5 + 0.5, 0.0);
    Out.SpecPow = float4(SpecPowerNorm, 0.0, 0.0, 0.0);

    return Out;
}

PS_GBUFFER_OUT RenderScenePS(VS_OUTPUT In)
{
    // Lookup mesh texture and modulate it with diffuse
	float3 DiffuseColor = diffuseColor.xyz;
	if (useDiffuseTexture)
	{
		DiffuseColor = DiffuseTexture.Sample(LinearSampler, In.UV);
	}
    
	DiffuseColor *= DiffuseColor;

    return PackGBuffer(DiffuseColor, normalize(In.Normal), specIntensity, specExp);
}
