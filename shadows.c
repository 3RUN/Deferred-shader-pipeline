
void terrain_mtl_event()
{
	int i;
	for(i = 0; i < 16; i++)
	{
		matScales[i] = my->skill[i];
	}
	mat_effect1 = matScales;
}

void lightsource_mtl_event()
{
	int i = 0, j = 0;
	var light_num = view_to_light(camDummy, 1, NULL);

	for(i = 0; i < light_num; i++)
	{
		view_to_light(camDummy, i+1, NULL);
		
		if(hit->entity == NULL)
		{
			wait(1);
			error("Every dynamic light has to be an entity");
			sys_exit(NULL);
		}
		else
		{
			switch(i)
			{
				case 0:
				{
					bmpCubeMap1 = (BMAP*)(hit->entity->OBJ_CUBEMAP);
					break;
				}
				case 1:
				{
					bmpCubeMap2 = (BMAP*)(hit->entity->OBJ_CUBEMAP);
					break;
				}
				case 2:
				{
					bmpCubeMap3 = (BMAP*)(hit->entity->OBJ_CUBEMAP);
					break;
				}
				case 3:
				{
					bmpCubeMap4 = (BMAP*)(hit->entity->OBJ_CUBEMAP);
					break;
				}
				case 4:
				{
					bmpCubeMap5 = (BMAP*)(hit->entity->OBJ_CUBEMAP);
					break;
				}
				case 5:
				{
					bmpCubeMap6 = (BMAP*)(hit->entity->OBJ_CUBEMAP);
					break;
				}
				case 6:
				{
					bmpCubeMap7 = (BMAP*)(hit->entity->OBJ_CUBEMAP);
					break;
				}
				case 7:
				{
					bmpCubeMap8 = (BMAP*)(hit->entity->OBJ_CUBEMAP);
					break;
				}
			}
			
			if(i < MAX_SHADOW_CASTER_LIGHTS)
			{
				for(j = 0; j < 6; j++)
				{
					view_to_light(camLightDepth[i][j], i+1, NULL);
					camLightDepth[i][j]->bmap = (BMAP*)(hit->entity->OBJ_CUBEMAP);
					set(camLightDepth[i][j], SHOW);
				}
			}
		}
	}
	
	for(i = light_num; i < 8; i++)
	{
		if(i < MAX_SHADOW_CASTER_LIGHTS)
		{
			for(j = 0; j < 6; j++)
			{
				reset(camLightDepth[i][j], SHOW);
			}
		}
		
		switch(i)
		{
			case 0:
			{
				bmpCubeMap1 = bmpDepthCubeEmpty;
				break;
			}
			case 1:
			{
				bmpCubeMap2 = bmpDepthCubeEmpty;
				break;
			}
			case 2:
			{
				bmpCubeMap3 = bmpDepthCubeEmpty;
				break;
			}
			case 3:
			{
				bmpCubeMap4 = bmpDepthCubeEmpty;
				break;
			}
			case 4:
			{
				bmpCubeMap5 = bmpDepthCubeEmpty;
				break;
			}
			case 5:
			{
				bmpCubeMap6 = bmpDepthCubeEmpty;
				break;
			}
			case 6:
			{
				bmpCubeMap7 = bmpDepthCubeEmpty;
				break;
			}
			case 7:
			{
				bmpCubeMap8 = bmpDepthCubeEmpty;
				break;
			}
			default:
			{
				wait(1);
				error("8 shadow caster dyn lights max!");
				sys_exit(NULL);
			}
		}
	}
	
	#ifdef DEBUG_PIPELINE
		wait(1);
		draw_text(str_printf(NULL, "Amount of visible lights: %d", (long)light_num), 10, 120, COLOR_WHITE);
	#endif
}

// create lightsource bmap for given entity
void create_lightsource_bmap(ENTITY *ent)
{
	if(!ent)
	{
		diag("ERROR! Can't create bmap for given light! Entity doesn't exist!");
		return;
	}
	
	BMAP *bmpTemp = bmap_createblack(CUBEMAP_SIZE * 6, CUBEMAP_SIZE, 12);
	ent->OBJ_CUBEMAP = (BMAP*)bmpTemp;
	bmap_to_cubemap(bmpTemp);
}

// remove lightsource bmaps
void remove_lightsource_bmaps_all()
{
	ENTITY *next = NULL;
	for(next = ent_next(NULL); next; next = ent_next(next)) 
	{
		// no bmap skill set ?
		if(!next->OBJ_CUBEMAP)
		{
			continue;
		}
		
		// remove bmap
		BMAP *bmpTemp = (BMAP*)next->OBJ_CUBEMAP;
		ptr_remove(bmpTemp);
		bmpTemp = NULL;
		next->OBJ_CUBEMAP = NULL;
	}
	next = NULL;
}

