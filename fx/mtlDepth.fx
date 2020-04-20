
const float4x4 matWorldViewProj;
const float4x4 matWorld;

float4 vecViewPos;
float4 vecSkill1;
float4 vecSkill5;
float4 vecSkill41;
float4 vecSkill57;
float4 vecTime;

texture entSkin1;

sampler TexSampler = sampler_state { Texture = <entSkin1>; Mipfilter = Linear; };


void DepthVS (
in float4 InPos	: POSITION,
in float4 InTex	: TEXCOORD0,
out float4 OutPos	: POSITION,
out float4 OutWorld: TEXCOORD0,
out float2 OutTex	: TEXCOORD1 )
{
	OutPos = mul ( InPos, matWorldViewProj );
	OutWorld = mul ( InPos, matWorld );
	OutTex = InTex.xy;
}

void wavedDepthVS (
in float4 inPos: POSITION,
in float2 inTex1: TEXCOORD0,
out float4 outPos: POSITION,
out float4 outWorld: TEXCOORD0,
out float2 outTex: TEXCOORD1 )
{
	outPos = mul ( inPos, matWorldViewProj );
	outWorld = mul( inPos, matWorld );
	outTex.xy = inTex1.xy;
	
	if ( vecSkill57.w > 0.5f )
	{
		float overallforce = 1.0f - saturate( (distance(outWorld.xyz, vecSkill5.xyz) - vecSkill1.x) / vecSkill1.y );
		
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
}

float4 DepthPS(
in float4 InWorld	: TEXCOORD0,
in float2 InTex	: TEXCOORD1 ) : COLOR0
{
	float4 Tex = tex2D ( TexSampler, InTex );
	float fAlpha = Tex.a > 0.5f ? 1.0f : 0;
	return float4( distance( InWorld.xyz, vecViewPos.xyz ), 0.0f, 0.0f, fAlpha );
}

technique Depth
{
	pass p0
	{
		ZWriteEnable = True;
		AlphaBlendEnable = True;
		AlphaTestEnable = True;
		
		Lighting	= False;
		
		CullMode = None;
		
		VertexShader = compile vs_3_0 wavedDepthVS();
		PixelShader  = compile ps_3_0 DepthPS();
	}
}	