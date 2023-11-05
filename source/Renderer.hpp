#pragma once

#include "VoxelRenderer.hpp"

namespace vw
{
	class Window;

	class Renderer
	{
	public:
		Renderer ( Window & window );
		
		void Render ();

	private:
		VoxelRenderer voxelRenderer;
	};
}