// dirty way to update lightsource bmaps
void update_lightsource_bmaps_all()
{
	ENTITY *next = NULL;
	for(next = ent_next(NULL); next; next = ent_next(next)) 
	{
		// no bmap skill set ?
		if(!next->OBJ_CUBEMAP)
		{
			continue;
		}
		
		// remove bmap
		BMAP *bmpTemp = (BMAP*)next->OBJ_CUBEMAP;
		ptr_remove(bmpTemp);
		bmpTemp = NULL;
		next->OBJ_CUBEMAP = NULL;
		
		// create new one
		bmpTemp = bmap_createblack(CUBEMAP_SIZE * 6, CUBEMAP_SIZE, 12);
		next->OBJ_CUBEMAP = (BMAP*)bmpTemp;
		bmap_to_cubemap(bmpTemp);
	}
	next = NULL;
}

void pipeline_create()
{
	int i = 0, j = 0;
	
	if(!render_zbuffer)
	{
		bmap_zbuffer(bmap_createblack(maxv(screen_size.x, CUBEMAP_SIZE * 6), maxv(screen_size.y, CUBEMAP_SIZE), 32));
	}
	else
	{
		error("zbuffer exists?");
	}
	
	// Create bmaps
	bmpSky = bmap_create(SKYBOX_FILE);
	bmap_to_cubemap(bmpSky);
	
	bmpDepthCubeEmpty = bmap_createblack(CUBEMAP_SIZE * 6, CUBEMAP_SIZE, 24);
	
	// Create render targets
	bmpCamera = bmap_createblack(screen_size.x, screen_size.y, 24);
	bmpWorld = bmap_createblack(screen_size.x, screen_size.y, 12222);
	bmpAmbient = bmap_createblack(screen_size.x, screen_size.y, 12222);
	bmpLightened = bmap_createblack(screen_size.x, screen_size.y, 24);
	bmpTranslucent = bmap_createblack(screen_size.x, screen_size.y, 24);
	
	// Create materials
	mtlPostBlur = mtl_create();
	effect_load(mtlPostBlur, "ppScreen.fx");
	
	mtlPostLight = mtl_create();
	mtlPostLight->skill1 = floatv(camera->clip_far);
	mtlPostLight->event = lightsource_mtl_event;
	mtlPostLight->flags = ENABLE_VIEW;
	mtlPostLight->technique = "Diffuse";
	effect_load(mtlPostLight, "ppLights.fx");
	
	mtlSky->skin1 = bmpSky;
	mtlSky->skill1 = floatv(SKYBOX_FOG_FACTOR);
	mtlSky->skill2 = floatv(vertex_snapping);
	
	mtlWaving->skill5 = floatd((camera->clip_far * 2) * d3d_lodfactor.x, 100);
	mtlWaving->skill6 = floatd((camera->clip_far * 2) * (d3d_lodfactor.y - d3d_lodfactor.x), 100);
	mtlWaving->skill7 = floatd((camera->clip_far * 2) * d3d_lodfactor.y, 100);
	mtlWaving->skill8 = floatd((camera->clip_far * 2) * (d3d_lodfactor.z - d3d_lodfactor.y), 100);
	
	mtlDepth->skill1 = mtlWaving->skill1;
	mtlDepth->skill2 = mtlWaving->skill2;
	mtlDepth->skill3 = mtlWaving->skill3;
	mtlDepth->skill4 = mtlWaving->skill4;
	mtlDepth->skill5 = floatv(camera->x);
	mtlDepth->skill6 = floatv(camera->z);
	mtlDepth->skill7 = floatv(camera->y);
	
	// Create depth views
	for(i = 0; i < MAX_SHADOW_CASTER_LIGHTS; i++)
	{
		for(j = 0; j < 6; j++)
		{
			camLightDepth[i][j] = view_create(1);
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
	
	// create postprocessing views
	camPostBlur = view_create(1);
	camPostBlur->flags |= PROCESS_TARGET | SHOW | CHILD;
	camPostBlur->material = mtlPostBlur;
	
	camPostLight = view_create(1);
	camPostLight->flags |= PROCESS_TARGET | SHOW | CHILD;
	camPostLight->material = mtlPostLight;
	
	// Set render targets
	camera->bg = pixel_for_vec(nullvector, 1, 8888);
	camera->bmap = bmpTranslucent;
	camera->target1 = bmpCamera;
	camera->target2 = bmpWorld;
	camera->target3 = bmpAmbient;
	camPostLight->bmap = bmpLightened;
	
	set(camera, NOSKY);
	
	// Link views
	camera->stage = camPostLight;
	camPostLight->stage = camPostBlur;
	
	entSky = ent_create(SPHERE_MDL, nullvector, NULL);
	vec_fill(&entSky->scale_x, (camera->clip_far * 0.99) / entSky->max_x);
	set(entSky, LIGHT | UNLIT | PASSABLE | CAST);
	entSky->material = mtlSky;
	
	PipelineReady = 1;
	
	while(PipelineReady == 1)
	{
		proc_mode = PROC_LATE;
		
		vec_set(&entSky->x, &camera->x);
		
		mtlDepth->skill5 = floatv(camera->x);
		mtlDepth->skill6 = floatv(camera->z);
		mtlDepth->skill7 = floatv(camera->y);
		
		// PSone visuals
		cutoff_distance = camera->clip_far * 2;
		mtlBlock->skill1 = floatv(vertex_snapping);
		mtlBlock->skill2 = floatv(cutoff_distance);
		mtlEntity->skill1 = floatv(vertex_snapping);
		mtlEntity->skill2 = floatv(cutoff_distance);
		mtlWaving->skill1 = floatv(vertex_snapping);
		mtlWaving->skill2 = floatv(cutoff_distance);
		mtlBright->skill1 = floatv(vertex_snapping);
		mtlBright->skill2 = floatv(cutoff_distance);
		mtlTerrain->skill1 = floatv(vertex_snapping);
		mtlTerrain->skill2 = floatv(cutoff_distance);
		mtlSky->skill4 = floatv(vertex_snapping);
		
		wait(1);
	}
}

void pipeline_remove()
{
	int i = 0, j = 0;
	
	if(render_zbuffer)
	{
		ptr_remove(render_zbuffer);
		render_zbuffer = NULL;
	}
	
	// Remove bmaps
	if(bmpSky)
	{
		ptr_remove(bmpSky);
		bmpSky = NULL;
	}
	
	if(bmpDepthCubeEmpty)
	{
		ptr_remove(bmpDepthCubeEmpty);
		bmpDepthCubeEmpty = NULL;
	}
	
	// Remove render targets
	if(bmpCamera)
	{
		ptr_remove(bmpCamera);
		bmpCamera = NULL;
	}
	
	if(bmpWorld)
	{
		ptr_remove(bmpWorld);
		bmpWorld = NULL;
	}
	
	if(bmpAmbient)
	{
		ptr_remove(bmpAmbient);
		bmpAmbient = NULL;
	}
	
	if(bmpLightened)
	{
		ptr_remove(bmpLightened);
		bmpLightened = NULL;
	}
	
	if(bmpTranslucent)
	{
		ptr_remove(bmpTranslucent);
		bmpTranslucent = NULL;
	}
	
	// Remove materials
	if(mtlPostBlur)
	{
		ptr_remove(mtlPostBlur);
		mtlPostBlur = NULL;
	}
	
	if(mtlPostLight)
	{
		ptr_remove(mtlPostLight);
		mtlPostLight = NULL;
	}
	
	// Create depth views
	for(i = 0; i < MAX_SHADOW_CASTER_LIGHTS; i++)
	{
		for(j = 0; j < 6; j++)
		{
			if(camLightDepth[i][j])
			{
				camLightDepth[i][j]->bmap = NULL;
				ptr_remove(camLightDepth[i][j]);
				camLightDepth[i][j] = NULL;
			}
		}
	}
	
	// remove views
	if(camPostBlur)
	{
		camPostBlur->bmap = NULL;
		camPostBlur->stage = NULL;
		ptr_remove(camPostBlur);
		camPostBlur = NULL;
	}
	
	if(camPostLight)
	{
		camPostLight->bmap = NULL;
		camPostLight->stage = NULL;
		ptr_remove(camPostLight);
		camPostLight = NULL;
	}
	
	// reset camera
	camera->bmap = NULL;
	camera->target1 = NULL;
	camera->target2 = NULL;
	camera->target3 = NULL;
	
	// unlink views
	camera->stage = NULL;
	
	// remove sky
	if(entSky)
	{
		ptr_remove(entSky);
		entSky = NULL;
	}
	
	PipelineReady = 0;
}

void pipeline_update()
{
	pipeline_remove();
	wait_for(pipeline_remove);
	
	update_lightsource_bmaps_all();
	wait_for(update_lightsource_bmaps_all);
	
	pipeline_create();
}