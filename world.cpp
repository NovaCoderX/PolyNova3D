// PolyNova3D (version 3.3)
/***************************************************************
 Copyright (C) 1999 Novasoft Consulting

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Library General Public
 License as published by the Free Software Foundation; either
 version 2 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Library General Public License for more details.

 You should have received a copy of the GNU Library General Public
 License along with this library; if not, write to the Free
 Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *****************************************************************/
#include "poly_nova_3d.h"
#include "ini_file.h"

#include <SDL/SDL.h>


static Uint32 lastFrameTime = 0;

/*****************************************************************************/
// Public
/*****************************************************************************/
World::World() {
	// All objects in the world.
	object_list_head = NULL;

	// Just the visible objects.
	visible_objects = NULL;
	maxVisibleObjects = 0;
	numVisibleObjects = 0;

	// Just the solid objects.
	solid_objects = NULL;
	maxSolidObjects = 0;
	numSolidObjects = 0;

	// Lighting.
	light_list_head = NULL;
	enabledLights = NULL;
	numEnabledLights = 0;
	maxEnabledLights = 0;

	pointLights = NULL;
	numPointLights = 0;
	maxPointLights = 0;

	display = NULL;
	scene = NULL;
	player = NULL;
	camera = NULL;
}

World::~World() {
	LogMessage("Shutting down the World.....\n");

	if (camera) {
		delete camera;
		camera = NULL;
	}

	if (player) {
		delete player;
		player = NULL;
	}

	if (scene) {
		delete scene;
		scene = NULL;
	}

	if (display) {
		delete display;
		display = NULL;
	}

	if (pointLights) {
		delete[] pointLights;
		pointLights = NULL;
	}

	if (enabledLights) {
		delete[] enabledLights;
		enabledLights = NULL;
	}

	if (solid_objects) {
		delete[] solid_objects;
		solid_objects = NULL;
	}

	if (visible_objects) {
		delete[] visible_objects;
		visible_objects = NULL;
	}
}

void World::create() {
	char sceneName[PATH_MAX];

	LogMessage("Creating the World.....\n");

	Open_IniFile();

	IniFile_GetString("scene_file", "lights.scn", sceneName, PATH_MAX);
	int screenWidth = IniFile_GetInteger("screen_width", 640);
	int	screenHeight = IniFile_GetInteger("screen_height", 480);
	bool enableDepthBuffer = (IniFile_GetInteger("enable_zbuffer", 1) != 0) ? true : false;
	unsigned int fov = IniFile_GetInteger("fov", 600);
	float nearZ = IniFile_GetFloat("nearZ", 1.0f);
	float farZ = IniFile_GetFloat("farZ", 140.0f);

	Close_IniFile();

	display = new Display();
	display->initialize(screenWidth, screenHeight, enableDepthBuffer, nearZ, farZ);

	// Open the scene file, any errors and FatalError() will be called directly.
	scene = new SceneDefinition();
	scene->loadScene(sceneName);

	// Create the player.
	player = new AstPlayer(scene->getPlayerHead(), scene->getPlayerBody());

	// Create the camera.
	camera = new Camera(player->getPlayerHead(), fov, nearZ, farZ);

	// Count objects and lights that have been just inserted into the World..
	BaseObject *object = object_list_head;
	while (object) {
		maxVisibleObjects++;

		if (object->isSolid()) {
			maxSolidObjects++;
		}

		object = object->next_in_list;
	}

	visible_objects = new BaseObject*[maxVisibleObjects];
	solid_objects = new BaseObject*[maxSolidObjects];

	object = object_list_head;
	while (object) {
		if (object->isSolid()) {
			if (numSolidObjects < maxSolidObjects) {
				solid_objects[numSolidObjects++] = object;
			} else {
				LogWarningMessage("Too many solid objects!");
			}
		}

		object = object->next_in_list;
	}

	LightSource *light = light_list_head;
	while (light != 0) {
		maxEnabledLights++;

		if (light->GetType() == LIGHTING_TYPE_POINT) {
			maxPointLights++;
		}

		light = light->next_in_list;
	}

	enabledLights = new LightSource*[maxEnabledLights];
	pointLights = new PointLight*[maxPointLights];

	// Now find all the point lights in the World and store them (for the light switches).
	light = light_list_head;
	while (light != 0) {
		if (light->GetType() == LIGHTING_TYPE_POINT) {
			if (numPointLights < maxPointLights)
				pointLights[numPointLights++] = ((PointLight*)light);
			else {
				LogWarningMessage("Too many point lights!\n");
			}
		}

		light = light->next_in_list;
	}

	// Seed.
	lastFrameTime = SDL_GetTicks();
}

