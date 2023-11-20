#pragma once

namespace vw
{
	enum class Sides { up, down, left, right, forward, back };

	extern std::unordered_map <Sides, glm::vec3> const directionVectors;
	extern std::unordered_map <Sides, Sides> const oppositeSides;
	extern std::array <Sides, 6> const sides;
	extern std::unordered_map <Sides, std::string> const sideNames;
}