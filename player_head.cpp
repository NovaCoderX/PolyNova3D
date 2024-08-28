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

PlayerHead::PlayerHead(const char *name, float maxLookUpAngle, float maxLookDownAngle) :
		BaseObject(name) {
	this->maxLookUpAngle = maxLookUpAngle;
	this->maxLookDownAngle = maxLookDownAngle;

	currentLookAngle = 0;
}

void PlayerHead::look(float amount) {
	float temp;

	temp = amount;

	// See if this increment will exceed our max values.
	if (temp >= 0) {
		if (temp + currentLookAngle > maxLookDownAngle) {
			temp = (maxLookDownAngle - currentLookAngle);
		}
	} else {
		if (temp + currentLookAngle < maxLookUpAngle) {
			temp = (maxLookUpAngle - currentLookAngle);
		}
	}

	movement.if_rotate_x = temp;
}

void PlayerHead::applyMovements() {
	// Reset head to start position.
	if (currentLookAngle != 0) {
		rotateAroundX(-currentLookAngle);
	}

	// Apply body movements.
	if (movement.if_rotate_y != 0) {
		rotateAroundY(movement.if_rotate_y);
	}

	if (movement.if_move_x != 0) {
		moveAlongX(movement.if_move_x);
	}

	if (movement.if_move_z != 0) {
		moveAlongZ(movement.if_move_z);
	}

	// Now it's OK to rotate the head on the X plane.
	if (movement.if_rotate_x != 0) {
		currentLookAngle += movement.if_rotate_x;

		// Reset.
		movement.if_rotate_x = 0;
	}

	// Rotate to old position + any new amount.
	if (currentLookAngle != 0) {
		rotateAroundX(currentLookAngle);
	}
}

void PlayerHead::collisionOccurred() {
	movement.reverse();

	// Reset head to start position.
	if (currentLookAngle != 0) {
		rotateAroundX(-currentLookAngle);
	}

	if (movement.if_move_x != 0) {
		moveAlongX(movement.if_move_x);
	}

	if (movement.if_move_z != 0) {
		moveAlongZ(movement.if_move_z);
	}

	// Rotate to old position.
	if (currentLookAngle != 0) {
		rotateAroundX(currentLookAngle);
	}

	movement.reset();
}

