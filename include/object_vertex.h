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
#ifndef __OBJECT_VERTEX_H
#define __OBJECT_VERTEX_H

class ObjectVertex: public NovaVertex {
public:
	NovaVertex vertexNormal;
	float planeConstant;

	ObjectVertex() {
		planeConstant = 0;
	}

	ObjectVertex(float x, float y, float z) :
			NovaVertex(x, y, z) {
		planeConstant = 0;
	}

	bool checkPlaneEquation(const NovaVertex &point) {
		if (point.dot(vertexNormal) + planeConstant >= 0)
			return false;
		else
			return true;
	}
};

#endif // __OBJECT_VERTEX_H

