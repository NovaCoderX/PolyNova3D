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


#ifndef __LIGHT_SOURCE_H
#define __LIGHT_SOURCE_H

// Types of LIGHTS.
enum LightType {
	LIGHTING_TYPE_UNDEFINED, LIGHTING_TYPE_AMBIENT, LIGHTING_TYPE_DIRECT, LIGHTING_TYPE_POINT
};


// Base class.
class LightSource {
	friend class AstObject;
	friend class World;

public:
	LightSource() {
		enabled = false;
		type = LIGHTING_TYPE_UNDEFINED;
		next_in_list = 0;
		prior_in_list = 0;
	}

	LightType GetType() {
		return type;
	}

	void SetEnabled(bool e) {
		enabled = e;
	}

	bool isEnabled() const {
		return enabled;
	}

	void SetColor(NovaColor new_color) {
		color = new_color;
	}

	void toggle() {
		if (enabled)
			enabled = false;
		else
			enabled = true;
	}

protected:
	LightSource *next_in_list;
	LightSource *prior_in_list;

	NovaColor color;
	LightType type;
	bool enabled;
};


#endif // __LIGHT_SOURCE_H
