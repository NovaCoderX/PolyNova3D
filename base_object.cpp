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
BaseObject::BaseObject(const char *name) {
	if (name) {
		setName(name);
	} else {
		setName("UNDEFINED");
	}

	next_in_list = prior_in_list = NULL;

	definition = NULL;

	objectToWorldMatrix.MakeIdentity();
	triangle_list_head = NULL;
	extents = NULL;
	solid = false;
}

BaseObject::~BaseObject() {
	if (extents) {
		delete extents;
		extents = NULL;
	}
}

void BaseObject::setObjectDefinition(ObjectDefinition *definition) {
	if (!definition) {
		FatalError("Object definition is invalid\n");
	}

	this->definition = definition;

	if (extents) {
		delete extents;
		extents = NULL;
	}

	// We need an object definition assigned before we can calculate the extents.
	extents = new ObjectExtents(this);
}

/*
 ** the following routines handle translation by creating a
 ** 4x3 translation matrix and applying it to the objectToWorldMatrix.
 */
static AstMatrix4x3 translation_matrix;

void BaseObject::moveAlongX(float amount) {
	translation_matrix.MakeIdentity();
	translation_matrix.MakeXTranslation(amount);
	objectToWorldMatrix = translation_matrix * objectToWorldMatrix;
}

void BaseObject::moveAlongY(float amount) {
	translation_matrix.MakeIdentity();
	translation_matrix.MakeYTranslation(amount);
	objectToWorldMatrix = translation_matrix * objectToWorldMatrix;
}

void BaseObject::moveAlongZ(float amount) {
	translation_matrix.MakeIdentity();
	translation_matrix.MakeZTranslation(amount);
	objectToWorldMatrix = translation_matrix * objectToWorldMatrix;
}

/*
 ** The following functions handle rotations.  Unlike translation,
 ** I have opted not to use straight matrix multiplies to implement
 ** rotation.  The reason for this is one of accuracy -- because of
 ** the limited accuracy of fixed point math (or floating point
 ** math, for that matter) matrix values tend to start skewing
 ** after many multiplies.  This may take a while to build up, but
 ** when it does the effects can be quite dramatic -- heavily
 ** distorted objects on screen.
 **
 ** To get around this we "fix" our matrices using an orthogonalization
 ** method whereby we compute the individual right, up, and forward
 ** vectors of the rotation matrix through cross products.  If this
 ** seems complicated -- don't worry about it, it works.
 */
static AstMatrix4x3 rotation_matrix;

void BaseObject::rotateAroundX(float degrees) {
	float m10 = objectToWorldMatrix[1][0], m20 = objectToWorldMatrix[2][0], m11 = objectToWorldMatrix[1][1],
			m21 = objectToWorldMatrix[2][1];

	rotation_matrix.MakeXRotation(degrees);

	float sm11 = rotation_matrix[1][1], sm12 = rotation_matrix[1][2], sm21 = rotation_matrix[2][1], sm22 = rotation_matrix[2][2];
	/*
	 ** Compute column 0
	 */
	objectToWorldMatrix[1][0] = (sm11 * m10) + (sm12 * m20);
	objectToWorldMatrix[2][0] = (sm21 * m10) + (sm22 * m20);

	/*
	 ** Compute column 1
	 */
	objectToWorldMatrix[1][1] = (sm11 * m11) + (sm12 * m21);
	objectToWorldMatrix[2][1] = (sm21 * m11) + (sm22 * m21);

	/*
	 ** Compute column 2 (as col0 x col1 )
	 */
	objectToWorldMatrix[0][2] = (objectToWorldMatrix[1][0] * objectToWorldMatrix[2][1])
			- (objectToWorldMatrix[2][0] * objectToWorldMatrix[1][1]);
	objectToWorldMatrix[1][2] = (objectToWorldMatrix[2][0] * objectToWorldMatrix[0][1])
			- (objectToWorldMatrix[0][0] * objectToWorldMatrix[2][1]);
	objectToWorldMatrix[2][2] = (objectToWorldMatrix[0][0] * objectToWorldMatrix[1][1])
			- (objectToWorldMatrix[0][1] * objectToWorldMatrix[1][0]);

	/*
	 ** Recompute column 0 as col1 x col2
	 */
	NovaVertex vu, vf, new_vr;

	objectToWorldMatrix.GetVF(vf);
	objectToWorldMatrix.GetVU(vu);
	new_vr = vu.cross(vf);
	objectToWorldMatrix.SetVR(new_vr);

	objectToWorldMatrix.NormalizeDirectionVectors();
}

