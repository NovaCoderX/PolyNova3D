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
#include <sys/time.h>

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

// Only 16 bit depth is supported.
#define DISPLAY_DEPTH 16

static SDL_Surface *screen = NULL;
static timeval startTime;


Display::Display() {
	screenWidth = screenHeight = 0;
	numFrames = 0;
	mouseMovementX = mouseMovementY = 0;
	depthTestEnabled = false;
}

Display::~Display() {
	LogMessage("Shutting down the Display.....\n");
	SDL_Quit();
}

void Display::initialize(int screenWidth, int screenHeight, bool enableDepthBuffer, float nearZ, float farZ) {
	if (screenWidth <= 0) {
		FatalError("Screen width must be greater than zero\n");
	}

	if (screenHeight <= 0) {
		FatalError("Screen height must be greater than zero\n");
	}

	if (nearZ <= 0) {
		FatalError("Camera nearZ must be greater than zero\n");
	}

	if (farZ <= 0) {
		FatalError("Camera farZ must be greater than zero\n");
	}

	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;

	LogMessage("Trying to open a %dx%d 16bit OpenGL screen using SDL\n", screenWidth, screenHeight);

	if (SDL_Init(SDL_INIT_NOPARACHUTE | SDL_INIT_VIDEO) == -1) {
		FatalError("Could not initialize SDL:\n%s", SDL_GetError());
	}

	// All screen res modes MUST be 4:3.
	screen = SDL_SetVideoMode(screenWidth, screenHeight, DISPLAY_DEPTH, SDL_OPENGL | SDL_FULLSCREEN);
	if (screen == NULL) {
		FatalError("Couldn't set a %dx%d video mode [%s]", screenWidth, screenHeight, SDL_GetError());
	}

	// Disable OS cursor by default.
	SDL_WM_GrabInput(SDL_GRAB_ON);

	// Only hide the mouse cursor if we are grabbed
	if (SDL_WM_GrabInput(SDL_GRAB_QUERY) == SDL_GRAB_ON) {
		SDL_ShowCursor(SDL_DISABLE);
	} else {
		SDL_ShowCursor(SDL_ENABLE);
	}

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0, screenWidth, 0, screenHeight, nearZ, farZ);

	if (enableDepthBuffer) {
		glEnable(GL_DEPTH_TEST);
		depthTestEnabled = true;
	}

	gettimeofday(&startTime, NULL);
}

static void handle_key_down(SDL_keysym *keysym) {
	switch (keysym->sym) {
	case SDLK_ESCAPE:
		exit(EXIT_SUCCESS);
		break;
	case SDLK_F1:
		g_world->toggleLightSwitch(0);
		break;
	case SDLK_F2:
		g_world->toggleLightSwitch(1);
		break;
	case SDLK_F3:
		g_world->toggleLightSwitch(2);
		break;
	case SDLK_F4:
		g_world->toggleLightSwitch(3);
		break;
	case SDLK_w:
		g_world->getPlayer()->setMoveForwards(true);
		break;
	case SDLK_s:
		g_world->getPlayer()->setMoveBackwards(true);
		break;
	case SDLK_a:
		g_world->getPlayer()->setStrafeLeft(true);
		break;
	case SDLK_d:
		g_world->getPlayer()->setStrafeRight(true);
		break;
	default:
		break;
	}
}

static void handle_key_up(SDL_keysym *keysym) {
	switch (keysym->sym) {
	case SDLK_w:
		g_world->getPlayer()->setMoveForwards(false);
		break;
	case SDLK_s:
		g_world->getPlayer()->setMoveBackwards(false);
		break;
	case SDLK_a:
		g_world->getPlayer()->setStrafeLeft(false);
		break;
	case SDLK_d:
		g_world->getPlayer()->setStrafeRight(false);
		break;
	default:
		break;
	}
}

// TODO - SDL hack, bug in 68K SDL needs fixing.
static bool ignoreFirstMouseMove = true;

void Display::captureUserInput() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		  case SDL_MOUSEMOTION:
			if (!ignoreFirstMouseMove) {
				mouseMovementX = event.motion.xrel;
				mouseMovementY = event.motion.yrel;
			} else {
				ignoreFirstMouseMove = false;
			}
			break;
		case SDL_KEYDOWN:
			handle_key_down(&event.key.keysym);
			break;
		case SDL_KEYUP:
			handle_key_up(&event.key.keysym);
			break;
		case SDL_QUIT:
			exit(EXIT_SUCCESS);
			break;
		}
	}
}

void Display::displayFrameRate() {
	timeval endTime;
	double elapsedtime;

	gettimeofday(&endTime, NULL);

    // compute and print the elapsed time in seconds.
    elapsedtime = (endTime.tv_sec - startTime.tv_sec) * 1000.0;      // sec to ms
    elapsedtime += (endTime.tv_usec - startTime.tv_usec) / 1000.0;   // us to ms

    LogMessage("\n***************************************************\n");
    LogMessage("GL_RENDERER = %s\n", (char *) glGetString(GL_RENDERER));
    LogMessage("GL_VERSION = %s\n", (char *) glGetString(GL_VERSION));
    LogMessage("GL_VENDOR = %s\n", (char *) glGetString(GL_VENDOR));

    float seconds = (elapsedtime / 1000.0);
    float fps = numFrames / seconds;
    LogMessage("Rendered %d frames in %6.3f seconds (%6.3f FPS)\n", numFrames, seconds, fps);
    LogMessage("***************************************************\n\n");
}

void Display::begin() {
	// Clear the back buffer and z-buffer ready for the next frame.
	if (depthTestEnabled) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	} else {
		glClear(GL_COLOR_BUFFER_BIT);
	}

	glBegin(GL_TRIANGLES);
}

void Display::drawTriangle(NovaTriangle *triangle) {
	for (int i = 0; i < 3; i++) {
		glColor3fv(triangle->scratch_screen_array[i].color);
		glVertex3i(triangle->scratch_screen_array[i].x, triangle->scratch_screen_array[i].y, triangle->scratch_screen_array[i].z);
	}
}

void Display::end() {
	glEnd();

	// Show the new frame.
	glFlush();
	SDL_GL_SwapBuffers();

	numFrames++;
}

