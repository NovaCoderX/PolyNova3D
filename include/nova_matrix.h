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
#ifndef __NOVA_MATRIX_H
#define __NOVA_MATRIX_H

/*
 ** Matrix Definitions
 */
class AstMatrix3x3;
class AstMatrix4x3;

/******************************************************************************/
class AstMatrix3x3 {
protected:
	float data[3][3];

public:

	void Transpose(void);

	const float* operator[](int a) const {
		return data[a];
	}
	float* operator[](int a) {
		return data[a];
	}
};
/******************************************************************************/
class AstMatrix4x3 {
protected:
	float data[4][3];

public:

	void MakeIdentity(void);

	void MakeXTranslation(float amount);
	void MakeYTranslation(float amount);
	void MakeZTranslation(float amount);

	void MakeXRotation(float amount);
	void MakeYRotation(float amount);
	void MakeZRotation(float amount);

	void NormalizeDirectionVectors(void);

	void GetRotationMatrix(AstMatrix3x3 &rm) const;
	void SetRotationMatrix(const AstMatrix3x3 &rm);
	void SetTranslation(float x, float y, float z) {
		data[3][0] = x;
		data[3][1] = y;
		data[3][2] = z;
	}
	void SetTranslation(const NovaVertex &v) {
		SetTranslation(v.x, v.y, v.z);
	}

	NovaVertex GetTranslation(void) const {
		return NovaVertex(data[3][0], data[3][1], data[3][2]);
	}

	void GetVU(NovaVertex &v) const {
		v.x = data[0][1];
		v.y = data[1][1];
		v.z = data[2][1];
	}
	void GetVR(NovaVertex &v) const {
		v.x = data[0][0];
		v.y = data[1][0];
		v.z = data[2][0];
	}
	void GetVF(NovaVertex &v) const {
		v.x = data[0][2];
		v.y = data[1][2];
		v.z = data[2][2];
	}

	void SetVU(const NovaVertex &v) {
		data[0][1] = v.x;
		data[1][1] = v.y;
		data[2][1] = v.z;
	}
	void SetVR(const NovaVertex &v) {
		data[0][0] = v.x;
		data[1][0] = v.y;
		data[2][0] = v.z;
	}
	void SetVF(const NovaVertex &v) {
		data[0][2] = v.x;
		data[1][2] = v.y;
		data[2][2] = v.z;
	}

	const float* operator[](int a) const {
		return data[a];
	}
	float* operator[](int a) {
		return data[a];
	}
};

AstMatrix4x3 operator *(const AstMatrix4x3 &a, const AstMatrix4x3 &b);
NovaVertex operator *(const NovaVertex &v, const AstMatrix3x3 &m);
NovaVertex operator *(const NovaVertex &v, const AstMatrix4x3 &m);


#endif // __NOVA_MATRIX_H
