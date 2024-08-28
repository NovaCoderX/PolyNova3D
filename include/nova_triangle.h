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
#ifndef __NOVA_TRIANGLE_H
#define __NOVA_TRIANGLE_H

class NovaTriangle {
public:
	NovaTriangle *next_in_list; // Used to create double linked list.
	NovaTriangle *prior_in_list; // Used to create double linked list.

	// Color in (255,255,255) light.
	NovaColor baseColor;

	float planeConstant;

	NovaVertex surfaceNormal;

	// Triangle vertex data (stored in LCS).
	ObjectVertex *staticVertex[3];

	// Used to calculate static vertex normals.
	SumVertex *sumVertex[3];

	// Used to transform the static vertex data to screen coordinates (each frame).
	RenderVertex *scratchVertex[3];

	// Projected vertex data (stored in VCS).
	DisplayVertex scratch_screen_array[3];

	NovaTriangle() {
		//baseColor = NovaColor(0, 0, 0);
		planeConstant = 0;
		//surfaceNormal.x = surfaceNormal.y = surfaceNormal.z = 0;
		next_in_list = 0;
		prior_in_list = 0;
	}

	void computePlaneAndNormals();

	bool checkPlaneEquation(const NovaVertex &point) {
		if (point.dot(surfaceNormal) + planeConstant >= 0)
			return false;
		else
			return true;
	}

	// Render loop.
	bool transform(const AstMatrix4x3 &matrix);
	bool project();
	void draw();
};

#endif // __NOVA_TRIANGLE_H
