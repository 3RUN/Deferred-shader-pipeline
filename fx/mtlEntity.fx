
// comment this to disable certain effects
#define VERTEX_SNAPPING
#define AFFINE_TEX_MAPPING
#define POLY_CUTOFF

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

float4 vecSkill1;

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
	// vertex snapping
	#ifdef VERTEX_SNAPPING
		float4 snapToPixel = mul(inPos, matWorldViewProj);
		float4 vertex = snapToPixel;
		vertex.xyz = snapToPixel.xyz / snapToPixel.w;
		vertex.x = floor((vecSkill1.x + 40) * vertex.x) / (vecSkill1.x + 40); // default 160
		vertex.y = floor(vecSkill1.x * vertex.y) / vecSkill1.x; // default 120
		vertex.xyz *= snapToPixel.w;
		outPos = vertex;
		#else
		outPos = mul(inPos, matWorldViewProj);
	#endif
	
	// affine texture mapping
	#ifdef AFFINE_TEX_MAPPING
		outPos *= inPos.w / length(mul(inPos, matWorldViewProj));
	#endif

	outWorld = mul( inPos, matWorld );
	outTex.xy = inTex1.xy;
	outTex.zw = inTex2.xy;
	
	// cut out polygons
	#ifdef POLY_CUTOFF
		float distance = length(mul(inPos, matWorldViewProj));
		if(distance > vecSkill1.y)
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
	float4 Shadow = (vecAmbient * vecLight) + float4(vecEmissive.xyz * vecColor.xyz, vecLight.w);
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
		ZWriteEnable = True;
		AlphaBlendEnable = True;
		
		VertexShader = compile vs_3_0 entVS();
		PixelShader  = compile ps_3_0 entPS();
	}
}