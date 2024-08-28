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
#ifndef __PLAYER_BODY_H
#define __PLAYER_BODY_H

class PlayerBody: public BaseObject {
public:
	PlayerBody(const char *name, float movementAcceleration, float movementDeceleration, float maxForwardVelocity,
			float maxBackwardVelocity, float maxStrafeVelocity);

	void setPlayerHead(PlayerHead *playerHead) {
		this->playerHead = playerHead;
	}

	void moveForwards(float timePassed);
	void moveBackwards(float timePassed);
	void decreaseMovement(float timePassed);

	void strafeLeft(float timePassed);
	void strafeRight(float timePassed);
	void decreaseStrafe(float timePassed);

	void turn(float amount);
	void stopTurning();

	void applyMovements();
	void collisionOccurred();

private:
	PlayerMovement movement;

	// How many movement units the player increases/decreases per time unit.
	float movementAcceleration; // Note - Must be a positive value!
	float movementDeceleration; // Note - Must be a negative value!

	// Specify maximum movement velocities allowed.
	float maxForwardVelocity;  // Note - Must be a positive value!
	float maxBackwardVelocity; // Note - Must be a negative value!

	float maxStrafeRightVelocity; // Note - Must be a positive value!
	float maxStrafeLeftVelocity;  // Note - Must be a negative value!

	PlayerHead *playerHead;
};

#endif // __PLAYER_BODY_H
