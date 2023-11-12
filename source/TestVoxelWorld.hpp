#pragma once

#include "VoxelWorld.hpp"

namespace vw
{
	class Window;

	class TestVoxelWorld : public VoxelWorld
	{
	public:
		TestVoxelWorld ( Application & application );
	};
}