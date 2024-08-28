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
 ** void AstMatrix3x3::Transpose( void )
 **
 ** The inverse of a 3x3 rotation matrix is its transpose, and thus
 ** this function transposes its data (flips it over the diagonal)
 ** to facilitate optimized inversion.
 */
void AstMatrix3x3::Transpose(void) {
	float tmp;

	tmp = data[0][1];
	data[0][1] = data[1][0];
	data[1][0] = tmp;

	tmp = data[0][2];
	data[0][2] = data[2][0];
	data[2][0] = tmp;

	tmp = data[1][2];
	data[1][2] = data[2][1];
	data[2][1] = tmp;
}
