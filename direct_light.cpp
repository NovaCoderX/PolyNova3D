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


void DirectLight::addLight(RenderVertex *vertex, AstMatrix3x3 &triRotation) {
	// Compute intensity value 0 -> 1 (brightest).
	float intensity = direction.dot(vertex->vertexNormal * triRotation) + 1;

	intensity = (intensity / 2);

	// Add the value of 'this' light-source to the sum of all light to hit 'this' triangles vertices.
	NovaColor additionalColor = color * intensity;
	vertex->lightSum += additionalColor;
}
