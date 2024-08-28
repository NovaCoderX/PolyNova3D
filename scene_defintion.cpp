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



/*****************************************************************************/
// Public
/*****************************************************************************/
SceneDefinition::SceneDefinition() {
	sceneFp = NULL;
	defs = NULL;
	numDefs = 0;
	baseObjects = NULL;
	playerHead = NULL;
	playerBody = NULL;
	ambientLights = NULL;
	directLights = NULL;
	pointLights = NULL;
}

SceneDefinition::~SceneDefinition() {
	if (sceneFp) {
		fclose(sceneFp);
		sceneFp = NULL;
	}

	if (ambientLights) {
		delete ambientLights;
		ambientLights = NULL;
	}

	if (directLights) {
		delete directLights;
		directLights = NULL;
	}

	if (pointLights) {
		delete pointLights;
		pointLights = NULL;
	}

	if (baseObjects) {
		delete[] baseObjects;
		baseObjects = NULL;
	}

	if (playerHead) {
		delete playerHead;
		playerHead = NULL;
	}

	if (playerBody) {
		delete playerBody;
		playerBody = NULL;
	}

	if (defs) {
		delete[] defs;
		defs = NULL;
	}
}

void SceneDefinition::loadScene(const char *fileName) {
	LogMessage("Loading scene: %s\n", fileName);

	// Open the file.
	if ((sceneFp = fopen(fileName, "rt")) == NULL) {
		FatalError("Could not open the scene file: %s", fileName);
	}

	LoadDefinitions();
	LoadObjects();
	loadPlayerObjects();
	loadAmbientLights();
	loadDirectLights();
	loadPointLights();

	// Close the file (no longer needed).
	if (sceneFp) {
		fclose(sceneFp);
		sceneFp = NULL;
	}
}

/*****************************************************************************/
// Private
/*****************************************************************************/

void SceneDefinition::moveToMainSection(const char *section_name) {
	char buffer[80];
	char error[255];	// Temp error msg.
	unsigned int temp;
	int ctr;
	bool found;

	found = false;

	while (!feof(sceneFp)) {
		fgets(buffer, 80, sceneFp);

		for (ctr = 0; ctr < 80; ctr++) {
			// Look for open brace.
			if (buffer[ctr] == '[') {
				found = true;

				// See if characters match.
				for (temp = 0; temp < strlen(section_name); temp++) {
					if (buffer[(ctr + 1) + temp] != section_name[temp]) {
						found = false;
						break;
					}
				}
				// Make sure there is a closing brace.
				if ((found == true) && (buffer[(ctr + 1) + temp] != ']'))
					found = false;

				// If found is still true a match was found, else continue.
				if (found)
					return;
			}  // IF.
		} // FOR.
	} // WHILE.

	if (!found) {
		strcpy(error, "Error finding main section name:");
		strcat(error, section_name);
		FatalError(error);
	}
}

void SceneDefinition::seekToMainSection(const char *section_name, int instance) {
	int ctr;

	rewind(sceneFp);	// Start at the start of file.

	// Move down file to correct occurrence of main-section.
	for (ctr = 0; ctr <= instance; ctr++) {
		moveToMainSection(section_name);
	}
}

void SceneDefinition::moveToOpenBrace(const char *section_name) {
	char buffer[80];
	int ctr;

	while (!feof(sceneFp)) {
		fgets(buffer, 80, sceneFp);

		for (ctr = 0; ctr < 80; ctr++) {
			if (buffer[ctr] == '\n' || buffer[ctr] == 0)
				break;

			if (buffer[ctr] == '{')
				return;	// Success.

			// Make sure we don't go past this section.
			switch (buffer[ctr]) {
			case '}':
				FatalError("Found a closing brace, when looking for opening in section: %s", section_name);
				break;

			case '<':
				FatalError("Found an opening sub-heading, when looking for opening brace in section: %s", section_name);
				break;

			case '>':
				FatalError("Found an closing sub-heading, when looking for opening brace in section: %s", section_name);
				break;

			case '[':
				FatalError("Found an opening main-heading, when looking for opening brace in section: %s", section_name);
				break;

			case ']':
				FatalError("Found an closing main-heading, when looking for opening brace in section: %s", section_name);
				break;
			}
		} // FOR.
	} // WHILE.

	FatalError("Could not find an opening brace in section: %s", section_name);
}

