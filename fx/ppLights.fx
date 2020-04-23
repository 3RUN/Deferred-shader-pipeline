
#include "mtlGlobalSettings.fx"

float4 vecViewPort;
float4 vecViewPos;
float4 vecViewDir;
float4 vecFog;
float4 vecFogColor;

float4x4 matViewProj;
float4x4 matEffect1;

float4 vecSkill1;

float fDLCount;
float4 vecDLPos[8];
float4 vecDLColor[8];

texture bmpWorld_bmap;
texture bmpCamera_bmap;
texture bmpAmbient_bmap;
texture bmpTranslucent_bmap;

sampler ColorSampler = sampler_state
{ 
	Texture = <bmpCamera_bmap>; 
	MipFilter = none;
	MinFilter = Point;
	MagFilter = Point;
};

sampler WorldSampler = sampler_state
{
	Texture = <bmpWorld_bmap>;
	MipFilter = none;
	MinFilter = Point;
	MagFilter = Point; 
};

sampler AmbientSampler = sampler_state
{ 
	Texture = <bmpAmbient_bmap>; 
	MipFilter = none;
	MinFilter = Point;
	MagFilter = Point;
};

sampler TransSampler = sampler_state
{ 
	Texture = <bmpTranslucent_bmap>; 
	MipFilter = none;
	MinFilter = Point;
	MagFilter = Point;
};

#ifdef SHADOWS
	texture bmpCubeMap1_bmap;
	texture bmpCubeMap2_bmap;
	texture bmpCubeMap3_bmap;
	texture bmpCubeMap4_bmap;
	texture bmpCubeMap5_bmap;
	texture bmpCubeMap6_bmap;
	texture bmpCubeMap7_bmap;
	texture bmpCubeMap8_bmap;
	
	samplerCUBE CubeMap1Sampler = sampler_state { Texture = <bmpCubeMap1_bmap>; Mipfilter = none; Minfilter = Point; Magfilter = Point; };
	samplerCUBE CubeMap2Sampler = sampler_state { Texture = <bmpCubeMap2_bmap>; Mipfilter = none; Minfilter = Point; Magfilter = Point; };
	samplerCUBE CubeMap3Sampler = sampler_state { Texture = <bmpCubeMap3_bmap>; Mipfilter = none; Minfilter = Point; Magfilter = Point; };
	samplerCUBE CubeMap4Sampler = sampler_state { Texture = <bmpCubeMap4_bmap>; Mipfilter = none; Minfilter = Point; Magfilter = Point; };
	samplerCUBE CubeMap5Sampler = sampler_state { Texture = <bmpCubeMap5_bmap>; Mipfilter = none; Minfilter = Point; Magfilter = Point; };
	samplerCUBE CubeMap6Sampler = sampler_state { Texture = <bmpCubeMap6_bmap>; Mipfilter = none; Minfilter = Point; Magfilter = Point; };
	samplerCUBE CubeMap7Sampler = sampler_state { Texture = <bmpCubeMap7_bmap>; Mipfilter = none; Minfilter = Point; Magfilter = Point; };
	samplerCUBE CubeMap8Sampler = sampler_state { Texture = <bmpCubeMap8_bmap>; Mipfilter = none; Minfilter = Point; Magfilter = Point; };
#endif

float3 doLight(samplerCUBE spl, int light, float3 wPos, float3 Normal) {
	float3 LightDir = wPos.xyz + Normal * fShadowOffset - vecDLPos[light].xyz;
	float fDistance = length(LightDir);
	if(fDistance > vecDLPos[light].w)
		return 0;
	LightDir /= fDistance;
	float fRadiance = saturate((vecDLPos[light].w - fDistance) / vecDLPos[light].w);
	
	#ifdef SHADOWS
		float fLight = 1.0f;
		if(light < MAX_SHADOW_CASTER_LIGHTS)
		{
			float depth = texCUBE ( spl, LightDir ).r;
			depth = (depth + vecDLPos[light].w);
			fLight = step(depth, vecDLPos[light].w - fDistance + fShadowOffset);		
		}
		#ifdef NORMALS
			#ifdef DIFFUSE
				fLight = min(fLight, saturate(-dot(LightDir.xyz, Normal.xyz)));
			#endif
		#endif
	#endif
	
	return vecDLColor[light].rgb * fRadiance * fLight * 2.0f;			
}

float pSize = 1.0f/2048.0f;


float4 DiffusePS ( in float2 inTex: TEXCOORD0 ) : COLOR0
{
	float2 Coord = inTex.xy;
	
	float3 World = tex2D ( WorldSampler, Coord ).rgb;
	#ifdef NORMALS
		float3 Tangent = tex2D ( WorldSampler, Coord + float2(vecViewPort.z, 0)).rgb - World;
		float3 Binormal = tex2D ( WorldSampler, Coord + float2(0, vecViewPort.w)).rgb - World;
		float3 Normal = normalize(cross(Tangent, Binormal));
	#else
		float3 Normal = 0;
	#endif
	float3 Dir = normalize ( World.xyz - vecViewPos.xyz );
	float fDepth = min ( distance ( World.xyz, vecViewPos.xyz ), vecSkill1.x );
	
	float4 Color = tex2D ( ColorSampler, Coord.xy );
	float3 fAmbient = tex2D ( AmbientSampler, Coord.xy ).rgb;
	
	#ifdef SHADOWS
		if(fDLCount > 0)
			fAmbient += doLight(CubeMap1Sampler, 0, World, Normal);
		if(fDLCount > 1)
			fAmbient += doLight(CubeMap2Sampler, 1, World, Normal);
		if(fDLCount > 2)
			fAmbient += doLight(CubeMap3Sampler, 2, World, Normal);
		if(fDLCount > 3)
			fAmbient += doLight(CubeMap4Sampler, 3, World, Normal);
		if(fDLCount > 4)
			fAmbient += doLight(CubeMap5Sampler, 4, World, Normal);
		if(fDLCount > 5)
			fAmbient += doLight(CubeMap6Sampler, 5, World, Normal);
		if(fDLCount > 6)
			fAmbient += doLight(CubeMap7Sampler, 6, World, Normal);
		if(fDLCount > 7)
			fAmbient += doLight(CubeMap8Sampler, 7, World, Normal);
	#endif
	
	Color.rgb *= fAmbient;
	
	float4 Translucent = tex2D ( TransSampler, Coord.xy );
	float TranslucentInfluence = sqrt(max(max(Translucent.r,Translucent.g),Translucent.b));// > 0 ? 1 : 0;
	Color.rgb = lerp ( Color.rgb, Translucent.rgb, TranslucentInfluence );
	
	#ifdef POSTERIZE
		Color.rgb = floor(pow(Color.rgb, 0.7f) * pPostSteps) / pPostSteps;
	#endif
	
	#ifdef FOG
		float Fog = 0;
		Fog = saturate ( ( fDepth - vecFog.x ) * vecFog.z );		
		Color.rgb = lerp ( Color.rgb, Translucent.rgb, Fog );
	#endif
	
	Color.a = 1.0f;
	return Color;
}

technique Diffuse
{
	pass p0
	{
		VertexShader = null;
		PixelShader  = compile ps_3_0 DiffusePS ();
	}
}