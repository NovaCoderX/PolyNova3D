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
#ifndef __WORLD_H
#define __WORLD_H

class World {
	friend class BaseObject;
	friend class SceneDefinition;
	friend class Camera;

public:
	World();
	~World();

	void create();

	Display* getDisplay() const {
		return display;
	}

	AstPlayer* getPlayer() const {
		return player;
	}

	Camera* getCamera() const {
		return camera;
	}

	void addObjectToWorld(BaseObject &object);
	void addLightToWorld(LightSource &light);
	void toggleLightSwitch(int index);
	void run();

private:
	// Maintains a linked list of all objects in the world.
	BaseObject *object_list_head;

	// This is array of pointers to objects which are to be drawn each frame.
	BaseObject **visible_objects;
	int numVisibleObjects;
	int maxVisibleObjects;

	// This is array of pointers to objects which need to be tested for collisions.
	BaseObject **solid_objects;
	int numSolidObjects;
	int maxSolidObjects;

	// Maintains a linked list of lights.
	LightSource *light_list_head;

	LightSource **enabledLights;
	int numEnabledLights;
	int maxEnabledLights;

	PointLight **pointLights;
	int numPointLights;
	int maxPointLights;

	Display *display;
	SceneDefinition *scene;
	AstPlayer *player;
	Camera *camera;

	void calculateVisibleObjects();
	void calculateEnabledLights();
};





#endif // __WORLD_H

