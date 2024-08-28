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
#ifndef __RENDER_VERTEX_H
#define __RENDER_VERTEX_H

enum RenderStage {
	INITIAL_RENDER_STAGE, RENDER_STAGE_TRANSFORMED, RENDER_STAGE_PROJECTED
};

class RenderVertex: public ObjectVertex {
public:
	// Projected vertex data (stored in VCS)
	int projectedX, projectedY, projectedZ;

	// The sum of all light to 'hit' this vertex (this frame).
	NovaColor lightSum;

	RenderVertex() {
		stage = INITIAL_RENDER_STAGE;
		projectedX = projectedY = projectedZ = 0;
		visible = false;
	}

	RenderVertex(float x, float y, float z) : ObjectVertex(x, y, z) {
		stage = INITIAL_RENDER_STAGE;
		projectedX = projectedY = projectedZ = 0;
		visible = false;
	}

	void setStage(RenderStage stage) {
		this->stage = stage;
	}

	RenderStage getStage() const {
		return stage;
	}

	void setVisible(bool visible) {
		this->visible = visible;
	}

	bool isVisible() const {
		return visible;
	}

	void operator =(const ObjectVertex &vertex);

	void reset() {
		stage = INITIAL_RENDER_STAGE;
		visible = false;
		lightSum.reset();
	}


protected:
	RenderStage stage;
	bool visible;
};

RenderVertex operator * (ObjectVertex &v, const AstMatrix4x3 &m);

#endif // __RENDER_VERTEX_H

