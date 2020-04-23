
#include "mtlGlobalSettings.fx"

float4 vecTime;
float4 vecLight;
float4 vecColor;
float4 vecAmbient;
float4 vecEmissive;
float4 vecSkill41;

#ifdef VERTEX_SNAPPING
	float vertex_snapping_flt;
#endif

#ifdef POLY_CUTOFF
	float cutoff_distance_flt;
#endif

float lod_0_flt;
float lod_1_flt;
float lod_2_flt;
float lod_3_flt;

float4 vecViewPos;
float4 vecFog;
float4 vecFogColor;

float4x4 matWorldViewProj;
float4x4 matWorld;

texture entSkin1;
sampler TexSampler = sampler_state { Texture = <entSkin1>; Mipfilter = Linear; Minfilter = Linear; Magfilter = Linear; };

void wavedVS (
in float4 inPos: POSITION,
in float2 inTex1: TEXCOORD0,
out float4 outPos: POSITION,
out float4 outWorld: TEXCOORD0,
out float2 outTex: TEXCOORD1 )
{
	outWorld = mul( inPos, matWorld );
	outTex.xy = inTex1.xy;
	
	
	float4 outPosTemp = mul(inPos, matWorldViewProj);
	
	float overallforce = 1.0f - saturate( (outPosTemp.z - lod_0_flt) / lod_1_flt );
	
	float force_x = vecSkill41.x * overallforce; 
	float force_y = vecSkill41.y * overallforce;
	float speed = sin((vecTime.w + (outWorld.x + outWorld.z)) * vecSkill41.z);
	
	if( inPos.y > 0 )
	{
		inPos.x += speed * force_x * inPos.y;
		inPos.z += speed * force_y * inPos.y;
		inPos.y -= 0.1 * abs(speed * (force_x + force_y)) * inPos.y;
	}
	
	outPosTemp = mul(inPos, matWorldViewProj); // Computed twice!!
	
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

float4 wavedPS (
in float4 inWorld: TEXCOORD0,
in float2 inTex: TEXCOORD1,
out float4 Color1 : COLOR1,
out float4 Color2 : COLOR2,
out float4 Color3 : COLOR3 ) : COLOR0
{
	float4 Tex = tex2D ( TexSampler, inTex.xy );
	float4 Shadow = (vecAmbient * vecLight) + float4(vecEmissive.xyz * vecColor.xyz, vecLight.w);
	float4 fAlpha = min(vecLight.w, Tex.a);
	
	float fDepth = distance(vecViewPos.xyz, inWorld.xyz);
	float Fog = saturate((fDepth - vecFog.x) * vecFog.z);
	fAlpha = min ( fAlpha, 1.0f - saturate( (fDepth - lod_2_flt) / lod_3_flt ) );
	
	clip(fAlpha - 0.001f);
	
	float4 Color0;
	Color0.rgb = 0;//Tex.rgb;
	Color0.rgb = lerp ( Color0.rgb, vecFogColor, Fog );
	Color0.a = fAlpha;// > 0.5f ? 1.0f : 0;
	
	Color1.rgb = Tex.rgb;
	Color1.a = fAlpha;
	
	Color2.rgb = inWorld.xyz;
	Color2.a = Color0.a;
	
	Color3.rgb = Shadow.rgb;
	Color3.a = Color0.a;
	
	return Color0;
}

technique
{
	pass p0
	{
		ZWriteEnable = True;
		AlphaBlendEnable = True;
		AlphaTestEnable = True;
		
		CullMode = None;
		
		VertexShader = compile vs_3_0 wavedVS();
		PixelShader  = compile ps_3_0 wavedPS();
	}

}

technique fallback { pass p0{} }