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
#ifndef __CAMERA_H
#define __CAMERA_H

class Camera {
public:
	Camera(BaseObject *cameraObject, unsigned int fov, float nearZ, float farZ);

	float horizontalProject(float value, float z) const {
		if (z > 0) {
			return ((value * hdf) / z) + HALF_SCREEN_WIDTH;
		} else {
			return (value * hdf) + HALF_SCREEN_WIDTH;
		}
	}

	float verticalProject(float value, float z) const {
		if (z > 0) {
			return ((value * vdf) / z) + HALF_SCREEN_HEIGHT;
		} else {
			return (value * vdf) + HALF_SCREEN_HEIGHT;
		}
	}

	void calculateWorldToCameraMatrix();

	AstMatrix4x3& getWorldToCameraMatrix() {
		return worldToCamera;
	}

	float getNearZ() {
		return nearZ;
	}

	float getFarZ() {
		return farZ;
	}

	NovaVertex getPositionWCS() const {
		return cameraObject->getPositionWCS();
	}

	bool checkProjectedPoint(NovaVertex &point) const;
	bool checkProjectedPoints(NovaVertex &topRight, NovaVertex &botLeft) const;
	bool checkProjectedPoints(DisplayVertex &topRight, DisplayVertex &botLeft) const;

private:
	BaseObject *cameraObject; // The object acting as the camera.
	AstMatrix4x3 worldToCamera; // LCS->WCS (inverse).

	// Res parameters.
	int SCREEN_WIDTH_ZERO_INDEXED;
	int SCREEN_HEIGHT_ZERO_INDEXED;
	int HALF_SCREEN_WIDTH;
	int HALF_SCREEN_HEIGHT;

	unsigned int fov;
	float vdf, hdf;
	float nearZ;
	float farZ;
};

#endif // __CAMERA_H