void BaseObject::rotateAroundY(float degrees) {
	float m00 = objectToWorldMatrix[0][0], m20 = objectToWorldMatrix[2][0], m01 = objectToWorldMatrix[0][1],
			m21 = objectToWorldMatrix[2][1];

	rotation_matrix.MakeYRotation(degrees);

	float sm00 = rotation_matrix[0][0], sm02 = rotation_matrix[0][2], sm20 = rotation_matrix[2][0], sm22 = rotation_matrix[2][2];

	/*
	 ** Compute column 0
	 */
	objectToWorldMatrix[0][0] = (sm00 * m00) + (sm02 * m20);
	objectToWorldMatrix[2][0] = (sm20 * m00) + (sm22 * m20);

	/*
	 ** Compute column 1
	 */
	objectToWorldMatrix[0][1] = (sm00 * m01) + (sm02 * m21);
	objectToWorldMatrix[2][1] = (sm20 * m01) + (sm22 * m21);

	/*
	 ** Compute column 2 (as col0 x col1 )
	 */
	objectToWorldMatrix[0][2] = (objectToWorldMatrix[1][0] * objectToWorldMatrix[2][1])
			- (objectToWorldMatrix[2][0] * objectToWorldMatrix[1][1]);
	objectToWorldMatrix[1][2] = (objectToWorldMatrix[2][0] * objectToWorldMatrix[0][1])
			- (objectToWorldMatrix[0][0] * objectToWorldMatrix[2][1]);
	objectToWorldMatrix[2][2] = (objectToWorldMatrix[0][0] * objectToWorldMatrix[1][1])
			- (objectToWorldMatrix[0][1] * objectToWorldMatrix[1][0]);

	/*
	 ** Recompute column 0 (as col1 x col2 )
	 */
	NovaVertex vu, vf, new_vr;

	objectToWorldMatrix.GetVF(vf);
	objectToWorldMatrix.GetVU(vu);
	new_vr = vu.cross(vf);
	objectToWorldMatrix.SetVR(new_vr);

	objectToWorldMatrix.NormalizeDirectionVectors();
}

void BaseObject::rotateAroundZ(float degrees) {
	float m00 = objectToWorldMatrix[0][0], m01 = objectToWorldMatrix[0][1], m10 = objectToWorldMatrix[1][0],
			m11 = objectToWorldMatrix[1][1];

	rotation_matrix.MakeZRotation(degrees);

	float sm00 = rotation_matrix[0][0], sm01 = rotation_matrix[0][1], sm10 = rotation_matrix[1][0], sm11 = rotation_matrix[1][1];

	/*
	 ** Compute column 0
	 */
	objectToWorldMatrix[0][0] = (sm00 * m00) + (sm01 * m10);

	objectToWorldMatrix[1][0] = (sm10 * m00) + (sm11 * m10);

	/*
	 ** Compute column 1
	 */
	objectToWorldMatrix[0][1] = (sm00 * m01) + (sm01 * m11);
	objectToWorldMatrix[1][1] = (sm10 * m01) + (sm11 * m11);

	/*
	 ** Compute column 2 (as col0 x col1 )
	 */
	objectToWorldMatrix[0][2] = (objectToWorldMatrix[1][0] * objectToWorldMatrix[2][1])
			- (objectToWorldMatrix[2][0] * objectToWorldMatrix[1][1]);
	objectToWorldMatrix[1][2] = (objectToWorldMatrix[2][0] * objectToWorldMatrix[0][1])
			- (objectToWorldMatrix[0][0] * objectToWorldMatrix[2][1]);
	objectToWorldMatrix[2][2] = (objectToWorldMatrix[0][0] * objectToWorldMatrix[1][1])
			- (objectToWorldMatrix[0][1] * objectToWorldMatrix[1][0]);

	/*
	 ** Recompute column 0 (as col1 x col2 )
	 */
	NovaVertex vu, vf, new_vr;

	objectToWorldMatrix.GetVF(vf);
	objectToWorldMatrix.GetVU(vu);
	new_vr = vu.cross(vf);
	objectToWorldMatrix.SetVR(new_vr);

	objectToWorldMatrix.NormalizeDirectionVectors();
}

