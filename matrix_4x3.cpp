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
 ** void AstAstMatrix4x3::MakeXRotation( float degrees )
 **
 ** Sets the rotation indices to the correct values to make
 ** this matrix a rotation matrix of about the given axis.
 */
void AstMatrix4x3::MakeXRotation(float degrees) {
	double radian;
	radian = (M_PI / 180) * degrees;

	data[1][1] = (float) cos(radian);
	data[2][2] = (float) cos(radian);
	data[2][1] = (float) -sin(radian);
	data[1][2] = (float) sin(radian);
}

void AstMatrix4x3::MakeYRotation(float degrees) {
	double radian;
	radian = (M_PI / 180) * degrees;

	data[0][0] = (float) cos(radian);
	data[0][2] = (float) -sin(radian);
	data[2][0] = (float) sin(radian);
	data[2][2] = (float) cos(radian);
}

void AstMatrix4x3::MakeZRotation(float degrees) {
	double radian;

	radian = (M_PI / 180) * degrees;

	data[0][0] = (float) cos(radian);
	data[1][1] = (float) cos(radian);
	data[0][1] = (float) sin(radian);
	data[1][0] = (float) -sin(radian);
}

/*
 ** void AstMatrix4x3::Make?Translation( amount )
 **
 ** Sets the correct matrix elements to make a translation matrix
 ** of "amount" units along the given axis.
 */
void AstMatrix4x3::MakeXTranslation(float amount) {
	data[3][0] = amount;
	data[3][1] = 0;
	data[3][2] = 0;
}
void AstMatrix4x3::MakeYTranslation(float amount) {
	data[3][0] = 0;
	data[3][1] = amount;
	data[3][2] = 0;
}
void AstMatrix4x3::MakeZTranslation(float amount) {
	data[3][0] = 0;
	data[3][1] = 0;
	data[3][2] = amount;
}

/*
 ** Private identity matrix data set
 */
const static float _identity_matrix_4x3[4][3] = { { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 } };

/*
 ** void AstMatrix4x3::MakeIdentity( void )
 **
 ** Makes the matrix into an identity matrix by copying the identity
 ** matrix source data values into its own data set.
 */
void AstMatrix4x3::MakeIdentity(void) {
	memcpy(data, _identity_matrix_4x3, sizeof(_identity_matrix_4x3));
}

/*
 ** AstMatrix4x3 operator * ( const AstMatrix4x3 &a, const AstMatrix4x3 &b )
 **
 ** Overloaded * operator that multiplies two matrices together.
 */
