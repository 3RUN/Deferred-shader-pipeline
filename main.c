#include <acknex.h>
#include <default.c>
#include <d3d9.h>

#define PRAGMA_POINTER

#define PRAGMA_PATH "fx"
#define PRAGMA_PATH "data"

#include "shadows.h"

#define dyn_blue skill1
#define dyn_green skill2
#define dyn_red skill3
#define dyn_range skill4
//uses: dyn_blue, dyn_green, dyn_red, dyn_range
action pointlight()
{
	set(my, FLAG1); // ignored by depth view
	
	vec_set(&my->blue, &my->skill1);
	my->lightrange = my->skill4;
	
	my->material = mtlBright;
	
	create_lightsource_bmap(my);
	
	VECTOR temp_pos;
	vec_set(&temp_pos, &my->x);

	while(my)
	{
		my->x = temp_pos.x + fsin(total_ticks * 8, 64);
		my->y = temp_pos.y + fcos(total_ticks * 8, 64);
		wait(1);
	}
}

action npc()
{
	set(my, SHADOW | CAST);
	my->eflags |= ANIMATE;
	my->material = mtlEntity;
}

#define ent_alpha skill1
#define ent_power skill2
//uses: ent_alpha, ent_power
action sprite()
{
	if(my->skill1 || my->skill2)
	{
		set(my, TRANSLUCENT);
		reset(my, SHADOW);
		if(my->skill1){ my->alpha = my->skill1; }
		else{ my->alpha = 50; }
		
		if(my->skill2){ my->skill41 = floatv(my->skill2); }
		else{ my->skill42 = floatv(2.5); }
	}
	else
	{
		my->alpha = 100;
		reset(my, TRANSLUCENT);
		set(my, SHADOW);
	}
	
	my->material = mtlEntity;
}

//uses: dyn_blue, dyn_green, dyn_red, dyn_range
action fading()
{
	set(my, TRANSLUCENT);
	reset(my, SHADOW | CAST);
	my->material = mtlEntity;
	
	if(my->skill1){ my->alpha = my->skill1; }
	else{ my->alpha = 50; }
	
	if(my->skill2){ my->skill41 = floatv(my->skill2); }
	else{ my->skill42 = floatv(2.5); }
	
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
		wait(-1);
		
		while(angle < 270)
		{
			my->alpha = 50 + fsin(angle, 50);
			angle += 10 * time_step;
			wait(1);
		}
		my->alpha = 0;
		wait(-1);
	}
}

action waving()
{
	set(my, SHADOW | CAST);
	reset(my, TRANSLUCENT);
	my->material = mtlWaving;
	
	// if we do have shadows, move them with the light source (used in depth.fx)
	if(is(my, SHADOW) && is(my, CAST)){ my->skill60 = floatv(1); }
	
	if(!my->skill1){ my->skill41 = floatv(0.05); }
	else{ my->skill41 = floatv(my->skill1); }
	
	if(!my->skill2){ my->skill42 = floatv(0.05); }
	else{ my->skill42 = floatv(my->skill2); }
	
	if(!my->skill3){ my->skill43 = floatv(0.2); }
	else{ my->skill43 = floatv(my->skill3); }
	
	my->emask &= DYNAMIC;
}

action terrain()
{
	my->skill1 = 16;
	my->skill2 = 8;
	my->skill3 = 4;
	set(my, SHADOW);
	my->material = mtlTerrain;
}

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
	p->size = 2;
	p->gravity = 0.2;
	p->lifespan = 150;
	p->event = p_alphafade;
}

void safe_video_switch(var _mode)
{
	video_mode = video_switch(_mode, 32, video_screen);
	if(!video_mode)
	{
		video_mode = video_switch(7, 32, video_screen);
	}
}

void decrease_resolution()
{
	safe_video_switch(video_mode - 1);
	pipeline_update();
}

void increase_resolution()
{
	safe_video_switch(video_mode + 1);
	pipeline_update();
}

function main ()
{
	on_f4 = decrease_resolution;
	on_f5 = increase_resolution;
	
	video_mode = 10;
	video_screen = 2;
	
	warn_level = 6;
	doppler_factor = 0;
	preload_mode = 3;
	shadow_stencil = -1;
	
	level_load("scene.wmb");
	wait(3);

	vec_set(&d3d_lodfactor, vector(12.5, 25, 50));

	camera->arc = 90;
	camera->clip_far = 2048;
	camera->clip_near = 0.1;
	
	camera->fog_start = 512;
	camera->fog_end = camera->clip_far * 0.9;
	sun_angle.roll = camera->clip_far * 0.8; // sun distance
	
	fog_color = 4;
	vec_set(&d3d_fogcolor4.blue, vector(128, 128, 128));
	vec_set(&sky_color.blue, &d3d_fogcolor4.blue);
	
	vec_set(&camera->x, vector(-151, -259, 178));
	vec_set(&camera->pan, vector(57, -31, 0));
	
	sun_light = 0;
	vec_set(&sun_color, vector(0, 0, 0));
	vec_set(&ambient_color, vector(0, 0, 0));
	
	pipeline_create();
	
	VECTOR temp_pos;
	vec_set(&temp_pos, nullvector);
	
	while(!key_esc)
	{
		camera->x = temp_pos.x + fsin(total_ticks * 0.75, 512);
		camera->y = temp_pos.y + fcos(total_ticks * 0.75, 512);
		camera->z = 200;
		
		vec_to_angle(&camera->pan, vec_diff(NULL, &temp_pos, &camera->x));
		
		effect(p_fountain, maxv(1, 10 * time_step), vector(-32, 0, 0), vector(0, 0, 10));
		wait(1);
	}
	
	sys_exit(NULL);
}