/*******************************************************************************************
*
*   raylib [models] example - Load models vox (MagicaVoxel)
*
*   Example originally created with raylib 4.0, last time updated with raylib 4.0
*
*   Example contributed by Johann Nadalutti (@procfxgen) and reviewed by Ramon Santamaria (@raysan5)
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2021-2024 Johann Nadalutti (@procfxgen) and Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#include "raymath.h"        // Required for: MatrixTranslate()

#define MAX_VOX_FILES  4

#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION            100
#endif

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
	// Initialization
	//--------------------------------------------------------------------------------------
	const int screenWidth = 800;
	const int screenHeight = 450;

	const char* voxFileNames[] = {
		"resources/models/vox/chr_knight.vox",
		"resources/models/vox/chr_sword.vox",
		"resources/models/vox/monu9.vox",
		"resources/models/vox/fez.vox"
	};

	rlInitWindow(screenWidth, screenHeight, "raylib [models] example - magicavoxel loading");

	// Define the camera to look into our 3d world
	Camera camera = { 0 };
	camera.position = (rlVector3){ 10.0f, 10.0f, 10.0f }; // Camera position
	camera.target = (rlVector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
	camera.up = (rlVector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
	camera.fovy = 45.0f;                                // Camera field-of-view Y
	camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

	//--------------------------------------------------------------------------------------
	// Load MagicaVoxel files
	rlModel models[MAX_VOX_FILES] = { 0 };

	for (int i = 0; i < MAX_VOX_FILES; i++)
	{
		// Load VOX file and measure time
		double t0 = rlGetTime() * 1000.0;
		models[i] = rlLoadModel(voxFileNames[i]);
		double t1 = rlGetTime() * 1000.0;

		rlTraceLog(LOG_WARNING, rlTextFormat("[%s] File loaded in %.3f ms", voxFileNames[i], t1 - t0));

		// Compute model translation matrix to center model on draw position (0, 0 , 0)
		rlBoundingBox bb = rlGetModelBoundingBox(models[i]);
		rlVector3 center = { 0 };
		center.x = bb.min.x + (((bb.max.x - bb.min.x) / 2));
		center.z = bb.min.z + (((bb.max.z - bb.min.z) / 2));

		rlMatrix matTranslate = MatrixTranslate(-center.x, 0, -center.z);
		models[i].transform = matTranslate;
	}

	int currentModel = 0;

	//--------------------------------------------------------------------------------------
	// Load voxel shader
	rlShader shader = rlLoadShader(rlTextFormat("resources/shaders/glsl%i/voxel_lighting.vs", GLSL_VERSION),
		rlTextFormat("resources/shaders/glsl%i/voxel_lighting.fs", GLSL_VERSION));

	// Get some required shader locations
	shader.locs[SHADER_LOC_VECTOR_VIEW] = rlGetShaderLocation(shader, "viewPos");
	// NOTE: "matModel" location name is automatically assigned on shader loading, 
	// no need to get the location again if using that uniform name
	//shader.locs[SHADER_LOC_MATRIX_MODEL] = rlGetShaderLocation(shader, "matModel");

	// Ambient light level (some basic lighting)
	int ambientLoc = rlGetShaderLocation(shader, "ambient");
	rlSetShaderValue(shader, ambientLoc, (float[4]) { 0.1f, 0.1f, 0.1f, 1.0f }, SHADER_UNIFORM_VEC4);

	// Assign out lighting shader to model
	for (int i = 0; i < MAX_VOX_FILES; i++)
	{
		rlModel m = models[i];
		for (int j = 0; j < m.materialCount; j++)
		{
			m.materials[j].shader = shader;
		}
	}

	// Create lights
	Light lights[MAX_LIGHTS] = { 0 };
	lights[0] = CreateLight(LIGHT_POINT, (rlVector3) { -20, 20, -20 }, Vector3Zero(), GRAY, shader);
	lights[1] = CreateLight(LIGHT_POINT, (rlVector3) { 20, -20, 20 }, Vector3Zero(), GRAY, shader);
	lights[2] = CreateLight(LIGHT_POINT, (rlVector3) { -20, 20, 20 }, Vector3Zero(), GRAY, shader);
	lights[3] = CreateLight(LIGHT_POINT, (rlVector3) { 20, -20, -20 }, Vector3Zero(), GRAY, shader);


	rlSetTargetFPS(60);               // Set our game to run at 60 frames-per-second

	//--------------------------------------------------------------------------------------
	rlVector3 modelpos = { 0 };
	rlVector3 camerarot = { 0 };

	// Main game loop
	while (!rlWindowShouldClose())    // Detect window close button or ESC key
	{
		// Update
		//----------------------------------------------------------------------------------
		if (rlIsMouseButtonDown(MOUSE_BUTTON_MIDDLE))
		{
			const rlVector2 mouseDelta = rlGetMouseDelta();
			camerarot.x = mouseDelta.x * 0.05f;
			camerarot.y = mouseDelta.y * 0.05f;
		}
		else
		{
			camerarot.x = 0;
			camerarot.y = 0;
		}

		rlUpdateCameraPro(&camera,
			(rlVector3) {
			(rlIsKeyDown(KEY_W) || rlIsKeyDown(KEY_UP)) * 0.1f -      // Move forward-backward
				(rlIsKeyDown(KEY_S) || rlIsKeyDown(KEY_DOWN)) * 0.1f,
				(rlIsKeyDown(KEY_D) || rlIsKeyDown(KEY_RIGHT)) * 0.1f -   // Move right-left
				(rlIsKeyDown(KEY_A) || rlIsKeyDown(KEY_LEFT)) * 0.1f,
				0.0f                                                // Move up-down
		},
			camerarot,
			rlGetMouseWheelMove() * -2.0f);                              // Move to target (zoom)

		// Cycle between models on mouse click
		if (rlIsMouseButtonPressed(MOUSE_BUTTON_LEFT)) currentModel = (currentModel + 1) % MAX_VOX_FILES;

		// Update the shader with the camera view vector (points towards { 0.0f, 0.0f, 0.0f })
		float cameraPos[3] = { camera.position.x, camera.position.y, camera.position.z };
		rlSetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);

		// Update light values (actually, only enable/disable them)
		for (int i = 0; i < MAX_LIGHTS; i++) UpdateLightValues(shader, lights[i]);

		//----------------------------------------------------------------------------------
		// Draw
		//----------------------------------------------------------------------------------
		rlBeginDrawing();

		rlClearBackground(RAYWHITE);

		// Draw 3D model
		rlBeginMode3D(camera);

		rlDrawModel(models[currentModel], modelpos, 1.0f, WHITE);
		rlDrawGrid(10, 1.0);

		// Draw spheres to show where the lights are
		for (int i = 0; i < MAX_LIGHTS; i++)
		{
			if (lights[i].enabled) rlDrawSphereEx(lights[i].position, 0.2f, 8, 8, lights[i].color);
			else rlDrawSphereWires(lights[i].position, 0.2f, 8, 8, rlColorAlpha(lights[i].color, 0.3f));
		}

		rlEndMode3D();

		// Display info
		rlDrawRectangle(10, 400, 340, 60, rlFade(SKYBLUE, 0.5f));
		rlDrawRectangleLines(10, 400, 340, 60, rlFade(DARKBLUE, 0.5f));
		rlDrawText("MOUSE LEFT BUTTON to CYCLE VOX MODELS", 40, 410, 10, BLUE);
		rlDrawText("MOUSE MIDDLE BUTTON to ZOOM OR ROTATE CAMERA", 40, 420, 10, BLUE);
		rlDrawText("UP-DOWN-LEFT-RIGHT KEYS to MOVE CAMERA", 40, 430, 10, BLUE);
		rlDrawText(rlTextFormat("File: %s", rlGetFileName(voxFileNames[currentModel])), 10, 10, 20, GRAY);

		rlEndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	// Unload models data (GPU VRAM)
	for (int i = 0; i < MAX_VOX_FILES; i++) rlUnloadModel(models[i]);

	rlCloseWindow();          // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}


