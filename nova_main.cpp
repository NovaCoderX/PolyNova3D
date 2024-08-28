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


// Global world pointer.
World *g_world = NULL;

static void nova_exit_handler() {
	if (g_world) {
		// Print the stats.
		if (g_world->getDisplay()) {
			g_world->getDisplay()->displayFrameRate();
		}

		// Delete World cleans up everything else.
		delete g_world;
		g_world = NULL;
	}

	// Shutdown the logger.
	LoggingShutdown();
}

extern "C" {
	void poly_nova_main(void) {
		atexit(nova_exit_handler);

		// Startup the logger.
		LoggingInit();

		// Create our world object (global pointer).
		g_world = new World();
		g_world->create();

		// And then run it...
		g_world->run();
	}
}