AstMatrix4x3 operator *(const AstMatrix4x3 &a, const AstMatrix4x3 &b) {
	static AstMatrix4x3 m;

	/*
	 **   m[0][0] = a[0][0] * b[0][0] +
	 **             a[0][1] * b[1][0] +
	 **             a[0][2] * b[2][0] +
	 **           ( a[0][3] * b[3][0] ) = 0;
	 */
	m[0][0] = (a[0][0] * b[0][0]) + (a[0][1] * b[1][0]) + (a[0][2] * b[2][0]);

	/*
	 **   m[0][1] = a[0][0] * b[0][1] +
	 **             a[0][1] * b[1][1] +
	 **             a[0][2] * b[2][1] +
	 **           ( a[0][3] * b[3][1] ) = 0;
	 */
	m[0][1] = (a[0][0] * b[0][1]) + (a[0][1] * b[1][1]) + (a[0][2] * b[2][1]);

	/*
	 **   m[0][2] = a[0][0] * b[0][2] +
	 **             a[0][1] * b[1][2] +
	 **             a[0][2] * b[2][2] +
	 **           ( a[0][3] * b[3][2] ) = 0;
	 */
	m[0][2] = (a[0][0] * b[0][2]) + (a[0][1] * b[1][2]) + (a[0][2] * b[2][2]);

	/*
	 **   m[1][0] = a[1][0] * b[0][0] +
	 **             a[1][1] * b[1][0] +
	 **             a[1][2] * b[2][0] +
	 **           ( a[1][3] * b[3][0] ) = 0;
	 */
	m[1][0] = (a[1][0] * b[0][0]) + (a[1][1] * b[1][0]) + (a[1][2] * b[2][0]);

	/*
	 **   m[1][1] = a[1][0] * b[0][1] +
	 **             a[1][1] * b[1][1] +
	 **             a[1][2] * b[2][1] +
	 **           ( a[1][3] * b[3][1] ) = 0;
	 */
	m[1][1] = (a[1][0] * b[0][1]) + (a[1][1] * b[1][1]) + (a[1][2] * b[2][1]);

	/*
	 **   m[1][2] = a[1][0] * b[0][2] +
	 **             a[1][1] * b[1][2] +
	 **             a[1][2] * b[2][2] +
	 **           ( a[1][3] * b[3][2] ) = 0;
	 */
	m[1][2] = (a[1][0] * b[0][2]) + (a[1][1] * b[1][2]) + (a[1][2] * b[2][2]);

	/*
	 **   m[2][0] = a[2][0] * b[0][0] +
	 **             a[2][1] * b[1][0] +
	 **             a[2][2] * b[2][0] +
	 **           ( a[2][3] * b[3][0] ) = 0;
	 */
	m[2][0] = (a[2][0] * b[0][0]) + (a[2][1] * b[1][0]) + (a[2][2] * b[2][0]);

	/*
	 **   m[2][1] = a[2][0] * b[0][1] +
	 **             a[2][1] * b[1][1] +
	 **             a[2][2] * b[2][1] +
	 **           ( a[2][3] * b[3][1] ) = 0;
	 */
	m[2][1] = (a[2][0] * b[0][1]) + (a[2][1] * b[1][1]) + (a[2][2] * b[2][1]);

	/*
	 **   m[2][2] = a[2][0] * b[0][2] +
	 **             a[2][1] * b[1][2] +
	 **             a[2][2] * b[2][2] +
	 **           ( a[2][3] * b[3][2] ) = 0;
	 */
	m[2][2] = (a[2][0] * b[0][2]) + (a[2][1] * b[1][2]) + (a[2][2] * b[2][2]);

	/*
	 **   m[3][0] = a[3][0] * b[0][0] +
	 **             a[3][1] * b[1][0] +
	 **             a[3][2] * b[2][0] +
	 **           ( a[3][3] * b[3][0] ) = b[3][0];
	 */
	m[3][0] = (a[3][0] * b[0][0]) + (a[3][1] * b[1][0]) + (a[3][2] * b[2][0]) + b[3][0];

	/*
	 **   m[3][1] = a[3][0] * b[0][1] +
	 **             a[3][1] * b[1][1] +
	 **             a[3][2] * b[2][1] +
	 **           ( a[3][3] * b[3][1] ) = b[3][1];
	 */
	m[3][1] = (a[3][0] * b[0][1]) + (a[3][1] * b[1][1]) + (a[3][2] * b[2][1]) + b[3][1];

	/*
	 **   m[3][2] = a[3][0] * b[0][2] +
	 **             a[3][1] * b[1][2] +
	 **             a[3][2] * b[2][2] +
	 **           ( a[3][3] * b[3][2] ) = b[3][2];
	 */
	m[3][2] = (a[3][0] * b[0][2]) + (a[3][1] * b[1][2]) + (a[3][2] * b[2][2]) + b[3][2];

	return m;
}

/*
 ** void AstMatrix4x3::GetRotationMatrix( Matrix3x3 &m ) const
 **
 ** This routine copies the rotation submatrix of the matrix into
 ** the 3x3 rotation matrix passed to the function.
 */
void AstMatrix4x3::GetRotationMatrix(AstMatrix3x3 &m) const {
	for (int i = 0; i < 3; i++)
		memcpy(m[i], data[i], sizeof(float) * 3);
}

/*
 ** void AstMatrix4x3::SetRotationMatrix( const Matrix3x3 &m )
 **
 ** Sets the rotation submatrix of the given matrix to the 3x3
 ** rotation matrix passed to this member function.
 */
void AstMatrix4x3::SetRotationMatrix(const AstMatrix3x3 &m) {
	for (int i = 0; i < 3; i++)
		memcpy(data[i], m[i], sizeof(float) * 3);
}

/*
 ** void AstMatrix4x3::NormalizeDirectionVectors( void )
 **
 ** This function normalizes the direction vectors of the matrix,
 ** preventing unwanted spurious scaling from creeping in as a
 ** a result of accumulated error.
 */
void AstMatrix4x3::NormalizeDirectionVectors(void) {
	NovaVertex v;

	GetVU(v);
	v.normalize();
	SetVU(v);

	GetVR(v);
	v.normalize();
	SetVR(v);

	GetVF(v);
	v.normalize();
	SetVF(v);
}

