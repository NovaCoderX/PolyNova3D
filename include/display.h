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

#ifndef __DISPLAY_H
#define __DISPLAY_H

class Display {
public:
	Display();
	~Display();

	void initialize(int screenWidth, int screenHeight, bool enableDepthBuffer, float nearZ, float farZ);

	int getScreenWidth() {
		return screenWidth;
	}

	int getScreenHeight() {
		return screenHeight;
	}

	void captureUserInput();

	void begin();
	void drawTriangle(NovaTriangle *triangle);
	void end();

	void displayFrameRate();

	void getMouseMovement(int &x, int &y) {
		x = mouseMovementX;
		y = mouseMovementY;

		// Reset.
		mouseMovementX = mouseMovementY = 0;
	}

private:
	int screenWidth, screenHeight;
	int numFrames;
	int mouseMovementX, mouseMovementY;
	bool depthTestEnabled;
};

#endif // __DISPLAY_H

