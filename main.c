#include <acknex.h>
#include <default.c>
#include <d3d9.h>

#define PRAGMA_POINTER

#define PRAGMA_PATH "fx"
#define PRAGMA_PATH "data"

#include "shadows.h"

// ------------------------------------------------------------------
// SKILL MACROS
// ------------------------------------------------------------------

// Light skills
#define dyn_blue      skill1
#define dyn_green     skill2
#define dyn_red       skill3
#define dyn_range     skill4

// Entity skills
#define ent_alpha     skill1

// Terrain skills
#define ter_scale1    skill1
#define ter_scale2    skill2
#define ter_scale3    skill3
#define ter_scale4    skill4
#define ter_scale5    skill5
#define ter_scale6    skill6
#define ter_scale7    skill7
#define ter_scale8    skill8
#define ter_scale9    skill9
#define ter_scale10   skill10
#define ter_scale11   skill11
#define ter_scale12   skill12
#define ter_scale13   skill13
#define ter_scale14   skill14
#define ter_scale15   skill15
#define ter_scale16   skill16

// Terrain skills
#define wav_force_x   skill1
#define wav_force_y   skill2
#define wav_speed     skill3

// ------------------------------------------------------------------
// MATERIALS
// ------------------------------------------------------------------

MATERIAL *mtlBlock =
{
	effect = "mtlBlock.fx";
}

MATERIAL *mtlEntity =
{
	effect = "mtlEntity.fx";
}

var evnWaving () {
	if(render_view != camera) 
	{
		mtl = mtlDepthWaving;
		mtl->skill1 = floatv(camera->x);
		mtl->skill2 = floatv(camera->z);
		mtl->skill3 = floatv(camera->y);
	}
	return 0;
}

MATERIAL *mtlWaving =
{
	effect = "mtlWaving.fx";
	event = evnWaving;
	flags = ENABLE_RENDER | OVERRIDE;
}

MATERIAL *mtlBright =
{
	ambient_blue = 255;
	ambient_green = 255;
	ambient_red = 255;
	emissive_blue = 255;
	emissive_green = 255;
	emissive_red = 255;
	
	effect = "mtlEntity.fx";
}

void terrain_mtl_event()
{
	float matScales[16] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	int i;
	for(i = 0; i < 16 /*ent_status(me, 8)*/; i += 1)
	{
		matScales[i] = my->skill[i];
	}
	mat_set(mtl->matrix, matScales);
}

MATERIAL *mtlTerrain = 
{
	effect = "mtlTerrain.fx";
	event = terrain_mtl_event;
}
	
// ------------------------------------------------------------------
// ACTIONS
// ------------------------------------------------------------------

var LightCount = 0;

//uses: dyn_blue, dyn_green, dyn_red, dyn_range
action pointlight()
{
	c_setminmax(my);
	ent_set_as_translucent(my);
	my->alpha = 100;
	
	my->material = mtlBright;
	
	vec_set(&my->blue, &my->dyn_blue);
	my->lightrange = my->dyn_range;
	
	my->skill20 = path_length(me);
	my->skill21 = 0;
	
	VECTOR temp_pos;
	vec_set(&temp_pos, &my->x);
	
	while(my)
	{
		if(!key_space) {
			my->skill21 += time_step * 3;
			if(my->skill21 > my->skill20)
				my->skill21 -= my->skill20;
			
			path_spline(me, &my->x, my->skill21);
		}
		wait(1);
	}
}

// ------------------------------------------------------------------

action npc()
{
	c_setminmax(my);
	ent_set_as_solid(my);
	my->eflags |= ANIMATE;
	my->material = mtlEntity;
}

// ------------------------------------------------------------------

//uses: ent_alpha
action sprite()
{
	c_setminmax(my);
	if(my->ent_alpha > 0)
	{
		ent_set_as_translucent(my);
		my->alpha = my->ent_alpha;
	}
	else
	{
		ent_set_as_solid(my);
	}
	
	my->material = mtlEntity;
	
	my->emask &= DYNAMIC;
}

// ------------------------------------------------------------------

action fading()
{
	c_setminmax(my);
	my->material = mtlEntity;
	
	ent_set_as_translucent(my);
	
	my->emask &= DYNAMIC;
	
	var angle = -90;
	
	while(my)
	{
		angle = -90;
		while(angle < 90)
		{
			my->alpha = 50 + fsin(angle, 50);
			angle += 10 * time_step;
			wait(1);
		}
		my->alpha = 100;
		ent_set_as_solid(my);
		
		wait(-2);
		ent_set_as_translucent(my);
		
		while(angle < 270)
		{
			my->alpha = 50 + fsin(angle, 50);
			angle += 10 * time_step;
			wait(1);
		}
		my->alpha = 0;
		wait(1);
	}
}

// ------------------------------------------------------------------

//uses: wav_force_x, wav_force_y, wav_speed
action waving()
{
	c_setminmax(my);
	ent_set_as_solid(my);
	my->material = mtlWaving;
	
	// if we do have shadows, move them with the light source (used in depth.fx)
	if(is(my, SHADOW) && is(my, CAST)){ my->skill60 = floatv(1); }
	
	if(!my->wav_force_x){ my->skill41 = floatv(0.05); }
	else{ my->skill41 = floatv(my->wav_force_x); }
	
	if(!my->wav_force_y){ my->skill42 = floatv(0.05); }
	else{ my->skill42 = floatv(my->wav_force_y); }
	
	if(!my->wav_speed){ my->skill43 = floatv(0.2); }
	else{ my->skill43 = floatv(my->wav_speed); }
	
	my->emask &= DYNAMIC;
}

// ------------------------------------------------------------------

