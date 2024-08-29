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

// The extents handler needs an object and its current definition.
ObjectExtents::ObjectExtents(BaseObject *requestorObject) {
	float lf_maxX, lf_maxY, lf_maxZ;
	float lf_minX, lf_minY, lf_minZ;

	this->requestorObject = requestorObject;

	this->boundingSphere = 0;

	ObjectDefinition *definition = requestorObject->getObjectDefinition();

	/*
	 ** Iterate through the object definition's vertices,
	 ** and store the distance of the vertex farthest away
	 ** from the object's origin.
	 */
	float farthest = 0;

	for (int i = 0; i < definition->getNumVertices(); i++) {
		if (definition->getStaticVertex(i).magnitude() > farthest) {
			farthest = definition->getStaticVertex(i).magnitude();
		}
	}

	boundingSphere = farthest;

	// Seed..
	NovaVertex lv_temp;

	lv_temp = definition->getStaticVertex(0);
	lf_maxX = lv_temp.x;
	lf_maxY = lv_temp.y;
	lf_maxZ = lv_temp.z;

	lf_minX = lv_temp.x;
	lf_minY = lv_temp.y;
	lf_minZ = lv_temp.z;

	for (int i = 1; i < definition->getNumVertices(); i++) {
		lv_temp = definition->getStaticVertex(i);

		if (lv_temp.x > lf_maxX) {
			lf_maxX = lv_temp.x;
		}

		if (lv_temp.y > lf_maxY) {
			lf_maxY = lv_temp.y;
		}

		if (lv_temp.z > lf_maxZ) {
			lf_maxZ = lv_temp.z;
		}

		if (lv_temp.x < lf_minX) {
			lf_minX = lv_temp.x;
		}

		if (lv_temp.y < lf_minY) {
			lf_minY = lv_temp.y;
		}

		if (lv_temp.z < lf_minZ) {
			lf_minZ = lv_temp.z;
		}
	}

	// Using these two points, calculate the other six points of our bounding box.
	extentsLCS[0] = NovaVertex(lf_maxX, lf_maxY, lf_minZ);
	extentsLCS[1] = NovaVertex(lf_minX, lf_maxY, lf_minZ);
	extentsLCS[2] = NovaVertex(lf_minX, lf_minY, lf_minZ); // Original min.
	extentsLCS[3] = NovaVertex(lf_maxX, lf_minY, lf_minZ);

	extentsLCS[4] = NovaVertex(lf_minX, lf_maxY, lf_maxZ);
	extentsLCS[5] = NovaVertex(lf_maxX, lf_maxY, lf_maxZ); // Original max.
	extentsLCS[6] = NovaVertex(lf_maxX, lf_minY, lf_maxZ);
	extentsLCS[7] = NovaVertex(lf_minX, lf_minY, lf_maxZ);
}

// Update extents based on the current WCS position/rotation of the object.
void ObjectExtents::updateExtents() {
	NovaVertex lv_temp;
	float lf_maxX, lf_maxY, lf_maxZ;
	float lf_minX, lf_minY, lf_minZ;

	for (int li_ctr = 0; li_ctr < 8; li_ctr++) {
		extentsWCS[li_ctr] = (extentsLCS[li_ctr] * requestorObject->getObjectToWorldMatrix());
	}

	// Initialize.
	lv_temp = extentsWCS[0];

	lf_maxX = lv_temp.x;
	lf_maxY = lv_temp.y;
	lf_maxZ = lv_temp.z;

	lf_minX = lv_temp.x;
	lf_minY = lv_temp.y;
	lf_minZ = lv_temp.z;

	for (int li_ctr = 1; li_ctr < 8; li_ctr++) {
		lv_temp = extentsWCS[li_ctr];

		if (lv_temp.x < lf_maxX)
			lf_maxX = lv_temp.x;

		if (lv_temp.y > lf_maxY)
			lf_maxY = lv_temp.y;

		if (lv_temp.z > lf_maxZ)
			lf_maxZ = lv_temp.z;

		if (lv_temp.x > lf_minX)
			lf_minX = lv_temp.x;

		if (lv_temp.y < lf_minY)
			lf_minY = lv_temp.y;

		if (lv_temp.z < lf_minZ)
			lf_minZ = lv_temp.z;
	}
}

