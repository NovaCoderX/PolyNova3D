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
#ifndef __BASE_OBJECT_H
#define __BASE_OBJECT_H

const int MAX_OBJECT_NAME = 40;


// Forward declarations.
class AmbientLight;
class DirectLight;
class PointLight;

class BaseObject {
	friend class World;
public:
	BaseObject(const char *name = 0);
	virtual ~BaseObject();

	const char* getName() const {
		return name;
	}

	void setName(const char *n) {
		strcpy(name, n);
	}

	void setObjectDefinition(ObjectDefinition *definition);

	ObjectDefinition *getObjectDefinition() {
		return definition;
	}

	NovaVertex getPositionWCS() const {
		return objectToWorldMatrix.GetTranslation();
	}

	void getRotationMatrixWCS(AstMatrix3x3 &rm3x3) {
		objectToWorldMatrix.GetRotationMatrix(rm3x3);
	}

	AstMatrix4x3& getObjectToWorldMatrix() {
		return objectToWorldMatrix;
	}

	float getBoundingSphere() const {
		return extents->getBoundingSphere();
	}

	void moveAlongX(float amount);
	void moveAlongY(float amount);
	void moveAlongZ(float amount);

	void moveTo(const NovaVertex &newposition) {
		objectToWorldMatrix.SetTranslation(newposition);
	}

	void moveTo(float x, float y, float z) {
		moveTo(NovaVertex(x, y, z));
	}

	void rotateAroundX(float degrees);
	void rotateAroundY(float degrees);
	void rotateAroundZ(float degrees);

	bool isVisibile();

	void setSolid(bool solid) {
		this->solid = solid;
	}

	bool isSolid() const {
		return solid;
	}

	void draw();

	ObjectExtents* getExtents() {
		return extents;
	}

protected:
	BaseObject *next_in_list;
	BaseObject *prior_in_list;

	char name[MAX_OBJECT_NAME];
	ObjectDefinition *definition;
	AstMatrix4x3 objectToWorldMatrix;	// LCS->WCS matrix.
	AstMatrix4x3 worldToObjectMatrix;  	// WCS->LCS matrix.

	// Maintains a linked list of visible triangles.
	NovaTriangle *triangle_list_head;

	//SphereExtents *sphereExtents;
	ObjectExtents *extents;
	bool solid;


	void AddToTriangleList(NovaTriangle &tri);
	void RemoveFromTriangleList(NovaTriangle &tri);

	void calculateWorldToObjectMatrix();

	// Core render loop.
	void begin();
	void backFaceCull();
	void transform();
	virtual void applyLighting();
	void addAmbientLight(AmbientLight *light);
	void addDirectLight(DirectLight *light);
	void addPointLight(PointLight *light);
	void project();
};

#endif // __BASE_OBJECT_H
