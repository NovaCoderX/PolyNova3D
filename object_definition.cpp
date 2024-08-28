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
ObjectDefinition::ObjectDefinition(void) {
	static_triangle_array = NULL;
	numTriangles = 0;

	static_vertex_array = NULL;
	sum_vertex_array = NULL;
	scratch_vertex_array = NULL;
	numVertices = 0;
}

ObjectDefinition::~ObjectDefinition() {
	if (static_triangle_array) {
		delete[] static_triangle_array;
		static_triangle_array = NULL;
	}

	if (static_vertex_array) {
		delete[] static_vertex_array;
		static_vertex_array = NULL;
	}

	if (sum_vertex_array) {
		delete[] sum_vertex_array;
		sum_vertex_array = NULL;
	}

	if (scratch_vertex_array) {
		delete[] scratch_vertex_array;
		scratch_vertex_array = NULL;
	}
}



/*****************************************************************************/
// Private
/*****************************************************************************/
void ObjectDefinition::allocateTriangles(int triangles) {
	numTriangles = triangles;

	static_triangle_array = new NovaTriangle[numTriangles];
	if (!static_triangle_array) {
		FatalError("Could not allocate static_triangle_array");
	}
}

// This static triangle uses these shared static vertices.
void ObjectDefinition::setTriangle(int eleNum, int zero, int one, int two, NovaColor color) {
	if (eleNum >= numTriangles) {
		FatalError("Triangle index out of range\n");
	}

	if (zero >= numVertices) {
		FatalError("Vertex index out of range\n");
	}

	if (one >= numVertices) {
		FatalError("Vertex index out of range\n");
	}

	if (two >= numVertices) {
		FatalError("Vertex index out of range\n");
	}

	static_triangle_array[eleNum].staticVertex[0] = &static_vertex_array[zero];
	static_triangle_array[eleNum].staticVertex[1] = &static_vertex_array[one];
	static_triangle_array[eleNum].staticVertex[2] = &static_vertex_array[two];

	static_triangle_array[eleNum].sumVertex[0] = &sum_vertex_array[zero];
	static_triangle_array[eleNum].sumVertex[1] = &sum_vertex_array[one];
	static_triangle_array[eleNum].sumVertex[2] = &sum_vertex_array[two];

	static_triangle_array[eleNum].scratchVertex[0] = &scratch_vertex_array[zero];
	static_triangle_array[eleNum].scratchVertex[1] = &scratch_vertex_array[one];
	static_triangle_array[eleNum].scratchVertex[2] = &scratch_vertex_array[two];

	static_triangle_array[eleNum].baseColor = color;
}

void ObjectDefinition::allocateVertices(int vertices) {
	numVertices = vertices;

	static_vertex_array = new ObjectVertex[numVertices];
	if (!static_vertex_array) {
		FatalError("Could not allocate static_vertex_array");
	}

	sum_vertex_array = new SumVertex[numVertices];
	if (!sum_vertex_array) {
		FatalError("Could not allocate sum_vertex_array");
	}

	scratch_vertex_array = new RenderVertex[numVertices];
	if (!scratch_vertex_array) {
		FatalError("Could not allocate scratch_vertex_array");
	}
}

void ObjectDefinition::setVertex(int eleNum, float x, float y, float z) {
	static_vertex_array[eleNum].x = x;
	static_vertex_array[eleNum].y = y;
	static_vertex_array[eleNum].z = z;
}

void ObjectDefinition::computePlaneAndNormals(void) {
	for (int i = 0; i < numTriangles; i++) {
		static_triangle_array[i].computePlaneAndNormals();
	}

	for (int i = 0; i < numVertices; i++) {
		static_vertex_array[i].planeConstant = sum_vertex_array[i].getPlaneConstant();
		static_vertex_array[i].vertexNormal = sum_vertex_array[i].getSurfaceNormal();
	}
}

void ObjectDefinition::scaleVertices(float scaleFactor) {
	if (scaleFactor != 0) {
		for (int i = 0; i < numVertices; i++) {
			static_vertex_array[i].x = (static_vertex_array[i].x / scaleFactor);
			static_vertex_array[i].y = (static_vertex_array[i].y / scaleFactor);
			static_vertex_array[i].z = (static_vertex_array[i].z / scaleFactor);
		}
	}
}




