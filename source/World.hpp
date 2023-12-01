#pragma once

#include "VoxelWorld.hpp"
#include "Player.hpp"
#include "Camera.hpp"

namespace vw
{
	class GUIRenderer;

	class World
	{
	public:
		World ( Application & application );

		void Update ( float );
		void FixedUpdate ();
		void Render ();
		void RenderGUI ( GUIRenderer & );

		Application & GetApplication ();
		VoxelWorld & GetVoxelWorld ();

	private:
		void RenderImGuiLayer ();
		void DrawCrosshair ();

		Application * application;
		Camera camera;
		VoxelWorld voxelWorld;
		Player player;
	};



	// Implementation
	inline Application & World::GetApplication () { return *application; }
	inline VoxelWorld & World::GetVoxelWorld () { return voxelWorld; }
}