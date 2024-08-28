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

#ifndef __PLAYER_MOVEMENT_H_
#define __PLAYER_MOVEMENT_H_

struct PlayerMovement {
	float if_move_x, if_move_y, if_move_z;
	float if_rotate_x, if_rotate_y, if_rotate_z;

	PlayerMovement(void) {
		if_move_x = if_move_y = if_move_z = 0;
		if_rotate_x = if_rotate_y = if_rotate_z = 0;
	}

	void reset(void) {
		if_move_x = if_move_y = if_move_z = 0;
		if_rotate_x = if_rotate_y = if_rotate_z = 0;
	}

	void reverse(void) {
		if (if_move_x != 0) {
			if_move_x = -if_move_x;
		}

		if (if_move_y != 0) {
			if_move_y = -if_move_y;
		}

		if (if_move_z != 0) {
			if_move_z = -if_move_z;
		}
	}
};

#endif // __PLAYER_MOVEMENT_H_
