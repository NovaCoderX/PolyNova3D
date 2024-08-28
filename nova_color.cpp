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

NovaColor operator *(NovaColor &a, float val) {
	int a_red, a_green, a_blue;

	//LogDebugMessage("color operator A called - %f6.3\n", val);

	a_red = (int) (a.getRed() * val);
	a_green = (int) (a.getGreen() * val);
	a_blue = (int) (a.getBlue() * val);

	// Must check for overflow.
	if (a_red > 255)
		a_red = 255;

	if (a_green > 255)
		a_green = 255;

	if (a_blue > 255)
		a_blue = 255;

	// And for underflow.
	if (a_red < 0)
		a_red = 0;

	if (a_green < 0)
		a_green = 0;

	if (a_blue < 0)
		a_blue = 0;

	return (NovaColor(a_red, a_green, a_blue));
}

void NovaColor::operator +=(NovaColor &a) {
	red += a.getRed();
	green += a.getGreen();
	blue += a.getBlue();

	// Must check for overflow.
	if (red > 255)
		red = 255;

	if (green > 255)
		green = 255;

	if (blue > 255)
		blue = 255;
}

void NovaColor::operator *=(float val) {
	red = (int) (red * val);
	green = (int) (green * val);
	blue = (int) (blue * val);

	//LogDebugMessage("color operator B called - %f6.3\n", val);

	// Must check for overflow.
	if (red > 255)
		red = 255;

	if (green > 255)
		green = 255;

	if (blue > 255)
		blue = 255;

	// And for underflow.
	if (red < 0)
		red = 0;

	if (green < 0)
		green = 0;

	if (blue < 0)
		blue = 0;

}


