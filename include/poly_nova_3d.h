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
#ifndef __POLY_NOVA_3D_H
#define __POLY_NOVA_3D_H

#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include <utils.h>
#include <logging.h>
#include <nova_color.h>
#include <nova_vertex.h> // base vertex
#include <object_vertex.h> // surface normal
#include <sum_vertex.h> // Used to calculate static vertex normals.
#include <render_vertex.h> // scratch render flags, light sum per frame
#include <display_vertex.h> // OpenGL wrapper
#include <nova_triangle.h>
#include <nova_matrix.h>
#include <object_extents.h>
#include <collision_service.h>
#include <object_definition.h>
#include <base_object.h>
#include <player_movement.h>
#include <player_head.h>
#include <player_body.h>
#include <player.h>
#include <light_source.h>
#include <ambient_light.h>
#include <direct_light.h>
#include <point_light.h>
#include <display.h>
#include <camera.h>
#include <scene_definition.h>
#include <world.h>


// Global pointers.
extern World* g_world;



#endif // __POLY_NOVA_3D_H

