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
#ifndef __NOVA_VERTEX_H
#define __NOVA_VERTEX_H

// Forward declaration.
class AstMatrix3x3;
class AstMatrix4x3;

class NovaVertex {
public:
	float x, y, z;

	NovaVertex(void) {
		x = y = z = 0;
	}

	NovaVertex(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	void operator *=(float scaler);
	void operator *=(const AstMatrix3x3 &m);

	void operator +=(const NovaVertex &v) {
		x += v.x;
		y += v.y;
		z += v.z;
	}

	void operator -=(const NovaVertex &v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
	}

	void reset(void) {
		x = y = z = 0;
	}

	NovaVertex cross(const NovaVertex &v) const;
	float dot(const NovaVertex &v) const;
	void normalize(void);
	float magnitude(void) const;
	float magnitudeSquared(void) const;
};

static inline NovaVertex operator -(const NovaVertex &v) {
	return NovaVertex(-v.x, -v.y, -v.z);
}

static inline NovaVertex operator +(const NovaVertex &a, const NovaVertex &b) {
	return NovaVertex(a.x + b.x, a.y + b.y, a.z + b.z);
}

static inline NovaVertex operator -(const NovaVertex &a, const NovaVertex &b) {
	return NovaVertex(a.x - b.x, a.y - b.y, a.z - b.z);
}

static inline bool operator ==(const NovaVertex &a, const NovaVertex &b) {
	if ((a.x == b.x) && (a.y == b.y) && (a.z == b.z))
		return true;
	else
		return false;
}

static inline bool operator !=(const NovaVertex &a, const NovaVertex &b) {
	if ((a.x != b.x) || (a.y != b.y) || (a.z != b.z))
		return true;
	else
		return false;
}

NovaVertex operator *(const NovaVertex &v, const AstMatrix3x3 &m);
NovaVertex operator *(const NovaVertex &v, const AstMatrix4x3 &m);

#endif //  __NOVA_VERTEX_H

