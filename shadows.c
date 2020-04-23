
ENTITY *entSky = NULL;

// lod factors
float lod_0 = 0;
float lod_1 = 0;
float lod_2 = 0;
float lod_3 = 0;

// angles of cubemap render views
var nCubemapAngles[6][2] = { 180, 0 , 90, 0, 0, 0, 270, 0, 90, -90, 90, 90 };

// global bitmap pointers
BMAP *bmpCamera      = "#2048x2048x24";
BMAP *bmpWorld       = "#2048x2048x14444";
BMAP *bmpAmbient     = "#2048x2048x24";
BMAP *bmpTranslucent = "#2048x2048x24";
BMAP *bmpSky         = SKYBOX_FILE;


// light depth cubemaps
BMAP *bmpDepthCubeEmpty = "#12x2x8";
BMAP *bmpCubeMap1 = CUBEMAP_DESC;
BMAP *bmpCubeMap2 = CUBEMAP_DESC;
BMAP *bmpCubeMap3 = CUBEMAP_DESC;
BMAP *bmpCubeMap4 = CUBEMAP_DESC;
BMAP *bmpCubeMap5 = CUBEMAP_DESC;
BMAP *bmpCubeMap6 = CUBEMAP_DESC;
BMAP *bmpCubeMap7 = CUBEMAP_DESC;
BMAP *bmpCubeMap8 = CUBEMAP_DESC;

BMAP *bmpCubeMaps[8];
void bmpCubeMaps_startup()
{
	wait(1);
	if(MAX_SHADOW_CASTER_LIGHTS > 0) {
		bmpCubeMaps[0] = bmpCubeMap1;
		bmap_to_cubemap(bmpCubeMap1);
	}
	else
	{
		BMAP_SAFE_REMOVE(bmpCubeMap1);
	}
	
	if(MAX_SHADOW_CASTER_LIGHTS > 1) {
		bmpCubeMaps[1] = bmpCubeMap2;
		bmap_to_cubemap(bmpCubeMap2);
	}
	else
	{
		BMAP_SAFE_REMOVE(bmpCubeMap2);
	}
	
	if(MAX_SHADOW_CASTER_LIGHTS > 2) {
		bmpCubeMaps[2] = bmpCubeMap3;
		bmap_to_cubemap(bmpCubeMap3);
	}
	else
	{
		BMAP_SAFE_REMOVE(bmpCubeMap3);
	}
	
	if(MAX_SHADOW_CASTER_LIGHTS > 3) {
		bmpCubeMaps[3] = bmpCubeMap4;
		bmap_to_cubemap(bmpCubeMap4);
	}
	else
	{
		BMAP_SAFE_REMOVE(bmpCubeMap4);
	}
	
	if(MAX_SHADOW_CASTER_LIGHTS > 4) {
		bmpCubeMaps[4] = bmpCubeMap5;
		bmap_to_cubemap(bmpCubeMap5);
	}
	else
	{
		BMAP_SAFE_REMOVE(bmpCubeMap5);
	}
	
	if(MAX_SHADOW_CASTER_LIGHTS > 5) {
		bmpCubeMaps[5] = bmpCubeMap6;
		bmap_to_cubemap(bmpCubeMap6);
	}
	else
	{
		BMAP_SAFE_REMOVE(bmpCubeMap6);
	}
	
	if(MAX_SHADOW_CASTER_LIGHTS > 6) {
		bmpCubeMaps[6] = bmpCubeMap7;
		bmap_to_cubemap(bmpCubeMap7);
	}
	else
	{
		BMAP_SAFE_REMOVE(bmpCubeMap7);
	}
	
	if(MAX_SHADOW_CASTER_LIGHTS > 7) {
		bmpCubeMaps[7] = bmpCubeMap8;
		bmap_to_cubemap(bmpCubeMap8);
	}
	else
	{
		BMAP_SAFE_REMOVE(bmpCubeMap8);
	}
	
	
}

float nextLightCount;
float nextLightPos[8][4];
float nextLightColor[8][4];

// renderers
VIEW *camDummy = { flags = PROCESS_TARGET; }
VIEW *camPostLight = { flags = PROCESS_TARGET; }