void SceneDefinition::moveToSubSection(const char *main_section_name, const char *sub_section_name) {
	int ctr;
	unsigned int temp;
	bool found;
	char buffer[80];
	char error[255];

	found = false;

	while (!feof(sceneFp)) {
		fgets(buffer, 80, sceneFp);

		for (ctr = 0; ctr < 80; ctr++) {
			// If we hit a carriage return give up, as rest of buffer contains rubbish!
			if (buffer[ctr] == '\n' || buffer[ctr] == 0)
				break;

			// Look for opening brace.
			if (buffer[ctr] == '<') {
				found = true;

				// See if characters match.
				for (temp = 0; temp < strlen(sub_section_name); temp++) {
					if (buffer[(ctr + 1) + temp] != sub_section_name[temp]) {
						found = false;
						break;
					}
				}
				// Make sure there is a closing brace.
				if ((found == true) && (buffer[(ctr + 1) + temp] != '>'))
					found = false;

				// If found is still true, a match was found, else continue.
				if (found)
					return;
			}  // IF.

			// Make sure we don't go past this section.
			switch (buffer[ctr]) {
			case '}':
				strcpy(error, "Could not find sub-heading name:");
				strcat(error, sub_section_name);
				strcat(error, ", in main-section:");
				strcat(error, main_section_name);
				FatalError(error);
				break;

			case '{':
				strcpy(error, "Found an opening brace, when looking for sub-heading:");
				strcat(error, sub_section_name);
				strcat(error, ", in main-section:");
				strcat(error, main_section_name);
				FatalError(error);
				break;

			case '[':
				strcpy(error, "Found an opening main-heading, when looking for sub-heading:");
				strcat(error, sub_section_name);
				strcat(error, ", in main-section:");
				strcat(error, main_section_name);
				FatalError(error);
				break;

			case ']':
				strcpy(error, "Found an closing main-heading, when looking for sub-heading:");
				strcat(error, sub_section_name);
				strcat(error, ", in main-section:");
				strcat(error, main_section_name);
				FatalError(error);
				break;
			}
		} // FOR.
	} // WHILE.

	if (!found) {
		strcpy(error, "Could not find sub-heading name:");
		strcat(error, sub_section_name);
		strcat(error, ", in main-section:");
		strcat(error, main_section_name);
		FatalError(error);
	}
}

int SceneDefinition::countEntries(const char *main_section_name, const char *sub_section_name, int instance) {
	// Counts the number of entries of a specfic instance of a definition or obj.
	int ctr;

	rewind(sceneFp);	// Start at the start of file.

	// Move down file to correct occurrence of main-section.
	for (ctr = 0; ctr <= instance; ctr++)
		moveToMainSection(main_section_name);

	moveToOpenBrace(main_section_name);
	moveToSubSection(main_section_name, sub_section_name);

	// We are now in the correct place to count entries.
	return countEntries();
}

int SceneDefinition::countEntries(void) {
	char buffer[80];
	int sum = 0;

	/*
	 ** Count each line that does not BEGIN with '[', '\n', or is
	 ** NULL as a valid entry, even thought it may not be.
	 */
	while (!feof(sceneFp)) {
		if (fgets(buffer, 80, sceneFp)) {
			if (buffer[1] == '<' || buffer[0] == '\n' || buffer[0] == '[' || buffer[0] == '}')
				break;
			else
				sum++;
		} else {
			break;
		}
	}

	return sum;
}

