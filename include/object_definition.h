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

#ifndef __OBJECT_DEFINITION_H
#define __OBJECT_DEFINITION_H

const int MAX_DEF_NAME = 40;

class ObjectDefinition {
	friend class SceneDefinition;
	friend class BaseObject;
	friend class PointLight;

public:
	ObjectDefinition();
	~ObjectDefinition();

	const char* getName(void) const {
		return name;
	}

	void setName(const char *defName) {
		strcpy(name, defName);
	}

	int getNumTriangles(void) const {
		return numTriangles;
	}

	int getNumVertices(void) const {
		return numVertices;
	}

	NovaVertex& getStaticVertex(int v) const {
		return static_vertex_array[v];
	}


private:
	char name[MAX_DEF_NAME];

	NovaTriangle *static_triangle_array;
	int numTriangles;

	// Triangle vertex data (stored in LCS).
	ObjectVertex *static_vertex_array;

	// Used to calculate static vertex normals.
	SumVertex *sum_vertex_array;

	// Triangle vertex data (stored in CCS).
	RenderVertex *scratch_vertex_array;

	int numVertices;

	// Used by Scene def when loading an object.
	void allocateVertices(int vertices);
	void setVertex(int eleNum, float x, float y, float z);

	void allocateTriangles(int triangles);
	void setTriangle(int eleNum, int zero, int one, int two, NovaColor color);

	void computePlaneAndNormals(void);
	void scaleVertices(float scaleFactor);
};

#endif // __OBJECT_DEFINITION_H