VIEW *camLD01 = { flags = NULL; }
VIEW *camLD02 = { flags = NULL; }
VIEW *camLD03 = { flags = NULL; }
VIEW *camLD04 = { flags = NULL; }
VIEW *camLD05 = { flags = NULL; }
VIEW *camLD06 = { flags = NULL; }
VIEW *camLD11 = { flags = NULL; }
VIEW *camLD12 = { flags = NULL; }
VIEW *camLD13 = { flags = NULL; }
VIEW *camLD14 = { flags = NULL; }
VIEW *camLD15 = { flags = NULL; }
VIEW *camLD16 = { flags = NULL; }
VIEW *camLD21 = { flags = NULL; }
VIEW *camLD22 = { flags = NULL; }
VIEW *camLD23 = { flags = NULL; }
VIEW *camLD24 = { flags = NULL; }
VIEW *camLD25 = { flags = NULL; }
VIEW *camLD26 = { flags = NULL; }
VIEW *camLD31 = { flags = NULL; }
VIEW *camLD32 = { flags = NULL; }
VIEW *camLD33 = { flags = NULL; }
VIEW *camLD34 = { flags = NULL; }
VIEW *camLD35 = { flags = NULL; }
VIEW *camLD36 = { flags = NULL; }
VIEW *camLD41 = { flags = NULL; }
VIEW *camLD42 = { flags = NULL; }
VIEW *camLD43 = { flags = NULL; }
VIEW *camLD44 = { flags = NULL; }
VIEW *camLD45 = { flags = NULL; }
VIEW *camLD46 = { flags = NULL; }
VIEW *camLD51 = { flags = NULL; }
VIEW *camLD52 = { flags = NULL; }
VIEW *camLD53 = { flags = NULL; }
VIEW *camLD54 = { flags = NULL; }
VIEW *camLD55 = { flags = NULL; }
VIEW *camLD56 = { flags = NULL; }
VIEW *camLD61 = { flags = NULL; }
VIEW *camLD62 = { flags = NULL; }
VIEW *camLD63 = { flags = NULL; }
VIEW *camLD64 = { flags = NULL; }
VIEW *camLD65 = { flags = NULL; }
VIEW *camLD66 = { flags = NULL; }
VIEW *camLD71 = { flags = NULL; }
VIEW *camLD72 = { flags = NULL; }
VIEW *camLD73 = { flags = NULL; }
VIEW *camLD74 = { flags = NULL; }
VIEW *camLD75 = { flags = NULL; }
VIEW *camLD76 = { flags = NULL; }

VIEW *camLightDepth[8][6];