int SceneDefinition::countMainSections(const char *section_name) {
	char buffer[80];
	int ctr;
	unsigned int temp;
	bool found;
	int sum;

	sum = 0;

	rewind(sceneFp);

	while (!feof(sceneFp)) {
		fgets(buffer, 80, sceneFp);

		for (ctr = 0; ctr < 80; ctr++) {
			// Look for open brace.
			if (buffer[ctr] == '[') {
				found = true;

				// See if characters match.
				for (temp = 0; temp < strlen(section_name); temp++) {
					if (buffer[(ctr + 1) + temp] != section_name[temp]) {
						found = false;
						break;
					}
				}
				// Make sure there is a closing brace.
				if ((found == true) && (buffer[(ctr + 1) + temp] != ']'))
					found = false;

				// If found is still true a match was found, else continue.
				if (found)
					sum++;
			}  // IF.
		} // FOR.
	} // WHILE.

	return sum;
}

void SceneDefinition::getDefinition(BaseObject &obj, const char *defName) {
	int ctr;
	char error[255];

	for (ctr = 0; ctr < numDefs; ctr++) {
		if ((strcmp(defName, defs[ctr].getName())) == 0) {
			obj.setObjectDefinition(&defs[ctr]);
			return;
		}
	}

	// If we get down here the definition was not found.
	strcpy(error, "Could not find definition name:");
	strcat(error, defName);
	strcat(error, ", in GetDefinition()");
	FatalError(error);
}
/*****************************************************************************/
void SceneDefinition::loadData(const char *sectionName, int &a, int &b, int &c) {
	// Loads a row of data into the specified pointers.
	// The disk file must already be in the right place.
	bool loaded = false;
	char buffer[80];
	char error[255];

	if (fgets(buffer, 80, sceneFp) != NULL)
		if ((buffer[0] != '}') || (buffer[0] != '[') || (buffer[0] != ']'))
			if (buffer[0] != 0 && buffer[0] != '\n') {
				sscanf(buffer, "%d,%d,%d", &a, &b, &c);
				loaded = true;
			}

	if (!loaded) {
		strcpy(error, "Could not load data for: ");
		strcat(error, sectionName);
		strcat(error, ", in LoadData()");
		FatalError(error);
	}
}

void SceneDefinition::loadData(const char *sectionName, float &a, float &b, float &c) {
	bool loaded = false;
	char buffer[80];
	char error[255];

	if (fgets(buffer, 80, sceneFp) != NULL)
		if ((buffer[0] != '}') || (buffer[0] != '[') || (buffer[0] != ']'))
			if (buffer[0] != 0 && buffer[0] != '\n') {
				sscanf(buffer, "%f,%f,%f", &a, &b, &c);
				loaded = true;
			}

	if (!loaded) {
		strcpy(error, "Could not load data for: ");
		strcat(error, sectionName);
		strcat(error, ", in LoadData()");
		FatalError(error);
	}
}

void SceneDefinition::loadData(const char *sectionName, char &a) {
	bool loaded = false;
	char buffer[80];
	char error[255];

	if (fgets(buffer, 80, sceneFp) != NULL)
		if ((buffer[0] != '}') || (buffer[0] != '[') || (buffer[0] != ']'))
			if (buffer[0] != 0 && buffer[0] != '\n') {
				sscanf(buffer, "%s", &a);
				loaded = true;
			}

	if (!loaded) {
		strcpy(error, "Could not load data for: ");
		strcat(error, sectionName);
		strcat(error, ", in LoadData()");
		FatalError(error);
	}
}

