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
 ** The equation for a plane is:
 **
 ** N dot P + D = 0
 **
 ** Where N is the plane's surface normal and P is a point
 ** on the plane and D is the plane's plane constant.
 **
 ** To compute the plane constant we solve for D:
 **
 ** D = -( N dot P )
 **
 ** N is computed as the cross product of two of triangle's sides, and
 ** one of the triangle's points is substituted for P.
 */
void NovaTriangle::computePlaneAndNormals() {
	NovaVertex side1, side2;

	side1 = (*staticVertex[1]) - (*staticVertex[0]);
	side2 = (*staticVertex[2]) - (*staticVertex[1]);

	surfaceNormal = side1.cross(side2);
	surfaceNormal.normalize();
	planeConstant = -surfaceNormal.dot((*staticVertex[0]));

	// Also update our sum vertices in the object definition.
	this->sumVertex[0]->addSurfaceNormal(surfaceNormal);
	this->sumVertex[0]->addPlaneConstant(planeConstant);

	this->sumVertex[1]->addSurfaceNormal(surfaceNormal);
	this->sumVertex[1]->addPlaneConstant(planeConstant);

	this->sumVertex[2]->addSurfaceNormal(surfaceNormal);
	this->sumVertex[2]->addPlaneConstant(planeConstant);
}

/*
 ** This function is used to transform an object's
 ** vertices, defined in the LCS, into the CCS.  This implies
 ** an intermediate transformation into the LCS, which is
 ** accomplished via the concatenation of two separate
 ** matrices, the world_to_camera_matrix and the local_to_world
 ** matrix.  We take the composite matrix and apply it against
 ** all of our object definition's vertices, then store the
 ** transformed vertices into our scratch_vertex_array.
 */
bool NovaTriangle::transform(const AstMatrix4x3 &matrix) {
	int invalidVerts = 0;

	for (int i = 0; i < 3; i++) {
		// We only transform a single (shared) vertex once per frame.
		if (scratchVertex[i]->getStage() != RENDER_STAGE_TRANSFORMED) {
			(*scratchVertex[i]) = (*staticVertex[i]) * matrix;

			scratchVertex[i]->setStage(RENDER_STAGE_TRANSFORMED);
		}

		// Check near/far Z limits.
		if (scratchVertex[i]->z < g_world->getCamera()->getNearZ() || scratchVertex[i]->z > g_world->getCamera()->getFarZ()) {
			invalidVerts++;
		}
	}

	if (invalidVerts == 3) {
		return false;
	}

	return true;
}

/*
 ** As the name implies, this routine projects the vertices in the
 ** object.  The vertices are retrieved from the scratch_vertex_array,
 ** which contains CCS data, and projected (using the camera, which
 ** contains field of view information) into the scratch_screen_array.
 */
bool NovaTriangle::project() {
	for (int i = 0; i < 3; i++) {
		// We only project a single (shared) vertex once per frame.
		if (scratchVertex[i]->getStage() != RENDER_STAGE_PROJECTED) {
			scratchVertex[i]->projectedX = g_world->getCamera()->horizontalProject(scratchVertex[i]->x, scratchVertex[i]->z);
			scratchVertex[i]->projectedY = g_world->getCamera()->verticalProject(scratchVertex[i]->y, scratchVertex[i]->z);
			scratchVertex[i]->projectedZ = ((int)-scratchVertex[i]->z);

			scratchVertex[i]->setStage(RENDER_STAGE_PROJECTED);
		}
	}

	// Convert the projected triangle into a rectangle.
	DisplayVertex topR, botL;

	// Seed.
	topR.x = scratchVertex[0]->projectedX;
	topR.y = scratchVertex[0]->projectedY;
	botL.x = scratchVertex[0]->projectedX;
	botL.y = scratchVertex[0]->projectedY;

	for (int i = 0; i < 3; i++) {
		if (scratchVertex[i]->projectedX < botL.x) {
			botL.x = scratchVertex[i]->projectedX;
		}

		if (scratchVertex[i]->projectedX > topR.x) {
			topR.x = scratchVertex[i]->projectedX;
		}

		if (scratchVertex[i]->projectedY < botL.y) {
			botL.y = scratchVertex[i]->projectedY;
		}

		if (scratchVertex[i]->projectedY > topR.y) {
			topR.y = scratchVertex[i]->projectedY;
		}
	}

	if (!g_world->getCamera()->checkProjectedPoints(topR, botL)) {
		return false;
	}

	return true;
}

void NovaTriangle::draw() {
	int red, green, blue;

	// We Need to apply the vertex lighting to this triangle before it can be drawn.
	for (int i = 0; i < 3; i++) {
		scratch_screen_array[i].x = scratchVertex[i]->projectedX;
		scratch_screen_array[i].y = scratchVertex[i]->projectedY;
		scratch_screen_array[i].z = scratchVertex[i]->projectedZ;

		// Get loading factor and calculate the new color for this triangle.
		red = baseColor.getMinusRed() + scratchVertex[i]->lightSum.getRed();
		green = baseColor.getMinusGreen() + scratchVertex[i]->lightSum.getGreen();
		blue = baseColor.getMinusBlue() + scratchVertex[i]->lightSum.getBlue();

		// Must check for underflow.
		if (red < 0) {
			red = 0;
		}

		if (green < 0) {
			green = 0;
		}

		if (blue < 0) {
			blue = 0;
		}

		scratch_screen_array[i].color[0] = (float) red / 255;
		scratch_screen_array[i].color[1] = (float) green / 255;
		scratch_screen_array[i].color[2] = (float) blue / 255;
	}

	g_world->getDisplay()->drawTriangle(this);
}