void camLightDepth_startup()
{
	if(MAX_SHADOW_CASTER_LIGHTS > 0)
	{
		camLightDepth[0][0] = camLD01;
		camLightDepth[0][1] = camLD02;
		camLightDepth[0][2] = camLD03;
		camLightDepth[0][3] = camLD04;
		camLightDepth[0][4] = camLD05;
		camLightDepth[0][5] = camLD06;
	} 
	else 
	{
		PTR_SAFE_REMOVE(camLD01);
		PTR_SAFE_REMOVE(camLD02);
		PTR_SAFE_REMOVE(camLD03);
		PTR_SAFE_REMOVE(camLD04);
		PTR_SAFE_REMOVE(camLD05);
		PTR_SAFE_REMOVE(camLD06);
	}
	
	if(MAX_SHADOW_CASTER_LIGHTS > 1)
	{
		camLightDepth[1][0] = camLD11;
		camLightDepth[1][1] = camLD12;
		camLightDepth[1][2] = camLD13;
		camLightDepth[1][3] = camLD14;
		camLightDepth[1][4] = camLD15;
		camLightDepth[1][5] = camLD16;
	} 
	else 
	{
		PTR_SAFE_REMOVE(camLD11);
		PTR_SAFE_REMOVE(camLD12);
		PTR_SAFE_REMOVE(camLD13);
		PTR_SAFE_REMOVE(camLD14);
		PTR_SAFE_REMOVE(camLD15);
		PTR_SAFE_REMOVE(camLD16);
	}
	
	if(MAX_SHADOW_CASTER_LIGHTS > 2)
	{
		camLightDepth[2][0] = camLD21;
		camLightDepth[2][1] = camLD22;
		camLightDepth[2][2] = camLD23;
		camLightDepth[2][3] = camLD24;
		camLightDepth[2][4] = camLD25;
		camLightDepth[2][5] = camLD26;
	} 
	else 
	{
		PTR_SAFE_REMOVE(camLD21);
		PTR_SAFE_REMOVE(camLD22);
		PTR_SAFE_REMOVE(camLD23);
		PTR_SAFE_REMOVE(camLD24);
		PTR_SAFE_REMOVE(camLD25);
		PTR_SAFE_REMOVE(camLD26);
	}
		
	if(MAX_SHADOW_CASTER_LIGHTS > 3)
	{
		camLightDepth[3][0] = camLD31;
		camLightDepth[3][1] = camLD32;
		camLightDepth[3][2] = camLD33;
		camLightDepth[3][3] = camLD34;
		camLightDepth[3][4] = camLD35;
		camLightDepth[3][5] = camLD36;
	} 
	else 
	{
		PTR_SAFE_REMOVE(camLD31);
		PTR_SAFE_REMOVE(camLD32);
		PTR_SAFE_REMOVE(camLD33);
		PTR_SAFE_REMOVE(camLD34);
		PTR_SAFE_REMOVE(camLD35);
		PTR_SAFE_REMOVE(camLD36);
	}
	
	if(MAX_SHADOW_CASTER_LIGHTS > 4)
	{
		camLightDepth[4][0] = camLD41;
		camLightDepth[4][1] = camLD42;
		camLightDepth[4][2] = camLD43;
		camLightDepth[4][3] = camLD44;
		camLightDepth[4][4] = camLD45;
		camLightDepth[4][5] = camLD46;
	} 
	else 
	{
		PTR_SAFE_REMOVE(camLD41);
		PTR_SAFE_REMOVE(camLD42);
		PTR_SAFE_REMOVE(camLD43);
		PTR_SAFE_REMOVE(camLD44);
		PTR_SAFE_REMOVE(camLD45);
		PTR_SAFE_REMOVE(camLD46);
	}
	
	if(MAX_SHADOW_CASTER_LIGHTS > 5)
	{
		camLightDepth[5][0] = camLD51;
		camLightDepth[5][1] = camLD52;
		camLightDepth[5][2] = camLD53;
		camLightDepth[5][3] = camLD54;
		camLightDepth[5][4] = camLD55;
		camLightDepth[5][5] = camLD56;
	} 
	else 
	{
		PTR_SAFE_REMOVE(camLD51);
		PTR_SAFE_REMOVE(camLD52);
		PTR_SAFE_REMOVE(camLD53);
		PTR_SAFE_REMOVE(camLD54);
		PTR_SAFE_REMOVE(camLD55);
		PTR_SAFE_REMOVE(camLD56);
	}
	
	if(MAX_SHADOW_CASTER_LIGHTS > 6)
	{
		camLightDepth[6][0] = camLD61;
		camLightDepth[6][1] = camLD62;
		camLightDepth[6][2] = camLD63;
		camLightDepth[6][3] = camLD64;
		camLightDepth[6][4] = camLD65;
		camLightDepth[6][5] = camLD66;
	} 
	else 
	{
		PTR_SAFE_REMOVE(camLD61);
		PTR_SAFE_REMOVE(camLD62);
		PTR_SAFE_REMOVE(camLD63);
		PTR_SAFE_REMOVE(camLD64);
		PTR_SAFE_REMOVE(camLD65);
		PTR_SAFE_REMOVE(camLD66);
	}
	
	if(MAX_SHADOW_CASTER_LIGHTS > 7)
	{
		camLightDepth[7][0] = camLD71;
		camLightDepth[7][1] = camLD72;
		camLightDepth[7][2] = camLD73;
		camLightDepth[7][3] = camLD74;
		camLightDepth[7][4] = camLD75;
		camLightDepth[7][5] = camLD76;
	} 
	else 
	{
		PTR_SAFE_REMOVE(camLD71);
		PTR_SAFE_REMOVE(camLD72);
		PTR_SAFE_REMOVE(camLD73);
		PTR_SAFE_REMOVE(camLD74);
		PTR_SAFE_REMOVE(camLD75);
		PTR_SAFE_REMOVE(camLD76);
	}
}

// ------------------------------------------------------------------

MATERIAL *mtlPostLight = {}

MATERIAL *mtlDepth = {}
MATERIAL *mtlDepthWaving = {}

MATERIAL *mtlSky = {}

void materials_startup() 
{
	wait(1);
	effect_load(mtlDepth, "mtlDepth.fx");
	effect_load(mtlDepthWaving, "mtlDepthWaving.fx");
	
	effect_load(mtlSky, "mtlSky.fx");
}

