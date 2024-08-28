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



void RenderVertex::operator =(const ObjectVertex &vertex) {
	x = vertex.x;
	y = vertex.y;
	z = vertex.z;

	vertexNormal = vertex.vertexNormal;
	planeConstant = vertex.planeConstant;
}

/*
 ** AstVector operator * ( const AstVector &v, AstAstMatrix4x3 &m )
 **
 ** This routine overloads the "*" operator so that a
 ** vector can be multiplied by a 4x3 matrix easily.  This routine
 ** will transform the vector and return the new vector.
 */
RenderVertex operator *(ObjectVertex &v, const AstMatrix4x3 &m) {
	RenderVertex result;

	// Initialize.
	result = v;

	result.x = (m[0][0] * v.x) + (m[1][0] * v.y) + (m[2][0] * v.z) + m[3][0];
	result.y = (m[0][1] * v.x) + (m[1][1] * v.y) + (m[2][1] * v.z) + m[3][1];
	result.z = (m[0][2] * v.x) + (m[1][2] * v.y) + (m[2][2] * v.z) + m[3][2];

	return result;
}
