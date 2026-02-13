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

// Only 32-bit depth is supported.
#define DISPLAY_DEPTH 32

static SDL_Surface *screen = NULL;
static timeval startTime;


Display::Display() {
	screenWidth = screenHeight = 0;
	enableDepthBuffer = false;
	nearZ = farZ = 0;
	numFrames = 0;
	fullscreen = false;
	mouseMovementX = mouseMovementY = 0;
	depthTestEnabled = false;
	mouseCaptured = false;
}

Display::~Display() {
	LogMessage("Shutting down the Display.....\n");
	SDL_Quit();
}

void Display::initialize(int screenWidth, int screenHeight, bool enableDepthBuffer, float nearZ, float farZ, bool fullscreen) {
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
	this->enableDepthBuffer = enableDepthBuffer;
	this->nearZ = nearZ;
	this->farZ = farZ;
	this->fullscreen = fullscreen;

	if (SDL_Init(SDL_INIT_NOPARACHUTE | SDL_INIT_VIDEO) == -1) {
		FatalError("Could not initialize SDL:\n%s", SDL_GetError());
	}

	Uint32 flags = SDL_OPENGL;
	if (fullscreen) {
		flags |= SDL_FULLSCREEN;
		LogMessage("Trying to open a %dx%d %d-bit OpenGL screen....\n", screenWidth, screenHeight, DISPLAY_DEPTH);
	} else {
		LogMessage("Trying to open a %dx%d %d-bit OpenGL window....\n", screenWidth, screenHeight, DISPLAY_DEPTH);
	}

	// All screen res modes MUST be 4:3.
	screen = SDL_SetVideoMode(screenWidth, screenHeight, DISPLAY_DEPTH, flags);
	if (screen == NULL) {
		FatalError("Couldn't set a %dx%d video mode [%s]", screenWidth, screenHeight, SDL_GetError());
	}

	SDL_WM_SetCaption("PolyNova v3", NULL);

	// Disable OS cursor by default.
	grabMouse(true);

	// Setup OpenGL.
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, screenWidth, 0, screenHeight, nearZ, farZ);

	if (enableDepthBuffer) {
		glEnable(GL_DEPTH_TEST);
		depthTestEnabled = true;
	} else {
		depthTestEnabled = false;
	}

	// Store the start time.
	gettimeofday(&startTime, NULL);
}

void Display::grabMouse(bool enable) {
	if (enable) {
		if (SDL_WM_GrabInput(SDL_GRAB_QUERY) == SDL_GRAB_OFF) {
			SDL_WM_GrabInput(SDL_GRAB_ON);
		}
	} else {
		// Only release capture when in window mode.
		if (!fullscreen) {
			if (SDL_WM_GrabInput(SDL_GRAB_QUERY) == SDL_GRAB_ON) {
				SDL_WM_GrabInput(SDL_GRAB_OFF);
			}
		}
	}

	if (SDL_WM_GrabInput(SDL_GRAB_QUERY) == SDL_GRAB_ON) {
		// Only hide the mouse cursor when input is grabbed.
		SDL_ShowCursor(SDL_DISABLE);
		mouseCaptured = true;
	} else {
		SDL_ShowCursor(SDL_ENABLE);
		mouseCaptured = false;
	}
}

void Display::toggleMouseGrab() {
	if (!fullscreen) {
		this->grabMouse(!mouseCaptured);
	}
}

void Display::toggleFullScreen() {
	int width, height;
	Uint8 depth;
	Uint16 pitch;
	void* pixels;
	Uint32 flags;

	// Store in case we need to revert.
	width = screen->w;
	height = screen->h;
	depth = screen->format->BitsPerPixel;
	pitch = screen->pitch;
	flags = screen->flags;

	// Make a backup of the screen pixels before they get wiped.
	pixels = SDL_malloc(screen->h * screen->pitch);
	if (pixels) {
		SDL_memcpy(pixels, screen->pixels, (screen->h * screen->pitch));
	} else {
		LogWarningMessage("Couldn't allocate a buffer to save the screen data\n");
	}

	// Toggle.
	fullscreen = (!fullscreen);

	if (fullscreen) {
		flags |= SDL_FULLSCREEN;
	} else {
		flags &= ~SDL_FULLSCREEN;
	}

	screen = SDL_SetVideoMode(width, height, depth, flags);
	if (!screen) {
		LogWarningMessage("Could not toggle full screen mode, will try restoring the old mode\n");

		// Try swapping back.
		fullscreen = (!fullscreen);

		if (fullscreen) {
			flags |= SDL_FULLSCREEN;
		} else {
			flags &= ~SDL_FULLSCREEN;
		}

		screen = SDL_SetVideoMode(width, height, depth, flags);
		if (!screen) {
			FatalError("Could not restore the screen mode\n");
		}
	}

	// We need to redraw the screen.
	if (pixels) {
		if ((screen->h == height) && (screen->pitch == pitch)) {
			SDL_memcpy(screen->pixels, pixels, (screen->h * screen->pitch));
		}

		SDL_free(pixels);
		SDL_UpdateRect(screen, 0, 0, 0, 0);
	}

	// Grab mouse if fullscreen.
	if (fullscreen) {
		grabMouse(true);
	}

	// Setup OpenGL (again).
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, screenWidth, 0, screenHeight, nearZ, farZ);

	if (enableDepthBuffer) {
		glEnable(GL_DEPTH_TEST);
		depthTestEnabled = true;
	} else {
		depthTestEnabled = false;
	}
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
	case SDLK_m:
		// Ctrl-m toggles mouse capture (in windowed mode)
		if (keysym->mod & KMOD_CTRL) {
			g_world->getDisplay()->toggleMouseGrab();
		}
		break;
	case SDLK_RETURN:
		// Alt-enter toggles full screen mode
		if (keysym->mod & KMOD_ALT) {
			g_world->getDisplay()->toggleFullScreen();
		}
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

void Display::processUserInput() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		  case SDL_MOUSEMOTION:
			if (!ignoreFirstMouseMove) {
				if (mouseCaptured) {
					mouseMovementX = event.motion.xrel;
					mouseMovementY = event.motion.yrel;
				}
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
	// Show the new frame.
	glEnd();
	//glFlush();
	SDL_GL_SwapBuffers();
	numFrames++;
}

