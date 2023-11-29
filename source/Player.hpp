#pragma once

namespace vw
{
	class World;

	class Player
	{
	public:
		Player ( World & );

		void Update ( float deltaTime );
		void Render ();

	private:
		static float const blockPlaceBreakDelay;

		World * world;
		float nextBlockPlaceBreakCountdown { 0.0f };
	};
}