void SceneDefinition::loadData(const char *sectionName, float &a) {
	bool loaded = false;
	char buffer[80];
	char error[255];

	if (fgets(buffer, 80, sceneFp) != NULL)
		if ((buffer[0] != '}') || (buffer[0] != '[') || (buffer[0] != ']'))
			if (buffer[0] != 0 && buffer[0] != '\n') {
				sscanf(buffer, "%f", &a);
				loaded = true;
			}

	if (!loaded) {
		strcpy(error, "Could not load data for: ");
		strcat(error, sectionName);
		strcat(error, ", in LoadData()");
		FatalError(error);
	}
}
/******************************************************************************/
void SceneDefinition::LoadDefinitions() {
	int ctr, inCtr;
	float x, y, z;
	int a, b, c;
	int red, green, blue;
	int num_verts, num_tri;
	char defName[20];
	char buffer[80];
	char mainSection[] = { "definition" };
	float factor;

	numDefs = countMainSections(mainSection);
	if (numDefs == 0) {
		FatalError("Could not find any object definitions");
	}

	// Create local instance.
	defs = new ObjectDefinition[numDefs];

	// Initialize.
	for (ctr = 0; ctr < numDefs; ctr++) {
		seekToMainSection(mainSection, ctr);
		moveToOpenBrace(mainSection);
		moveToSubSection(mainSection, "defname");
		loadData(mainSection, defName[0]);
		defs[ctr].setName(defName);

		// Find out how many vertices 'this' object has. [rewinds file]
		num_verts = countEntries(mainSection, "vertices", ctr);

		if (num_verts == 0) {
			FatalError("No vertices found");
			return;
		}

		// Instantiate vertex array.
		defs[ctr].allocateVertices(num_verts);
		seekToMainSection(mainSection, ctr);
		moveToOpenBrace(mainSection);
		moveToSubSection(mainSection, "vertices");

		// Load vertex data.
		for (inCtr = 0; inCtr < num_verts; inCtr++) {
			if (fgets(buffer, 80, sceneFp) != NULL) {
				sscanf(buffer, "%f,%f,%f", &x, &y, &z);
				defs[ctr].setVertex(inCtr, x, y, z);
			}
		}

		// Find out how many triangles 'this' object has. [rewinds file]
		num_tri = countEntries(mainSection, "triangles", ctr);
		if (num_tri == 0) {
			FatalError("No triangles found");
			return;
		}

		// Instantiate triangle array.
		defs[ctr].allocateTriangles(num_tri);
		seekToMainSection(mainSection, ctr);
		moveToOpenBrace(mainSection);
		moveToSubSection(mainSection, "triangles");

		// Load triangle data.
		for (inCtr = 0; inCtr < num_tri; inCtr++) {
			if (fgets(buffer, 80, sceneFp) != NULL) {
				sscanf(buffer, "%u,%u,%u:%u,%u,%u", &a, &b, &c, &red, &green, &blue);
				defs[ctr].setTriangle(inCtr, a, b, c, NovaColor(red, green, blue));
			}
		}

		moveToSubSection(mainSection, "scaling");
		loadData(mainSection, factor);
		defs[ctr].scaleVertices(factor);
		defs[ctr].computePlaneAndNormals();
	} // FOR NUM_DEFS.
}

void SceneDefinition::LoadObjects() {
	int i, num_Objs;
	float x, y, z;
	char objName[20];
	char defName[20];
	char solid[10];
	char mainSection[] = { "object" };

	num_Objs = countMainSections(mainSection);
	if (num_Objs == 0)
		return;	// This is not an error.

	// Create local instances.
	baseObjects = new BaseObject[num_Objs];

	for (i = 0; i < num_Objs; i++) {
		seekToMainSection(mainSection, i);
		moveToOpenBrace(mainSection);
		moveToSubSection(mainSection, "object_name");
		loadData(mainSection, objName[0]);
		baseObjects[i].setName(objName);

		// Copy the required definition into 'this' object.
		moveToSubSection(mainSection, "definition");
		loadData(mainSection, defName[0]);
		getDefinition(baseObjects[i], defName);

		// Load WCS position and rotation.
		moveToSubSection(mainSection, "position");
		loadData(mainSection, x, y, z);
		baseObjects[i].moveTo(x, y, z);

		moveToSubSection(mainSection, "rotation");
		loadData(mainSection, x, y, z);
		baseObjects[i].rotateAroundX(x);
		baseObjects[i].rotateAroundY(y);
		baseObjects[i].rotateAroundZ(z);

		moveToSubSection(mainSection, "solid");
		loadData(mainSection, solid[0]);
		if ((strcmp("NO", solid)) == 0) {
			baseObjects[i].setSolid(false);
		} else {
			baseObjects[i].setSolid(true);
		}

		g_world->addObjectToWorld(baseObjects[i]);
	}
}

