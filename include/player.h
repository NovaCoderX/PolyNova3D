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
#ifndef __AST_PLAYER_H
#define __AST_PLAYER_H

class AstPlayer {
public:
	AstPlayer(PlayerHead *head, PlayerBody *body);
	~AstPlayer();

	void setMoveBackwards(bool toggle) {
		this->moveBackwards = toggle;
	}

	void setMoveForwards(bool toggle) {
		this->moveForwards = toggle;
	}

	void setStrafeLeft(bool toggle) {
		this->strafeLeft = toggle;
	}

	void setStrafeRight(bool toggle) {
		this->strafeRight = toggle;
	}

	void doSomething(float timePassed);

	// Hack, should be part of World.
	PlayerBody* getPlayerBody() {
		return playerBody;
	}

	PlayerHead* getPlayerHead() {
		return playerHead;
	}

	// Did the player just hit this solid object?
	bool checkCollision(BaseObject *solidObject) {
		return collisionService->checkCollision(solidObject);
	}

	void collisionOccured() {
		playerHead->collisionOccurred();
		playerBody->collisionOccurred();
	}


private:
	// The object acting as the camera.
	PlayerHead *playerHead;

	// The object that the camera is sitting on.
	PlayerBody *playerBody;

	// This represents wants the player wants to do.
	bool strafeLeft, strafeRight, moveForwards, moveBackwards;

	CollisionService *collisionService;
};

#endif // __AST_PLAYER_H
