#pragma once

#include "Camera.hpp"

namespace vw
{
	class Window;
	class Renderer;

	class World
	{
	public:
		World ( Window &, Renderer & );

		void Update ( float deltaTime );

	private:
		Window * window;
		Camera camera;
	};
}