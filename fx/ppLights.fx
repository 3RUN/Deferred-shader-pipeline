
#define FOG
#define SHADOWS

#define MAX_SHADOW_CASTER_LIGHTS 5

float fShadowOffset = 6.0f;
float fShadowFactor = 1.027f;

float4 vecViewPort;
float4 vecViewPos;
float4 vecViewDir;
float4 vecFog;
float4 vecFogColor;

float4x4 matViewProj;
float4x4 matEffect1;

float4 vecSkill1;

float iLights;
float4 vecLightPos[8];
float4 vecLightColor[8];

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
	MinFilter = point;
	MagFilter = point; 
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

float4 DiffusePS ( in float2 inTex: TEXCOORD0 ) : COLOR0
{
	float2 Coord = inTex.xy;
	
	float3 World = tex2D ( WorldSampler, Coord ).rgb; //Clamped
	float3 Dir = normalize ( World.xyz - vecViewPos.xyz );
	float fDepth = min ( distance ( World.xyz, vecViewPos.xyz ), vecSkill1.x );
	
	float4 Color = tex2D ( ColorSampler, Coord.xy ); //Clamped
	float3 fAmbient = tex2D ( AmbientSampler, Coord.xy ).rgb; //Clamped
	
	for ( int i=0; i<min(iLights,MAX_SHADOW_CASTER_LIGHTS ); i++ )
	{
		float3 LightDir = normalize ( World.xyz - vecLightPos[i].xyz );
		float fDistance = distance ( World.xyz, vecLightPos[i].xyz ); //length ( LightDir );
		float fRadiance = saturate ( ( vecLightPos[i].w - fDistance ) / vecLightPos[i].w );
		float fShadow = 1.0f;
		
		#ifdef SHADOWS
			if ( ( fRadiance > 0 ) && ( i < MAX_SHADOW_CASTER_LIGHTS ) )
			{
				float fDynamicShadow = 1.0f;
				
				if ( i == 0 )
				{
					fDynamicShadow = texCUBE ( CubeMap1Sampler, LightDir ).r;
				}
				else if ( i == 1 )
				{
					fDynamicShadow = texCUBE ( CubeMap2Sampler, LightDir ).r;
				}
				else if ( i == 2 )
				{
					fDynamicShadow = texCUBE ( CubeMap3Sampler, LightDir ).r;
				}
				else if ( i == 3 )
				{
					fDynamicShadow = texCUBE ( CubeMap4Sampler, LightDir ).r;
				}
				else if ( i == 4 )
				{
					fDynamicShadow = texCUBE ( CubeMap5Sampler, LightDir ).r;
				}
				else if ( i == 5 )
				{
					fDynamicShadow = texCUBE ( CubeMap6Sampler, LightDir ).r;
				}
				else if ( i == 6 )
				{
					fDynamicShadow = texCUBE ( CubeMap7Sampler, LightDir ).r;
				}
				else
				{
					fDynamicShadow = texCUBE ( CubeMap8Sampler, LightDir ).r;
				}
				
				fDynamicShadow = fDynamicShadow > 1 ? fDynamicShadow : vecLightPos[i].w;
				fShadow = fDynamicShadow + fShadowOffset > fDistance ? 1 : 0;		
			}
		#endif
		
		fAmbient.rgb += vecLightColor[i].rgb * fRadiance * fShadow * 2;			
	}
	
	Color.rgb *= fAmbient;
	
	float4 Translucent = tex2D ( TransSampler, Coord.xy );
	float TranslucentInfluence = sqrt(max(max(Translucent.r,Translucent.g),Translucent.b));// > 0 ? 1 : 0;
	
	float Fog = 0;
	#ifdef FOG
		Fog = saturate ( ( fDepth - vecFog.x ) * vecFog.z );		
		Color.rgb = lerp ( Color.rgb, Translucent.rgb, Fog );
	#endif
	
	Color.rgb = lerp ( Color.rgb, Translucent.rgb, TranslucentInfluence );
	
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