void World::addObjectToWorld(BaseObject &object) {
	/*
	 ** If no objects have been inserted into the world yet
	 ** then make this object the head of the object list.
	 */
	if (object_list_head == 0) {
		object_list_head = &object;
		object_list_head->prior_in_list = 0;
		object_list_head->next_in_list = 0;
	} else {
		/*
		 ** Otherwise find the last object in the g_world and then
		 ** set the passed object as the new last object in the
		 ** g_world.
		 */
		BaseObject *last_in_list = object_list_head;

		/*
		 ** Find the last object in the g_world
		 */
		while (last_in_list->next_in_list != 0) {
			/*
			 ** See if the passed object has already been inserted
			 */
			if (last_in_list == &object)
				return;

			last_in_list = last_in_list->next_in_list;
		}

		/*
		 ** Adjust pointers
		 */
		last_in_list->next_in_list = &object;
		object.prior_in_list = last_in_list;
		object.next_in_list = 0;
	}
}

void World::addLightToWorld(LightSource &light) {
	/*
	 ** If no objects have been inserted into the world yet
	 ** then make this object the head of the object list.
	 */
	if (light_list_head == 0) {
		light_list_head = &light;
		light_list_head->prior_in_list = 0;
		light_list_head->next_in_list = 0;
	} else {
		/*
		 ** Otherwise find the last object in the world and then
		 ** set the passed object as the new last object in the
		 ** world.
		 */
		LightSource *last_in_list = light_list_head;

		/*
		 ** Find the last object in the world
		 */
		while (last_in_list->next_in_list != 0) {
			/*
			 ** See if the passed object has already been inserted
			 */
			if (last_in_list == &light)
				return;

			last_in_list = last_in_list->next_in_list;
		}

		/*
		 ** Adjust pointers
		 */
		last_in_list->next_in_list = &light;
		light.prior_in_list = last_in_list;
		light.next_in_list = 0;
	}
}

void World::run() {
	Uint32 currentFrameTime;

	while (true) {
		// First capture any pending user input.
		display->captureUserInput();

		// That update the World based on that user input in a linear fashion.
		currentFrameTime = SDL_GetTicks();
		player->doSomething((currentFrameTime - lastFrameTime) / 1000.0);
		lastFrameTime = currentFrameTime;

		// If a collision occurs (as a result of the last user input) then we need to move the object in the
		// opposite direction it just moved by the number of units it just moved.
		for (int i = 0; i < numSolidObjects; i++) {
			if (player->checkCollision(solid_objects[i])) {
				player->collisionOccured();
				break;
			}
		}

		// Reset counts for this frame.
		numVisibleObjects = numEnabledLights = 0;

		// Calculate our world_to_camera_matrix so that we can transform object positions into the CCS.
		camera->calculateWorldToCameraMatrix();

		calculateVisibleObjects();
		calculateEnabledLights();

		// Clear the back buffer.
		display->begin();

		// Draw any visible objects using the enabled lights.
		for (int i = 0; i < numVisibleObjects; i++) {
			visible_objects[i]->draw();
		}

		// Show the updated frame.
		display->end();
	}
}

void World::toggleLightSwitch(int index) {
	if (index >= 0 && index < numPointLights) {
		pointLights[index]->toggle();
	}
}

/*****************************************************************************/
// Private
/*****************************************************************************/
void World::calculateVisibleObjects() {
	// Iterate through the list of objects in the World.
	BaseObject *object = object_list_head;

	while (object) {
		/*
		 ** Determine visibility, and if visible store the address
		 ** of the object in the next available slot in the
		 ** visible_objects array.
		 */
		if (object->isVisibile()) {
			if (numVisibleObjects < maxVisibleObjects) {
				visible_objects[numVisibleObjects++] = object;
			} else {
				LogWarningMessage("Too many visible objects!\n");
			}
		} //else {
		//	LogDebugMessage("Obj = %s, def = %s is invisible\n", object->getName(), object->getObjectDefinition()->getName());
		//}

		object = object->next_in_list;
	}
}

void World::calculateEnabledLights() {
	// Iterate through the list of lights in the world.
	LightSource *light = light_list_head;

	while (light != 0) {
		if (light->isEnabled()) {
			if (numEnabledLights < maxEnabledLights)
				enabledLights[numEnabledLights++] = light;
			else {
				LogWarningMessage("Too many enabled lights!\n");
			}
		}

		light = light->next_in_list;
	}
}

