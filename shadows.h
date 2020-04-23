#ifndef _SHADOWS_H_
	#define _SHADOWS_H_
	// originally created by José Miguel Rodríguez Chávarri, aka txesmi
	// refactoring by Magomet Kochkarov, aka 3RUN
	// made static by José Miguel Rodríguez Chávarri, aka txesmi
	
	#define DEBUG_PIPELINE
	//#define PRECOMPILED_SHADERS
	
	// safe remove macros
	#define BMAP_SAFE_REMOVE(_bmp) if((_bmp) != NULL) bmap_remove((BMAP*)(_bmp)); (_bmp) = NULL
	#define PTR_SAFE_REMOVE(_ptr) if((_ptr) != NULL) ptr_remove((void*)(_ptr)); (_ptr) = NULL
	
	// skybox
	#define SKYBOX_FILE "skycube+6.tga"
	float sky_fog_factor = 0.98;
	
	// this svariable is set to 1 when the pipeline is ready
	// set in the second frame when everything went right
	var PipelineReady = 0;

	// PSone visuals
	float vertex_snapping = 120; // good range is from 50-200 (smaller value = less snapping)
	float cutoff_distance = 256; // distance for polygon cutoff (in quants)

	// omnidireccional shadows cubemap resolution
	#define CUBEMAP_SIZE 256
	#define CUBEMAP_DESC "#1536x256x12"
	
	// Number of maximum shadow caster dynamic lights rendered at the same time
	// it affects the performance dramatically, try to keep as small as possible
	#define MAX_SHADOW_CASTER_LIGHTS 4 

	// function that should be called when we need to update pipeline (f.e. on resolution change)
	void pipeline_update();
	
	// make an entity be drawn as a solid object
	// that casts and receives shadows
	void ent_set_as_solid(ENTITY *ent);
	
	// make an entity be drawn as a translucent object
	// that does not cast or receive shadows
	void ent_set_as_translucent(ENTITY *ent);
	
	#include "shadows.c"
#endif