bool BaseObject::isVisibile() {
	static NovaVertex locationCCS;

	locationCCS = (getPositionWCS() * g_world->getCamera()->getWorldToCameraMatrix());

	// Check near/far Z limits.
	if ((locationCCS.z + extents->getBoundingSphere()) < g_world->getCamera()->getNearZ()
			|| (locationCCS.z + extents->getBoundingSphere()) > g_world->getCamera()->getFarZ()) {
		return false;
	}

	// If the object's origin is within the view port then it's definitely visible..
	if (g_world->getCamera()->checkProjectedPoint(locationCCS)) {
		return true;
	}

	// Take the objects extents into account.
	NovaVertex topR = NovaVertex(extents->getBoundingSphere() + locationCCS.x, extents->getBoundingSphere() + locationCCS.y, locationCCS.z);
	NovaVertex botL = NovaVertex(-extents->getBoundingSphere() + locationCCS.x, -extents->getBoundingSphere() + locationCCS.y,
			locationCCS.z);
	return g_world->getCamera()->checkProjectedPoints(topR, botL);
}

void BaseObject::draw() {
	begin(); // Create our starting triangle list, object as defined.
	backFaceCull(); // Remove back facing triangles.
	transform(); // Convert triangles to CCS.
	project();  // Project triangles to the screen.
	applyLighting(); // Apply lighting to the remaining (visible) triangles.

	// Draw remaining (visible) triangles.
	NovaTriangle *currentT = triangle_list_head;
	while (currentT) {
		currentT->draw();

		currentT = currentT->next_in_list;
	}
}

/*****************************************************************************/
// Private
/*****************************************************************************/
void BaseObject::AddToTriangleList(NovaTriangle &tri) {
	/*
	 ** If no objects have been inserted into the g_world yet
	 ** then make this object the head of the object list.
	 */
	if (triangle_list_head == 0) {
		triangle_list_head = &tri;
		triangle_list_head->prior_in_list = 0;
		triangle_list_head->next_in_list = 0;
	} else {
		/*
		 ** Otherwise find the last object in the g_world and then
		 ** set the passed object as the new last object in the
		 ** g_world.
		 */
		NovaTriangle *last_in_list = triangle_list_head;

		/*
		 ** Find the last object in the g_world
		 */
		while (last_in_list->next_in_list != 0) {
			/*
			 ** See if the passed object has already been inserted
			 */
			if (last_in_list == &tri)
				return;

			last_in_list = last_in_list->next_in_list;
		}

		/*
		 ** Adjust pointers
		 */
		last_in_list->next_in_list = &tri;
		tri.prior_in_list = last_in_list;
		tri.next_in_list = 0;
	}
}

void BaseObject::RemoveFromTriangleList(NovaTriangle &tri) {
	NovaTriangle *previousT = tri.prior_in_list;
	NovaTriangle *nextT = tri.next_in_list;

	// Have to treat list head removal differently.
	if (&tri == triangle_list_head) {
		triangle_list_head = nextT;	// Assign new list head.
		if (!triangle_list_head == 0)
			triangle_list_head->prior_in_list = 0;

		return;
	}

	if (!previousT == 0)
		previousT->next_in_list = nextT;

	// Set prior reference of the next triangle if there is one.
	if (!nextT == 0)
		nextT->prior_in_list = previousT;
}

// Calculates WCS->LCS matrix.
void BaseObject::calculateWorldToObjectMatrix() {
	NovaVertex position;
	AstMatrix3x3 rm3x3;

	/*
	 ** Get object's rotation matrix and translation.
	 */
	objectToWorldMatrix.GetRotationMatrix(rm3x3);

	/*
	 ** Invert the rotation matrix (a transpose is the
	 ** the same as an inversion when working with
	 ** rotation matrices).
	 */
	rm3x3.Transpose();

	/*
	 ** Transform its location by the inverted rotation matrix
	 */
	position = getPositionWCS() * rm3x3;

	/*
	 ** Create world_to_object_matrix
	 */
	worldToObjectMatrix.SetRotationMatrix(rm3x3);
	worldToObjectMatrix.SetTranslation(-position);
	worldToObjectMatrix.NormalizeDirectionVectors();
}

//////////////////////////////////////////////
// Core render loop.
//////////////////////////////////////////////
void BaseObject::begin() {
	// Create a list of (original) triangles from the definition.
	triangle_list_head = 0;

	// Reset all scratch_vertices for this object.
	for (int i = 0; i < definition->getNumVertices(); i++) {
		definition->scratch_vertex_array[i].reset();
	}

	// To start with, assume all triangles are visible to the camera.
	for (int i = 0; i < definition->getNumTriangles(); i++) {
		AddToTriangleList(definition->static_triangle_array[i]);
	}
}

