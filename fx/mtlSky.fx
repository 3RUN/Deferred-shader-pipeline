
#include "mtlGlobalSettings.fx"

float4x4 matWorldViewProj;
float4x4 matWorld;

float4 vecViewPos;
float4 vecFog;
float4 vecFogColor;

#ifdef VERTEX_SNAPPING
	float vertex_snapping_flt;
#endif

float sky_fog_factor_flt;

texture mtlSkin1;

samplerCUBE skyCUBE = sampler_state 
{
	Texture = <mtlSkin1>;
	Mipfilter = None;
	Minfilter = None;
	Magfilter = None;
};

void VS(
in float4 inPos: POSITION,
out float4 outPos: POSITION,
out float4 outWorld: TEXCOORD0)
{
	outWorld = mul(inPos, matWorld);
	
	// vertex snapping
	#ifdef VERTEX_SNAPPING
		float4 outPosTemp = mul(inPos, matWorldViewProj);
		float4 vertex;
		vertex.xyz = outPosTemp.xyz / outPosTemp.w;
		vertex.x = floor((vertex_snapping_flt + 40) * vertex.x) / (vertex_snapping_flt + 40); // default 160
		vertex.y = floor(vertex_snapping_flt * vertex.y) / vertex_snapping_flt; // default 120
		vertex.xyz *= outPosTemp.w;
		vertex.w = outPosTemp.w;
		outPos = vertex;
	#else
		outPos = mul(inPos, matWorldViewProj);
	#endif
	
}

float4 PS(in float4 worldPos: TEXCOORD0,
	out float4 Color1 : COLOR1,
	out float4 Color2 : COLOR2,
	out float4 Color3 : COLOR3 ) : COLOR0
{
	float3 Dir = normalize(worldPos.xyz - vecViewPos.xyz);
	float4 Color0 = texCUBE(skyCUBE, Dir);
	Color0.a = 1.0f;
	
	float Fog = 1.0f - pow(Dir.y, sky_fog_factor_flt);
	Color0.rgb = lerp(Color0.rgb, vecFogColor.rgb, Fog);
	
	Color1 = Color0;
	
	Color2.rgb = worldPos.xyz;
	Color2.a = 1;
	
	Color3 = 1;
	
	return Color0;
}

technique Sky
{
	pass p0
	{
		CullMode = None;
		
		VertexShader = compile vs_3_0 VS();
		PixelShader  = compile ps_3_0 PS();
	}
}

technique fallback { pass one { } }