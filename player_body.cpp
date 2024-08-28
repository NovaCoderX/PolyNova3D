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

PlayerBody::PlayerBody(const char *name, float movementAcceleration, float movementDeceleration, float maxForwardVelocity,
		float maxBackwardVelocity, float maxStrafeVelocity) :
		BaseObject(name) {

	if (movementAcceleration <= 0) {
		FatalError("Movement acceleration value is invalid\n");
	}

	if (movementDeceleration >= 0) {
		FatalError("Movement deceleration value is invalid\n");
	}

	if (maxForwardVelocity <= 0) {
		FatalError("Forward velocity value is invalid\n");
	}

	if (maxBackwardVelocity >= 0) {
		FatalError("Backward velocity value is invalid\n");
	}

	if (maxStrafeVelocity <= 0) {
		FatalError("Max strafe velocity value is invalid\n");
	}

	this->movementAcceleration = movementAcceleration;
	this->movementDeceleration = movementDeceleration;
	this->maxForwardVelocity = maxForwardVelocity;
	this->maxBackwardVelocity = maxBackwardVelocity;
	this->maxStrafeRightVelocity = maxStrafeVelocity;
	this->maxStrafeLeftVelocity = -maxStrafeVelocity;

	playerHead = NULL;
}

void PlayerBody::moveForwards(float timePassed) {
	float lf_temp, af_max;

	// Increase the movement of the body.
	lf_temp = movement.if_move_z;
	lf_temp += (movementAcceleration * timePassed);

	if (lf_temp > 0) {
		// Currently moving forwards.
		af_max = (maxForwardVelocity * timePassed);

		if (lf_temp > af_max) {
			// We are moving too quickly.
			lf_temp = af_max;
		}
	}

	// Must keep the head in sync with the body.
	playerHead->movement.if_move_z = movement.if_move_z = lf_temp;
}

void PlayerBody::moveBackwards(float timePassed) {
	float temp, max;

	// Increase the movement of the body.
	temp = movement.if_move_z;
	temp += (movementDeceleration * timePassed);

	if (temp < 0) {
		// Currently moving backwards.
		max = (maxBackwardVelocity * timePassed);

		if (temp < max) {
			// We are moving too quickly.
			temp = max;
		}
	}

	// Must keep the head in sync with the body.
	playerHead->movement.if_move_z = movement.if_move_z = temp;
}

void PlayerBody::decreaseMovement(float timePassed) {
	float temp;

	temp = movement.if_move_z;

	if (temp != 0) {
		// We are currently moving either backwards or forwards.
		if (temp > 0) {
			// We are moving forwards.
			temp += movementDeceleration;

			// Make sure we haven't gone too far!
			if (temp < 0) {
				// Stop moving.
				temp = 0;
			}
		} else {
			// We are moving backwards.
			temp += -movementDeceleration;

			// Make sure we haven't gone too far!
			if (temp > 0) {
				// Stop moving.
				temp = 0;
			}
		}
	}

	// Must keep the head in sync with the body.
	playerHead->movement.if_move_z = movement.if_move_z = temp;
}

void PlayerBody::strafeLeft(float timePassed) {
	float temp, max;

	// Increase the movement of the body.
	temp = movement.if_move_x;
	temp += (movementDeceleration * timePassed);

	if (temp < 0) {
		// Currently moving to the left.
		max = (maxStrafeLeftVelocity * timePassed);

		if (temp < max) {
			// We are moving too quickly.
			temp = max;
		}
	}

	// Must keep the head in sync with the body.
	playerHead->movement.if_move_x = movement.if_move_x = temp;
}

void PlayerBody::strafeRight(float timePassed) {
	float temp, max;

	// Increase the movement of the body.
	temp = movement.if_move_x;
	temp += (movementAcceleration * timePassed);

	if (temp > 0) {
		// Currently moving to the right.
		max = (maxStrafeRightVelocity * timePassed);

		if (temp > max) {
			// We are moving too quickly.
			temp = max;
		}
	}

	// Must keep the head in sync with the body.
	playerHead->movement.if_move_x = movement.if_move_x = temp;
}

void PlayerBody::decreaseStrafe(float timePassed) {
	float temp;

	temp = movement.if_move_x;

	if (temp != 0) {
		// We are currently moving either left or right.
		if (temp > 0) {
			// We are moving right.
			temp += movementDeceleration;

			// Make sure we haven't gone too far!
			if (temp < 0) {
				// Stop moving.
				temp = 0;
			}
		} else {
			// We are moving left.
			temp += -movementDeceleration;

			// Make sure we haven't gone too far!
			if (temp > 0) {
				// Stop moving.
				temp = 0;
			}
		}
	}

	// Must keep the head in sync with the body.
	playerHead->movement.if_move_x = movement.if_move_x = temp;
}

void PlayerBody::turn(float amount) {
	// Must keep the head in sync with the body.
	playerHead->movement.if_rotate_y = movement.if_rotate_y = amount;
}

void PlayerBody::stopTurning() {
	// Must keep the head in sync with the body.
	playerHead->movement.if_rotate_y = movement.if_rotate_y = 0;
}

void PlayerBody::applyMovements() {
	if (movement.if_rotate_y != 0) {
		rotateAroundY(movement.if_rotate_y);
	}

	if (movement.if_move_x != 0) {
		moveAlongX(movement.if_move_x);
	}

	if (movement.if_move_z != 0) {
		moveAlongZ(movement.if_move_z);
	}
}

void PlayerBody::collisionOccurred() {
	movement.reverse();

	if (movement.if_move_x != 0) {
		moveAlongX(movement.if_move_x);
	}

	if (movement.if_move_z != 0) {
		moveAlongZ(movement.if_move_z);
	}

	movement.reset();
}