void SceneDefinition::loadPlayerObjects() {
	float x, y, z;
	char objName[20];
	char defName[20];
	char mainSection[] = { "player_head" };
	float maxLookUpAngle, maxLookDownAngle;
	float movementAcceleration, movementDeceleration;
	float maxForwardVelocity, maxBackwardVelocity;
	float maxStrafeVelocity;

	if (countMainSections(mainSection) == 0) {
		FatalError("Could not find the Player's head object");
	}

	seekToMainSection(mainSection, 0);
	moveToOpenBrace(mainSection);
	moveToSubSection(mainSection, "object_name");
	loadData(mainSection, objName[0]);

	moveToSubSection(mainSection, "max_lookup_angle");
	loadData(mainSection, maxLookUpAngle);

	moveToSubSection(mainSection, "max_lookdown_angle");
	loadData(mainSection, maxLookDownAngle);

	// Create object instance.
	playerHead = new PlayerHead(objName, maxLookUpAngle, maxLookDownAngle);

	// Set Definition details.
	moveToSubSection(mainSection, "definition");
	loadData(mainSection, defName[0]);
	getDefinition(*playerHead, defName);

	// Load WCS position and rotation.
	moveToSubSection(mainSection, "position");
	loadData(mainSection, x, y, z);
	playerHead->moveTo(x, y, z);

	moveToSubSection(mainSection, "rotation");
	loadData(mainSection, x, y, z);
	playerHead->rotateAroundX(x);
	playerHead->rotateAroundY(y);
	playerHead->rotateAroundZ(z);

	// Finished loading player head, now load player body.
	strcpy(mainSection, "player_body");

	if (countMainSections(mainSection) == 0) {
		FatalError("Could not find the Player's body object");
	}

	seekToMainSection(mainSection, 0);
	moveToOpenBrace(mainSection);
	moveToSubSection(mainSection, "object_name");
	loadData(mainSection, objName[0]);

	moveToSubSection(mainSection, "movement_acceleration");
	loadData(mainSection, movementAcceleration);

	moveToSubSection(mainSection, "movement_deceleration");
	loadData(mainSection, movementDeceleration);

	moveToSubSection(mainSection, "max_forward_velocity");
	loadData(mainSection, maxForwardVelocity);

	moveToSubSection(mainSection, "max_backward_velocity");
	loadData(mainSection, maxBackwardVelocity);

	moveToSubSection(mainSection, "max_strafe_velocity");
	loadData(mainSection, maxStrafeVelocity);

	// Create object instance.
	playerBody = new PlayerBody(objName, movementAcceleration, movementDeceleration, maxForwardVelocity, maxBackwardVelocity, maxStrafeVelocity);

	// Set Definition details.
	moveToSubSection(mainSection, "definition");
	loadData(mainSection, defName[0]);
	getDefinition(*playerBody, defName);

	// Load WCS position and rotation.
	moveToSubSection(mainSection, "position");
	loadData(mainSection, x, y, z);
	playerBody->moveTo(x, y, z);

	moveToSubSection(mainSection, "rotation");
	loadData(mainSection, x, y, z);
	playerBody->rotateAroundX(x);
	playerBody->rotateAroundY(y);
	playerBody->rotateAroundZ(z);
}