// ------------------------------------------------------------------

void lightsource_mtl_event()
{
	LPD3DXEFFECT _fx = mtl->d3deffect;
	_fx->SetFloat("fDLCount", nextLightCount);
	_fx->SetVectorArray("vecDLPos", nextLightPos, 8);
	_fx->SetVectorArray("vecDLColor", nextLightColor, 8);
	
	int i = 0, j = 0;
	nextLightCount = view_to_light(camDummy, 1, NULL);
	var shaded_num = minv(nextLightCount, MAX_SHADOW_CASTER_LIGHTS);

	for(i = 0; i < shaded_num; i += 1)
	{
		for(j = 0; j < 6; j += 1)
		{
			view_to_light(camLightDepth[i][j], i + 1, NULL);
			camLightDepth[i][j]->bmap = bmpCubeMaps[i];
			set(camLightDepth[i][j], SHOW);
		}
		nextLightPos[i][0] = hit->entity->x;
		nextLightPos[i][1] = hit->entity->z;
		nextLightPos[i][2] = hit->entity->y;
		nextLightPos[i][3] = hit->entity->lightrange;
		nextLightColor[i][0] = hit->entity->red / 255.0;
		nextLightColor[i][1] = hit->entity->green / 255.0;
		nextLightColor[i][2] = hit->entity->blue / 255.0;
//		nextLightColor[i][3] = hit->entity->lightrange;
	}
	
	for(i = shaded_num; i < MAX_SHADOW_CASTER_LIGHTS; i += 1)
	{
		for(j = 0; j < 6; j += 1)
		{
			reset(camLightDepth[i][j], SHOW);
		}
	}
	
	for(i = shaded_num; i < nextLightCount; i += 1)
	{
		view_to_light(camDummy, i + 1, NULL);
		nextLightPos[i][0] = hit->entity->x;
		nextLightPos[i][1] = hit->entity->z;
		nextLightPos[i][2] = hit->entity->y;
		nextLightPos[i][3] = hit->entity->lightrange;
		nextLightColor[i][0] = hit->entity->red / 255.0;
		nextLightColor[i][1] = hit->entity->green / 255.0;
		nextLightColor[i][2] = hit->entity->blue / 255.0;
//		nextLightColor[i][3] = hit->entity->lightrange;
	}
	
	#ifdef DEBUG_PIPELINE
		wait(1);
		draw_text(str_printf(NULL, "Amount of visible lights: %d", (long)nextLightCount), 10, 120, COLOR_WHITE);
		draw_text(str_printf(NULL, "Amount of shadowed lights: %d", (long)shaded_num), 10, 140, COLOR_WHITE);
	#endif
}

void pipeline_update()
{
	int i;
	
	// rebuilt light depth cubemaps
	for(i = 0; i< MAX_SHADOW_CASTER_LIGHTS; i +=1)
		bmap_to_cubemap(bmpCubeMaps[i]);
	
	// due to an engine bug it needs to reset main cameras rendering dX viewport on resolution changes
	// the following code lines do the trick
	bmap_rendertarget(bmpTranslucent, 0, 0); 
	bmap_rendertarget(NULL, 0, 0);
}

var engine_level_load(STRING *levelName);

var pipeline_level_load(STRING *levelName)
{
	PTR_SAFE_REMOVE(entSky);
	
	engine_level_load(levelName);
	
	entSky = ent_create(SPHERE_MDL, nullvector, NULL);
	vec_fill(&entSky->scale_x, (camera->clip_far * 0.99) / entSky->max_x);
	set(entSky, LIGHT | UNLIT | PASSABLE);
	entSky->material = mtlSky;
}