//uses: ter_scale1, ter_scale2, ter_scale3, ter_scale4, ter_scale5, ter_scale6, ter_scale7, ter_scale8, ter_scale9, ter_scale10, ter_scale11, ter_scale12, ter_scale13, ter_scale14, ter_scale15, ter_scale16, 
action terrain()
{
	c_setminmax(my);
	
	if(my->ter_scale1 == 0) my->ter_scale1 = 16;
	if(my->ter_scale2 == 0) my->ter_scale2 = 8;
	if(my->ter_scale3 == 0) my->ter_scale3 = 4;
	ent_set_as_solid(my);
	my->material = mtlTerrain;
	
	wait(1);
	
	my->emask &= ~DYNAMIC;
}

// ------------------------------------------------------------------

action hero ()
{
	ent_set_as_solid(my);
	my->material = mtlEntity;
	camera->genius = my;
	c_updatehull(my, 1);
	my->max_z -= 16;
	
	ANGLE angCam;
	vec_fill(&angCam, 0);
	
	while(1)
	{
		if(proc_status(def_move) == 0) 
		{
			angCam.pan = ang(angCam.pan - mickey.x * 0.2); 
			angCam.tilt = clamp(angCam.tilt - mickey.y * 0.2, -85, 90); 
			camera->pan += ang(angCam.pan - camera->pan) * time_step * 2;
			camera->tilt += ang(angCam.tilt - camera->tilt) * time_step * 2;
			my->pan = camera->pan;
			
			VECTOR vMove;
			vMove.x = key_w - key_s;
			vMove.y = key_a - key_d;
			vMove.z = 0;
			vec_rotate(&vMove, vector(my->pan, 0, 0));
			vec_normalize(&vMove, 7 * time_step);
			var dist = c_move(me, NULL, &vMove, IGNORE_MODELS | IGNORE_SPRITES | GLIDE);
			if(dist > 0)
				ent_animate(me, "walk", total_ticks * 8, ANM_CYCLE);
			else
				ent_animate(me, "stand", total_ticks * 8, ANM_CYCLE);
			
			VECTOR vT;
			vec_set(&vT, vector(0, 0, -100));
			vec_add(&vT, &my->x);
			dist = c_trace(&my->x, &vT, IGNORE_MODELS | IGNORE_SPRITES);
			my->z -= dist - my->max_z;
			
			
			camera->x = my->x;
			camera->y = my->y;
			camera->z = my->z + my->max_z * 0.6;
		}
		
		wait(1);
	}
}

// ------------------------------------------------------------------
// PARTICLES
// ------------------------------------------------------------------

function vec_randomize(VECTOR *vec, var range)
{
	vec_set(vec, vector(random(1) - 0.5, random(1) - 0.5, random(1) - 0.5));
	vec_normalize(vec, random(range));
}

function p_alphafade(PARTICLE *p)
{
	p->lifespan -= time_step;
}

function p_fountain(PARTICLE *p)
{
	VECTOR temp_vec;
	vec_randomize(&temp_vec, 2);
	vec_add(&p->vel_x, &temp_vec);
	vec_set(&p->blue, COLOR_WHITE);
	set(p, MOVE | TRANSLUCENT);
	p->alpha = 100;
	p->size = 4;
	p->gravity = 0.2;
	p->lifespan = 150;
	p->event = p_alphafade;
}

// ------------------------------------------------------------------
// VIDEO RESOLUTION
// ------------------------------------------------------------------

void safe_video_switch(var _mode)
{
	var propX = camera->size_x / screen_size.x;
	var propY = camera->size_y / screen_size.y;
	
	video_mode = video_switch(_mode, 32, video_screen);
	if(!video_mode)
	{
		video_mode = video_switch(7, 32, video_screen);
	}
	
	camera->pos_x = floor(camera->pos_x * propX);
	camera->pos_y = floor(camera->pos_y * propY);
	camera->size_x = floor(screen_size.x * propX);
	camera->size_y = floor(screen_size.y * propY);
	
	pipeline_update();
}

void decrease_resolution()
{
	safe_video_switch(video_mode - 1);
}

void increase_resolution()
{
	safe_video_switch(video_mode + 1);
}

// ------------------------------------------------------------------
// MAIN
// ------------------------------------------------------------------

function main ()
{
	on_f4 = decrease_resolution;
	on_f5 = increase_resolution;
	
	video_mode = 10;
	video_screen = 2;
	fps_max = 60;
	
	warn_level = 6;
	doppler_factor = 0;
	preload_mode = 3;
	shadow_stencil = -1;
	
	vec_set(&d3d_lodfactor, vector(12.5, 25, 50));
	
	vec_set(&screen_color, vector(0,0,0)); 
	
	wait(1);
	
	level_load("scene.wmb");
	
//	camera->size_x = 640;
//	camera->size_y = 480;
//	camera->pos_x = 100;
//	camera->pos_y = 100;
	
	camera->arc = 90;
	camera->clip_far = 2048;
	camera->clip_near = 0.1;
	
	camera->fog_start = 512;
	camera->fog_end = camera->clip_far * 0.9;
	
	fog_color = 4;
	vec_set(&d3d_fogcolor4.blue, vector(1, 1, 1));
	vec_set(&sky_color.blue, &d3d_fogcolor4.blue);
	
	vec_set(&camera->x, vector(-151, -259, 178));
	vec_set(&camera->pan, vector(57, -31, 0));
	
	sun_light = 0;
	vec_set(&sun_color, vector(0, 0, 0));
	vec_set(&ambient_color, vector(0, 0, 0));
	
	VECTOR temp_pos;
	vec_set(&temp_pos, nullvector);
	
	while(!key_esc)
	{
		effect(p_fountain, maxv(1, 10 * time_step), vector(-32, 0, 0), vector(0, 0, 10));
		
		wait(1);
	}
	
	sys_exit(NULL);
}