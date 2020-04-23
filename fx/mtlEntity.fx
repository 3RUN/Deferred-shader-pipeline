
#include "mtlGlobalSettings.fx"

float4 vecLight;
float4 vecColor;
float4 vecAmbient;
float4 vecEmissive;

float4x4 matWorldViewProj;
float4x4 matWorld;

float4 vecViewPos;
float4 vecFog;
float4 vecFogColor;

float iLights;
float4 vecLightPos[8];
float4 vecLightColor[8];

#ifdef VERTEX_SNAPPING
	float vertex_snapping_flt;
#endif

#ifdef POLY_CUTOFF
	float cutoff_distance_flt;
#endif

texture entSkin1;
texture entSkin2;
sampler TexSampler = sampler_state { Texture = <entSkin1>; Mipfilter = Linear; Minfilter = Linear; Magfilter = Linear; };
sampler ShadowSampler = sampler_state { Texture = <entSkin2>; Mipfilter = Linear; Minfilter = Linear; Magfilter = Linear; };

void entVS (
in float4 inPos: POSITION,
in float2 inTex1: TEXCOORD0,
in float2 inTex2: TEXCOORD1,
in float4 inNormal: NORMAL,
out float4 outPos: POSITION,
out float4 outWorld: TEXCOORD0,
out float4 outTex: TEXCOORD1 )
{
	outWorld = mul( inPos, matWorld );
	outTex.xy = inTex1.xy;
	outTex.zw = inTex2.xy;
	
	float4 outPosTemp = mul(inPos, matWorldViewProj);
	
	// vertex snapping
	#ifdef VERTEX_SNAPPING
		float4 vertex;
		vertex.xyz = outPosTemp.xyz / outPosTemp.w;
		vertex.x = floor((vertex_snapping_flt + 40) * vertex.x) / (vertex_snapping_flt + 40); // default 160
		vertex.y = floor(vertex_snapping_flt * vertex.y) / vertex_snapping_flt; // default 120
		vertex.xyz *= outPosTemp.w;
		vertex.w = outPosTemp.w;
		outPos = vertex;
	#else
		outPos = outPosTemp;
	#endif
	
	// affine texture mapping
	#ifdef AFFINE_TEX_MAPPING
		outPos *= inPos.w / length(outPosTemp);
	#endif
	
	// cut out polygons
	#ifdef POLY_CUTOFF
		float distance = length(outPosTemp);
		if(distance > cutoff_distance_flt)
		{
			outPos.w = 0;
		}
	#endif
}

float4 entPS (
in float4 inWorld: TEXCOORD0,
in float4 inTex: TEXCOORD1,
out float4 Color1 : COLOR1,
out float4 Color2 : COLOR2,
out float4 Color3 : COLOR3 ) : COLOR0
{
	float4 Tex = tex2D ( TexSampler, inTex.xy );
	float4 Shadow = (vecAmbient * vecLight) + float4(vecEmissive.xyz * vecColor.xyz, 0);
	float fAlpha = min ( Tex.a, vecLight.w );
	
	clip(fAlpha - 0.001f);
	
	float4 Color0;
	float fDepth = distance(vecViewPos.xyz, inWorld.xyz);
	float Fog = saturate((fDepth - vecFog.x) * vecFog.z);

	Color0.rgb = Tex.rgb;
	Color0.rgb = lerp ( Color0.rgb*Shadow.rgb, vecFogColor, Fog );
	Color0.a = fAlpha;// < 1 ? fAlpha : 0;
	
	Color1.rgb = Tex.rgb;
	Color1.a = Color0.a;
	
	Color2.rgb = inWorld.xyz;
	Color2.a = fAlpha < 1 ? 0 : 1;
	
	if ( vecLight.w < 1 )
	{
		for ( int i=0; i<iLights; i++ )
		{
			float fDistance = distance ( vecLightPos[i].xyz, inWorld.xyz );
			float fRadiance = saturate ( ( vecLightPos[i].w - fDistance ) / vecLightPos[i].w );
			Shadow.rgb += vecLightColor[i].rgb * fRadiance;
		}
		
	}
	Color3.rgb = Shadow.rgb;
	Color3.a = Color0.a;
	
	return Color0;
}

technique
{
	pass p0
	{
		ZEnable = True;
		ZWriteEnable = True;
		AlphaBlendEnable = True;
		AlphaTestEnable = True;
		
		VertexShader = compile vs_3_0 entVS();
		PixelShader  = compile ps_3_0 entPS();
	}
}