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

// Should probably pass in object references for head, body and the camera (camera should not be an object!)
AstPlayer::AstPlayer(PlayerHead *head, PlayerBody *body) {
	playerHead = head;
	playerBody = body;

	playerBody->setPlayerHead(playerHead);

	strafeLeft = strafeRight = moveForwards = moveBackwards = false;

	// We only check collisions using the player's body.
	collisionService = new CollisionService(body);
}

AstPlayer::~AstPlayer() {
	if (collisionService) {
		delete collisionService;
		collisionService = NULL;
	}
}

void AstPlayer::doSomething(float timePassed) {
	int mouseMovementX, mouseMovementY;

	g_world->getDisplay()->getMouseMovement(mouseMovementX, mouseMovementY);

	if (mouseMovementX != 0) {
		playerBody->turn(mouseMovementX * 0.5);
	} else {
		// No longer turning.
		playerBody->stopTurning();
	}

	if (strafeLeft || strafeRight) {
		if (strafeLeft) {
			playerBody->strafeLeft(timePassed);
		} else {
			playerBody->strafeRight(timePassed);
		}
	} else {
		// Remove (some) velocity if we are currently moving forwards or backwards.
		playerBody->decreaseStrafe(timePassed);
	}

	if (moveForwards || moveBackwards) {
		if (moveForwards) {
			playerBody->moveForwards(timePassed);
		} else {
			playerBody->moveBackwards(timePassed);
		}
	} else {
		// Remove (some) velocity if we are currently moving forwards or backwards.
		playerBody->decreaseMovement(timePassed);
	}

	// Apply the current movement values.
	playerBody->applyMovements();

	if (mouseMovementY != 0) {
		playerHead->look(mouseMovementY * 0.5);
	}

	playerHead->applyMovements();
}

