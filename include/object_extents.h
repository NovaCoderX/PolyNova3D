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

#ifndef __OBJECT_EXTENTS_H
#define __OBJECT_EXTENTS_H

#define NUM_EXTENTS	8


// Forward deceleration.
class BaseObject;

class ObjectExtents {
public:
	ObjectExtents(BaseObject *requestorObject);

	BaseObject* getRequestorObject() {
		return requestorObject;
	}

	float getBoundingSphere() const {
		return boundingSphere;
	}

	int getNumExtents() const {
		return NUM_EXTENTS;
	}

	NovaVertex& getExtent(int index) {
		return extentsWCS[index];
	}

	// Update based on the current WCS position/rotation of the object.
	void updateExtents();

protected:
	BaseObject *requestorObject;
	float boundingSphere;
	NovaVertex extentsLCS[NUM_EXTENTS];
	NovaVertex extentsWCS[NUM_EXTENTS];

};

#endif // __OBJECT_EXTENTS_H
