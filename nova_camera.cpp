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
 ** This is a helper function used by the camera class to
 ** determine the horizontal and vertical distance factors used
 ** when doing perspective mappings.  Recall that the perspective
 ** transformation distance factor determines the field of view
 ** as follows:
 **
 **   tan( fov/2 ) = screen_width / ( 2 * df
 **
 **   thus solving for df gives us:
 **
 **   df = screen_width / ( 2 * tan( fov / 2 ) )
 */
static inline float computeDistanceFactor(unsigned fov, unsigned width) {
	int half_width = width / 2;
	double tangent = tan((fov / 10.0) * (M_PI / 180.0));

	return (half_width / (float) tangent);
}

Camera::Camera(BaseObject *cameraObject, unsigned int fov, float nearZ, float farZ) {
	this->cameraObject = cameraObject;

	worldToCamera.MakeIdentity();

	// All screen res modes MUST be 4:3.
	int screenWidth = g_world->getDisplay()->getScreenWidth();
	int screenHeight = g_world->getDisplay()->getScreenHeight();

	SCREEN_WIDTH_ZERO_INDEXED = screenWidth - 1;
	SCREEN_HEIGHT_ZERO_INDEXED = screenHeight - 1;
	HALF_SCREEN_WIDTH = screenWidth / 2;
	HALF_SCREEN_HEIGHT = screenHeight / 2;

	this->fov = fov;
	hdf = computeDistanceFactor(fov / 2, screenWidth);
	vdf = computeDistanceFactor(fov / 2, screenHeight);
	this->nearZ = nearZ;
	this->farZ = farZ;
}

// Calculates the inverse of the camera LCS->WCS matrix.
// This is stored as the camera's worldToObject matrix which
// can be multiplied by an objects LCS->WCS matrix to compute THAT objects
// LCS->CCS matrix.
void Camera::calculateWorldToCameraMatrix() {
	NovaVertex position;
	AstMatrix3x3 rm3x3;

	/*
	 ** Get camera's rotation matrix and translation.
	 */
	cameraObject->getRotationMatrixWCS(rm3x3);

	/*
	 ** Invert the rotation matrix (a transpose is the
	 ** the same as an inversion when working with
	 ** rotation matrices).
	 */
	rm3x3.Transpose();

	/*
	 ** Transform its location by the inverted rotation matrix
	 */
	position = cameraObject->getPositionWCS() * rm3x3;

	/*
	 ** Create world_to_object_matrix
	 */
	worldToCamera.SetRotationMatrix(rm3x3);
	worldToCamera.SetTranslation(-position);
	worldToCamera.NormalizeDirectionVectors();
}

bool Camera::checkProjectedPoint(NovaVertex &location) const {
	DisplayVertex dest;

	dest.x = horizontalProject(location.x, location.z);
	dest.y = verticalProject(location.y, location.z);

	if ((dest.x < 0 || dest.x > SCREEN_WIDTH_ZERO_INDEXED) || (dest.y < 0 || dest.y > SCREEN_HEIGHT_ZERO_INDEXED)) {
		return false;
	}

	return true;
}

bool Camera::checkProjectedPoints(NovaVertex &topRight, NovaVertex &botLeft) const {
	DisplayVertex topR, botL;

	topR.x = horizontalProject(topRight.x, topRight.z);
	topR.y = verticalProject(topRight.y, topRight.z);

	botL.x = horizontalProject(botLeft.x, botLeft.z);
	botL.y = verticalProject(botLeft.y, botLeft.z);

	if ((topR.x >= 0) && (botL.y < SCREEN_HEIGHT_ZERO_INDEXED) && (botL.x < SCREEN_WIDTH_ZERO_INDEXED) && (topR.y >= 0)) {
		return true;
	}

	return false;
}

bool Camera::checkProjectedPoints(DisplayVertex &topRight, DisplayVertex &botLeft) const {
	if ((topRight.x >= 0) && (botLeft.y < SCREEN_HEIGHT_ZERO_INDEXED) && (botLeft.x < SCREEN_WIDTH_ZERO_INDEXED) && (topRight.y >= 0)) {
		return true;
	}

	return false;
}

