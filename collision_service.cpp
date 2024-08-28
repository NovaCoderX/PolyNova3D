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
CollisionService::CollisionService(BaseObject *requestorObject) {
	this->requestorObject = requestorObject;
}

bool CollisionService::checkCollision(BaseObject *solidObject) {
	//LogDebugMessage("CollisionService::checkCollision(%s) - START\n", object->getName());

	// First check a see if the bounding spheres intersect.
	if (!sphereToSphere(solidObject)) {
		return false;
	}

	//LogDebugMessage("CollisionService::checkCollision(%s) - sphere to sphere hit\n", solidObject->getName());

	// A bounding spheres intersection occurred, we then test using a more sophisticated method.
	return sphereToBox(solidObject);
}


/*****************************************************************************/
// Private
/*****************************************************************************/
bool CollisionService::sphereToSphere(BaseObject *solidObject) {
	static NovaVertex between;

	between = requestorObject->getPositionWCS() - solidObject->getPositionWCS();
	if (between.magnitude() < (requestorObject->getBoundingSphere() + solidObject->getBoundingSphere())) {
		return true;
	}

	return false;
}

bool CollisionService::sphereToBox(BaseObject *solidObject) {
	static NovaVertex between;

	solidObject->getExtents()->updateExtents();

	//LogDebugMessage("sphereToWall() - object->getBoundingSphere() = %f6.3\n", solidObject->getBoundingSphere());

	for (int i = 0; i < solidObject->getExtents()->getNumExtents(); i++) {
		between = requestorObject->getPositionWCS() - solidObject->getExtents()->getExtent(i);
		//LogDebugMessage("sphereToWall(%d) - lv_between.magnitude() = %f6.3\n", i, lv_between.magnitude());
		if (between.magnitude() < solidObject->getBoundingSphere()) {
			return true;
		}
	}

	return false;
}