void pipeline_startup()
{
	int i = 0, j = 0;
	
	// engines level_load function override
	engine_level_load = level_load;
	level_load = pipeline_level_load;
	
	light_view = camera;
	//on_frame = lightsource_mtl_event;
	
	while(engine_status(0) != 5)
		wait(1);
	
	// Resize engines depthstencil and backbuffer
	if(render_zbuffer != NULL) 
	{
		#ifdef DEBUG_PIPELINE
			error("ZBuffer already exists!");
		#endif		
		PTR_SAFE_REMOVE(render_zbuffer);
	}
	bmap_zbuffer(bmap_createblack(maxv(2048, CUBEMAP_SIZE * 6), maxv(2048, CUBEMAP_SIZE), 32));
	
	// Config materials
	mtlPostLight->skill1 = floatv(camera->clip_far);
	mtlPostLight->event = lightsource_mtl_event;
	mtlPostLight->flags = ENABLE_VIEW;
	mtlPostLight->technique = "Diffuse";
	effect_load(mtlPostLight, "ppLights.fx");
	
	bmap_to_cubemap(bmpSky);
	mtlSky->skin1 = bmpSky;
	
	lod_0 = (double)camera->clip_far * 2.0 * (double)d3d_lodfactor.x / 100.0;
	lod_1 = (double)camera->clip_far * 2.0 * (double)(d3d_lodfactor.y - d3d_lodfactor.x) / 100.0;
	lod_2 = (double)camera->clip_far * 2.0 * (double)d3d_lodfactor.y / 100.0;
	lod_3 = (double)camera->clip_far * 2.0 * (double)(d3d_lodfactor.z - d3d_lodfactor.y) / 100.0;
	
	while(engine_status(0) != 5)
		wait(1);
	// Create depth views
	for(i = 0; i < MAX_SHADOW_CASTER_LIGHTS; i += 1)
	{
		for(j = 0; j < 6; j++)
		{
			camLightDepth[i][j]->arc = 90;
			camLightDepth[i][j]->size_x = CUBEMAP_SIZE;
			camLightDepth[i][j]->size_y = CUBEMAP_SIZE;
			camLightDepth[i][j]->pos_x = j * CUBEMAP_SIZE;
			camLightDepth[i][j]->pos_y = 0;
			camLightDepth[i][j]->pan = nCubemapAngles[j][0];
			camLightDepth[i][j]->tilt = nCubemapAngles[j][1];
			camLightDepth[i][j]->bmap = bmpDepthCubeEmpty;
			camLightDepth[i][j]->flags |= NOFOG | NOPARTICLE | NOSHADOW | SHADOW | NOFLAG1; 
			camLightDepth[i][j]->flags &= ~SHOW; 
			camLightDepth[i][j]->clip_near = 1; 
			camLightDepth[i][j]->clip_far = 100; 
			camLightDepth[i][j]->fog_start = 90000; 
			camLightDepth[i][j]->fog_end = 100000; 
			camLightDepth[i][j]->material = mtlDepth;
		}
	}
	
	// Set render targets
	camera->bg = pixel_for_vec(nullvector, 1, 8888);
	camera->bmap = bmpTranslucent;
	camera->target1 = bmpCamera;
	camera->target2 = bmpWorld;
	camera->target3 = bmpAmbient;
	camera->flags |= NOSKY;
	if(camera->size_x == 0) {
		camera->size_x = camPostLight->size_x = screen_size.x;
		camera->size_y = camPostLight->size_y = screen_size.y;
	}
	
	// create postprocessing views
	camPostLight->material = mtlPostLight;
	camPostLight->flags |= PROCESS_TARGET | SHOW | CHILD;
	
	// Link views
	camera->stage = camPostLight;
	
	// PSone visuals
	cutoff_distance = camera->clip_far * 2;
	
	PipelineReady = 1;
	
	while(PipelineReady == 1)
	{
		proc_mode = PROC_LATE;
		
		camPostLight->pos_x = camera->pos_x;
		camPostLight->pos_y = camera->pos_y;
		camPostLight->size_x = camera->size_x;
		camPostLight->size_y = camera->size_y;
		
		if(entSky != NULL) {
			vec_set(&entSky->x, &camera->x);
			vec_fill(&entSky->scale_x, (camera->clip_far * 0.99) / entSky->max_x);
		}
		
		wait(1);
	}
}

void ent_set_as_solid(ENTITY *ent) 
{
	#ifdef DEBUG_PIPELINE
		if(ent == NULL)
		{
			error("set_as_solid_ent parameter can't be NULL!");
			return;
		}
	#endif
	
	set(ent, SHADOW);
	reset(ent, TRANSLUCENT | FLAG1);
}

void ent_set_as_translucent(ENTITY *ent)
{
	#ifdef DEBUG_PIPELINE
		if(ent == NULL)
		{
			error("set_as_translucent_ent parameter can't be NULL!");
			return;
		}
	#endif
	
	reset(ent, SHADOW);
	set(ent, TRANSLUCENT | FLAG1);
}