void SceneDefinition::loadAmbientLights() {
	int num_lights;
	int red, green, blue;
	char enabled[10];
	char mainSection[] = { "ambientlight" };

	num_lights = countMainSections(mainSection);

	if (num_lights == 0)
		return;	// This is not an error.

	if (num_lights > 1) {
		LogWarningMessage("Only a single ambient light source is supported\n");
	}

	// Create instances.
	ambientLights = new AmbientLight();

	// Initialize (only one ambient light source supported).
	seekToMainSection(mainSection, 0);
	moveToOpenBrace(mainSection);

	moveToSubSection(mainSection, "color");
	loadData(mainSection, red, green, blue);
	ambientLights->SetColor(NovaColor(red, green, blue));

	moveToSubSection(mainSection, "enabled");
	loadData(mainSection, enabled[0]);

	if ((strcmp("NO", enabled)) == 0) {
		ambientLights->SetEnabled(false);
	} else {
		ambientLights->SetEnabled(true);
	}

	g_world->addLightToWorld(*ambientLights);
}

void SceneDefinition::loadDirectLights() {
	int num_lights;
	int red, green, blue;
	float x,y,z;
	char enabled[10];
	char mainSection[] = { "directlight" };

	num_lights = countMainSections(mainSection);

	if (num_lights == 0)
		return;	// This is not an error.

	// Create instances.
	directLights = new DirectLight[num_lights];

	// Initialize.
	for (int ctr = 0; ctr < num_lights; ctr++) {
		seekToMainSection(mainSection, 0);
		moveToOpenBrace(mainSection);

		moveToSubSection(mainSection, "color");
		loadData(mainSection, red, green, blue);
		directLights[ctr].SetColor(NovaColor(red, green, blue));

		moveToSubSection(mainSection, "direction");
		loadData(mainSection, x, y, z);
		directLights[ctr].SetDirection(NovaVertex(x,y,z));

		moveToSubSection(mainSection, "enabled");
		loadData(mainSection, enabled[0]);

		if ((strcmp("NO", enabled)) == 0) {
			directLights[ctr].SetEnabled(false);
		} else {
			directLights[ctr].SetEnabled(true);
		}

		g_world->addLightToWorld(directLights[ctr]);
	}
}

void SceneDefinition::loadPointLights() {
	int ctr, num_objs;
	float x, y, z;
	char objName[20];
	char defName[20];
	int red, green, blue;
	float maxDistance;
	char enabled[10];
	char mainSection[] = { "pointlight" };

	num_objs = countMainSections(mainSection);
	if (num_objs == 0)
		return;	// This is not an error.

	// Create instances.
	pointLights = new PointLight[num_objs];

	// Initialize.
	for (ctr = 0; ctr < num_objs; ctr++) {
		seekToMainSection(mainSection, ctr);
		moveToOpenBrace(mainSection);
		moveToSubSection(mainSection, "object_name");
		loadData(mainSection, objName[0]);
		pointLights[ctr].setName(objName);

		// Copy the required definition into 'this' object.
		moveToSubSection(mainSection, "definition");
		loadData(mainSection, defName[0]);
		getDefinition(pointLights[ctr], defName);

		// Load WCS position and rotation.
		moveToSubSection(mainSection, "position");
		loadData(mainSection, x, y, z);
		pointLights[ctr].moveTo(x, y, z);

		moveToSubSection(mainSection, "rotation");
		loadData(mainSection, x, y, z);
		pointLights[ctr].rotateAroundX(x);
		pointLights[ctr].rotateAroundY(y);
		pointLights[ctr].rotateAroundZ(z);

		moveToSubSection(mainSection, "color");
		loadData(mainSection, red, green, blue);
		pointLights[ctr].SetColor(NovaColor(red, green, blue));

		moveToSubSection(mainSection, "max_distance");
		loadData(mainSection, maxDistance);
		pointLights[ctr].setMaxDistance(maxDistance);

		moveToSubSection(mainSection, "enabled");
		loadData(mainSection, enabled[0]);

		if ((strcmp("NO", enabled)) == 0) {
			pointLights[ctr].SetEnabled(false);
		} else {
			pointLights[ctr].SetEnabled(true);
		}

		g_world->addObjectToWorld(pointLights[ctr]);
		g_world->addLightToWorld(pointLights[ctr]);
	}
}

