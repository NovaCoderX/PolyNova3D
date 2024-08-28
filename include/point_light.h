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

#ifndef __POINT_LIGHT_H
#define __POINT_LIGHT_H

class PointLight: public LightSource, public BaseObject {
public:
	PointLight() {
		// Override.
		type = LIGHTING_TYPE_POINT;

		maxDistance = 0;
	}

	float getMaxDistance() const {
		return maxDistance;
	}

	void setMaxDistance(float maxDistance) {
		this->maxDistance = maxDistance;
	}

	void addLight(RenderVertex &vertex, NovaVertex &lightV);



protected:
	// Specifies how far away from the light-source the light is cast.
	float maxDistance;

	// We have to handle lighting differently for point light objects.
	void applyLighting();
};

#endif // __POINT_LIGHT_H

