#ifndef _SHADOWS_H_
	#define _SHADOWS_H_
	// originally created by José Miguel Rodríguez Chávarri, aka txesmi
	// refactoring by Magomet Kochkarov, aka 3RUN
	
	#define DEBUG_PIPELINE
	
	// skybox
	#define SKYBOX_FILE "skycube+6.tga"
	#define SKYBOX_FOG_FACTOR 0.98

	// terrain shader texture scales
	float matScales[16] = { 16, 8, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

	var nCubemapAngles[6][2] = { 180, 0 , 90, 0, 0, 0, 270, 0, 90, -90, 90, 90 };
	var PipelineReady = 0;

	// PSone visuals
	var vertex_snapping = 120; // good range is from 50-200 (smaller value = less snapping)
	var cutoff_distance = 256; // distance for polygon cutoff (in quants)

	// onidireccional shadows
	#define CUBEMAP_SIZE 256

	#define MAX_SHADOW_CASTER_LIGHTS 5 // this affects the performance, try to keep as small as possible
	#define OBJ_CUBEMAP skill99

	ENTITY *entSky;

	BMAP *bmpDepthCubeEmpty;
	BMAP *bmpCubeMap1, *bmpCubeMap2, *bmpCubeMap3, *bmpCubeMap4, *bmpCubeMap5, *bmpCubeMap6, *bmpCubeMap7, *bmpCubeMap8;
	BMAP *bmpCamera, *bmpWorld, *bmpAmbient, *bmpLightened, *bmpTranslucent;
	BMAP *bmpSky;

	// postprocessing views
	VIEW *camPostBlur, *camPostLight, *camLightDepth[MAX_SHADOW_CASTER_LIGHTS][6];

	VIEW *camDummy = { flags = PROCESS_TARGET; }

	// materials
	MATERIAL *mtlPostBlur;
	MATERIAL *mtlPostLight;

	MATERIAL *mtlDepth =
	{
		effect = "mtlDepth.fx";
	}

	MATERIAL *mtlBlock =
	{
		effect = "mtlBlock.fx";
	}

	MATERIAL *mtlEntity =
	{
		effect = "mtlEntity.fx";
	}

	MATERIAL *mtlWaving =
	{
		effect = "mtlWaving.fx";
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

	MATERIAL *mtlSky = 
	{
		effect = "mtlSky.fx";
	}

	void terrain_mtl_event();

	MATERIAL *mtlTerrain = 
	{
		effect = "mtlTerrain.fx";
		event = terrain_mtl_event;
	}
	
	// function that updates all cubemaps for light sources within the view (camera) frustum
	void lightsource_mtl_event();
	
	// create lightsource bmap for given entity
	void create_lightsource_bmap(ENTITY *ent);
	
	// remove lightsource bmaps
	void remove_lightsource_bmaps_all();

	// as we save all lightsource cubemaps in their skills
	// we need to recreate them when we (f.e.) change resolution
	// this is pretty dirty way that removes previous and instantly creates new cubemap
	void update_lightsource_bmaps_all();
	
	// create pipeline
	void pipeline_create();
	
	// remove pipeline and all it's stuff
	void pipeline_remove();
	
	// function that should be called when we need to update pipeline (f.e. on resolution change)
	// it removes all previous pipeline stuff
	// updates lightsource cubemaps
	// creates new pipeline
	void pipeline_update();

	#include "shadows.c"
#endif