void BaseObject::backFaceCull() {
	static NovaVertex camera_position;

	/*
	 ** We're going to do our calculations in this object's LCS,
	 ** so we a need world_to_object_matrix.
	 */
	calculateWorldToObjectMatrix();

	/*
	 ** We transform the camera's position in the WCS, into a point
	 ** in the object's LCS by applying the world_to_object_matrix
	 ** to the camera position.
	 */
	camera_position = g_world->getCamera()->getPositionWCS() * worldToObjectMatrix;

	/*
	 ** For each of the triangles, determine if the camera is in
	 ** front of or behind the plane of that triangle.
	 */
	NovaTriangle *currentT = triangle_list_head;

	while (currentT != 0) {
		// If triangle is invisible remove it from linked list.
		if (currentT->checkPlaneEquation(camera_position)) {
			//LogDebugMessage("BackFaceCull - hidden so removing..\n");
			RemoveFromTriangleList(*currentT);
		}

		currentT = currentT->next_in_list;
	}
}

void BaseObject::transform() {
	static AstMatrix4x3 lcs2ccs_matrix;

	/*
	 ** Create a composite LCS to CCS matrix that consists
	 ** of the product of the objectToWorldMatrix and the
	 ** global world_to_camera matrix.
	 */
	lcs2ccs_matrix = objectToWorldMatrix * g_world->getCamera()->getWorldToCameraMatrix();

	// Transform all 'static' vertices shared by the triangles into CCS.
	NovaTriangle *currentT = triangle_list_head;

	while (currentT) {
		if (!currentT->transform(lcs2ccs_matrix)) {
			RemoveFromTriangleList(*currentT);
		}

		currentT = currentT->next_in_list;
	}
}

void BaseObject::project() {
	NovaTriangle *currentT = triangle_list_head;

	while (currentT) {
		if (!currentT->project()) {
			RemoveFromTriangleList(*currentT);
		}

		currentT = currentT->next_in_list;
	}

	// At this point in the render pipeline, we use the remaining visible triangles to mark the visible scratch vertices.
	currentT = triangle_list_head;
	while (currentT) {
		// Mark all visible vertices.
		currentT->scratchVertex[0]->setVisible(true);
		currentT->scratchVertex[1]->setVisible(true);
		currentT->scratchVertex[2]->setVisible(true);

		currentT = currentT->next_in_list;
	}
}

void BaseObject::applyLighting() {
	// We only apply the lighting to a single (shared) vertex once per frame.
	// For each light source in the world we must handle the type of light differently.
	for (int i = 0; i < g_world->numEnabledLights; i++) {
		switch (g_world->enabledLights[i]->GetType()) {
		case LIGHTING_TYPE_AMBIENT:
			addAmbientLight((AmbientLight*) g_world->enabledLights[i]);
			break;

		case LIGHTING_TYPE_DIRECT:
			addDirectLight((DirectLight*) g_world->enabledLights[i]);
			break;

		default:
			//LIGHTING_TYPE_POINT:
			addPointLight((PointLight*) g_world->enabledLights[i]);
			break;
		}
	}
}

void BaseObject::addAmbientLight(AmbientLight *light) {
	for (int i = 0; i < definition->getNumVertices(); i++) {
		if (definition->scratch_vertex_array[i].isVisible()) {
			light->addLight(&definition->scratch_vertex_array[i]);
		}
	}
}

void BaseObject::addDirectLight(DirectLight *light) {
	static AstMatrix3x3 objectRotation;

	// Update triangle surface normal, with the current direction.
	objectToWorldMatrix.GetRotationMatrix(objectRotation);

	for (int i = 0; i < definition->getNumVertices(); i++) {
		if (definition->scratch_vertex_array[i].isVisible()) {
			light->addLight(&definition->scratch_vertex_array[i], objectRotation);
		}
	}
}

void BaseObject::addPointLight(PointLight *light) {
	// We transform the point_light's position in the WCS into a point
	// in the object's LCS by applying the world_to_object_matrix.
	NovaVertex lightPosition = light->getPositionWCS() * worldToObjectMatrix;

	// We use the bounding sphere to see if this object is close enough to the light source.
	if ((lightPosition.magnitude() - this->getBoundingSphere()) < light->getMaxDistance()) {
		// We must convert to CCS.
		NovaVertex lightV = light->getPositionWCS() * g_world->getCamera()->getWorldToCameraMatrix();

		for (int i = 0; i < definition->getNumVertices(); i++) {
			if (definition->scratch_vertex_array[i].isVisible()) {
				if (!definition->scratch_vertex_array[i].checkPlaneEquation(lightPosition)) {
					light->addLight(definition->scratch_vertex_array[i], lightV);
				}
			}
		}
	}
}

