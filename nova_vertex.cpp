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


/*
 ** AstVector AstVector::Cross( const AstVector &b ) const
 **
 ** Computes the cross product of this vector and the passed
 ** vector.  The cross product of two 3D vectors will result
 ** in a vector normal (perpendicular) to the plane defined
 ** by the two initial 3D vectors.
 */
NovaVertex NovaVertex::cross(const NovaVertex &b) const {
	NovaVertex result;

	result.x = (y * b.z) - (z * b.y);
	result.y = (z * b.x) - (x * b.z);
	result.z = (x * b.y) - (y * b.x);

	return result;
}

/*
 ** AstFixedpoint_t AstVector::Dot ( const AstVector & ) const
 **
 ** This routine computes the dot product between two fixed point
 ** vectors.  The dot product of two vectors determines the angle
 ** between them and is computed as:
 **
 **     A.X * B.X + A.Y * B.Y + A.Z * B.Z
 **
 ** The result is the cosine of the angle between the two vectors,
 ** and is useful for shading (where the angle determines how
 ** directly the light hits a surface) and backface culling
 ** (where the angle determines if the viewer and a face are facing
 ** each other).
 */
float NovaVertex::dot(const NovaVertex &b) const {
	return ((x * b.x) + (y * b.y) + (z * b.z));
}

/*
 ** void AstVector::operator *= ( AstFixedpoint_t scaler )
 **
 ** This routine overloads the "*=" operator so that a vector
 ** can be multiplied by a fixed point scaler easily.  This
 ** routine will scale the vector.
 */
void NovaVertex::operator *=(float s) {
	x = (x * s);
	y = (y * s);
	z = (z * s);
}

void NovaVertex::operator *=(const AstMatrix3x3 &m) {
	float save_x, save_y, save_z;

	save_x = x;
	save_y = y;
	save_z = z;

	x = (m[0][0] * save_x) + (m[1][0] * save_y) + (m[2][0] * save_z);
	y = (m[0][1] * save_x) + (m[1][1] * save_y) + (m[2][1] * save_z);
	z = (m[0][2] * save_x) + (m[1][2] * save_y) + (m[2][2] * save_z);
}

/*
 ** AstVector operator * ( const AstVector &v, AstAstMatrix3x3 &m )
 ** void AstVector::operator *= ( AstAstMatrix3x3 &m )
 **
 ** This routine overloads the "*" and "*=" operator so that a
 ** vector can be multiplied by a 3x3 matrix easily.  This routine
 ** will transform (since this is a 3x3 matrix, the transformation
 ** will in all likelihood be a rotation) the passed vector and
 ** return the transformed vector.
 */
NovaVertex operator *(const NovaVertex &v, const AstMatrix3x3 &m) {
	NovaVertex result;

	result.x = (m[0][0] * v.x) + (m[1][0] * v.y) + (m[2][0] * v.z);
	result.y = (m[0][1] * v.x) + (m[1][1] * v.y) + (m[2][1] * v.z);
	result.z = (m[0][2] * v.x) + (m[1][2] * v.y) + (m[2][2] * v.z);

	return result;
}

/*
 ** AstVector operator * ( const AstVector &v, AstAstMatrix4x3 &m )
 **
 ** This routine overloads the "*" operator so that a
 ** vector can be multiplied by a 4x3 matrix easily.  This routine
 ** will transform the vector and return the new vector.
 */
NovaVertex operator *(const NovaVertex &v, const AstMatrix4x3 &m) {
	NovaVertex result;

	result.x = (m[0][0] * v.x) + (m[1][0] * v.y) + (m[2][0] * v.z) + m[3][0];
	result.y = (m[0][1] * v.x) + (m[1][1] * v.y) + (m[2][1] * v.z) + m[3][1];
	result.z = (m[0][2] * v.x) + (m[1][2] * v.y) + (m[2][2] * v.z) + m[3][2];

	return result;
}

/*
 ** AstFixedpoint_t AstVector::Magnitude( void ) cpmst
 **
 ** This function calculates the length, or magnitude, of a
 ** vector.  This function is required when normalizing the
 ** vector.  The magnitude of a vector is equal to the distance
 ** from (0,0,0) to the vector, or:
 **
 **     sqrt( X*X + Y*Y + Z*Z )
 */
float NovaVertex::magnitude(void) const {
	return ((float) sqrt((x * x) + (y * y) + (z * z)));
}

float NovaVertex::magnitudeSquared(void) const {
	return ((x * x) + (y * y) + (z * z));
}

/*
 ** AstFixedpoint_t AstVector::Normalize( void )
 **
 ** This function normalizes a vector, which means that its
 ** length is reduced to 1.  Such a vector is known as a
 ** normalized or unit vector.
 */
void NovaVertex::normalize(void) {
	float temp = magnitude();

	x = (x / temp);
	y = (y / temp);
	z = (z / temp);
}

