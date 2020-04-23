
const float4x4 matWorldViewProj;
const float4x4 matWorld;

float4 vecViewPos;
float4 vecSkill1;
float4 vecSkill41;
float4 vecTime;

float lod_0_flt;
float lod_1_flt;

texture entSkin1;

sampler TexSampler = sampler_state { Texture = <entSkin1>; Mipfilter = Point; };


void wavedDepthVS (
in  float4 inPos    : POSITION,
in  float2 inTex1   : TEXCOORD0,
out float4 outPos   : POSITION,
out float4 outWorld : TEXCOORD0,
out float2 outTex   : TEXCOORD1 )
{
	outPos = mul ( inPos, matWorldViewProj );
	outWorld = mul( inPos, matWorld );
	outTex.xy = inTex1.xy;
	
	float overallforce = 1.0f - saturate( (distance(outWorld.xyz, vecSkill1.xyz) - lod_0_flt) / lod_1_flt );
	
	if ( overallforce > 0 )
	{
		float force_x = vecSkill41.x * overallforce; 
		float force_y = vecSkill41.y * overallforce;
		float speed = sin((vecTime.w + (outWorld.x + outWorld.z)) * vecSkill41.z);
		
		if( inPos.y > 0 )
		{
			inPos.x += speed * force_x * inPos.y;
			inPos.z += speed * force_y * inPos.y;
			inPos.y -= 0.1 * abs(speed * (force_x + force_y)) * inPos.y;
		}
		
		outWorld = mul( inPos, matWorld );
		outPos = mul( inPos, matWorldViewProj );
	}
}

float4 DepthPS(
in float4 InWorld	: TEXCOORD0,
in float2 InTex	: TEXCOORD1) : COLOR0
{
	float4 Tex = tex2D ( TexSampler, InTex );
	clip(Tex.a - 0.5f);
	return float4( -distance( InWorld.xyz, vecViewPos.xyz ), 0.0f, 0.0f, 1.0f );
}

technique DepthWaving
{
	pass p0
	{
		ZEnable = True;
		ZWriteEnable = True;
		AlphaBlendEnable = False;
		AlphaTestEnable = False;
		
		Lighting	= False;
		
		CullMode = None;
		
		VertexShader = compile vs_3_0 wavedDepthVS();
		PixelShader  = compile ps_3_0 DepthPS();
	}
}	