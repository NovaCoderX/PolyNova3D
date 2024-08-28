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

#ifndef __NOVA_COLOR_H
#define __NOVA_COLOR_H

struct NovaColor {
	int red, green, blue;

	NovaColor(void) {
		red = green = blue = 0;
	}

	NovaColor(int r, int g, int b) {
		red = r;
		green = g;
		blue = b;
	}

	void reset(void) {
		red = green = blue = 0;
	}

	int getRed() const {
		return red;
	}

	int getGreen() const {
		return green;
	}

	int getBlue() const {
		return blue;
	}

	int getMinusRed(void) {
		return (red - 255);
	}

	int getMinusGreen(void) {
		return (green - 255);
	}

	int getMinusBlue(void) {
		return (blue - 255);
	}

	void operator +=(NovaColor &a);

	void operator *=(float val);
};

NovaColor operator * (NovaColor &a, float val);

#endif // __NOVA_COLOR_H

